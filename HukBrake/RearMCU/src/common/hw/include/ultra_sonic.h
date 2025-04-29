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
bool 				sonicInit(void);
double	 		getSonicDist(void);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_ULTRA_SONIC_H_ */
