#ifndef __CANCAN_H	 
#define __CANCAN_H	 
#include "sys.h"	    
#include "can.h"   
	
////CAN1接收RX0中断使能
//#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
//										 							 				    
//u8 can_init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
// 
//u8 can_send_msg(Message *msg);						//发送数据

//u8 CAN1_Receive_Msg(u8 *buf);							//接收数据
//#endif




	
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化

u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

u8 can_send_msg(Message *msg);						//发送数据

void CANOPEN_Node_Start(void);						//canopen节点启动

void CANOPEN_Reset_comunication(void);			//节点复位

#endif














