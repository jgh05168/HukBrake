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

	sonicInit();
	ledInit();
	uartInit();
	flashInit();
	canInit();
}
