#ifndef __RAD_H
#define __RAD_H
#include "sys.h" 
//#include "led.h" 
//#include "key.h" 
#include "HEAD.h" 


#define UltalEn PEout(9)				//�������豸ʹ�����


typedef struct
{
	u8 channel[8];						//�ֱ�洢8��ͨ�������� 		//��λCM
	u8 deviceEn;							//����������ʹ��   0���رճ�����   1���򿪳�����
	u8 safeDistance[8];				//8��ͨ������İ�ȫ����			//��λCM
	u8 error;          				//���������ݽ��մ���        
}UltrosonicDef;
extern UltrosonicDef Ultrosonic;

void ultrosonic_GPIO_Init(void);					
UltrosonicDef ultrosonic_Read(u8 * buff);			//
u8 ultrosonic_check(u8 * buff);				//У��
void ultrosonic_process(void);
UltrosonicDef ultrosonic_Read(u8 * buff);
u8 ultrosonic_check(u8 * buff);
#endif

