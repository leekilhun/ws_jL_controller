/*
 * ap.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */


/*

  boot sequence
  . use uart_2 port

  1. reset button count

 */

#include "ap.h"

void apInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure = {0,};

	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStructure.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}




void apMain(void)
{
  uint32_t pre_time;


  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 150)
    {

      pre_time = millis();
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);

    }


  }
}


