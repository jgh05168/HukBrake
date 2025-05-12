/*
 * main.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */

#include "main.h"



static void threadMain(void const *argument);


int main(void)
{
  hwInit();
  apInit();

	// 스레드 정의 !
  osThreadDef(threadMain, threadMain, _HW_DEF_RTOS_THREAD_PRI_MAIN, 0, _HW_DEF_RTOS_THREAD_MEM_MAIN);

  // 스레드 생성 확인 ! -> 오류 발생 시 로그 출력시키기
  if (osThreadCreate(osThread(threadMain), NULL) != NULL)
  {
    logPrintf("threadMain \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadMain \t\t: Fail\r\n");
    while(1);
  }

  osKernelStart();	// 이 커널 스타트 시, threadMain 실행된다.
  return 0;
}


static void threadMain(void const *argument)
{
  UNUSED(argument);

  apMain();
}
