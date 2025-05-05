/*
 * i2c.h
 *
 *  Created on: May 5, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_I2C_H_
#define SRC_COMMON_HW_INCLUDE_I2C_H_


#include "hw_def.h"

#ifdef _USE_HW_I2C

#define I2C_MAX_CH				HW_I2C_MAX_CH


extern I2C_HandleTypeDef hi2c1;


bool 			i2cInit(void);
bool 			i2cOpen(uint8_t ch, uint32_t speed);



#endif

#endif /* SRC_COMMON_HW_INCLUDE_I2C_H_ */
