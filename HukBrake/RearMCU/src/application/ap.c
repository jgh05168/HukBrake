/*
 * ap.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#include "ap.h"



void apInit(void)
{
	/* MCU 통신 세팅 */
	uartOpen(_DEF_UART1, 115200);
	i2cOpen(_DEF_I2C1, 100000);
	/* CAN 세팅 */
	setCanFilter(0x7F0, 0x10C, 0x7F0, 0x10C);
	canOpen();
	/* LCD 세팅 */
	lcdInit(2);


}


// main.c : 모든 상위 작업들을 여기서 할 수 있도록
void apMain(void)
{
	double t;
	while (1)
	{
		t = getSonicDist();
		char buf[16];
		sprintf(buf, "%.2f", (float)t);
		lcdClear();
		lcdSetCursor(0, 0);
		lcdPrintStr(buf);
		canSendDataDouble(CAN_STDID_ULTRASONIC, t);
//		logPrintf("dist = %.2f\r\n", t);
		uint32_t motor_data = canGetMotorData();
		if (motor_data == -1){
			lcdSetCursor(1, 5);
			char emergency_buf[] = "EMERGENCY";
			lcdPrintStr(emergency_buf);
		}
		delay(30);

	}

}
