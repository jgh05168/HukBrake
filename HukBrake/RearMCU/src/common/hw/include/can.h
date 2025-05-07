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


// stdid define
#define CAN_STDID_ULTRASONIC		0x100
#define CAN_STDID_MOTOR					0x101


extern CAN_HandleTypeDef hcan;

void 				canInit(void);
void 				setCanFilter(uint32_t filterMaskHigh, uint32_t filterIdHigh, uint32_t filterMaskLow, uint32_t filterIdLow);
void 				canOpen(void);
void 				canSendDataDouble(uint32_t StdId, double data);
uint32_t 		canGetMotorData(void);
void 				canRxMotorCallback();
void 				HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan); // CAN RX의 FIFO0에 데이터가 수신이 되었을 때 걸리는 인터럽트 함수



#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

#endif /* SRC_COMMON_HW_INCLUDE_CAN_H_ */
