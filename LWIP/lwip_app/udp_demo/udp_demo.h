#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H
#include "sys.h"
#include "FreeRTOS.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//NETCONN API��̷�ʽ��UDP���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
 
#define UDP_DEMO_RX_BUFSIZE		2000	//����udp���������ݳ���
#define UDP_DEMO_PORT			8089	//����udp���ӵı��ض˿ں�
#define LWIP_SEND_DATA			0X80    //���������ݷ���


extern u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP�������ݻ�����
extern const u8 *udp_demo_sendbuf;//UDP������������
extern u8 udp_flag;		//UDP���ݷ��ͱ�־λ



BaseType_t udp_demo_init(void);
#endif

