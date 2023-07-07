

#ifndef __USART3_H
#define __USART3_H	 

//////////////////////////////////////////////////////////////////////////////////	 
				  
////////////////////////////////////////////////////////////////////////////////// 	   
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
 

#define USART3_MAX_RECV_LEN		100//400					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		100//400					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

//#define USART3_MAX_RECV_LEN		100 //400					//�����ջ����ֽ���
//#define USART3_MAG_RECV_LEN		7 //400						//�ŵ�����������������
//#define USART3_MAX_SEND_LEN		100 //400					//����ͻ����ֽ���
//#define USART3_MAX_SEND_LEN		100 //400					//����ͻ����ֽ���
//#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

//#define u8    unsigned char
//#define uint8    unsigned char
//#define uint16   unsigned short int
//#define uint32   unsigned long
//#define int16    short int
//#define int32    long

#define CRC16_ENABLE 0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
#define CMD_MAX_SIZE 20        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE 80  	 /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/


#define USART3_REC_LEN  			20  	//�����������ֽ��� 20

//extern u8  USART3_Right_BUF[USART3_MAG_RECV_LEN]; 	//��ȷ���ݽ��ջ���
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; //���ͻ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬���	


//extern u8 cmd_buffer[CMD_MAX_SIZE];//�����ָ���
void usart3_init(u32 bound);
//void  SendChar(u8 t);

#endif













