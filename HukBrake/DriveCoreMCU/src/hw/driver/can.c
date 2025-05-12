/*
 * can.c
 *
 *  Created on: Apr 28, 2025
 *      Author: kyn05
 */


#include "can.h"



/* CAN 관련 변수(필터, 헤더 등) */
CAN_FilterTypeDef			canFilter;
CAN_RxHeaderTypeDef		canRxHeader;
CAN_TxHeaderTypeDef		canTxHeader;
uint8_t 							canRxData[8];
uint8_t 							canTxData[8];
uint32_t 							canTxMailbox;

/* 센서 데이터 */
double 								can_rx_ultra;


CAN_HandleTypeDef hcan;


void canInit(void)
{
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 9;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_5TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = ENABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK)
	{
		Error_Handler();
	}

	/* 센서 데이터 초기화 */
	can_rx_ultra = 100.0;
}


bool setCanFilter(uint32_t filterMaskHigh, uint32_t filterIdHigh, uint32_t filterMaskLow, uint32_t filterIdLow)
{
	bool ret = true;

	canFilter.FilterMaskIdHigh 				= filterMaskHigh 	<< 5;
	canFilter.FilterIdHigh 						= filterIdHigh 		<< 5;
	canFilter.FilterMaskIdLow					= filterMaskLow 	<< 5;
	canFilter.FilterIdLow							= filterIdLow 		<< 5;

	canFilter.FilterMode							= CAN_FILTERMODE_IDMASK;			// 필터를 마스킹 모드로 설정
	canFilter.FilterScale							= CAN_FILTERSCALE_16BIT;			// 필터 스케일을 16bit로
	canFilter.FilterFIFOAssignment		= CAN_FILTER_FIFO0;						// FIFO0 사용
	canFilter.FilterBank 							= 0;
	canFilter.FilterActivation				= ENABLE;


	if (HAL_CAN_ConfigFilter(&hcan, &canFilter) != HAL_OK)
	{
		return false;
	}

	return ret;

}

bool canOpen(void)
{
	if (HAL_CAN_ActivateNotification(&hcan,
																	 CAN_IT_RX_FIFO0_MSG_PENDING |
																	 CAN_IT_BUSOFF									) != HAL_OK)
	{
		return false;
	}
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		return false;
	}

	return true;
}


void canSendDataInt(uint32_t StdId, uint32_t data)
{
	canTxHeader.StdId 		= StdId;
	canTxHeader.RTR 			= CAN_RTR_DATA;
	canTxHeader.IDE				= CAN_ID_STD;				// standard can(CAN2.0A)
	canTxHeader.DLC				= 4;								// DLC : Data Length Code

	memcpy(canTxData, &data, sizeof(uint32_t));

	canTxMailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
	HAL_CAN_AddTxMessage(&hcan, &canTxHeader, canTxData, &canTxMailbox);
}


double canGetUltrasonicData(void)
{
	return can_rx_ultra;
}



void canRxUltrasonicCallback(void)
{
	memcpy(&can_rx_ultra, canRxData, sizeof(double));
}


/* CAN RX0 인터럽트 메서드 -------------------------------------------------- */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) // CAN RX의 FIFO0에 데이터가 수신이 되었을 때 걸리는 인터럽트 함수
{
  if (hcan->Instance == CAN1) // CAN1을 사용한다면
  {
    // FIFO0에 받아진 데이터 (canRxHeadr) 정보를 사용자가 만든 수신공간인 canRxData에 받아오는 (저장하는) HAL_CAN_GetRxMessage 함수 호출
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, &canRxData) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
    	if (canRxHeader.IDE == CAN_ID_STD && canRxHeader.StdId == CAN_STDID_ULTRASONIC)
    	{
      	canRxUltrasonicCallback();
    	}
    }
  }
}


/* CAN_MSP -------------------------------------------------- */

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}
