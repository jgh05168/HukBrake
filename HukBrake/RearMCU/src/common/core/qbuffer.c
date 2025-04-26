/*
 * qbuffer.c
 *
 *  Created on: Apr 24, 2025
 *      Author: kyn05
 */


#include "qbuffer.h"



void qbufferInit(void)
{

}


bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length)
{
  bool ret = true;

  p_node->in    = 0;
  p_node->out   = 0;
  p_node->len   = length;
  p_node->p_buf = p_buf;

  return ret;
}


bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t next_in;

  for (int i=0;i<length;i++)
  {
  	// 다음 삽입 인덱스 계산
  	next_in = (p_node->in + 1) % p_node->len;

  	// 삽입할 수 있는지에 대해 체크
  	if (next_in != p_node->out)
  	{
  		// 버퍼가 null인 경우에는, 인덱스만 관리
  		if (p_node->p_buf != NULL)
  		{
  			p_node->p_buf[p_node->in] = p_data[i];
  		}
  		p_node->in = next_in;
  	}
  	// 버퍼가 꽉 찬 경우, 삽입 실패 및 for문 탈출
  	else
  	{
  		ret = false;
  		break;
  	}
  }

  return ret;
}


bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;


  for (int i=0; i<length; i++)
  {
  	if (p_node->p_buf != NULL && p_data != NULL)
  	  {
  	    // 데이터 복사
  	    p_data[i] = p_node->p_buf[p_node->out];
  	  }

  	  // 읽은 후, out 인덱스 갱신
  	  if (p_node->out != p_node->in)
  	  {
  	    p_node->out = (p_node->out + 1) % p_node->len;
  	  }
  	  else
  	  {
  	    ret = false;
  	    break;
  	  }
  }


  return ret;
}


uint32_t qbufferAvailable(qbuffer_t *p_node)
{
  uint32_t ret;

  ret = (p_node->len + p_node->in - p_node->out) % p_node->len;

  return ret;
}


void qbufferFlush(qbuffer_t *p_node)
{
	p_node->in = 0;
	p_node->out = 0;
}
