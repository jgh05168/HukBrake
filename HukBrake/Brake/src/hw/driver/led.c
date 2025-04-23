/*
 * led.c
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */


#include "led.h"


typedef struct
{
	GPIO_TypeDef		*GPIOx;
	uint16_t 				pinNum;
	GPIO_PinState		onState;
	GPIO_PinState		offState;
}led_tbl_t;


// LED 킬 떄, 이거랑 CLK_ENABLE만 수정하면 된다.
led_tbl_t led_tbl[LED_MAX_CH] =
		{
				{LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET, GPIO_PIN_SET},
		};


bool ledInit(void)
{
	bool ret = true;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);


  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  for (int i=0;i<LED_MAX_CH;i++)
  	{
  	  GPIO_InitStruct.Pin = led_tbl[i].pinNum;
  	  HAL_GPIO_Init(led_tbl[i].GPIOx, &GPIO_InitStruct);

  		ledOff(i);
  	}

  return ret;
}


// ch : 체널 개수
void ledOn(uint8_t ch)
{
	if (ch > LED_MAX_CH)
		return;

	HAL_GPIO_WritePin(led_tbl[ch].GPIOx, led_tbl[ch].pinNum, led_tbl[ch].offState);
}


void ledOff(uint8_t ch)
{
	if (ch > LED_MAX_CH)
			return;

	HAL_GPIO_WritePin(led_tbl[ch].GPIOx, led_tbl[ch].pinNum, led_tbl[ch].offState);
}


void ledToggle(uint8_t ch)
{
	if (ch > LED_MAX_CH)
			return;

	HAL_GPIO_TogglePin(led_tbl[ch].GPIOx, led_tbl[ch].pinNum);
}
