#ifndef __USART6_H
#define __USART6_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2011/6/14
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//////////////////////////////////////////////////////////////////////////////////
#define USART6_REC_LEN 			10  	//RFID 最大读取数据长度 
#define Smc345_REC_LEN 			8  	//RFID 最大读取数据长度
#define RFID_REC_LEN 			9  	//RFID 最大读取数据长度 
//#define USART6_REC_LEN  			8 	//陀螺仪读取数据最大长度
#define EN_USART6_RX 			1		//使能（1）/禁止（0）串口1接收	 
//#define USART6_REC_LEN 			9  	//RFID 最大读取数据长度 
#define FRAME_HEADER      0X7B //Frame_header //帧头
#define FRAME_TAIL        0X7D //Frame_tail   //帧尾

//#define USART6_REC_LEN  			8 	//陀螺仪读取数据最大长度 
//#define USART6_REC_LEN 			9  	//RFID 最大读取数据长度 
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
	
#define CRC16_ENABLE 0         /*!< 如果需要CRC16校验功能，修改此宏为1(此时需要在VisualTFT工程中配CRC校验)*/
#define CMD_MAX_SIZE 30        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
#define QUEUE_MAX_SIZE 80  	 /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/

//extern u8  USART6_RX_BUF[USART6_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART6_SD_BUF[USART6_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART6_RX_STA;         		//接收状态标记	

extern uint8 cmd_buffer[CMD_MAX_SIZE];//大彩屏指令缓存
void usart6_init(u32 bound);
void  SendChar(uchar t);






extern float Vz,Move_X;


/*****用于存放陀螺仪加速度计三轴数据的结构体*********************************/
typedef struct __Mpu6050_Data_ 
{
	short X_data; //2 bytes //2个字节
	short Y_data; //2 bytes //2个字节
	short Z_data; //2 bytes //2个字节
}Mpu6050_Data;

/*******The structure of the serial port sending data************/
/*******串口发送数据的结构体*************************************/
typedef struct _SEND_DATA_  
{
	unsigned char buffer[SEND_DATA_SIZE];
	struct _Sensor_Str_
	{
		unsigned char Frame_Header; //1个字节
		short X_speed;	            //2 bytes //2个字节
		short Y_speed;              //2 bytes //2个字节
		short Z_speed;              //2 bytes //2个字节
		short Power_Voltage;        //2 bytes //2个字节
		Mpu6050_Data Accelerometer; //6 bytes //6个字节
		Mpu6050_Data Gyroscope;     //6 bytes //6个字节	
		unsigned char Frame_Tail;   //1 bytes //1个字节
	}Sensor_Str;
}SEND_DATA;

typedef struct _RECEIVE_DATA_  
{
	unsigned char buffer[RECEIVE_DATA_SIZE];
	struct _Control_Str_
	{
		unsigned char Frame_Header; //1 bytes //1个字节
		float X_speed;	            //4 bytes //4个字节
		float Y_speed;              //4 bytes //4个字节
		float Z_speed;              //4 bytes //4个字节
		unsigned char Frame_Tail;   //1 bytes //1个字节
	}Control_Str;
}RECEIVE_DATA;

extern RECEIVE_DATA Receive_Data;



//typedef struct
//{	
//	u8 scm345_cmd[5];
//	float scm345Angle;
//}Smc345Def;
//extern Smc345Def Smc345;




extern u8  USART6_RX_BUF[USART6_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART6_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void usart6_init(u32 bound);
float bcd_to_hex(u8 * BCD);
void SCM_345_contralCmd(u8 lenth,u8 * cmd);
u8 Check_Sum(unsigned char Count_Number,unsigned char Mode);
void data_transition(void);
void USART6_SEND(void);
void usart6_send(u8 data);

void SLAM_AutoContral(void);






#endif


