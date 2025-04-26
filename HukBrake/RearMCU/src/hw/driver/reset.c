/*
 * reset.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */



#include "reset.h"
#include "rtc.h"


static uint32_t reset_count = 0;


bool resetInit(void)
{
	bool ret = true;

	// 만약 reset 핀이 눌렸다면,
	if (RCC->CSR & (1 << 26))
		{
			// 자기 자신을 1부터 1씩 증가시키는 명령
			rtcBackupRegWrite(1, rtcBackupRegRead(1) + 1);
			delay(500);		// 500ms 이전에 얼마나 누르는지 설정하는 것임

			reset_count = rtcBackupRegRead(1);
		}

	// rtc 핀 초기화
	rtcBackupRegWrite(1, 0);


	return ret;
}


uint32_t resetGetCount(void)
{
	return reset_count;
}


