#ifndef __VEHICLDE_KEY_H
#define __VEHICLDE_KEY_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 


/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define IN_AUTOSetup 			!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12) 		//�豸������ť�ź�
#define IN_JOGSetup				!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)			//ң������
#define IN_EmerStop 	    !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) 		//��ͣ��ť�ź�


typedef	struct
{
	u8 SONSetup;				//С��ʹ�ܾ���			1ʹ������    0ʹ��δ����
	u8 Setup;						//С��������־λ
	u8 AJMode;					//С�����Զ�״̬		1�Զ�״̬    0�ֶ�״̬
	u8 AUTOmode;        //С���Զ�ģʽ     0�Զ�ģʽδ����  1�����Զ�ģʽ   2�����Զ�ģʽ 
	u8 TeachMode;        //С��ʾ��ģʽ	  0�ر�ʾ��ģʽ  	1��ʾ��ģʽ
	u8 MapDownMode;			//��ͼ����ģʽ   0����ģʽδ����  1�������ع����� 2�������
	u8 EmerStop;				//С����ͣ״̬  1�����ڼ�ͣ״̬    0��ͣ״̬
	u8 SevAlm[4];			  //�ŷ����������� 1����������   0�������޹���    [0]=1:�ŷ�1����  [1]=1:�ŷ�2���� [2]=1:�ŷ�3����   [3]=1:�ŷ�4����
	u8 AutoRun;					//�Զ�ģʽ����   =0���Զ�����״̬δ����     =1���Զ�����״̬����
	u8 BatteryCap;      //AGV�������
	u8 temp;						//��ʱ����
	u8 SLAMAutoMark;		//=1����һ��SLAM����  =0���ر�
	float Wheel_R;               //����ֱ��
	float pi1;                 	//Բ����
	float vehicle_width;					//С�������ּ��
	float vehicle_longth;				//С��ǰ���ּ��
	
}AGVStatusDef;

extern AGVStatusDef AGVStatus;

extern u8 AGVModeChange;


u8 Vehkey_scan(u8 mode1);
void AGVsta_Scan(u8 mode2);
void Vehicle_key_Init(void);
void Vehicle_key_Init(void);
void KEYEXTIX_Init(void);

#endif
