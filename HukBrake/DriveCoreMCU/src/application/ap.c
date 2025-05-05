/*
 * ap.c
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */


#include "ap.h"



/* RTOS variables */
osMutexId uartMutexHandle;
/*
 * Mutex 잠금 설정 : osMutexWait(uartMutexHandle, osWaitForever);
 * Mutex 잠금 해제 : osMutexRelease(uartMutexHandle);
 */


static void threadMotor(void const *argument);


void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
	motorOpen(_DEF_MOTOR1);
	/* CAN 세팅 */
	setCanFilter(0x7F0, 0x10C, 0x7F0, 0x10C);				// 예시로 잡아둠
	canOpen();


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

	// mutex 설정
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
		ledToggle(_DEF_LED1);
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

	motorSetSpeed(_DEF_MOTOR1, 100);
	motorDir(FORWARD);

  while(1)
  {
		double rcv_ultrasonic_data = canGetUltrasonicData();

		// 거리가 10cm보다 가까워지면, 긴급정지
		if (rcv_ultrasonic_data < 10)
		{
			// 모터 제어 thread priority 최고로 높이기
			osThreadSetPriority(osThreadGetId(), osPriorityRealtime);
			// 모터 정지 !!!
			motorEmergencyState(_DEF_MOTOR1);
		}
		else
		{
			if (osThreadGetPriority(osThreadGetId()) != osPriorityNormal)
			{
				osThreadSetPriority(osThreadGetId(), osPriorityNormal);
			}
			if (!motorAvailable(_DEF_MOTOR1))
			{
				motorOpen(_DEF_MOTOR1);
			}
			motorSetSpeed(_DEF_MOTOR1, 100);
			motorDir(FORWARD);

			// 전후진 모터 속도 제어
			osMutexWait(uartMutexHandle, osWaitForever);
			logPrintf("Rx Data : %.2f\r\n", rcv_ultrasonic_data);
			osMutexRelease(uartMutexHandle);  // 뮤텍스 해제
			delay(30);

		}

  }
}

