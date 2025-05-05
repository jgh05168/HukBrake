/*
 * lcd.c
 *
 *  Created on: May 5, 2025
 *      Author: kyn05
 */


#include "lcd.h"



extern I2C_HandleTypeDef 		hi2c1;


void lcdInit(uint8_t lcd_addr)
{
	// Step 1~3: 3번 0x30 전송 (8-bit 명령, upper nibble만)
	lcdSendInternal(lcd_addr, 0x30, 0);
	delay(HAL_MAX_DELAY);
	lcdSendInternal(lcd_addr, 0x30, 0);
	delay(HAL_MAX_DELAY);
	lcdSendInternal(lcd_addr, 0x30, 0);
	delay(HAL_MAX_DELAY);

	// Step 4: 0x20 전송 → 4-bit mode 진입
	lcdSendInternal(lcd_addr, 0x20, 0);
	delay(HAL_MAX_DELAY);

	// 이후는 정식 명령어로 4-bit 모드에서 전송
	lcdSendCommand(lcd_addr, 0x28); // 4-bit, 2 line, 5x8 font
	lcdSendCommand(lcd_addr, 0x08); // Display OFF
	lcdSendCommand(lcd_addr, 0x01); // Clear display
	delay(HAL_MAX_DELAY);                      // Clear 명령은 딜레이 필요
	lcdSendCommand(lcd_addr, 0x06); // Entry mode: Increment, No shift
	lcdSendCommand(lcd_addr, 0x0C); // Display ON, Cursor OFF, Blink OFF
}

bool i2cScan(void)
{
	for(uint16_t i = 0; i < 128; i++)
	{
		if (HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10) != HAL_OK)
		{
			return false;
		}

	}
	return true;
}

HAL_StatusTypeDef lcdSendInternal(uint8_t lcd_addr, uint8_t data, uint8_t flags)
{
	HAL_StatusTypeDef res;
	for(;;)
	{
		res = HAL_I2C_IsDeviceReady(&hi2c1, lcd_addr, 1, HAL_MAX_DELAY);
		if(res == HAL_OK)
			break;
	}

	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
	data_arr[1] = up|flags|BACKLIGHT;
	data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
	data_arr[3] = lo|flags|BACKLIGHT;

	res = HAL_I2C_Master_Transmit(&hi2c1, lcd_addr, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
	delay(LCD_DELAY_MS);
	return res;
}

void lcdSendCommand(uint8_t lcd_addr, uint8_t cmd)
{
	lcdSendInternal(lcd_addr, cmd, 0);
}

void lcdSendData(uint8_t lcd_addr, uint8_t data)
{
	lcdSendInternal(lcd_addr, data, PIN_RS);
}

void lcdSendString(uint8_t lcd_addr, char *str)
{
	while(*str)
	{
		lcdSendData(lcd_addr, (uint8_t)(*str));
		str++;
	}
}
