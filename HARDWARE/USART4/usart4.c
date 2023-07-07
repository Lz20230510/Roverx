//#include "sys.h"
//#include "usart4.h"	
//#include "HEAD.h"
//#include "Motor.h"
//#include "Vehicle.h"
//////////////////////////////////////////////////////////////////////////////////// 	 
////如果使用ucos,则包括下面的头文件即可.
//#if SYSTEM_SUPPORT_OS
//#include "FreeRTOS.h"					//FreeRTOS使用	 
////#include "mpu6050.h"
//#endif
////////////////////////////////////////////////////////////////////////////////////	 
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK STM32F4探索者开发板
////串口1初始化		   
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2014/6/10
////版本：V1.5
////版权所有，盗版必究。
////Copyright(C) 广州市星翼电子科技有限公司 2009-2019
////All rights reserved
////********************************************************************************
////V1.3修改说明 
////支持适应不同频率下的串口波特率设置.
////加入了对printf的支持
////增加了串口接收命令功能.
////修正了printf第一个字符丢失的bug
////V1.4修改说明
////1,修改串口初始化IO的bug
////2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
////3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
////4,修改了EN_USART1_RX的使能方式
////V1.5修改说明
////1,增加了对UCOSII的支持
//////////////////////////////////////////////////////////////////////////////////// 	  
// 

//////////////////////////////////////////////////////////////////////
//////加入以下代码,支持printf函数,而不需要选择use MicroLIB	  

//BluetoothDef Bluetooth;
////#if 1
////#pragma import(__use_no_semihosting)             
//////标准库需要的支持函数                 
////struct __FILE 
////{ 
////	int handle; 
////}; 

////FILE __stdout;       
//////定义_sys_exit()以避免使用半主机模式    
////void _sys_exit(int x) 
////{ 
////	x = x; 
////} 
//////重定义fputc函数 
////int fputc(int ch, FILE *f)
////{ 	
////	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
////	USART2->DR = (u8) ch;      
////	return ch;
////}
////#endif
// 
//#if EN_UART4_RX   //如果使能了接收
////串口1中断服务程序
////注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u8 UART4_SD_BUF[UART4_REC_LEN];			//发送缓冲区 ，最大USART_REC_LEN_S个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
//u16 UART4_RX_STA=0;       //接收状态标记	


//void uart4_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:抢占优先级，u8 SubPriority子优先级
//{
//  //GPIO端口设置
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能USART1时钟
//	
//	
// 
//	//串口1对应引脚复用映射
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); //GPIOA9复用为USART1
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); //GPIOA10复用为USART1
//	
//	//USART1端口配置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0; //GPIOA9与GPIOA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

//   //USART1 初始化设置
//	USART_InitStructure.USART_BaudRate = bound;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//  USART_Init(UART4, &USART_InitStructure); //初始化串口1
//	
//  USART_Cmd(UART4, ENABLE);  //使能串口1 
//	
//#if EN_UART4_RX	
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

//	//Usart1 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

//#endif

//}



//void UART4_IRQHandler(void)                	//串口2中断服务程序
//{
//	u8 Res;
//	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res =USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
////    USART_SendData(USART2,Res);
//		if((Res>=0x41&&Res<=0x4A) || (Res>=0x58&&Res<=0x5A))	Bluetooth.buff=Res;
//		else Bluetooth.value=0x00;			
//		
//		if(Res==0x58 && VehicleSpd<=0.5) VehicleSpd=VehicleSpd+0.1;
//		if(Res==0x59 && VehicleSpd>=0.0) VehicleSpd=VehicleSpd-0.1;
//		
//	} 
//} 
//#endif	
///**************************蓝牙遥控器控制********************* 
////函数名：u8 HC_05_Trans(u8 res)
////输入参数：u8 res：串口数据
////返回值：0
//---------------------------------------------------
//0x00	 ：   无指令，停车			
//---------------------------------------------------
//0x41	A：		前进
//---------------------------------------------------
//0x45	E：		后退
//---------------------------------------------------
//0x47	G：		左转
//---------------------------------------------------
//0x43	C：		右转
//---------------------------------------------------
//0x48	H：		左前
//---------------------------------------------------
//0x42	B：		右前
//---------------------------------------------------
//0x46	F：		左后
//---------------------------------------------------
//0x44	D：		右后
//---------------------------------------------------
//0x4A 	J：		遥感模式
//---------------------------------------------------
//0x58	X：		速度+
//---------------------------------------------------
//0x59	Y：		速度-
//---------------------------------------------------
//---------------------------------------------------
//****************************************************************/ 

//u8 HC_05_Trans(u8 res)
//{
//	switch(res)
//	{		
//		case 0x41:	//前进
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,90);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x41;
//		}break;			
//		case 0x45:	//后退
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,90);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x45;
//		}break;			
//		case 0x47:	//左转
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,97.63);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x47;
//		}break;			
//		case 0x43:	//右转
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,82.37);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x43;
//		}break;			
//		case 0x48:	//左前
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,95.04);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x48;
//		}break;			
//		case 0x42:	//右前
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,85);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x42;
//		}break;			
//		case 0x46:	//左后
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,97.63);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x46;
//		}break;			
//		case 0x44:	//右后
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,82.37);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x44;
//		}break;			
//		case 0x4A:	//遥杆模式
//		{
//		
//			return 0x4A;
//		}break;			
//		case 0x58:	//速度加
//		{
//			
//			return 0x58;
//		}break;			
//		case 0x59:	//速度减
//		{
//			
//			return 0x59;
//		}break;	
//		case 0x5A:	//停止
//		{
//			//TIM_SetCompare1(TIM4,Median);	//修改比较值，修改占空比
//			Wheel_control(0,0,MotorMsg);
//			return 0x59;
//		}break;	
//		default:			//空指令
//		{
//			TIM_SetCompare1(TIM4,Median);	//修改比较值，修改占空比
//			Wheel_control(0,0,MotorMsg);
//			return 0x00;
//		}break;				
//		
//	}
//}
///**************************************************************************
//Function: Serial port 1 sends data
//Input   : The data to send
//Output  : none
//函数功能：串口1发送数据
//入口参数：要发送的数据
//返回  值：无
//**************************************************************************/
////void usart1_send(u8 data)
////{
////	USART1->DR = data;
////	while((USART1->SR&0x40)==0);	
////}//以前程序 2023.3.28

////void usart2_send(u8 data)
////{
////	USART2->DR = data;
////	while((USART2->SR&0x40)==0);	
////}



///************************名称电量询问发送程序*********************/
////void Battery_Send(u8 len,u8 *cmd)
////输入参数：u8 len：数据长度，      u8 *cmd：发送的数据
////
///*****************************************************************/
//void Battery_Send(u8 len,u8 *cmd)
//{
//	u8 t;
//	for(t=0;t<len;t++)
//	{
//		USART_SendData(USART1, cmd[t]);         //向串口1发送数据
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//	}
//}


///************************获取电池状态信息数据************************/
////函数形式：u8 Battery_Capacity(u8 * cmd)
////输入参数：串口数据
////返回值：电量信息参数
////
///****************************************************************/
//BatteryMsgDef Battery_Capacity(u8 * cmd)
//{
//	u8 i;
//	u16 static checkStor=0;
//	u16 checkTemp;
//	BatteryMsgDef static batteryMsg;
//	union MultiType static multiType;
//	
//	checkStor=0;
//	if(cmd[0]==0xDD && cmd[33]==0x77)			//当首帧尾帧正确
//	{
//		for(i=2;i<31;i++)
//		{
//			checkStor +=(u16)cmd[i];				//数据内容累加
//		}
//		checkStor=(checkStor^0xFFFF)+1;		//求校验
//		checkTemp=((u16)cmd[31]<<8)+(u16)cmd[32];			//接收数据的校验值
//		
//		if(checkStor==checkTemp)			//当校验正确，则数据可以发送给参数存储结构体
//		{
//			
//			
//			multiType.float_=((u16)cmd[4]<<8) + (u16)cmd[5];			//总电压
//			batteryMsg.total_Vot=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[6]<<8) + (u16)cmd[7];				//电流
//			batteryMsg.current=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[8]<<8) + (u16)cmd[9];						//剩余容量
//			batteryMsg.SOC=multiType.float_/100;	

//			
//			batteryMsg.cycle=((u16)cmd[12]<<8) + (u16)cmd[13];						//充电循环次数
//			batteryMsg.protectStatus=((u16)cmd[20]<<8) + (u16)cmd[21];						//保护状态
//			batteryMsg.RSOC=(int)cmd[23];					//电池剩余容量百分比
//		}		
//	}
//	return batteryMsg;
//}


//u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num)
//{
//	u8 lenth=0;
//	char static temp_str[130];
//	sprintf(&temp_str[0],"%.1lf",OA);
//	VehicleParamt.OA_ve=&temp_str[0];
//	sprintf(&temp_str[15*1],"%.1lf",OB);
//	VehicleParamt.OB_ve=&temp_str[15*1];
//	sprintf(&temp_str[15*2],"%.2f",angle);
//	VehicleParamt.angle=&temp_str[15*2];
//	
//	sprintf(&temp_str[15*3],"%.5lf",lont_du);
//	VehicleParamt.longti=&temp_str[15*3];
//	sprintf(&temp_str[15*4],"%.5lf",lat_du);
//	VehicleParamt.lati=&temp_str[15*4];
//	sprintf(&temp_str[15*5],"%.1f",angle_near);
//	VehicleParamt.angle_near=&temp_str[15*5];
//	sprintf(&temp_str[15*6],"%.1lf",OA_near);
//	VehicleParamt.OA_near=&temp_str[15*6];
//	sprintf(&temp_str[15*7],"%.1lf",OB_near);
//	VehicleParamt.OB_near=&temp_str[15*7];
//	sprintf(&temp_str[15*8],"%d",num);
//	VehicleParamt.OAOB_num=&temp_str[15*8];
//	
//	
//	printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
//	VehicleParamt.fir_fram,VehicleParamt.OA_ve,VehicleParamt.OB_ve,
//	VehicleParamt.angle,VehicleParamt.longti,VehicleParamt.lati,
//	VehicleParamt.angle_near,VehicleParamt.OA_near,VehicleParamt.OB_near,VehicleParamt.OAOB_num,
//	VehicleParamt.last_fram);
//	printf("\r\n");
//	UART4_RX_STA=0;				
//	
//	return lenth;
//}


/////******************* 接收上位机数据，并根据上位机报文，进行相关操作*******************************/
//////函数形式：MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
//////输入参数：u8 * usartBuff：串口BUFF，  u16 len:数据长度信息 
//////返回参数：数据编号
//////函数功能：接收上位机数据，并根据上位机报文，进行相关操作    =0：无操作   =1：存储地图   =2：地图存储区格式化  =3：发送电子地图基本信息  =254:无任何任务
/////*********************************************************************/
////MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
////{

////	u16 i,k,j=0;
////	int mode;
////	u8 lenth,stor[8];//数据长度,逗号位置存储
////	char static temp[20];
////	char buff;	
////	MapPotDef static mapPot,mapPotTemp;
////	union MultiType multiType;
////	
////	buff=usartBuff[3];
////	sscanf((char*)&buff,"%d",&mode);						//获取上位机发送的数据类型
////	j=0;	
////	
////	for(i=0;i<len;i++)				//获取逗号位置
////	{
////		buff=usartBuff[i];			//获取单个字符数据
////		if(buff==',')
////		{
////			stor[j]=i;
////			j++;
////		}		
////	}		
////	switch(mode)
////	{
////		case 0: break;
////		case 1:											//发送回来的是OA OB angle数据   //例子：FR,1,-33.6193923403310,0149.835464517472,-0289.516314197572,1,OR   格式:FR,功能码,点（OA值）,点（OB值）,点（航向角）,点编号,OR
////		{
////			k=0;
////			j=1;
////			memset(temp,0,strlen(temp));				
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
////			sscanf(temp,"%f",&mapPot.OA);						//获取上位机发送的数据类型    获得OA
////			
////			j=2;
////			memset(temp,0,strlen(temp));
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
////			sscanf(temp,"%f",&mapPot.OB);						//获取上位机发送的数据类型		获得OB

////			j=3;
////			memset(temp,0,strlen(temp));
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
////			sscanf(temp,"%f",&mapPot.angle);						//获取上位机发送的数据类型	获得angle

////			j=4;
////			memset(temp,0,strlen(temp));
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
////			sscanf(temp,"%d",&mapPot.Num);						//获取上位机发送的数据类型		获得地图位置点信息		

////			mapPot.mode=(u8)mode;				//任务模式
////		}break;	
////		case 2:											//地图存储区格式化   //格式  :FR,功能码,OR
////		{		
////			mapPot.mode=(u8)mode;				//任务模式
////		}break;
////		case 3:											//发送电子地图基本信息 //例子：FR,3,118.4652105558884,31.376517521623,2600,20,OR   格式：FR,功能码,经度值(度格式),维度值(度格式),地图上总的点数量,每个直线段上点数量,OR
////		{
////			k=0;
////			j=1;
////			memset(temp,0,strlen(temp));				
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
////			sscanf(temp,"%lf",&mapPot.longti_du);						//获取上位机发送的数据类型    获得经度数据（度格式）
////			multiType.double_=mapPot.longti_du;							
////			Paramt_BUFF[Pg_OutsideParmt][1]=multiType.u32Array_[0];				//double类型数据低32位
////			Paramt_BUFF[Pg_OutsideParmt][2]=multiType.u32Array_[1];				//double类型数据高32位
////			
////			
////			j=2;
////			memset(temp,0,strlen(temp));				
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
////			sscanf(temp,"%lf",&mapPot.lati_du);						//获取上位机发送的数据类型    获得维度数据（度格式）
////			multiType.double_=mapPot.lati_du;		
////			Paramt_BUFF[Pg_OutsideParmt][3]=multiType.u32Array_[0];				//double类型数据低32位
////			Paramt_BUFF[Pg_OutsideParmt][4]=multiType.u32Array_[1];				//double类型数据高32位
////			
////			j=3;
////			memset(temp,0,strlen(temp));
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
////			sscanf(temp,"%d",&mapPot.grossNumPot);						//获取上位机发送的数据类型		获得地图上总的点数量,
////			multiType.u32_=mapPot.grossNumPot;
////			Paramt_BUFF[Pg_OutsideParmt][10]=multiType.u32_;			//导入参数缓冲区
////			
////			
////			j=4;
////			memset(temp,0,strlen(temp));
////			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
////			sscanf(temp,"%d",&mapPot.numPot);						//获取上位机发送的数据类型		获得每个直线段上点数量,
////			multiType.u32_=mapPot.numPot;
////			Paramt_BUFF[Pg_OutsideParmt][11]=multiType.u32_;			//导入参数缓冲区

////			mapPot.mode=(u8)mode;				//任务模式
////		}break;
////		default:
////		{
////			mapPot.mode=254;
////		}break;
////	}
////	return 	mapPot;
////}



/////*******************************地图下载程序********************************/
//////函数形式：void Map_Download(void)
//////输入参数：
//////输出参数：
//////返回参数：
//////函数说明：地图下载相关函数
/////*************************************************************************/
////void Map_Download(void)
////{
////	switch(MapPot.mode)
////	{
////		case 0: break;					//
////		case 1:									//电子地图下载		
////		{
////					//Map_Save2FLASH(MapPot,FloatSize);		//地图数据存储	
////			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//当存储区数据为 0xFFFFFFFF,才能存储地图数据	
////			MapPot.mode=0;						//模式关闭，不在存储
////		}break;
////		case 2:									//电子地图参数存储区格式化
////		{
////			SetScreen(25);														//初始化完成，切换画面初始化
////			STMFLASH_Clear(MAPOA_Addr,1);								//OA存储区总清
////			STMFLASH_Clear(MAPOB_Addr,1);								//OB存储区总清
////			STMFLASH_Clear(MAPAngle_Addr,1);								//angle存储区总清
////			SetScreen(4);														//人机界面初始化完成，切换回参数运行3
////			MapPot.mode=0;						//模式关闭，不在存储
////		}break;
////		case 3:									//电子地图初始数据存储
////		{			
////			MapPot.mode=0;						//模式关闭，不在存储
////		}break;
////		default:break;
////	}	
////}


/////*******************************地图错误检查***************************/
//////函数形式：u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
//////输入参数：u16 lastPotNum:下载的最后一个点值，OAOB极限值
//////返回参数：返回错误点的数量
//////函数说明：对地图存储区的地图进行错误检查
//////
//////
/////*********************************************************************/
////u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
////{
////	MapPotDef mapPot;
////	u16 errNum,i;			//错误点数量
////	for(i=1;i<lastPotNum+1;i++)
////	{	
////		mapPot=Map_ReadFromFLASH(i,4);				//读取电子地图存储区数据
////		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) errNum++;					//当发现有错误地图信息，则错误值累加	
////	}
////	return errNum;
////}



/////***************************电子地图错误点修复*************************/
//////函数形式：void MapTable_repair(u16 lastPotNum,float OAOBLimt)
//////输入参数：u16 lastPotNum:下载的最后一个点值，OAOB极限值
//////返回参数：
//////函数说明：对地图存储区的错误地图进行修复
//////
//////
/////*********************************************************************/
////void MapTable_repair(u16 lastPotNum,float OAOBLimt)
////{
////	MapPotDef mapPot;
////	u16 i;			
////	for(i=1;i<lastPotNum+1;i++)
////	{	
////		mapPot=Map_ReadFromFLASH(i,4);			//读取存储区地图数据
////		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) 	//当发现有错误地图信息，开始异常地图信息修复
////		{
////			if(i==1)				//当异常点位第一组数据
////			{
////				for(i=2;i<lastPotNum+1;i++)   //第一个数据往后检查还有错误点
////				{
////					mapPot=Map_ReadFromFLASH(i,4);			//读取存储区地图数据
////					if(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt) 	//当发现错误地图无异常，则将该地图信号组赋值到第一组数据
////					{
////						mapPot.Num=mapPot.Num-1;
////						Map_Save2FLASH(mapPot,FloatSize);		//当存储区数据为 0xFFFFFFFF,才能存储地图数据
////						break;			//跳出循环
////					}
////				}		
////			}
////			else
////			{
////				mapPot=Map_ReadFromFLASH(i-1,4);			//读取存储区地图数据
////				mapPot.Num=mapPot.Num+1;
////				Map_Save2FLASH(mapPot,FloatSize);		//当存储区数据为 0xFFFFFFFF,才能存储地图数据							
////			}				
////		}
////	}	
////	MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);				//地图错误点数量
////}































////#include "H.h"

////uint8 cmd_buffer[CMD_MAX_SIZE];//大彩屏指令缓存

////u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
////u8 USART2_SD_BUF[USART2_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//////接收状态
//////bit15，	接收完成标志
//////bit14，	接收到0x0d
//////bit13~0，	接收到的有效字节数目
////u16 USART2_RX_STA=0;       //接收状态标记	




////void usart2_init(u32 bound)
////{
////   //GPIO端口设置
////  GPIO_InitTypeDef GPIO_InitStructure;
////	USART_InitTypeDef USART_InitStructure;
////	NVIC_InitTypeDef NVIC_InitStructure;
////	
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//// 
////	//串口1对应引脚复用映射
////	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
////	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1
////	
////	//USART1端口配置
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
////	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

////   //USART1 初始化设置
////	USART_InitStructure.USART_BaudRate = bound;//波特率设置
////	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
////	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
////	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
////	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
////	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
////  USART_Init(USART2, &USART_InitStructure); //初始化串口1
////	
////  USART_Cmd(USART2, ENABLE);  //使能串口1 
////	USART_ClearFlag(USART2, USART_FLAG_TC);	
////	
////	
////#if EN_USART2_RX
////	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

////	//Usart1 NVIC 配置
////  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级0
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
////	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
////#endif
////	
////}



////void USART2_IRQHandler(void)
////{
////	u8 Res;
////   if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//接收中断有效,若接收数据寄存器满
////   {													
////			uint8_t data = USART_ReceiveData(USART2);
////			queue_push(data);
////		 
////			if((USART2_RX_STA&0x8000)==0)//接收未完成
////			{
////				if(USART2_RX_STA&0x4000)//接收到了0x0d
////				{
////					if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
////					else USART2_RX_STA|=0x8000;	//接收完成了 
////				}
////				else //还没收到0X0D
////				{	
////					if(Res==0x0d)USART2_RX_STA|=0x4000;
////					else
////					{
////						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
////						USART2_RX_STA++;
////						if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
////					}		 
////				}
////			} 		 	 
////	  }
////}

////void SendChar(uchar t)
////{
////    USART_SendData(USART2,t);
////    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
////    while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET));//等待串口发送完毕
////}