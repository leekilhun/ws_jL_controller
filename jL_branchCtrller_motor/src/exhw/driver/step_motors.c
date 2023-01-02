/*
 * step_motors.c
 *
 *  Created on: 2023. 1. 1.
 *      Author: gns2l
 */

#include "step_motors.h"

#ifdef _USE_EXHW_STEP_MOTORS
#include "math.h"

// Struct to hold state for each motor
typedef struct {
	int32_t speed;
	int32_t target_speed;
	int32_t acceleration;
	int32_t deceleration;
	int32_t steps_remaining;
	int32_t steps_per_interrupt;
	uint32_t direction;
	uint32_t gear_ratio;
} motor_state_t;



/*

1.	timer interrupt를 이용한 s-curve 속도 제어 코드 작성
2.	2개의 step motor에 대한 기어비를 설정한다.
3.	timer interrupt 콜백 함수에서 s-curve 속도 제어 알고리즘을 적용한다.
4.	2개의 step motor에 대한 제어 신호를 생성한다.
5.	2개의 step motor에 제어 신호를 전송한다.
6.	timer interrupt 주기를 조절하여 s-curve 속도 제어의 정확도를 조절한다


 */

#define PI 3.14159265


static void set_motor1_target_position(int32_t position);
static void set_motor2_target_position(int32_t position);

static void set_motor1_speed(float speed);
static void set_motor2_speed(float speed);

static void timer_init();


// Motor 1 variables
int32_t motor1_target_position = 0;  // Target position for motor 1
int32_t motor1_current_position = 0; // Current position of motor 1
float motor1_gear_ratio = 1.0;       // Gear ratio for motor 1

// Motor 2 variables
int32_t motor2_target_position = 0;  // Target position for motor 2
int32_t motor2_current_position = 0; // Current position of motor 2
float motor2_gear_ratio = 1.0;       // Gear ratio for motor 2


// Timer variables
float timer_frequency = 1000000.0; // Frequency of timer interrupt in Hz
float acceleration_time = 1.0;    // Time it takes for motors to reach maximum speed in seconds
float deceleration_time = 1.0;    // Time it takes for motors to come to a stop in seconds


volatile uint8_t next_cnt = 0;

#ifdef _USE_HW_CLI
static void cliStepMotors(cli_args_t *args);
#endif




bool step_motorsInit(void)
{
	timer_init();

#ifdef _USE_HW_CLI
  cliAdd("step_motors", cliStepMotors);
#endif

	return false;
}


void step_motorsSetPosition(axis_dir axis, int32_t steps)
{
	if (axis ==  AXIS_1)
	{
		set_motor1_target_position(steps);
	}
	else if (axis ==  AXIS_2)
	{
		set_motor2_target_position(steps);
	}
}


void step_motorsRunAndWait(void)
{
	while (motor1_current_position != motor1_target_position
			|| motor2_current_position != motor2_target_position)
	{
	}
}





// Function to initialize timer
void timer_init(void)
{
	// Enable clock for TIM3
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
	//RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;

	// Set prescaler value
	//(uint32_t)(SystemCoreClock/1000000) - 1 ; //

	/* 100us interrupt setting*/
	//TIM3->PSC = 640 - 1; // Set prescaler to 48, giving a timer frequency of 1 MHz
	//TIM3->ARR = (10-1);//(uint32_t)(1000000.0 / timer_frequency) -1; // Set auto-reload value based on desired timer frequency


	/* 10us interrupt setting*/
	//TIM3->PSC = 64 - 1;
	//TIM3->ARR = (10-1);

	/* 5us interrupt setting*/
	TIM3->PSC = 64 - 1;
	TIM3->ARR = 5 -1;

	/* 2us interrupt setting*/
	//TIM3->PSC = 32 - 1;
	//TIM3->ARR = 4 -1;

	/* 1us interrupt setting*/
	//TIM3->PSC = 16 - 1;
	//TIM3->ARR = 4 -1;


	// Set auto-reload value
  //(1000-1);//

	// Enable interrupt for TIM3
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);

	// Start timer
	TIM3->CR1 |= TIM_CR1_CEN;



}

void test_timer(){
	//gpioPinToggle(12);
	//return;

	if (++next_cnt == 40)
	{
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
		//gpioPinToggle(12);
		//gpioPinWrite(12, true);
		//delayUs(1);
		//gpioPinWrite(12, false);
		next_cnt = 0;
	}
}

// Interrupt handler for TIM3
void TIM3_IRQHandler(void)
{
	test_timer();

	// Clear interrupt flag
	TIM3->SR &= ~TIM_SR_UIF;

	return;


	// Calculate s-curve acceleration/deceleration values for both motors
	float motor1_acceleration = (2.0 * (motor1_target_position - motor1_current_position) / (acceleration_time * timer_frequency)) / motor1_gear_ratio;
	float motor1_deceleration = (2.0 * (motor1_target_position - motor1_current_position) / (deceleration_time * timer_frequency)) / motor1_gear_ratio;

	float motor2_acceleration = (2.0 * (motor2_target_position - motor2_current_position) / (acceleration_time * timer_frequency)) / motor2_gear_ratio;
	float motor2_deceleration = (2.0 * (motor2_target_position - motor2_current_position) / (deceleration_time * timer_frequency)) / motor2_gear_ratio;

	// Calculate current speed for both motors
	float motor1_speed = 0.0;
	if (motor1_current_position < motor1_target_position)
	{
		motor1_speed = motor1_acceleration * (1.0 - cos((float)(motor1_current_position - motor1_target_position) / (float)(motor1_target_position - motor1_current_position) * PI)) / 2.0;
	}
	else if (motor1_current_position > motor1_target_position)
	{
		motor1_speed = motor1_deceleration * (1.0 - cos((float)(motor1_current_position - motor1_target_position) / (float)(motor1_current_position - motor1_target_position) * PI)) / 2.0;
	}


	float motor2_speed = 0.0;
	if (motor2_current_position < motor2_target_position)
	{
		motor2_speed = motor2_acceleration * (1.0 - cos((float)(motor2_current_position - motor2_target_position) / (float)(motor2_target_position - motor2_current_position) * PI)) / 2.0;
	}
	else if (motor2_current_position > motor2_target_position)
	{
		motor2_speed = motor2_deceleration * (1.0 - cos((float)(motor2_current_position - motor2_target_position) / (float)(motor2_current_position - motor2_target_position) * PI)) / 2.0;
	}

	// Update current positions of both motors
	motor1_current_position += motor1_speed;
	motor2_current_position += motor2_speed;

	// Set motor speeds based on calculated values
	set_motor1_speed(motor1_speed);
	set_motor2_speed(motor2_speed);

	// Clear interrupt flag
	TIM3->SR &= ~TIM_SR_UIF;
}

/*
int main(void)
{
  // Initialize timer
  timer_init();

  while (1)
  {
    // Set target positions for both motors
    set_motor1_target_position(1000);
    set_motor2_target_position(2000);

    // Wait until both motors have reached their target positions
    while (motor1_current_position != motor1_target_position || motor2_current_position != motor2_target_position)
    {
    }

    // Set new target positions for both motors
    set_motor1_target_position(2000);
    set_motor2_target_position(1000);

    // Wait until both motors have reached their target positions
    while (motor1_current_position != motor1_target_position || motor2_current_position != motor2_target_position)
    {
    }
  }
}
 */

// Function to set target position for motor 1
void set_motor1_target_position(int32_t position)
{
	motor1_target_position = position;
}

// Function to set target position for motor 2
void set_motor2_target_position(int32_t position)
{
	motor2_target_position = position;
}

// Function to set speed of motor 1
//set_motor_speed(0.5);
void set_motor1_speed(float speed)
{
	// Set motor 1 speed based on calculated value

	// Calculate pulse count width based on desired motor speed
	// Convert speed value to pulse count width in microseconds
	uint32_t pulse_count_width = (uint32_t)(speed * 1000.0);

	// Set pulse count width value
	TIM3->CCR1 = pulse_count_width;

	// Start timer to generate pulse train signal
	TIM3->CR1 |= TIM_CR1_CEN;

// Example usage: set motor speed to 50%

}

// Function to set speed of motor 2
void set_motor2_speed(float speed)
{
	// Set motor 2 speed based on calculated value
}



#ifdef _USE_HW_CLI
void cliStepMotors(cli_args_t *args)
{
	 bool ret = false;

	if (args->argc == 1 && args->isStr(0, "info") == true)
	{
		ret = true;
	}

	if (args->argc == 2 && args->isStr(0, "move") == true)
	{
		uint32_t pulse = (uint32_t)args->getData(1);
		set_motor1_target_position(pulse);
		{
			cliPrintf("set_motor1_target_position : %d OK\n", pulse);
			ret = true;
		}
	}

	if (args->argc == 3 && args->isStr(0, "pulse") == true)
	{
		uint32_t pulse = (uint32_t)args->getData(1);
		uint32_t us = (uint32_t)args->getData(2);
    const uint8_t gpio_out_pin = 11;

		gpioPinWrite(gpio_out_pin, false);
		for ( int i = 0 ; i<pulse; i++)
		{
			gpioPinWrite(gpio_out_pin, true);
			delayUs(1);
			gpioPinWrite(gpio_out_pin, false);

			delayUs(us);
		}
		{
			cliPrintf("pulse: %d[pulse], interval :%d[us] \n", pulse, us);
			ret = true;
		}
	}


  if (ret != true)
  {
    cliPrintf( "step_motors info\n");
    cliPrintf( "step_motors move [position]\n");
    cliPrintf( "step_motors pulse [pulse] [us]\n");

  }


}
#endif




#endif
