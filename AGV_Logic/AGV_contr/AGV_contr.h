#ifndef __AGV_CONTR_H__
#define __AGV_CONTR_H__
#include "sys.h"
#include "HEAD.h"
#include "usart3.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define NO PEout(8)			// 	
#define NO1 PEout(9)		// 	
#define OT_SON PEout(10)		// 	  �ŷ�ʹ�����

#define IN_SEV1 PDin(15)  //�ŷ�1����
#define IN_SEV2 PDin(0)  //�ŷ�2����
#define IN_SEV3 PDin(1)  //�ŷ�3����
#define IN_SEV4 PEin(7)  //�ŷ�4����

void SON1234_Init(void);
void Sov_on_1234_Init(void);

void Inside_Auto(void);													//�����Զ�ģʽ
void Outside_Auto(void);												//�����Զ�ģʽ
void JogMode_control(void);											//�ֶ�����ģʽ
void TeachMode_control(void);											//�ֶ�����ģʽ
void AGVParamter_Init(void);												//AGV ������ʼ��		
void MapDownLoad_control(void);										//���ӵ�ͼ���ش���
#endif
