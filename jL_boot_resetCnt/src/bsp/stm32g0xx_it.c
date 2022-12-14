/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "reset.h"
#include "stm32g0xx_it.h"

//#include "tickTimer.h"
//#include "sw_timer.h"


/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{

}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
#ifndef  _USE_HW_RTOS  //rtos
void SVC_Handler(void)
{
}
#endif
/**
  * @brief This function handles Pendable request for system service.
  */
#ifndef  _USE_HW_RTOS //rtos
void PendSV_Handler(void)
{
}
#endif


/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{


#ifndef  _USE_HW_RTOS //rtos
  HAL_IncTick();
  //resetISR();
#else
  osSystickHandler();
//  tickTimerISR();
//  swTimerISR();
#endif
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/


