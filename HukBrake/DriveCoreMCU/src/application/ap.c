/*
 * ap.c
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */


#include "ap.h"
#include "task.h"


osMutexId uartMutexHandle;


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

	osMutexDef(myMutex01);
	uartMutexHandle = osMutexCreate(osMutex(myMutex01));
}



// main.c : 모든 상위 작업들을 여기서 할 수 있도록
void apMain(void)
{
	ledOn(_DEF_LED1);
	uint32_t tick = mills();
	uint32_t t;
	while (1)
	{
		osMutexWait(uartMutexHandle, osWaitForever);
		t = mills();

		logPrintf("LED: %lu\r\n", t);
		osMutexRelease(uartMutexHandle);  // 뮤텍스 해제

		tick += 500;  // 다음 wake-up 시점
		uint32_t Delay = (tick >mills()) ? (tick - mills()) : 0;
		if (Delay > 0) {
			delay(Delay); // 정확한 delay 시간을 설정
		}
	}

}


// 스레드 콜백 함수(MOTOR)
static void threadMotor(void const *argument)
{
  UNUSED(argument);

	setMotorSpeed(_DEF_MOTOR1, 100);
	uint32_t tick = mills();
	uint32_t t;
  while(1)
  {
  	osMutexWait(uartMutexHandle, osWaitForever);
		t = mills();
		logPrintf("SPEED: %lu\r\n", mills());
		osMutexRelease(uartMutexHandle);  // 뮤텍스 해제

		tick += 500;  // 다음 wake-up 시점
		uint32_t Delay = (tick >mills()) ? (tick - mills()) : 0;
		if (Delay > 0) {
			delay(Delay); // 정확한 delay 시간을 설정
		}
  }
}
