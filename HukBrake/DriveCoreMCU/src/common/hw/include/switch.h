/*
 * switch.h
 *
 *  Created on: May 13, 2025
 *      Author: kyn05
 */

#ifndef SRC_COMMON_HW_INCLUDE_SWITCH_H_
#define SRC_COMMON_HW_INCLUDE_SWITCH_H_



#include "hw_def.h"


#ifdef _USE_HW_SWITCH


/* SWITCH Method -------------------------------------------------*/
bool 		switchInit(void);
bool 		switchState(void);

#endif

#endif /* SRC_COMMON_HW_INCLUDE_SWITCH_H_ */
