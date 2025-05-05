/*
 * ap.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#include "ap.h"



static void threadLcd(void const *argument);


void apInit(void)
{
	/* MCU 통신 세팅 */
	uartOpen(_DEF_UART1, 115200);
	i2cOpen(_DEF_I2C1, 100000);
	/* CAN 세팅 */
	setCanFilter(0x7F0, 0x10C, 0x7F0, 0x10C);
	canOpen();


	osThreadDef(threadLcd, threadLcd, _HW_DEF_RTOS_THREAD_PRI_LCD, 0, _HW_DEF_RTOS_THREAD_MEM_LCD);
	if (osThreadCreate(osThread(threadLcd), NULL) != NULL)
	{
		logPrintf("threadLcd \t\t: OK\r\n");
	}
	else
	{
		logPrintf("threadLcd \t\t: Fail\r\n");
		while(1);
	}
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


// 스레드 콜백 함수(LCD)
static void threadLcd(void const *argument)
{
  UNUSED(argument);

  /* LCD Init */
  i2cScan();
  /*
   * LCD는 I2C가 사용가능할 때에만 동작할 수 있기 때문에,
   * hw.c에서 init해주지 않고, thread에서 i2c scan 이후에 init해준다.
   */
	lcdInit(LCD_ADDR);

  while (1)
  {
  	lcdSendCommand(LCD_ADDR, 0b10000000);
    lcdSendString(LCD_ADDR, "Using 1602 LCD");

  }
}
