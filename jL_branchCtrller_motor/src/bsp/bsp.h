/*
 * bsp.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

#include "stm32g0xx_hal.h"

#ifdef _USE_HW_RTOS
#include "rtos.h"
#endif

#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"


	bool bspInit(void);
	void bspDeInit(void);
	void delay(uint32_t ms);
	uint32_t millis(void);
	void Error_Handler(void);
	void logPrintf(const char *fmt, ...);


#if 0
#define LOG_ERROR_(fmt, ...) \
		logPrintf("%s:%d:error: " fmt "%s\n", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LOG_ERROR_(__VA_ARGS__, "")
#else
#define LOG_ERROR(...)
#endif


#ifdef __cplusplus
}
#endif


#endif /* SRC_BSP_BSP_H_ */
