/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STATUS_LED_Pin GPIO_PIN_14
#define STATUS_LED_GPIO_Port GPIOC
#define ch_OUT_3_Pin GPIO_PIN_1
#define ch_OUT_3_GPIO_Port GPIOC
#define SPI1_INT_Pin GPIO_PIN_0
#define SPI1_INT_GPIO_Port GPIOB
#define SPI1_CD_Pin GPIO_PIN_1
#define SPI1_CD_GPIO_Port GPIOB
#define I2C_INT_Pin GPIO_PIN_15
#define I2C_INT_GPIO_Port GPIOB
#define I2C_INT_EXTI_IRQn EXTI4_15_IRQn
#define OUT_7_Pin GPIO_PIN_6
#define OUT_7_GPIO_Port GPIOC
#define OUT_6_Pin GPIO_PIN_7
#define OUT_6_GPIO_Port GPIOC
#define OUT_5_Pin GPIO_PIN_8
#define OUT_5_GPIO_Port GPIOD
#define OUT_4_Pin GPIO_PIN_9
#define OUT_4_GPIO_Port GPIOD
#define OUT_1_Pin GPIO_PIN_15
#define OUT_1_GPIO_Port GPIOA
#define OUT_0_Pin GPIO_PIN_8
#define OUT_0_GPIO_Port GPIOC
#define OUT_2_Pin GPIO_PIN_9
#define OUT_2_GPIO_Port GPIOC
#define LED_RESET_Pin GPIO_PIN_0
#define LED_RESET_GPIO_Port GPIOD
#define LED_STOP_Pin GPIO_PIN_1
#define LED_STOP_GPIO_Port GPIOD
#define LED_START_Pin GPIO_PIN_2
#define LED_START_GPIO_Port GPIOD
#define IN_0_Pin GPIO_PIN_3
#define IN_0_GPIO_Port GPIOD
#define IN_1_Pin GPIO_PIN_4
#define IN_1_GPIO_Port GPIOD
#define IN_2_Pin GPIO_PIN_5
#define IN_2_GPIO_Port GPIOD
#define IN_3_Pin GPIO_PIN_6
#define IN_3_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
