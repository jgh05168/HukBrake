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
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK)
	{
		Error_Handler();
	}
}


void setCanFilter(uint32_t filterMaskHigh, uint32_t filterIdHigh, uint32_t filterMaskLow, uint32_t filterIdLow)
{
	canFilter.FilterMaskIdHigh 				= filterMaskHigh 	<< 5;
	canFilter.FilterIdHigh 						= filterIdHigh 		<< 5;
	canFilter.FilterMaskIdLow					= filterMaskLow 	<< 5;
	canFilter.FilterIdLow							= filterIdLow 		<< 5;

	canFilter.FilterMode							= CAN_FILTERMODE_IDMASK;			// 필터를 마스킹 모드로 설정
	canFilter.FilterScale							= CAN_FILTERSCALE_16BIT;			// 필터 스케일을 16bit로
	canFilter.FilterFIFOAssignment		= CAN_FILTER_FIFO0;						// FIFO0 사용
	canFilter.FilterBank 							= 0;
	canFilter.FilterActivation				= ENABLE;

	HAL_CAN_ConfigFilter(&hcan, &canFilter);

}

void canOpen(void)
{
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan);
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
