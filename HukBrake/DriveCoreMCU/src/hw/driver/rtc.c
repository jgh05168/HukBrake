/*
 * rtc.c
 *
 *  Created on: Apr 18, 2025
 *      Author: kyn05
 */


#include "rtc.h"



static RTC_HandleTypeDef hrtc;


bool rtcInit(void)
{
	bool ret = true;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();


	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef DateToUpdate = {0};

	/** Initialize RTC Only
	*/
	hrtc.Instance						 = RTC;
	hrtc.Init.AsynchPrediv	 = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut				 = RTC_OUTPUTSOURCE_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initialize RTC and set the Time and Date
	*/
	sTime.Hours = 0x0;
	sTime.Minutes = 0x0;
	sTime.Seconds = 0x0;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
	DateToUpdate.Month = RTC_MONTH_JANUARY;
	DateToUpdate.Date = 0x1;
	DateToUpdate.Year = 0x0;

	if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

	return ret;
}


void rtcBackupRegWrite(uint32_t idx, uint32_t data)
{
	/**
	  * @brief  Writes a data in a specified RTC Backup data register.
	  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
	  *                the configuration information for RTC.
	  * @param  BackupRegister: RTC Backup data Register number.
	  *          This parameter can be: RTC_BKP_DRx where x can be from 1 to 10 (or 42) to
	  *                                 specify the register (depending devices).
	  * @param  Data: Data to be written in the specified RTC Backup data register.
	  * @retval None
	  */
	HAL_RTCEx_BKUPWrite(&hrtc, idx, data);

}

uint32_t rtcBackupRegRead(uint32_t idx)
{
	/**
	  * @brief  Reads data from the specified RTC Backup data Register.
	  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
	  *                the configuration information for RTC.
	  * @param  BackupRegister: RTC Backup data Register number.
	  *          This parameter can be: RTC_BKP_DRx where x can be from 1 to 10 (or 42) to
	  *                                 specify the register (depending devices).
	  * @retval Read value
	  */
	return HAL_RTCEx_BKUPRead(&hrtc, idx);
}





void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}


void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}
