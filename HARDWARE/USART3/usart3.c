#include "HEAD.h" 
#include "stdarg.h"
#include "usart3.h" 

#include "stdio.h"	 	 
#include "string.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F4开发板
//串口3驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/8/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	
//串口发送缓存区 	
__align(8) 
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
#ifdef USART3_RX_EN   								//如果使能了接收   	  
//串口接收缓存区 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
//u8 USART3_Right_BUF[USART3_MAG_RECV_LEN]; 		//正确数据接收缓冲
//u16 USART3_RX_STA=0; 
//通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART3_RX_STA=0;   	 
void USART3_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 
 
	res =USART_ReceiveData(USART3);		
	if((USART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
	{ 
		if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//还可以接收数据
		{
			TIM_SetCounter(TIM7,0);//计数器清空        				 
			if(USART3_RX_STA==0)		
				TIM_Cmd(TIM7, ENABLE);  //使能定时器7 
			USART3_RX_BUF[USART3_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			USART3_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}  	
 }										 
}  
#endif	



void usart3_init(u32 bound)
	{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PB10，PB11

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
  USART_Cmd(USART3, ENABLE);  //使能串口3 
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;							//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;				//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;							//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																//根据指定的参数初始化VIC寄存器、

	TIM7_Int_Init(1000-1,8400-1);		//100ms中断
	USART3_RX_STA=0;		//清零
	TIM_Cmd(TIM7, DISABLE); //关闭定时器7
	
	}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);  //等待上次传输完成 
		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]); 	 //发送数据到串口3 
	}
	
}







////串口3,printf 函数
////确保一次发送数据不超过USART3_MAX_SEND_LEN字节
//void u3_printf(char* fmt,...)  
//{  
////	u16 i,j;
////	va_list ap;
////	va_start(ap,fmt);
////	vsprintf((char*)USART3_TX_BUF,fmt,ap);
////	va_end(ap);
////	i=strlen((const char*)USART3_TX_BUF);//此次发送数据的长度
////	for(j=0;j<i;j++)//循环发送数据
////	{
////	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);  //等待上次传输完成 
////		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]); 	 //发送数据到串口3 
////	}
////	
//}






