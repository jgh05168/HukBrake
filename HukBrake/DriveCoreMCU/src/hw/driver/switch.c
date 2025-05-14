/*
 * switch.c
 *
 *  Created on: May 13, 2025
 *      Author: kyn05
 */

#include "switch.h"


bool switchInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


bool switchState(void)
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_SET)
	{
		return true;
	}
	else
	{
		return false;
	}
}
