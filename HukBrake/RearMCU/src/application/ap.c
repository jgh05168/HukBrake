/*
 * ap.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#include "ap.h"



void apInit(void)
{
	uartOpen(_DEF_UART1, 57600);
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
