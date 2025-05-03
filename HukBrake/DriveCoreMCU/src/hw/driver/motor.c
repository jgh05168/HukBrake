/*
 * motor.c
 *
 *  Created on: Apr 25, 2025
 *      Author: kyn05
 */


#include "motor.h"


static bool 			is_open[MOTOR_MAX_CH];



TIM_HandleTypeDef htim2;

static TIM_ClockConfigTypeDef sClockSourceConfig = {0};
static TIM_MasterConfigTypeDef sMasterConfig = {0};
static TIM_OC_InitTypeDef sConfigOC = {0};


bool motorInit(uint32_t prescaler, uint32_t period)
{
	// tim 채널 초기화
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = prescaler;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = period;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		return false;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		return false;
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		return false;
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;


	for (int i=0;i<MOTOR_MAX_CH;i++)
		{
			is_open[i] = false;
		}

	// motor direct control
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*Configure GPIO pin : PB0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*Configure GPIO pin : PC1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	return true;
}


bool motorOpen(uint8_t ch)
{
	bool ret = true;

	switch(ch)
	{
		case _DEF_MOTOR1:
			if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
			{
				return false;
			}
			HAL_TIM_MspPostInit(&htim2);
      if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2) != HAL_OK)
			{
				return false;
			}

			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

			is_open[ch] = true;
			break;
		case _DEF_MOTOR2:
			if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
			{
				return false;
			}
			if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK)
			{
				return false;
			}

			HAL_TIM_MspPostInit(&htim2);

			is_open[ch] = true;
			break;
		case _DEF_MOTOR3:
			is_open[ch] = true;
			ret = true;
			break;
		case _DEF_MOTOR4:
			is_open[ch] = true;
			ret = true;
			break;
	}

	return ret;
}


bool motorClose(uint8_t ch)
{
	switch (ch)
	{
		case _DEF_MOTOR1:
			if (HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2) != HAL_OK)
			{
				return false;
			}
			is_open[ch] = false;
			break;
		case _DEF_MOTOR2:
			if (HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_3) != HAL_OK)
			{
				return false;
			}
			is_open[ch] = false;
			break;
	}

	return true;
}


bool motorAvailable(uint8_t ch)
{
	return is_open[ch];
}


void motorSetSpeed(uint8_t ch, uint8_t speed)
{
	if (is_open[ch] == false)
	{
		return;
	}
	if (speed > 100)
	{
		speed = 100;
	}

	uint32_t compare = (speed * (__HAL_TIM_GET_AUTORELOAD(&htim2) + 1)) / 100;

	switch(ch)
	  {
	    case _DEF_MOTOR1:
	      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, compare);
	      break;
	    case _DEF_MOTOR2:
	    	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, compare);
				break;
			case _DEF_MOTOR3:
				break;
			case _DEF_MOTOR4:
				break;
	  }
}


uint32_t motorGetSpeed(uint8_t ch)
{
  uint32_t speed = 0;

  if (is_open[ch] == false)
  {
  	return speed;
  }

	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);      						// ARR
	uint32_t ccr2, ccr3;

  switch (ch)
  {
    case _DEF_MOTOR1:
    	ccr2 = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2);  // CCR2
			speed = (ccr2 * 100) / (arr + 1);
			break;
    case _DEF_MOTOR2:
			ccr3 = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_3);  // CCR3
			speed = (ccr3 * 100) / (arr + 1);
			break;
		case _DEF_MOTOR3:
			break;
		case _DEF_MOTOR4:
			break;
  }

  return speed;
}


void motorDir(uint8_t dir)
{
	if (dir == FORWARD)
	{
		delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	}
	else
	{
		delay(100);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	}
}


void motorStop(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}


void motorEmergencyState(uint8_t ch)
{
	switch (ch)
	{
		case _DEF_MOTOR1:
			// 1. 모터 PWM 차단
			motorClose(ch);
			// 2. 릴레이 신호 HIGH
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

			break;
	}
}





/* HAL_TIM_INIT ---------------------------------------------- */

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM2)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA1     ------> TIM2_CH2
    PA2     ------> TIM2_CH3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  }
}
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  }
}
