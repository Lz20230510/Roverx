
#include "sys.h"
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

void TIM7_Int_Init(u16 arr,u16 psc);

extern u8 a2[44]; 
extern float a1[3],w1[3],h1[3],Angle1[3];


void TIM6_Int_Init(u16 arr,u16 psc);
u8 Time7_Timekeep(u8 time,u8 timekeep,u8 start,u8 Num,u8 reset);																//���㵱ǰ������ʱ��
