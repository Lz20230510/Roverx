#ifndef _TIMER1_H
#define _TIMER1_H
#include "sys.h"
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

typedef struct
{
	u8 flag;
}Time1MarkDef;

extern Time1MarkDef Time1Mark;

typedef struct 
{
	u8 start[8];							//������ʱ��
	u8 time[8];							//��ʱ��ʱ�䳤��
	u16 tiAdd;						//��ʱ���ۼ���
	u8 mark[8];							//��ʱ��ɱ�־
	u8 reMark;							//��ת���
	
}TimeAddDef;
extern TimeAddDef Tim6Add,Tim7Add,Tim1Add;



void TIM1_Init(u16 arr,u16 psc);
void Time1_Task(void);
#endif
