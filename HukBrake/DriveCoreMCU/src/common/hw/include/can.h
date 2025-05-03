/*
 * can.h
 *
 *  Created on: Apr 28, 2025
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

// 송신 stdid define
#define CAN_STDID_ULTRASONIC		0x100


extern CAN_HandleTypeDef hcan;

void canInit(void);
bool setCanFilter(uint32_t filterMaskHigh, uint32_t filterIdHigh, uint32_t filterMaskLow, uint32_t filterIdLow);
bool canOpen(void);

double canGetUltrasonicData(void);


void canRxUltrasonicCallback(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan); // CAN RX의 FIFO0에 데이터가 수신이 되었을 때 걸리는 인터럽트 함수



#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

#endif /* SRC_COMMON_HW_INCLUDE_CAN_H_ */
