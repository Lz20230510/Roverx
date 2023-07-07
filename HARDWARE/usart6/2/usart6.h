#ifndef __USART6_H
#define __USART6_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//////////////////////////////////////////////////////////////////////////////////
#define USART6_REC_LEN 			10  	//RFID ����ȡ���ݳ��� 
#define Smc345_REC_LEN 			8  	//RFID ����ȡ���ݳ���
#define RFID_REC_LEN 			9  	//RFID ����ȡ���ݳ��� 
//#define USART6_REC_LEN  			8 	//�����Ƕ�ȡ������󳤶�
#define EN_USART6_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����	 
//#define USART6_REC_LEN 			9  	//RFID ����ȡ���ݳ��� 
	 

typedef struct
{	
	u8 scm345_cmd[5];
	float scm345Angle;
}Smc345Def;
extern Smc345Def Smc345;




extern u8  USART6_RX_BUF[USART6_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART6_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void usart6_init(u32 bound);
float bcd_to_hex(u8 * BCD);
void SCM_345_contralCmd(u8 lenth,u8 * cmd);
#endif


