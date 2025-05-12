/*
 * ap.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#include "ap.h"


static void threadSonar(void const *argument);
static void threadLcd(void const *argument);


void apInit(void)
{
	/* MCU 통신 세팅 */
	uartOpen(_DEF_UART1, 115200);
	i2cOpen(_DEF_I2C1, 100000);
	/* CAN 세팅 */
	setCanFilter(0x7FF, 0x101, 0x7FF, 0x100);
	canOpen();



	/* thread on */
	osThreadDef(threadSonar, threadSonar, _HW_DEF_RTOS_THREAD_PRI_SONAR, 0, _HW_DEF_RTOS_THREAD_MEM_SONAR);
	osThreadDef(threadLcd, threadLcd, _HW_DEF_RTOS_THREAD_PRI_LCD, 0, _HW_DEF_RTOS_THREAD_MEM_LCD);
	if (osThreadCreate(osThread(threadSonar), NULL) != NULL)
	{
		logPrintf("threadSonar \t\t: OK\r\n");
	}
	else
	{
		logPrintf("threadSonar \t\t: Fail\r\n");
		while(1);
	}
	if (osThreadCreate(osThread(threadLcd), NULL) != NULL)
	{
		logPrintf("threadLcd \t\t: OK\r\n");
	}
	else
	{
		logPrintf("threadLcd \t\t: Fail\r\n");
		while(1);
	}

}


// main.c : 모든 상위 작업들을 여기서 할 수 있도록
void apMain(void)
{
	while (1)
	{
//		t = getSonicDist();
//		char buf[16];
//		sprintf(buf, "%.2f", (float)t);
//		lcdClear();
//		lcdSetCursor(0, 0);
////		lcdPrintStr(buf);
//		canSendDataDouble(CAN_STDID_ULTRASONIC, t);
////		logPrintf("dist = %.2f\r\n", t);
//		uint32_t motor_data = canGetMotorData();
//		if (motor_data == -1)
//		{
//			lcdSetCursor(1, 5);
//			char emergency_buf[] = "EMERGENCY";
//			lcdPrintStr(emergency_buf);
//		}
//		else
//		{
//			itoa(motor_data, buf, 10);
//			lcdPrintStr(buf);
//			logPrintf("motor_speed : %d\r\n", motor_data);
//		}
//		delay(30);

	}

}


static void threadSonar(void const *argument)
{
	UNUSED(argument);

	while (1)
	{
		// 초음파센서의 tim과 freeRTOS task는 별도 동작한다. => 상관 x
		double t = getSonicDist();

		canSendDataDouble(CAN_STDID_ULTRASONIC, t);

		// freeRTOS 사용해서 log 출력 시, delay 필수
		delay(30);
	}
}


static void threadLcd(void const *argument)
{
	UNUSED(argument);

	/* LCD 세팅(freeRTOS thread 세팅 후에 lcd 초기화 해주어야 tic이 꼬이지 않음) */
	lcdInit(2);

	char			 	buf[16];
	char 				speed_buf[16];
	char 				direction_buf[16];
	uint32_t 		motor_data = 0;
	bool 				emergency_flag = 0;

	while (1)
	{
		motor_data = canGetMotorData();

		if (motor_data == -1)
		{
			if (!emergency_flag)
			{
				lcdClear();
				emergency_flag = 1;
			}

			char emergency_buf[] = "EMERGENCY";
			lcdLeftToRight();
			lcdSetCursor(3, 0);
			lcdPrintStr(emergency_buf);
			lcdSetCursor(3, 1);
			lcdPrintStr(emergency_buf);
		}
		else
		{
			if (emergency_flag)
			{
				lcdClear();
				emergency_flag = 0;
			}

			lcdSetCursor(2, 0);
			if (motor_data == 100)
			{
		    strcpy(direction_buf, "FORWARD");
			}
			else
			{
				strcpy(direction_buf, "BACKWARD");
			}
			lcdPrintStr(direction_buf);

			lcdSetCursor(2, 1);
			itoa(motor_data, buf, 10);
			sprintf(speed_buf, "Speed : %skm", buf);
			lcdPrintStr(speed_buf);
			logPrintf("motor_speed : %d\r\n", motor_data);
		}
		delay(30);
	}
}
