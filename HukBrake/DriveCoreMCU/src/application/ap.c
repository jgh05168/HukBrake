/*
 * ap.c
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */


#include "ap.h"
#include "task.h"



static void threadMotor(void const *argument);


void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
	motorOpen(_DEF_MOTOR1);

	osThreadDef(threadMotor, threadMotor, _HW_DEF_RTOS_THREAD_PRI_MOTOR, 0, _HW_DEF_RTOS_THREAD_MEM_MOTOR);
	if (osThreadCreate(osThread(threadMotor), NULL) != NULL)
	{
		logPrintf("threadMotor \t\t: OK\r\n");
	}
	else
	{
		logPrintf("threadMotor \t\t: Fail\r\n");
		while(1);
	}
}



// main.c : 모든 상위 작업들을 여기서 할 수 있도록
void apMain(void)
{
	ledOn(_DEF_LED1);

	while (1)
	{
		logPrintf("LED: %lu\r\n", mills());
		delay(500);
	}

}


// 스레드 콜백 함수(MOTOR)
static void threadMotor(void const *argument)
{
  UNUSED(argument);

	setMotorSpeed(_DEF_MOTOR1, 100);
  while(1)
  {
//    logPrintf("Speed : %lu\r\n", getMoterSpeed(_DEF_MOTOR1));
		logPrintf("SPEED: %lu\r\n", mills());
		delay(100);
  }
}
