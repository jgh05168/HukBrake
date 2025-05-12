/*
 * hw_def.h
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "def.h"
#include "bsp.h"


/* RTOS Define --------------------------------------------*/
#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4)			// 메모리 사이즈를 byte 단위로 전환(기본 : uint32_t)

/* 스레드 2개 정의 (main, led) */
// 1. 스레드 우선순위
#define _HW_DEF_RTOS_THREAD_PRI_MAIN          osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_SONAR         osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_LCD		        osPriorityNormal


// 2. 스레드 스택 메모리
#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_SONAR         _HW_DEF_RTOS_MEM_SIZE(   1024)
#define _HW_DEF_RTOS_THREAD_MEM_LCD           _HW_DEF_RTOS_MEM_SIZE(   1024)

#define _USE_HW_RTOS



/* GPIO Define --------------------------------------------*/



/* Private Define -----------------------------------------*/
#define _USE_HW_FLASH
#define _USE_HW_ULTRASONIC

#define _USE_HW_LED
#define 		HW_LED_MAX_CH					1U

#define _USE_HW_LCD
#define 		HW_LCD_MAX_CH					1U

#define _USE_HW_UART
#define 		HW_UART_MAX_CH 				1U

#define _USE_HW_I2C
#define 		HW_I2C_MAX_CH					1U


#endif /* SRC_HW_HW_DEF_H_ */
