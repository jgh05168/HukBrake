/*
 * ap.c
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */


#include "ap.h"



/* CAN variables */
volatile    uint8_t                 can_rx_flag = 0;

extern      CAN_FilterTypeDef       canFilter;
extern      CAN_RxHeaderTypeDef     canRxHeader;
extern      CAN_TxHeaderTypeDef     canTxHeader;
extern      uint8_t                 canRxData[8];
extern      uint8_t                 canTxData[8];
extern      uint32_t                canTxMailbox;

extern 			CAN_HandleTypeDef 			hcan;


/* RTOS variables */
osMutexId uartMutexHandle;


static void threadMotor(void const *argument);


void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
	motorOpen(_DEF_MOTOR1);
	/* CAN 세팅 */
	setCanFilter(0x7F3, 0x10C, 0x7F3, 0x10C);				// 예시로 잡아둠
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

	setMotorSpeed(_DEF_MOTOR1, 100);
//	uint32_t tick = mills();
//	uint32_t t;
  while(1)
  {
//  	osMutexWait(uartMutexHandle, osWaitForever);
//		t = mills();
//		logPrintf("SPEED: %lu\r\n", mills());
//		osMutexRelease(uartMutexHandle);  // 뮤텍스 해제
//
//		tick += 500;  // 다음 wake-up 시점
//		uint32_t Delay = (tick >mills()) ? (tick - mills()) : 0;
//		if (Delay > 0) {
//			delay(Delay); // 정확한 delay 시간을 설정
//		}
  	if (can_rx_flag == 1)
  	{
  		osMutexWait(uartMutexHandle, osWaitForever);
  		double receive_data;
  		memcpy(&receive_data, canRxData, sizeof(double));
  		logPrintf("Rx Data : %.2f\r\n", receive_data);
  		osMutexRelease(uartMutexHandle);

  		delay(30);
  		can_rx_flag = 0;
  	}
  }
}



/* CAN RX0 인터럽트 메서드 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) // CAN RX의 FIFO0에 데이터가 수신이 되었을 때 걸리는 인터럽트 함수
{
  if (hcan->Instance == CAN1) // CAN1을 사용한다면
  {
    // FIFO0에 받아진 데이터 (canRxHeadr) 정보를 사용자가 만든 수신공간인 canRxData에 받아오는 (저장하는) HAL_CAN_GetRxMessage 함수 호출
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, &canRxData[0]) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
    	can_rx_flag = 1; // can 수신 인터럽트 플래그 발생.
    }
  }
}
