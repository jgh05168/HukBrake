/*
 * bsp.h
 *
 *  Created on: Apr 17, 2025
 *      Author: kyn05
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#include "def.h"
#include "hw_def.h"


// printf 로그출력용 1 : 출력 o, 0 : 출력 x
#define _USE_LOG_PRINTF				1

#if _USE_LOG_PRINTF
#define logPrintf(fmt, ...)			printf(fmt, ##__VA_ARGS__)
#else
#define logPrintf(fmt, ...)
#endif

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"


void bspInit(void);

void delay(uint32_t ms);
uint32_t mills(void);


void Error_Handler(void);


#endif /* SRC_BSP_BSP_H_ */
