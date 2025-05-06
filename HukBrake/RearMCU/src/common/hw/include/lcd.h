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


extern I2C_HandleTypeDef 		hi2c1;


/* Command */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/* Entry Mode */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* Display On/Off */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/* Cursor Shift */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/* Function Set */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/* Backlight */
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

/* Enable Bit */
#define ENABLE 0x04

/* Read Write Bit */
#define RW 0x0

/* Register Select Bit */
#define RS 0x01

/* Device I2C Address */
#define DEVICE_ADDR     (0x27 << 1)



void 			lcdInit(uint8_t rows);
void 			lcdClear();
void 			lcdHome();
void 			lcdNoDisplay();
void 			lcdDisplay();
void 			lcdNoBlink();
void 			lcdBlink();
void 			lcdNoCursor();
void 			lcdCursor();
void 			lcdScrollDisplayLeft();
void 			lcdScrollDisplayRight();
void 			lcdPrintLeft();
void 			lcdPrintRight();
void 			lcdLeftToRight();
void 			lcdRightToLeft();
void 			lcdShiftIncrement();
void 			lcdShiftDecrement();
void 			lcdNoBacklight();
void 			lcdBacklight();
void 			lcdAutoScroll();
void 			lcdNoAutoScroll();
void 			lcdCreateSpecialChar(uint8_t, uint8_t[]);
void 			lcdPrintSpecialChar(uint8_t);
void 			lcdSetCursor(uint8_t, uint8_t);
void 			lcdSetBacklight(uint8_t new_val);
void 			lcdLoadCustomCharacter(uint8_t char_num, uint8_t *rows);
void 			lcdPrintStr(const char[]);

#endif


#endif /* SRC_COMMON_HW_INCLUDE_LCD_C_ */
