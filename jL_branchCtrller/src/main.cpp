/*
 * main.cpp
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */




#include "main.h"


static void mainThread(void const *argument);

int main(void)
{
	/* board service packet initialize*/
  bspInit();

  /* rtos kernal start */
  osThreadDef(mainThread, mainThread, _HW_DEF_RTOS_THREAD_PRI_MAIN, 0, _HW_DEF_RTOS_THREAD_MEM_MAIN);
  if (osThreadCreate(osThread(mainThread), NULL) == NULL)
  {

  	/* if create rtos fails */
    ledInit();

    while(1)
    {
      delay(100);
    }
  }

  osKernelStart();

  return 0;
}


void mainThread(void const *argument)
{
  UNUSED(argument);

  hwInit();
  exhwInit();
  apInit();
  apMain();

}
