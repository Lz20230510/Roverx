#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
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
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
#define USART2_REC_LEN  			200  	//�����������ֽ��� 20
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����



extern u8  USART2_SD_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN_S���ֽ�.ĩ�ֽ�Ϊ���з� 	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	


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
//};
//VehicleParamtDef;
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
	
//extern BatteryMsgDef BatteryMsg;
//extern u8 Ask_Battery[7];						//ѯ�ʵ�ص�������



typedef struct
{
	u8 buff;
	u8 value;
}BluetoothDef;

extern BluetoothDef Bluetooth;

//����봮���жϽ��գ��벻Ҫע�����º궨��
//void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority );
//void Battery_Send(u8 len,u8 *cmd);       								//���������ȡ���ݷ���

//u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num);

//BatteryMsgDef Battery_Capacity(u8 * cmd);								//���������ȡ
//void Battery_Send(u8 len,u8 *cmd);       								//���������ȡ���ݷ���
//MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len);			//��λ�����͵ĵ��ӵ�ͼ���ݴ���
//void Map_Download(void);						//��ͼ���س���
//u16 MapTable_check(u16 lastPotNum,float OAOBLimt);			//����ͼ�洢���Ĵ��������
//void MapTable_repair(u16 lastPotNum,float OAOBLimt);			//������޸�
u8 HC_05_Trans(u8 res);
//void usart1_send(u8 data);

#endif






























//#ifndef __USART2_H
//#define __USART2_H


//#include "stdio.h"	
//#include "stm32f4xx_conf.h"
//#include "sys.h" 

//#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

//#define uchar    unsigned char
//#define uint8    unsigned char
//#define uint16   unsigned short int
//#define uint32   unsigned long
//#define int16    short int
//#define int32    long

//#define CRC16_ENABLE 0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
//#define CMD_MAX_SIZE 30        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
//#define QUEUE_MAX_SIZE 80  	 /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/


//#define USART2_REC_LEN  			25  	//�����������ֽ��� 25
//	  	
//extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u8  USART2_SD_BUF[USART2_REC_LEN]; //���ͻ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART2_RX_STA;         		//����״̬���	


//extern uint8 cmd_buffer[CMD_MAX_SIZE];//�����ָ���
//void usart2_init(u32 bound);
//void  SendChar(uchar t);


//#endif
