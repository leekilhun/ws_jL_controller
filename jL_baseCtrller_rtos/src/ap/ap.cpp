/*
 * ap.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "ap.h"


static void threadCli(void const *argument);
static void threadCmdLcd(void const *argument);
static void threadCmdReceiveNonBlock(void const *argument);
static void threadEvent(void const *argument);
static void threadCmdPc(void const *argument);
static void threadCmdMotor(void const *argument);
static void threadPcReceiveNonBlock(void const *argument);

void apInit(void)
{

	/* rtos initial*/
	{
		osThreadDef(threadCli, threadCli, _HW_DEF_RTOS_THREAD_PRI_CLI, 0, _HW_DEF_RTOS_THREAD_PRI_CLI);
		if (osThreadCreate(osThread(threadCli), NULL) != NULL)
		{
			logPrintf("threadCli \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadCli \t\t: Fail\r\n");
		}
		/**/
		osThreadDef(threadCmdLcd, threadCmdLcd, _HW_DEF_RTOS_THREAD_PRI_LCD_CMD, 0, _HW_DEF_RTOS_THREAD_MEM_LCD_CMD);
		if (osThreadCreate(osThread(threadCmdLcd), NULL) != NULL)
		{
			logPrintf("threadCmdLcd \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadCmdLcd \t\t: Fail\r\n");
		}
		/**/
		osThreadDef(threadCmdReceiveNonBlock, threadCmdReceiveNonBlock, _HW_DEF_RTOS_THREAD_PRI_FM_RECEIVE, 0, _HW_DEF_RTOS_THREAD_MEM_FM_RECEIVE);
		if (osThreadCreate(osThread(threadCmdReceiveNonBlock), NULL) != NULL)
		{
			logPrintf("threadCmdReceiveNonBlock \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadCmdReceiveNonBlock \t\t: Fail\r\n");
		}
		/**/
		osThreadDef(threadCmdPc, threadCmdPc, _HW_DEF_RTOS_THREAD_PRI_PC_CMD, 0, _HW_DEF_RTOS_THREAD_MEM_PC_CMD);
		if (osThreadCreate(osThread(threadCmdPc), NULL) != NULL)
		{
			logPrintf("threadCmdPc \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadCmdPc \t\t: Fail\r\n");
		}

		/**/
		osThreadDef(threadEvent, threadEvent, _HW_DEF_RTOS_THREAD_PRI_EVENT, 0, _HW_DEF_RTOS_THREAD_MEM_EVENT);
		if (osThreadCreate(osThread(threadEvent), NULL) != NULL)
		{
			logPrintf("threadEvent \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadEvent \t\t: Fail\r\n");
		}
		/**/
		osThreadDef(threadCmdMotor, threadCmdMotor, _HW_DEF_RTOS_THREAD_PRI_MOTOR_CMD, 0, _HW_DEF_RTOS_THREAD_MEM_MOTOR_CMD);
		if (osThreadCreate(osThread(threadCmdMotor), NULL) != NULL)
		{
			logPrintf("threadCmdMotor \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadCmdMotor \t\t: Fail\r\n");
		}
		/**/
		osThreadDef(threadPcReceiveNonBlock, threadPcReceiveNonBlock, _HW_DEF_RTOS_THREAD_PRI_PC_RECEIVE, 0, _HW_DEF_RTOS_THREAD_MEM_PC_RECEIVE);
		if (osThreadCreate(osThread(threadPcReceiveNonBlock), NULL) != NULL)
		{
			logPrintf("threadPcReceiveNonBlock \t\t: OK\r\n");
		}
		else
		{
			logPrintf("threadPcReceiveNonBlock \t\t: Fail\r\n");
		}

	}


}


void apMain(void)
{
	uint32_t pre_ms = millis();

	while (1)
	{
		if (millis() - pre_ms >=500)
		{
			pre_ms = millis();
			ledToggle(_DEF_LED1);
		}
		delay(2);
	}

}

/*######################################################
  cli
  ######################################################*/
void threadCli(void const *argument)
{

	 cliOpen(_DEF_UART2, 115200);

	UNUSED(argument);
	while (1)
	{

		cliMain();
		delay(2);
	}
}


/*######################################################
  Event Task  -> cnProcess step
  ######################################################*/
void threadEvent(void const *argument)
{
	UNUSED(argument);
	while (1)
	{

		delay(2);
	}
}


/*######################################################
  remote control pc
  ######################################################*/
void threadCmdPc(void const *argument)
{
	UNUSED(argument);
	while (1)
	{
		delay(2);
	}
}

/*######################################################
   receive PC Remote Control Communication Task
  ######################################################*/
void threadPcReceiveNonBlock(void const *argument)
{
	UNUSED(argument);

	while (1)
	{
		delay(2);
	}
}



/*######################################################
   Nextion Lcd Communication
  ######################################################*/
void threadCmdLcd(void const *argument)
{

	while (1)
	{

		delay(2);
	}
}


/*######################################################
  block api
  ######################################################*/
void threadCmdMotor(void const *argument)
{
	UNUSED(argument);

	while (1)
	{
		delay(2);
	}
}




/*######################################################
   receive Communication Task
  ######################################################*/
void threadCmdReceiveNonBlock(void const *argument)
{
	UNUSED(argument);

	while (1)
	{
		delay(2);
	}
}





