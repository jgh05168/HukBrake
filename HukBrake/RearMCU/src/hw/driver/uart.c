/*
 * uart.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */



#include "uart.h"
#include "qbuffer.h"

// 하나의 .c에서 사용되는 변수는 static 사용하기.
static bool 				is_open[UART_MAX_CH];
static qbuffer_t 		qbuffer[UART_MAX_CH];		// 수신용 버퍼
static uint8_t		 	rx_buf[256];						// 원하는 사이즈로 조절
//static uint8_t 			rx_data[UART_MAX_CH];		// 수신받는 데이터(인터럽트 시 사용)



UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;



bool uartInit(void)
{
	for (int i=0;i<UART_MAX_CH;i++)
		{
			is_open[i] = false;
		}

	return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{
	bool ret = false;


	switch(ch)
	{
		case _DEF_UART1:
			// uart 채널 초기화
			huart2.Instance = USART2;
			huart2.Init.BaudRate = 115200;
			huart2.Init.WordLength = UART_WORDLENGTH_8B;
			huart2.Init.StopBits = UART_STOPBITS_1;
			huart2.Init.Parity = UART_PARITY_NONE;
			huart2.Init.Mode = UART_MODE_TX_RX;
			huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			huart2.Init.OverSampling = UART_OVERSAMPLING_16;

			HAL_UART_DeInit(&huart2);			// 재연결 시, 오작동 방지를 위해 껐다 켜주기 위함

			// 수신용 버퍼 생성
			qbufferCreate(&qbuffer[ch], rx_buf, 256);

			// dma 초기화
		  /* DMA controller clock enable */
		  __HAL_RCC_DMA1_CLK_ENABLE();
		  /* DMA interrupt init */
		  /* DMA1_Channel5_IRQn interrupt configuration */
		  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
		  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);



			if (HAL_UART_Init(&huart2) == HAL_OK)
			{
				ret = true;
				is_open[ch] = true;

				// 데이터 수신(DMA 방식, 한 바이트 수신)
				if (HAL_UART_Receive_DMA(&huart2, (uint8_t *)&rx_buf[0], 256) != HAL_OK)		// rx_buf에 자동으로 데이터를 전송해줌(자세한건 reference manual 참고)
				{
					ret = false;
				}
				// DMA 사용 시, Flush 기능 추가
				qbuffer[ch].in = qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR;
				qbuffer[ch].out = qbuffer[ch].in;


//				// 데이터 수신(인터럽트 방식, 한 바이트 수신)
//				if (HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_data[_DEF_UART1], 1) != HAL_OK)
//				{
//					ret = false;
//				}


			}
			break;
		case _DEF_UART2:
			is_open[ch] = true;
			ret = true;
			break;
		case _DEF_UART3:
			is_open[ch] = true;
			ret = true;
			break;
		case _DEF_UART4:
			is_open[ch] = true;
			ret = true;
			break;
	}

	return ret;
}


uint32_t uartAvailable(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
		{
			case _DEF_UART1:
				qbuffer[ch].in = (qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR);		// DMA : 초기 카운트는 256으로 세팅. 이후, 감소하는 식임
				ret = qbufferAvailable(&qbuffer[ch]);
				break;
			case _DEF_UART2:
				break;
			case _DEF_UART3:
				break;
			case _DEF_UART4:
				break;
		}

	return ret;
}


uint8_t uartRead(uint8_t ch)
{
	uint8_t ret = 0;

	switch(ch)
	{
		case _DEF_UART1:
				qbufferRead(&qbuffer[_DEF_UART1], &ret, 1);
			break;
		case _DEF_UART2:
			break;
		case _DEF_UART3:
			break;
		case _DEF_UART4:
			break;
	}

	return ret;
}


uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint16_t length)
{
	uint32_t ret = 0;
	HAL_StatusTypeDef status;
	switch(ch)
		{
			case _DEF_UART1:
				status = HAL_UART_Transmit(&huart2, p_data, length, 100);
				if (status == HAL_OK)
				{
					ret = length;
				}
				break;
			case _DEF_UART2:
				break;
			case _DEF_UART3:
				break;
			case _DEF_UART4:
				break;
		}


	return ret;
}


uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  uint16_t len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = uartWrite(ch, (uint8_t *)buf, len);

  va_end(args);


  return ret;
}


uint32_t uartGetBaud(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
			{
				case _DEF_UART1:
						ret = huart2.Init.BaudRate;
					break;
				case _DEF_UART2:
					break;
				case _DEF_UART3:
					break;
				case _DEF_UART4:
					break;
			}

	return ret;
}






/* INTERRUPT CODE BEGIN */

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
// 인터럽트 수신 콜백 : 간단하게 저속으로 사용하기는 괜찮지만, 고속 통신 시 DMA 방식으로 사용
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#if 0			// 인터럽트 사용 시, 해제
  if (huart->Instance == USART1)
  {
  	// 수신된 버퍼 저장
  	qbufferWrite(&qbuffer[_DEF_UART1], &rx_data[_DEF_UART1], 1);

  	// 다시 Receive 호출해야댐
  	HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_data[_DEF_UART1], 1);
  }
#endif
}

// 인터럽트 에러 콜백
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
  	uint32_t err = HAL_UART_GetError(huart);
		printf("UART Error Code: 0x%lX\n", err);

		if (err & HAL_UART_ERROR_PE)  printf("Parity Error\n");
		if (err & HAL_UART_ERROR_NE)  printf("Noise Error\n");
		if (err & HAL_UART_ERROR_FE)  printf("Framing Error\n");
		if (err & HAL_UART_ERROR_ORE) printf("Overrun Error\n");
		if (err & HAL_UART_ERROR_DMA) printf("DMA Transfer Error\n");
  }

}

/* INTERRUPT CODE END */






/* UART Msp BEGIN */

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}
