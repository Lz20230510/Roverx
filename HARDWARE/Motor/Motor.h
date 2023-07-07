#ifndef __MOTOR_H_
#define __MOTOR_H_
#include  "sys.h"
#include  "HEAD.h"



//����������д���ݽṹ��
//��Ϊ������		0:����   1���ҵ��
//��Ϊ������   0:��ǰ���   1����ǰ���   2�������   3���Һ���
//��Ϊ������   0:��ǰת����   1����ǰת����   2�����ת����   3���Һ�ת����   4:��ǰ���ߵ��   5����ǰ���ߵ��   6��������ߵ��   7���Һ����ߵ��
typedef struct 
{
	u16 WtStatus;			//���д������
	int WtSpd;				//д�ٶ�ֵ				r/min
	u16 WtFAcc;				//д���ٶ�ֵ			r/s^2
	u16 WtRAcc;				//д���ٶ�ֵ     r/s^2
	u16 RdStatus;			//��ȡ���״̬��     
	int RdSpd;				//��ȡ���ʵʱ�ٶ�		r/min
	float RdCurrent;		//��ȡ���ʵʱ����   A
	float RdSpdTrans;				//��ȡ���ʵʱ�ٶ�ת��ֵ		m/s
	
	float Ratio;				//������ٱȼ���
	u8 MotorReady;		//��������ź�
	u8 MotorErr;			//��������ź�
	u8 MotorEn;       //���ʹ��״̬
	
}MotorMsgDef[MotorCount];
extern MotorMsgDef MotorMsg;				//�������������ݽṹ��


void MotorValue_Read(MotorMsgDef motorMsgIn,u8 * MotorReadyIO,u8 * En);


#endif		    

