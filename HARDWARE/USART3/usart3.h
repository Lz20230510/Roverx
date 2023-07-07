

#ifndef __USART3_H
#define __USART3_H	 

//////////////////////////////////////////////////////////////////////////////////	 
				  
////////////////////////////////////////////////////////////////////////////////// 	   
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
 

#define USART3_MAX_RECV_LEN		100//400					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		100//400					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

//#define USART3_MAX_RECV_LEN		100 //400					//最大接收缓存字节数
//#define USART3_MAG_RECV_LEN		7 //400						//磁导航传感器接收数据
//#define USART3_MAX_SEND_LEN		100 //400					//最大发送缓存字节数
//#define USART3_MAX_SEND_LEN		100 //400					//最大发送缓存字节数
//#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收

//#define u8    unsigned char
//#define uint8    unsigned char
//#define uint16   unsigned short int
//#define uint32   unsigned long
//#define int16    short int
//#define int32    long

#define CRC16_ENABLE 0         /*!< 如果需要CRC16校验功能，修改此宏为1(此时需要在VisualTFT工程中配CRC校验)*/
#define CMD_MAX_SIZE 20        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
#define QUEUE_MAX_SIZE 80  	 /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/


#define USART3_REC_LEN  			20  	//定义最大接收字节数 20

//extern u8  USART3_Right_BUF[USART3_MAG_RECV_LEN]; 	//正确数据接收缓冲
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记	


//extern u8 cmd_buffer[CMD_MAX_SIZE];//大彩屏指令缓存
void usart3_init(u32 bound);
//void  SendChar(u8 t);

#endif













