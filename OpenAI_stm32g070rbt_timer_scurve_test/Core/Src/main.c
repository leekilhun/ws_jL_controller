#include "main.h"
#include "stm32g0xx.h"
#include "s_curve.h"

#define PULSE_PER_REV 2000
#define MAX_SPEED 3000

// Timer period in microseconds
#define TIMER_PERIOD (1000000 / (MAX_SPEED * PULSE_PER_REV))

// Timer prescaler value
#define TIMER_PRESCALER 1

// Array to hold s_curve values for each pin
uint8_t s_curve_values[9];

void setup_gpio(void)
{
	// Enable clock for GPIOC
	//RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
//	__HAL_RCC_GPIOC_CLK_ENABLE();
	// Set all 9 pins of GPIOC as output
	GPIOC->MODER = (GPIOC->MODER & ~(0x3FF << (2 * 0))) | (0x555 << (2 * 0));
}

void setup_timer(void)
{
	// Enable clock for TIM3
	//RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;

	// Set timer prescaler and period
	TIM3->PSC = TIMER_PRESCALER - 1;
	TIM3->ARR = TIMER_PERIOD - 1;

	// Enable timer update interrupt
	TIM3->DIER |= TIM_DIER_UIE;

	// Enable timer
	TIM3->CR1 |= TIM_CR1_CEN;

	// Set timer interrupt priority
	NVIC_SetPriority(TIM3_IRQn, 0);

	// Enable timer interrupt
	NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler(void)
{
	// Check if update interrupt flag is set
	if (TIM3->SR & TIM_SR_UIF) {
		// Clear update interrupt flag
		TIM3->SR &= ~TIM_SR_UIF;

		// Update s_curve values
		update_s_curve_values(s_curve_values, 9);

		// Set output pins to s_curve values
		GPIOC->ODR = (GPIOC->ODR & ~(0x1FF << 0)) | ((s_curve_values[0] << 0) | (s_curve_values[1] << 1) | (s_curve_values[2] << 2) |
				(s_curve_values[3] << 3) | (s_curve_values[4] << 4) | (s_curve_values[5] << 5) |
				(s_curve_values[6] << 6) | (s_curve_values[7] << 7) | (s_curve_values[8] << 8));
	}
}

int main(void)
{
	// Setup GPIO and timer
	setup_gpio();
	setup_timer();

	while (1) {
		// Wait
		// Wait for interrupt
	}

	return 0;
}
