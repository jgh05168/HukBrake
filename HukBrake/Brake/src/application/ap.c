/*
 * ap.c
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */


#include "ap.h"



static void threadLed(void const *argument);


void apInit(void)
{
	uartOpen(_DEF_UART1, 57600);

	osThreadDef(threadLed, threadLed, _HW_DEF_RTOS_THREAD_PRI_LED, 0, _HW_DEF_RTOS_THREAD_MEM_LED);
	if (osThreadCreate(osThread(threadLed), NULL) != NULL)
	{
		logPrintf("threadLed \t\t: OK\r\n");
	}
	else
	{
		logPrintf("threadLed \t\t: Fail\r\n");
		while(1);
	}
}



// main.c : 모든 상위 작업들을 여기서 할 수 있도록
void apMain(void)
{

	while (1)
	{
		logPrintf("Hello RTOS : %lu\r\n", mills());
		delay(500);
	}

}


// 스레드 콜백 함수(LED)
static void threadLed(void const *argument)
{
  UNUSED(argument);


  while(1)
  {
    ledToggle(_DEF_LED1);
    delay(500);
  }
}
