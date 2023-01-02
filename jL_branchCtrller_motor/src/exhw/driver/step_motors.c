/*
 * step_motors.c
 *
 *  Created on: 2023. 1. 1.
 *      Author: gns2l
 */

#include "step_motors.h"

#ifdef _USE_EXHW_STEP_MOTORS

#include "s_curve.h"
#include "math.h"



//#define TABLE_LEN   200
#define TABLE_PSC     1
#define MAX_FREQ    800
#define START_FREQ  100
#define RUN   0
#define STOP  1


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


#define state_idle  0
#define state_accelerate  1
#define state_decelerate  2
#define state_driving  3

typedef struct
{
	long                stepPosition; // current position of stepper (total of all movements)

	volatile int8_t     dir;          // current direction of movement, used to keep
	volatile uint32_t   totalSteps ;  // number of  steps requested for current movement
	volatile uint32_t   stepCount;    // number of steps completed in current movement

	volatile float      speedScale;   // used to slow down this motor to make coordinated movement with other motors

	volatile uint8_t    state;
	volatile uint8_t    is_busy;
	volatile uint8_t    edge;


	volatile uint32_t   count;
	volatile uint32_t   speed;

	volatile int   accel_index;
	volatile int   vel_index;
	volatile int   decel_index;

	volatile bool       moveDone ;

	bool                is_init;
	bool                is_start;

} motor_tbl_t;


static uint8_t remainingMotorsFlag = 0;

static unsigned int SS_table_len(unsigned int* table);
static void SS_curve_gen(int steps);

//////////////////////////////////////////////

static void step_motorsTimerInit();

static void step_motorsPulse();

void step_motorsISR(void);


#ifdef _USE_HW_CLI
static void cliStepMotors(cli_args_t *args);
#endif

motor_tbl_t motor_1 ={};

bool step_motorsInit(void)
{

	step_motorsTimerInit();

#ifdef _USE_HW_CLI
	cliAdd("step_motors", cliStepMotors);
#endif



	return false;
}

void step_motorsTest()
{
	SS_curve_gen(abs(10000));
}

void step_motorsPulse()
{
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
}

void step_motorsTimerInit(void)
{
	// Enable clock for TIM3
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;

	// Set prescaler value
	//(uint32_t)(SystemCoreClock/1000000) - 1 ; //

	/* 100us interrupt setting*/
	//TIM3->PSC = 640 - 1; // Set prescaler to 48, giving a timer frequency of 1 MHz
	//TIM3->ARR = (10-1);//(uint32_t)(1000000.0 / timer_frequency) -1; // Set auto-reload value based on desired timer frequency


	/* 10us interrupt setting*/
	TIM3->PSC = 64 - 1;
	TIM3->ARR = (10-1);


	// Enable interrupt for TIM3
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);

	// Start timer
	//TIM3->CR1 |= TIM_CR1_CEN;
}


void TIM3_IRQHandler(void)
{
	step_motorsISR();
}




void step_motorsISR(void)
{
	motor_tbl_t * pM = &motor_1;
	TIM_TypeDef * pTim = TIM3;

	step_motorsPulse();

	switch (pM->state)
	{
		//======
		case state_idle:
			if (remainingMotorsFlag)
				pM->state = state_accelerate;
			break;

		case state_accelerate:
			if(pM->stepCount < pM->accel_index)
			{
				pTim->CCR1 = pM->speed / s_curve_table[pM->stepCount];
				pTim->ARR  = pM->speed / s_curve_table[pM->stepCount];
			}
			// max speed
			else if((pM->stepCount >= pM->accel_index)
					&& (pM->stepCount <= pM->accel_index + pM->vel_index))
			{
				pM->state = state_driving;
			}
			// deceleration
			else
			{
				pM->state = state_decelerate;
			}
			break;

		case state_decelerate:
			pTim->CCR1 = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
			pTim->ARR  = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
			pM->count++;

			if(pM->stepCount == pM->totalSteps)
			{
				pTim->DIER &= ~TIM_DIER_UIE;
				/*
				TIM3->CR1 &= ~TIM_CR1_CEN;
				TIM3->EGR |= TIM_EGR_UG;
				*/


				pM->count = 0;
				pM->stepCount = 0;
				pM->moveDone = true;
				pM->state = state_idle;
			}

			break;
		case state_driving:
			if((pM->stepCount > pM->accel_index)
					&& (pM->stepCount > (pM->accel_index + pM->vel_index)))
			{
				pM->state = state_decelerate;
			}

			pTim->CCR1 = pM->speed / s_curve_table[pM->accel_index-1];
			pTim->ARR  = pM->speed / s_curve_table[pM->accel_index-1];
			break;
			//======
		default:
			break;
	}


	if(pM->dir == CW)     {pM->stepPosition++;}
	else                  {pM->stepPosition--;}
	pM->stepCount++;
	if (pM->moveDone == true) remainingMotorsFlag &= ~(1 << AXIS_1); //Off  motor timer_cal


}

unsigned int SS_table_len(unsigned int* table)
{
	int i;

	for(i = 0;i < TABLE_LEN;i++)
	{
		table[i] = table[i] / TABLE_PSC;

		if(table[i] >= MAX_FREQ-2) return i;
	}

	return TABLE_LEN;
}




void SS_curve_gen(int steps)
{

	motor_1.accel_index  = SS_table_len(s_curve_table);
	motor_1.decel_index  = motor_1.accel_index;
	motor_1.vel_index    = steps - motor_1.accel_index - motor_1.decel_index;
	motor_1.totalSteps   = steps;
	motor_1.count        = 0;
	motor_1.state        = 0;

}



void step_motorsMoveRel (int steps, uint32_t speed)
{
	if (speed>105)    speed=100;

	int position = steps - motor_1.stepPosition ;
	if (position < 0)
	{
		motor_1.dir = CCW;
	}
	else
	{
		motor_1.dir = CW;
	}

	SS_curve_gen(abs(position));

	motor_1.speed = u32_Speed[speed];
	motor_1.moveDone = false;
	motor_1.state = state_idle;

	remainingMotorsFlag = 1;

	// Start timer
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);

	TIM3->CR1 |= TIM_CR1_CEN;

	while (remainingMotorsFlag)
	{	}
	remainingMotorsFlag = 0;
}












#if 0





/*

1.	timer interrupt를 이용한 s-curve 속도 제어 코드 작성
2.	2개의 step motor에 대한 기어비를 설정한다.
3.	timer interrupt 콜백 함수에서 s-curve 속도 제어 알고리즘을 적용한다.
4.	2개의 step motor에 대한 제어 신호를 생성한다.
5.	2개의 step motor에 제어 신호를 전송한다.
6.	timer interrupt 주기를 조절하여 s-curve 속도 제어의 정확도를 조절한다


 */




#define PI 3.14159265

uint32_t before_arr = 0;

static void set_motor1_target_position(int32_t position);
static void set_motor2_target_position(int32_t position);

static void set_motor1_speed(float speed);
static void set_motor2_speed(float speed);




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





bool step_motorsInit(void)
{

	step_motorsTimerInit();

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


void step_motorsPulse()
{
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
}


void test_timer(){
	//gpioPinToggle(12);
	//return;

	if (++next_cnt == 10)
	{
		TIM3->CCR4;
		before_arr =before_arr + 1;
		TIM3->ARR = before_arr;
		step_motorsPulse();
		next_cnt = 0;
	}



}


void step_motorsTimerInit(void)
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
	TIM3->PSC = 64 - 1;
	TIM3->ARR = (10-1);
	before_arr = (10-1);
	/* 5us interrupt setting*/
	//TIM3->PSC = 64 - 1;
	//TIM3->ARR = 5 -1;

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


#endif

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
		//	set_motor1_target_position(pulse);
		step_motorsMoveRel(pulse,100);
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



#if 0 //


#define ACCELERATED_SPEED_LENGTH 3000  // Define the number of points of acceleration (in fact, it is also the meaning of 3000 subdivision steps), adjust this parameter to change the acceleration point.
#define FRE_MIN 3000                   // The lowest running frequency, adjust this parameter to adjust the minimum running speed
#define FRE_MAX 24000                  // The highest running frequency, adjust this parameter to adjust the maximum speed at a constant speed
int step_to_run;                       // The number of steps to run
int g_motor_dir_s;
float fre[ACCELERATED_SPEED_LENGTH];             // The frequency of each step in the array storage acceleration process
unsigned short period[ACCELERATED_SPEED_LENGTH]; // Autoload value of each step timer in the array storage acceleration process

static void step_motors_CalculateSModelLine(float fre[], unsigned short period[], float len, float fre_max, float fre_min, float flexible);


bool step_motorsInit(void)
{

	step_motors_CalculateSModelLine(fre,period,ACCELERATED_SPEED_LENGTH,FRE_MAX,FRE_MIN,4);

	timer_init();

#ifdef _USE_HW_CLI
	cliAdd("step_motors", cliStepMotors);
#endif



	return false;
}



void step_motors_CalculateSModelLine(float fre[], unsigned short period[], float len, float fre_max, float fre_min, float flexible)
{
	int i = 0;
	float deno;
	float melo;
	float delt = fre_max - fre_min;
	for (; i < len; i++)
	{
		melo = flexible * (i - len / 2) / (len / 2);
		deno = 1.0 / (1 + expf(-melo)); // expf is a library function of exponential(e)
		fre[i] = delt * deno + fre_min;
		period[i] = (unsigned short)(6400000.0 / fre[i]); // 10000000 is the timer driver frequency
	}

	return;
}


// Interrupt handler for TIM3
int i = 0;
void TIM3_IRQHandler(void)
{

	step_motorsPulse();
	// Clear interrupt flag
	TIM3->SR &= ~TIM_SR_UIF;

	if (i < ACCELERATED_SPEED_LENGTH && g_motor_dir_s == 0) // Accelerate until the number of steps reaches ACCELERATED_SPEED_LENGTH
	{
		TIM3->ARR = period[i];
		TIM3->CCR4 = period[i] / 2;
		i++;
		step_to_run = 20000; // Set the number of steps to run here, of course, you can write it in the main function, there is no count of steps for acceleration and deceleration.
	}
	else if (step_to_run > 0) // constant speed
	{
		step_to_run--;
	}
	else if (i > 0 && g_motor_dir_s == 1) // Motor deceleration
	{
		i--;
		TIM3->ARR = period[i];
		TIM3->CCR4 = period[i] / 2;
	}

	if (i == ACCELERATED_SPEED_LENGTH || i == 0)
	{
		g_motor_dir_s = (~g_motor_dir_s) & 0x01;
	}

	return;
}


#endif


#endif
