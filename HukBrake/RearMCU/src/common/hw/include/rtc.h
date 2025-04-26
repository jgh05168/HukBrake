/*
 * rtc.h
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_RTC_H_
#define SRC_COMMON_HW_INCLUDE_RTC_H_


#include "hw_def.h"


#ifdef _USE_HW_RTC

bool rtcInit(void);

void 					rtcBackupRegWrite(uint32_t idx, uint32_t data);
uint32_t 			rtcBackupRegRead(uint32_t idx);


#endif


#endif /* SRC_COMMON_HW_INCLUDE_RTC_H_ */
