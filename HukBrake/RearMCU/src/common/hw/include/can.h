/*
 * can.h
 *
 *  Created on: May 1, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_CAN_H_
#define SRC_COMMON_HW_INCLUDE_CAN_H_

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


extern CAN_HandleTypeDef hcan;

void canInit(void);
void setCanFilter(uint32_t filterMaskHigh, uint32_t filterIdHigh, uint32_t filterMaskLow, uint32_t filterIdLow);
void canOpen(void);
void sendCanData(uint32_t StdId, double data);


#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

#endif /* SRC_COMMON_HW_INCLUDE_CAN_H_ */
