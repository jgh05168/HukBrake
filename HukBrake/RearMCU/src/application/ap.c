/*
 * ap.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#include "ap.h"



void apInit(void)
{
	/* MCU 통신 세팅 */
	uartOpen(_DEF_UART1, 115200);
	i2cOpen(_DEF_I2C1, 100000);
	/* CAN 세팅 */
	setCanFilter(0x7F0, 0x10C, 0x7F0, 0x10C);
	canOpen();

}


// main.c : 모든 상위 작업들을 여기서 할 수 있도록
void apMain(void)
{
	double t;
	while (1)
	{
		t = getSonicDist();
		sendCanData(0x100, t);
//		logPrintf("dist = %.2f\r\n", t);
		delay(30);

	}

}
