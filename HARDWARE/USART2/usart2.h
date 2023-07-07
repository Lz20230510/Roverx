#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "map.h" 
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
#define USART2_REC_LEN  			200  	//定义最大接收字节数 20
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收



extern u8  USART2_SD_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN_S个字节.末字节为换行符 	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	


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
//	float total_Vot;    //总电压
//	float current;    	//电流
//	float SOC;    			//剩余容量
//	float biaocheng_SOC;    			//标称容量
//	u16 cycle;					//循环次数
//	u16 date;     //日期
//	u16 protectStatus;	//保护状态  0未保护  1保护
//	u8 softwareVer;		//软件版本
//	u8 RSOC;					//剩余容量百分比
//	u8 FET;						//充电及放电MOS状态，bit0充电 bit1放电 0关闭 1打开
//	u8 batteryNum;		//电池串联数
//	
//}BatteryMsgDef;
	
//extern BatteryMsgDef BatteryMsg;
//extern u8 Ask_Battery[7];						//询问电池电量数据



typedef struct
{
	u8 buff;
	u8 value;
}BluetoothDef;

extern BluetoothDef Bluetooth;

//如果想串口中断接收，请不要注释以下宏定义
//void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority );
//void Battery_Send(u8 len,u8 *cmd);       								//电池容量读取数据发送

//u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num);

//BatteryMsgDef Battery_Capacity(u8 * cmd);								//电池容量读取
//void Battery_Send(u8 len,u8 *cmd);       								//电池容量读取数据发送
//MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len);			//上位机发送的电子地图数据处理
//void Map_Download(void);						//地图下载程序
//u16 MapTable_check(u16 lastPotNum,float OAOBLimt);			//检查地图存储区的错误点数量
//void MapTable_repair(u16 lastPotNum,float OAOBLimt);			//错误点修复
u8 HC_05_Trans(u8 res);
//void usart1_send(u8 data);

#endif






























//#ifndef __USART2_H
//#define __USART2_H


//#include "stdio.h"	
//#include "stm32f4xx_conf.h"
//#include "sys.h" 

//#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收

//#define uchar    unsigned char
//#define uint8    unsigned char
//#define uint16   unsigned short int
//#define uint32   unsigned long
//#define int16    short int
//#define int32    long

//#define CRC16_ENABLE 0         /*!< 如果需要CRC16校验功能，修改此宏为1(此时需要在VisualTFT工程中配CRC校验)*/
//#define CMD_MAX_SIZE 30        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
//#define QUEUE_MAX_SIZE 80  	 /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/


//#define USART2_REC_LEN  			25  	//定义最大接收字节数 25
//	  	
//extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u8  USART2_SD_BUF[USART2_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART2_RX_STA;         		//接收状态标记	


//extern uint8 cmd_buffer[CMD_MAX_SIZE];//大彩屏指令缓存
//void usart2_init(u32 bound);
//void  SendChar(uchar t);


//#endif
