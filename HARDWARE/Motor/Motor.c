/******************************************************************************
* �ļ�����Motor.c
* ��  ����
* ��  �ߣ�
* ��  �ڣ�
* ��汾��   
* �ļ����������������س���
*******************************************************************************/
#include "HEAD.h"
#include "Motor.h"




/*************************��ǰ�ĵ��ʵʱ���ݶ�д��ԭʼ���ݣ�****************************/
//��������ʽ��void MotorValue_Read(MotorMsgDef motorMsgIn,u8 * MotorReadyIO,u8 En)
//���봫�Σ�MotorWtMsgDef motorWtMsgIn�����д״̬�ṹ�壬u8 * MotorReadyIO����������ź� u8 * En�����ʹ���ź�
//������Σ�
//����ֵ��MotorMsgDef  �����״̬�ṹ��
/************************************************************************/
void MotorValue_Read(MotorMsgDef motorMsgIn,u8 * MotorReadyIO,u8 * En)
{	
//	float k;			//����ϵ��
//	k=DriveMotor_Ratio(0.18,30.0);    //���ٱȼ���
	int SignL,SignR;
	
	if(AGVStatus.EmerStop == 1)
	{
		
		motorMsgIn[0].WtStatus = 5;
		motorMsgIn[0].WtSpd = 0;
		
		WtStatus_1=5;									//д���ֿ�����
		WtSpd_1=0;
		
		motorMsgIn[1].WtStatus = 5;
		motorMsgIn[1].WtSpd = 0;
		
		WtStatus_2=5;									//д���ֿ�����
		WtSpd_2=0;
	}
	else if(MotorCount==2 &&MotorMode==0)										//���ֲ��ٳ�
	{	
		//�����	
		if(RdStatus_1==0x01) SignL=-1;		
		else SignL=1;
		motorMsgIn[0].RdStatus=RdStatus_1;									//����״̬��ȡ
		motorMsgIn[0].RdSpd=SignL*(int)RdSpd_1;												//�����ٶȶ�ȡ    r/mim
		motorMsgIn[0].RdSpdTrans=motorMsgIn[0].RdSpd/MotorMsg[0].Ratio;			//���ʵʱ�ٶ�    m/s		
		motorMsgIn[0].RdCurrent=(float)RdCurrent_1/100;			//���ֵ�����ȡ		A	
		motorMsgIn[0].MotorErr=RdAlarm_1;										//��ȡ��������
		WtStatus_2=motorMsgIn[0].WtStatus;									//д���ֿ�����
		WtSpd_2=motorMsgIn[0].WtSpd/2;												//д�����ٶ�      r/mim
		
		//�Ҳ���	
		if(RdStatus_2==0x02) SignR=-1;		
		else SignR=1;
		motorMsgIn[1].RdStatus=RdStatus_2;									//����״̬��ȡ
		motorMsgIn[1].RdSpd=SignR*RdSpd_2;												//�����ٶȶ�ȡ    r/mim
		motorMsgIn[1].RdSpdTrans=motorMsgIn[1].RdSpd/MotorMsg[0].Ratio;			//���ʵʱ�ٶ�    m/s
		motorMsgIn[1].RdCurrent=(float)RdCurrent_2/100;			//���ֵ�����ȡ		A
		motorMsgIn[1].MotorErr=RdAlarm_2;										//��ȡ��������
		WtStatus_1=motorMsgIn[1].WtStatus;									//д���ֿ�����
		WtSpd_1=motorMsgIn[1].WtSpd/2;												//д�����ٶ�   		r/mim
	}
}




