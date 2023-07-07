#ifndef __USART6_H
#define __USART6_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "HEAD.h"
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
#define FRAME_HEADER      0X7B //Frame_header //֡ͷ
#define FRAME_TAIL        0X7D //Frame_tail   //֡β

//#define USART6_REC_LEN  			8 	//�����Ƕ�ȡ������󳤶� 
//#define USART6_REC_LEN 			9  	//RFID ����ȡ���ݳ��� 
#define SEND_DATA_SIZE     24
#define RECEIVE_DATA_SIZE  11	 
#define barry 0
#define dacai 1                                                                                                                          

#define uchar    unsigned char



#define uchar    unsigned char
#define uint8    unsigned char
#define uint16   unsigned short int
#define uint32   unsigned long
#define int16    short int
#define int32    long
	
#define CRC16_ENABLE 0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
#define CMD_MAX_SIZE 30        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE 80  	 /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

//extern u8  USART6_RX_BUF[USART6_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART6_SD_BUF[USART6_REC_LEN]; //���ͻ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u16 USART6_RX_STA;         		//����״̬���	

extern uint8 cmd_buffer[CMD_MAX_SIZE];//�����ָ���
void usart6_init(u32 bound);
void  SendChar(uchar t);






extern float Vz,Move_X;


/*****���ڴ�������Ǽ��ٶȼ��������ݵĽṹ��*********************************/
typedef struct __Mpu6050_Data_ 
{
	short X_data; //2 bytes //2���ֽ�
	short Y_data; //2 bytes //2���ֽ�
	short Z_data; //2 bytes //2���ֽ�
}Mpu6050_Data;

/*******The structure of the serial port sending data************/
/*******���ڷ������ݵĽṹ��*************************************/
typedef struct _SEND_DATA_  
{
	unsigned char buffer[SEND_DATA_SIZE];
	struct _Sensor_Str_
	{
		unsigned char Frame_Header; //1���ֽ�
		short X_speed;	            //2 bytes //2���ֽ�
		short Y_speed;              //2 bytes //2���ֽ�
		short Z_speed;              //2 bytes //2���ֽ�
		short Power_Voltage;        //2 bytes //2���ֽ�
		Mpu6050_Data Accelerometer; //6 bytes //6���ֽ�
		Mpu6050_Data Gyroscope;     //6 bytes //6���ֽ�	
		unsigned char Frame_Tail;   //1 bytes //1���ֽ�
	}Sensor_Str;
}SEND_DATA;

typedef struct _RECEIVE_DATA_  
{
	unsigned char buffer[RECEIVE_DATA_SIZE];
	struct _Control_Str_
	{
		unsigned char Frame_Header; //1 bytes //1���ֽ�
		float X_speed;	            //4 bytes //4���ֽ�
		float Y_speed;              //4 bytes //4���ֽ�
		float Z_speed;              //4 bytes //4���ֽ�
		unsigned char Frame_Tail;   //1 bytes //1���ֽ�
	}Control_Str;
}RECEIVE_DATA;

extern RECEIVE_DATA Receive_Data;



//typedef struct
//{	
//	u8 scm345_cmd[5];
//	float scm345Angle;
//}Smc345Def;
//extern Smc345Def Smc345;




extern u8  USART6_RX_BUF[USART6_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART6_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void usart6_init(u32 bound);
float bcd_to_hex(u8 * BCD);
void SCM_345_contralCmd(u8 lenth,u8 * cmd);
u8 Check_Sum(unsigned char Count_Number,unsigned char Mode);
void data_transition(void);
void USART6_SEND(void);
void usart6_send(u8 data);

void SLAM_AutoContral(void);






#endif


