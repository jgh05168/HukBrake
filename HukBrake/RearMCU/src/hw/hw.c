/*
 * hw.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */


#include "hw.h"



void hwInit(void)
{
	bspInit();
	rtcInit();

	resetInit();
	ledInit();
	uartInit();
	flashInit();
}
