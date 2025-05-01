/*
 * ap.c
 *
 *  Created on: Apr 24, 2025
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



void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
	/* CAN 세팅 */
	setCanFilter(0x7FF, 0x10C, 0x7FF, 0x10C);
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
