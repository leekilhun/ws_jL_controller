/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */



#include "ap.h"
void apInit(void)
{

	i2cBegin(_DEF_I2C1, I2C_FREQ_400KHz);

	uartOpen(_DEF_UART2, 115200);
	cliOpen(_DEF_UART3, 115200);

}

void apMain(void)
{
  uint32_t pre_ms = millis();

	while (1)
	{
		if (millis() - pre_ms >=100)
		{

			pre_ms = millis();
			ledToggle(_DEF_LED1);

		}

		cliMain();
	}

}
