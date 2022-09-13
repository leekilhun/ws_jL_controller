/*
 * bsp.c
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 *
 *  for stm32g070 bootloader fw
 *  to reduce the file size, the function is simplified.
 *  logPrintf  -> (system callback function)print
 *
 *  LL_RCC_LSI_Enable - to use reset count function.
 */





#include "bsp.h"

void SystemClock_Config(void);



#define FLASH_USER_START_ADDR   (FLASH_BASE + (62 * FLASH_PAGE_SIZE))   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (FLASH_BASE + FLASH_SIZE - 1)   /* End @ of user Flash area */

#define DATA_64                 ((uint64_t)0x1234567812345678)
#define DATA_32                 ((uint32_t)0x12345678)

uint32_t FirstPage = 0, NbOfPages = 0;
uint32_t Address = 0, PageError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct = {0};

static uint32_t GetPage(uint32_t Address);

bool bspInit(void)
{
	HAL_Init();

	SystemClock_Config();


#if 1

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/


	/* Get the 1st page to erase */
	FirstPage = GetPage(FLASH_USER_START_ADDR);

	/* Get the number of pages to erase from 1st page */
	NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page        = FirstPage;
	EraseInitStruct.NbPages     = NbOfPages;

	/* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		/*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PageError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
		 */
		/* Infinite loop */
		while (1)
		{
			/* indicate error in Erase operation */
			HAL_Delay(100);
		}
	}


	/* Program the user Flash area word by word
	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	Address = FLASH_USER_START_ADDR;
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, DATA_64) == HAL_OK)
	{
		Address = Address + 8;
	}
	else
	{

		//to indicate error in Write operation
		HAL_Delay(100);
	}

	/*while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, DATA_64) == HAL_OK)
		{
			Address = Address + 8;
		}
		else
		{
			 Error occurred while writing data in Flash memory.
	         User can add here some code to deal with this error
			while (1)
			{
				  to indicate error in Write operation
				HAL_Delay(100);
			}
		}
	}*/

	/* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();

#if 0
	/* Check if the programmed data is OK
	      MemoryProgramStatus = 0: data programmed correctly
	      MemoryProgramStatus != 0: number of words not programmed correctly ******/
	Address = FLASH_USER_START_ADDR;
	MemoryProgramStatus = 0x0;

	while (Address < FLASH_USER_END_ADDR)
	{
		data32 = *(__IO uint32_t *)Address;

		if (data32 != DATA_32)
		{
			MemoryProgramStatus++;
		}
		Address = Address + 4;
	}

	/*Check if there is an issue to program data*/
	if (MemoryProgramStatus == 0)
	{
		/* No error detected. Switch on LED4*/

	}
	else
	{
		/* Error detected. LED4 will blink with 1s period */
		while (1)
		{
			HAL_Delay(1000);
		}
	}


#endif





#endif



	return true;
}

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}


void bspDeInit(void)
{
  HAL_RCC_DeInit();

  // arm M0
  // Disable Interrupts
  NVIC->ICER[0] = 0xFFFFFFFF;
  __DSB();
  __ISB();

  SysTick->CTRL = 0;
}

void SystemClock_Config(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
	while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
	{
	}

	/* HSI configuration and activation */
	LL_RCC_HSI_Enable();
	while(LL_RCC_HSI_IsReady() != 1)
	{
	}

	/* LSI configuration and activation */
	LL_RCC_LSI_Enable();
	while(LL_RCC_LSI_IsReady() != 1)
	{
	}

	LL_PWR_EnableBkUpAccess();
	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
	LL_RCC_PLL_Enable();
	LL_RCC_PLL_EnableDomain_SYS();
	while(LL_RCC_PLL_IsReady() != 1)
	{
	}

	/* Set AHB prescaler*/
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	/* Sysclk activation on the main PLL */
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
	}

	/* Set APB1 prescaler*/
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	LL_SetSystemCoreClock(64000000);

	/* Update the time base */
	if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
	{
		Error_Handler();
	}


}

void Error_Handler(void)
{
  while(1)
  {
  }

}




/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}
