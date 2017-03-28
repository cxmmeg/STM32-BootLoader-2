//*================================================================================================
//* �ļ���				: timer.h
//* ��������			: ��ʱ���������
//* ����    			: ��ΰ��
//* �汾				: 0.01
//* �������ڡ�ʱ��		: 2011/04/24 14:44
//* ����޸����ڡ�ʱ��	: 
//* �޸�ԭ��			: 
//*================================================================================================
#ifndef	__TIMER_H
#define __TIMER_H

#include "stm32f10x_conf.h"

#define	TIMER_ONE_TIME	0x01
#define TIMER_CYCLE		0x02

extern void USART1_Timer3_Config(void);
extern void USART1_Timer3_Stop(void);
extern void USART1_Timer3_Start(uint32_t time);

#endif
