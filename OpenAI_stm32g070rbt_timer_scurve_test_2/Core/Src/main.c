#include "main.h"
#include "stm32g0xx.h"


#define FORWARD  1

#define MOTOR1_STEP_PIN  GPIO_PIN_0
#define MOTOR1_DIR_PIN   GPIO_PIN_1
#define MOTOR2_STEP_PIN  GPIO_PIN_2
#define MOTOR2_DIR_PIN   GPIO_PIN_3

#define MOTOR1_GEAR_RATIO  10
#define MOTOR2_GEAR_RATIO  5

#define ACCELERATION_RATE  1000  // Steps/s^2
#define MAX_SPEED           1000  // Steps/s

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

motor_state_t motor1;
motor_state_t motor2;

GPIO_InitTypeDef gpio_init;
TIM_HandleTypeDef tim_handle;

void init_gpio(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configure motor step and direction pins as outputs
	gpio_init.Pin = MOTOR1_STEP_PIN | MOTOR1_DIR_PIN | MOTOR2_STEP_PIN | MOTOR2_DIR_PIN;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_init);
}

void init_timers(void) {
	__HAL_RCC_TIM3_CLK_ENABLE();

	tim_handle.Instance = TIM3;
	tim_handle.Init.Prescaler = 1;
	tim_handle.Init.Period = 1000;
	tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&tim_handle);

	// Enable update interrupt
	__HAL_TIM_ENABLE_IT(&tim_handle, TIM_IT_UPDATE);

	HAL_TIM_Base_Start(&tim_handle);

	// Enable TIM3 interrupt in NVIC
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void set_acceleration(motor_state_t *motor, int32_t acceleration) {
  motor->acceleration = acceleration;
}

void set_deceleration(motor_state_t *motor, int32_t deceleration) {
  motor->deceleration = deceleration;
}


void set_speed(motor_state_t *motor, int32_t speed) {
	// Calculate number of steps per interrupt based on gear ratio and speed
	motor->steps_per_interrupt = speed * motor->gear_ratio / 1000;
	motor->target_speed = speed;
}

void move(motor_state_t *motor, uint32_t direction, int32_t steps) {
	motor->direction = direction;
	motor->steps_remaining = steps;
}

void TIM3_IRQHandler(void) {
	// Clear update interrupt flag
	__HAL_TIM_CLEAR_IT(&tim_handle, TIM_IT_UPDATE);

	// Update speed for each motor
	if (motor1.steps_remaining > 0) {
		if (motor1.speed < motor1.target_speed) {
			motor1.speed += motor1.acceleration;
			if (motor1.speed > motor1.target_speed) {
				motor1.speed = motor1.target_speed;
			}
		} else if (motor1.speed > motor1.target_speed) {
			motor1.speed -= motor1.deceleration;
			if (motor1.speed < motor1.target_speed) {
				motor1.speed = motor1.target_speed;
			}
		}

		motor1.steps_remaining -= motor1.steps_per_interrupt;
		if (motor1.steps_remaining <= 0) {
			motor1.steps_remaining = 0;
			motor1.speed = 0;
		}

		if (motor1.direction == FORWARD) {
			HAL_GPIO_WritePin(GPIOA, MOTOR1_STEP_PIN, GPIO_PIN_SET);
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			HAL_GPIO_WritePin(GPIOA, MOTOR1_STEP_PIN, GPIO_PIN_RESET);
		} else {
			HAL_GPIO_WritePin(GPIOA, MOTOR1_DIR_PIN, GPIO_PIN_SET);
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			HAL_GPIO_WritePin(GPIOA, MOTOR1_DIR_PIN, GPIO_PIN_RESET);
		}
	}

	if (motor2.steps_remaining > 0) {
		if (motor2.speed < motor2.target_speed) {
			motor2.speed += motor2.acceleration;
			if (motor2.speed > motor2.target_speed) {
				motor2.speed = motor2.target_speed;
			}
		} else if (motor2.speed > motor2.target_speed) {
			motor2.speed -= motor2.deceleration;
			if (motor2.speed < motor2.target_speed) {
				motor2.speed = motor2.target_speed;
			}
		}

		motor2.steps_remaining -= motor2.steps_per_interrupt;
		if (motor2.steps_remaining <= 0) {
			motor2.steps_remaining = 0;
			motor2.speed = 0;
		}

		if (motor2.direction == FORWARD) {
			HAL_GPIO_WritePin(GPIOA, MOTOR1_STEP_PIN, GPIO_PIN_SET);
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			HAL_GPIO_WritePin(GPIOA, MOTOR1_STEP_PIN, GPIO_PIN_RESET);
		} else {
			HAL_GPIO_WritePin(GPIOA, MOTOR2_DIR_PIN, GPIO_PIN_SET);
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			__NOP();
			HAL_GPIO_WritePin(GPIOA, MOTOR2_DIR_PIN, GPIO_PIN_RESET);
		}
	}
}

int main(void) {
	// Initialize GPIO and timers
	init_gpio();
	init_timers();

	// Set initial state for motors
	motor1.gear_ratio = MOTOR1_GEAR_RATIO;
	motor2.gear_ratio = MOTOR2_GEAR_RATIO;
	set_speed(&motor1, 0);
	set_speed(&motor2, 0);
	set_acceleration(&motor1, ACCELERATION_RATE);
	set_acceleration(&motor2, ACCELERATION_RATE);
	set_deceleration(&motor1, ACCELERATION_RATE);
	set_deceleration(&motor2, ACCELERATION_RATE);

	// Move motors forward for 500 steps at maximum speed
	move(&motor1, FORWARD, 500);
	set_speed(&motor1, MAX_SPEED);
	move(&motor2, FORWARD, 500);
	set_speed(&motor2, MAX_SPEED);

	while (1) {
		// Wait for interrupts to drive motors
	}
}
