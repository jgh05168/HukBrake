/*
 * lcd.h
 *
 *  Created on: May 5, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_LCD_C_
#define SRC_COMMON_HW_INCLUDE_LCD_C_

#include "hw_def.h"


#ifdef _USE_HW_LCD

#define LCD_MAX_CH					HW_LCD_MAX_CH

#define LCD_ADDR 						(0x27 << 1)
#define PIN_RS    					(1 << 0)
#define PIN_EN    					(1 << 2)
#define BACKLIGHT 					(1 << 3)
#define LCD_DELAY_MS				5


extern I2C_HandleTypeDef 		hi2c1;


void 										lcdInit(uint8_t lcd_addr);
bool 										i2cScan(void);
HAL_StatusTypeDef 			lcdSendInternal(uint8_t lcd_addr, uint8_t data, uint8_t flags);
void 										lcdSendCommand(uint8_t lcd_addr, uint8_t cmd);
void 										lcdSendData(uint8_t lcd_addr, uint8_t data);
void 										lcdSendString(uint8_t lcd_addr, char *str);



#endif


#endif /* SRC_COMMON_HW_INCLUDE_LCD_C_ */
