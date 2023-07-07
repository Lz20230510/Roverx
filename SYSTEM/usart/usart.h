#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//#include "HEAD.h" 
#include "map.h" 
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
//2,�޸���USART1_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART1_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 
#define USART1_REC_LEN  			100  	//�����������ֽ��� 100
#define Battery_REC_LEN  			34  	//�����������ֽ��� 34
#define Battery_SED_LEN  			7  	//����������ֽ��� 7
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  Battery_RX_BUF[Battery_REC_LEN]; //���ջ���,���Battery_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART1_RX_STA;//����״̬���	
extern u8 Ask_Battery[7];						//ѯ�ʵ�ص�������


typedef struct 
{
	char * fir_fram;           //PA
	char * OA_ve;
	char * OB_ve;
	char * angle;
	char * longti;
	char * lati;
	char * angle_near;
	char * OA_near;
	char * OB_near;
	char * OAOB_num;
	char * last_fram;           //PA

}VehicleParamtDef;
extern VehicleParamtDef VehicleParamt;

typedef struct 
{
	float total_Vot;    //�ܵ�ѹ
	float current;    	//����
	float SOC;    			//ʣ������
	float biaocheng_SOC;    			//�������
	u16 cycle;					//ѭ������
	u16 date;     //����
	u16 protectStatus;	//����״̬  0δ����  1����
	u8 softwareVer;		//����汾
	u8 RSOC;					//ʣ�������ٷֱ�
	u8 FET;						//��缰�ŵ�MOS״̬��bit0��� bit1�ŵ� 0�ر� 1��
	u8 batteryNum;		//��ش�����
	
}BatteryMsgDef;
	
extern BatteryMsgDef BatteryMsg;

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority );
u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num);


BatteryMsgDef Battery_Capacity(u8 * cmd);								//���������ȡ
void Battery_Send(u8 len,u8 *cmd);       								//���������ȡ���ݷ���
MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len);			//��λ�����͵ĵ��ӵ�ͼ���ݴ���
void Map_Download(void);						//��ͼ���س���
u16 MapTable_check(u16 lastPotNum,float OAOBLimt);			//����ͼ�洢���Ĵ��������
void MapTable_repair(u16 lastPotNum,float OAOBLimt);			//������޸�
#endif









//#ifndef __USART_H
//#define __USART_H
//#include "stdio.h"	
//#include "stm32f4xx_conf.h"
//#include "sys.h" 
//#include "map.h" 
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////Mini STM32������
////����1��ʼ��		   
////����ԭ��@ALIENTEK
////������̳:www.openedv.csom
////�޸�����:2011/6/14
////�汾��V1.4
////��Ȩ���У�����ؾ���
////Copyright(C) ����ԭ�� 2009-2019
////All rights reserved
////********************************************************************************
////V1.3�޸�˵�� 
////֧����Ӧ��ͬƵ���µĴ��ڲ���������.
////�����˶�printf��֧��
////�����˴��ڽ��������.
////������printf��һ���ַ���ʧ��bug
////V1.4�޸�˵��
////1,�޸Ĵ��ڳ�ʼ��IO��bug
////2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
////3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
////4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//////////////////////////////////////////////////////////////////////////////////// 	
//#define USART_REC_LEN  			200  	//�����������ֽ��� 20
//#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����



//extern u8  USART_SD_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN_S���ֽ�.ĩ�ֽ�Ϊ���з� 	  	
//extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART_RX_STA;         		//����״̬���	


//typedef struct 
//{
//	char * fir_fram;           //PA
//	char * OA_ve;
//	char * OB_ve;
//	char * angle;
//	char * longti;
//	char * lati;
//	char * angle_near;
//	char * OA_near;
//	char * OB_near;
//	char * OAOB_num;
//	char * last_fram;           //PA

//}VehicleParamtDef;
//extern VehicleParamtDef VehicleParamt;

//typedef struct 
//{
//	float total_Vot;    //�ܵ�ѹ
//	float current;    	//����
//	float SOC;    			//ʣ������
//	float biaocheng_SOC;    			//�������
//	u16 cycle;					//ѭ������
//	u16 date;     //����
//	u16 protectStatus;	//����״̬  0δ����  1����
//	u8 softwareVer;		//����汾
//	u8 RSOC;					//ʣ�������ٷֱ�
//	u8 FET;						//��缰�ŵ�MOS״̬��bit0��� bit1�ŵ� 0�ر� 1��
//	u8 batteryNum;		//��ش�����
//	
//}BatteryMsgDef;
//	
//extern BatteryMsgDef BatteryMsg;
//extern u8 Ask_Battery[7];						//ѯ�ʵ�ص�������



//typedef struct
//{
//	u8 buff;
//	u8 value;
//}BluetoothDef;

//extern BluetoothDef Bluetooth;

////����봮���жϽ��գ��벻Ҫע�����º궨��
//void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority );
//void Battery_Send(u8 len,u8 *cmd);       								//���������ȡ���ݷ���

//u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num);

//BatteryMsgDef Battery_Capacity(u8 * cmd);								//���������ȡ
//void Battery_Send(u8 len,u8 *cmd);       								//���������ȡ���ݷ���
//MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len);			//��λ�����͵ĵ��ӵ�ͼ���ݴ���
//void Map_Download(void);						//��ͼ���س���
//u16 MapTable_check(u16 lastPotNum,float OAOBLimt);			//����ͼ�洢���Ĵ��������
//void MapTable_repair(u16 lastPotNum,float OAOBLimt);			//������޸�
//u8 HC_05_Trans(u8 res);
//void usart1_send(u8 data);

//#endif


