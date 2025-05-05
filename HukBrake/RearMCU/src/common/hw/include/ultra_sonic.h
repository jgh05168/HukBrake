/*
 * ultra_sonic.h
 *
 *  Created on: Apr 29, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_ULTRA_SONIC_H_
#define SRC_COMMON_HW_INCLUDE_ULTRA_SONIC_H_


#include "hw_def.h"


#ifdef _USE_HW_ULTRASONIC

/* SONIC Method -------------------------------------------------*/

#define		TIMCLOCK 					72000000
#define 	PRESCALER 				72

void 				MX_TIM1_Init(void);
void 				MX_TIM2_Init(void);

void 				delay_us(uint16_t us);

bool 				sonicInit(void);
double	 		getSonicDist(void);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_ULTRA_SONIC_H_ */
