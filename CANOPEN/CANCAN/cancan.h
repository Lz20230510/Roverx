#ifndef __CANCAN_H	 
#define __CANCAN_H	 
#include "sys.h"	    
#include "can.h"   
	
////CAN1����RX0�ж�ʹ��
//#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    
//										 							 				    
//u8 can_init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
// 
//u8 can_send_msg(Message *msg);						//��������

//u8 CAN1_Receive_Msg(u8 *buf);							//��������
//#endif




	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��

u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������

u8 CAN1_Receive_Msg(u8 *buf);							//��������

u8 can_send_msg(Message *msg);						//��������

void CANOPEN_Node_Start(void);						//canopen�ڵ�����

void CANOPEN_Reset_comunication(void);			//�ڵ㸴λ

#endif














