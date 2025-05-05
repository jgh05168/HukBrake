/*
 * i2c.c
 *
 *  Created on: May 5, 2025
 *      Author: kyn05
 */


#include "i2c.h"


static bool 				is_open[I2C_MAX_CH];

I2C_HandleTypeDef 	hi2c1;



bool i2cInit(void)
{
	for (int i=0;i<I2C_MAX_CH;i++)
		{
			is_open[i] = false;
		}

	return true;
}


bool i2cOpen(uint8_t ch, uint32_t speed)
{
	bool ret = false;


	switch(ch)
	{
		case _DEF_I2C1:
			hi2c1.Instance = I2C1;
			hi2c1.Init.ClockSpeed = speed;						// ex) 100000
			hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
			hi2c1.Init.OwnAddress1 = 0;
			hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
			hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
			hi2c1.Init.OwnAddress2 = 0;
			hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
			hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
			if (HAL_I2C_Init(&hi2c1) != HAL_OK)
			{
				Error_Handler();
			}

			is_open[ch] = true;
			ret = true;
			break;
		case _DEF_I2C2:
			is_open[ch] = true;
			ret = true;
			break;
	}

	return ret;
}








/* I2C Msp ---------------------------------------------- */

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}
