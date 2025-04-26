/*
 * motor.h
 *
 *  Created on: Apr 25, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_MOTOR_H_
#define SRC_COMMON_HW_INCLUDE_MOTOR_H_


#include "hw_def.h"


#ifdef _USE_HW_MOTOR

#define MOTOR_MAX_CH			HW_MOTOR_MAX_CH
#define FORWARD						1
#define BACKWARD					0

void 			HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
bool 			motorInit(uint32_t prescaler, uint32_t period);
bool			motorOpen(uint8_t ch);
void 			setMotorSpeed(uint8_t ch, uint8_t speed);
uint32_t 	getMoterSpeed(uint8_t ch);
void			motorDir(uint8_t dir);
void 			motorStop(void);

#endif


#endif /* SRC_COMMON_HW_INCLUDE_MOTOR_H_ */
