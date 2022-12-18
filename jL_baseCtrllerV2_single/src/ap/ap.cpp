/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.h"

uint32_t loop_ms = 0;

void apInit(void)
{

	i2cBegin(_DEF_I2C1, I2C_FREQ_400KHz);

	uartOpen(_DEF_UART2, 115200);
	cliOpen(_DEF_UART3, 115200);

}


void apMain(void)
{
	uint32_t pre_ms = millis();
	uint32_t check_ms = millis();
	while (1)
	{

		loop_ms = millis() - check_ms;
		check_ms = millis();
		if (millis() - pre_ms >=500)
		{
			pre_ms = millis();
			ledToggle(_DEF_LED1);

			//logPrintf("uart1 Test\r\n");
			//uartPrintf(_DEF_UART2, "uart2 Test\r\n");


		}


		cliMain();

	}

}




