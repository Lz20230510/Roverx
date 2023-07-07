#include "sys.h"
#include "usart6.h"
#include "HEAD.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos 使用	  
//#endif
 
#if EN_USART6_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART6_RX_STA=0;       //接收状态标记	
u8 mark=0;

//初始化IO 串口6 
//bound:波特率
void usart6_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
 
	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource6,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource7,GPIO_AF_USART6); //GPIOG14复用为USART6
	
	//USART6端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOG9与GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10

   //USART6 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART6, ENABLE);  //使能串口1 
	
	
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
	
}

 void USART6_IRQHandler(void)                	//串口6中断服务程序
{
	u8 Res;
	u32 static RFID_valueStor;
	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART6);//(USART6->DR);	//读取接收到的数据			
		
//		if(AGVStatus.AUTOmode==1)				//室内模式
//		{
//			mark=1;
//			USART6_RX_BUF[USART6_RX_STA]=Res;
//			USART6_RX_STA++;
//			if(USART6_RX_STA>(RFID_REC_LEN-1))USART6_RX_STA=0;	
//			
//			StationCardNum.StationCardNumstor=0;							//站点卡号临时存储器清零
//			RFID_value=RFID_receive(USART6_RX_BUF);
//			if(RFID_valueStor!=RFID_value)													//当读取到不同卡号
//			{
//				Station_CardNum(RFID_value);																				//站点卡号获取	
//				StatnNum_Search(StationCardNum.cardNum,BranchPath.selectPathNum);				//搜索当前的卡号对应的分支路径数据	
//				BranchPath.selectPathNum=AGVPathJump_Cmd(BranchPath.jumpCmd,BranchPath.selectPathNum);		//路径跳转指令	
//			
//				BranchPath.uart4Mark=1;																										//读取到RFID信号标志
//				mark=2;
//			}		
//			RFID_valueStor=RFID_value;
//				
//		}
//		else if(AGVStatus.AUTOmode==2||AGVStatus.TeachMode==1)			//室外模式或者示教模式
//		{
//			if(Res==0x68)	USART6_RX_STA=0;								//当接收到0x68
//			USART6_RX_BUF[USART6_RX_STA]=Res;
//			USART6_RX_STA++;
//			if(USART6_RX_STA>(Smc345_REC_LEN-1))USART6_RX_STA=0;				//数据接收完成，清零
//			Smc345.scm345Angle =bcd_to_hex(USART6_RX_BUF);  
//		}
	} 
			 
} 





  
#endif	


///////////////////////////bcd码转浮点数//////////////////////////////
//函数名称：float bcd_to_hex(u8 * BCD)
//输入：BCD值
//输出：
//返回：浮点数
//////////////////////////////////////////////////////////////////
float bcd_to_hex(u8 * BCD)
{
	float temp;
	if (*BCD==0x68)
	{
		temp=(float) ((*(BCD+4)>>4)*10 + (*(BCD+4)&0x0f))*100;
		temp+=(float) ((*(BCD+5)>>4)*10 + (*(BCD+5)&0x0f));
		temp+=(float) ((*(BCD+5)>>4)*10 + (*(BCD+5)&0x0f))/100;
	}

  return temp;
}


///////////////////////陀螺仪数据发送//////////////////////////////
//函数名称：SCM_345_contralCmd(u8 lenth,u8 * cmd)
//输入：u8 lenth:数据长度   u8 * cmd：指令
//输出：
//返回：浮点数
//////////////////////////////////////////////////////////////////
void SCM_345_contralCmd(u8 lenth,u8 * cmd)
{
	u8 t;
	for(t=0;t<lenth;t++)
	{
		USART_SendData(USART6, cmd[t]);         //向串口1发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}

}




