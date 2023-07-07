#include "sys.h"
#include "usart6.h"
#include "HEAD.h"
//#include "mpu6050.h"
#include "mpu9250.h"
#include "vehicle.h" 




////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos 使用	  
//#endif
u8 REBUFF;
SEND_DATA Send_Data;
RECEIVE_DATA Receive_Data;
float Vz,Move_X;

uint8 cmd_buffer[CMD_MAX_SIZE];//大彩屏指令缓存

u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART6_SD_BUF[USART6_REC_LEN]; //发送缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART6_RX_STA=0;       //接收状态标记	


#if barry
#if EN_USART6_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART6_RX_STA=0;       //接收状态标记	
u8 mark=0;

//初始化IO 串口6 
//bound:波特率
void usart6_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟
 
	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOG14复用为USART6
	
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
	static u8 Count=0;
	u8 Usart_Receive;
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断
	{
		Usart_Receive =USART_ReceiveData(USART6);//(USART6->DR);	//读取接收到的数据			
		USART_SendData(USART6,Usart_Receive);
//		USART_SendData(USART6,accel[2]);	
//    if(Time_count<CONTROL_DELAY)
//		  //开机10秒前不处理数据
//		  return 0;	
		//串口数据填入数组
    Receive_Data.buffer[Count]=Usart_Receive;
		//确保数组第一个数据为FRAME_HEADER
		if(Usart_Receive == FRAME_HEADER||Count>0) 
			Count++; 
		else 
			Count=0;
		if (Count == 11)                            //验证数据包的长度
		{   
				Count=0;                                  //为串口数据重新填入数组做准备
				if(Receive_Data.buffer[10] == FRAME_TAIL) //验证数据包的帧尾
				{					
					AGVStatus.SLAMAutoMark=1;
					
			  }
		}
  }		  		 
} 
 

  
#endif	
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

//////SCM_345_contralCmd 本来位置




/**************************************************************************
Function: Calculates the check bits of data to be sent/received
Input   : Count_Number: The first few digits of a check; Mode: 0-Verify the received data, 1-Validate the sent data
Output  : Check result
函数功能：计算要发送/接收的数据校验结果
入口参数：Count_Number：校验的前几位数；Mode：0-对接收数据进行校验，1-对发送数据进行校验
返回  值：校验结果
**************************************************************************/
u8 Check_Sum(unsigned char Count_Number,unsigned char Mode)
{
	unsigned char check_sum=0,k;
	//对要发送的数据进行校验
	if(Mode==1)
	for(k=0;k<Count_Number;k++)
	{
	check_sum=check_sum^Send_Data.buffer[k];
	}
	//Verify the data received
	//对接收到的数据进行校验
	if(Mode==0)
	for(k=0;k<Count_Number;k++)
	{
	check_sum=check_sum^Receive_Data.buffer[k];
	}
	return check_sum;	
}


/**************************************************************************
Function: The data sent by the serial port is assigned
Input   : none
Output  : none
函数功能：串口发送的数据进行赋值
入口参数：无
返回  值：无
**************************************************************************/
void data_transition(void)
{
	Send_Data.Sensor_Str.Frame_Header = FRAME_HEADER; //Frame_header //帧头
	Send_Data.Sensor_Str.Frame_Tail = FRAME_TAIL;     //Frame_tail //帧尾
	
	//Forward kinematics solution, from the current speed of each wheel to calculate the current speed of the three axis
	//运动学正解，从各车轮当前速度求出三轴当前速度
	Send_Data.Sensor_Str.X_speed = (MotorMsg[0].RdSpd+MotorMsg[1].RdSpd)/MotorMsg[0].Ratio/2*1000; //小车x轴速度
	Send_Data.Sensor_Str.Y_speed = 0;
	Send_Data.Sensor_Str.Z_speed = (MotorMsg[1].RdSpd-MotorMsg[0].RdSpd)/MotorMsg[0].Ratio/Vehicle_W * 1000;//小车z轴速度
//	Send_Data.Sensor_Str.Z_speed = (MotorMsg[0].RdSpd+MotorMsg[1].RdSpd)/MotorMsg[0].Ratio/2    Vehicle_L * 1000;//小车z轴速度
	
	//The acceleration of the triaxial acceleration //加速度计三轴加速度
	Send_Data.Sensor_Str.Accelerometer.X_data= accel[1]; //The accelerometer Y-axis is converted to the ros coordinate X axis //加速度计Y轴转换到ROS坐标X轴
	Send_Data.Sensor_Str.Accelerometer.Y_data= accel[0]; //The accelerometer X-axis is converted to the ros coordinate y axis //加速度计X轴转换到ROS坐标Y轴
	Send_Data.Sensor_Str.Accelerometer.Z_data= accel[2]; //The accelerometer Z-axis is converted to the ros coordinate Z axis //加速度计Z轴转换到ROS坐标Z轴
	
	//The Angle velocity of the triaxial velocity //角速度计三轴角速度
	Send_Data.Sensor_Str.Gyroscope.X_data=  gyro[1]; //The Y-axis is converted to the ros coordinate X axis //角速度计Y轴转换到ROS坐标X轴
	Send_Data.Sensor_Str.Gyroscope.Y_data= -gyro[0]; //The X-axis is converted to the ros coordinate y axis //角速度计X轴转换到ROS坐标Y轴
	if(Send_Data.buffer[1]==0) 
		//If the motor control bit makes energy state, the z-axis velocity is sent normall
	  //如果电机控制位使能状态，那么正常发送Z轴角速度
		Send_Data.Sensor_Str.Gyroscope.Z_data=gyro[2];  
	else  
		//If the robot is static (motor control dislocation), the z-axis is 0
    //如果机器人是静止的（电机控制位失能），那么发送的Z轴角速度为0		
		Send_Data.Sensor_Str.Gyroscope.Z_data=0;  
	
	//Battery voltage (this is a thousand times larger floating point number, which will be reduced by a thousand times as well as receiving the data).
	//电池电压(这里将浮点数放大一千倍传输，相应的在接收端在接收到数据后也会缩小一千倍)
	Send_Data.Sensor_Str.Power_Voltage = 22.222*1000; 
	
	Send_Data.buffer[0]=Send_Data.Sensor_Str.Frame_Header; //Frame_heade //帧头
	
	if((MotorMsg[0].RdStatus==0x01||MotorMsg[0].RdStatus==0x02||MotorMsg[0].RdStatus==0x03)&&
		(MotorMsg[1].RdStatus==0x01||MotorMsg[1].RdStatus==0x02||MotorMsg[1].RdStatus==0x03))
		Send_Data.buffer[1]=1;                        //Car software loss marker //小车软件失能标志位
	else Send_Data.buffer[1]=0;
  

	
	//The three-axis speed of / / car is split into two eight digit Numbers
	//小车三轴速度,各轴都拆分为两个8位数据再发送
	Send_Data.buffer[2]=Send_Data.Sensor_Str.X_speed >>8; 
	Send_Data.buffer[3]=Send_Data.Sensor_Str.X_speed ;    
	Send_Data.buffer[4]=Send_Data.Sensor_Str.Y_speed>>8;  
	Send_Data.buffer[5]=Send_Data.Sensor_Str.Y_speed;     
	Send_Data.buffer[6]=Send_Data.Sensor_Str.Z_speed >>8; 
	Send_Data.buffer[7]=Send_Data.Sensor_Str.Z_speed ;    
	
	//The acceleration of the triaxial axis of / / imu accelerometer is divided into two eight digit reams
	//IMU加速度计三轴加速度,各轴都拆分为两个8位数据再发送
	Send_Data.buffer[8]=Send_Data.Sensor_Str.Accelerometer.X_data>>8; 
	Send_Data.buffer[9]=Send_Data.Sensor_Str.Accelerometer.X_data;   
	Send_Data.buffer[10]=Send_Data.Sensor_Str.Accelerometer.Y_data>>8;
	Send_Data.buffer[11]=Send_Data.Sensor_Str.Accelerometer.Y_data;
	Send_Data.buffer[12]=Send_Data.Sensor_Str.Accelerometer.Z_data>>8;
	Send_Data.buffer[13]=Send_Data.Sensor_Str.Accelerometer.Z_data;
	
	//The axis of the triaxial velocity of the / /imu is divided into two eight digits
	//IMU角速度计三轴角速度,各轴都拆分为两个8位数据再发送
	Send_Data.buffer[14]=Send_Data.Sensor_Str.Gyroscope.X_data>>8;
	Send_Data.buffer[15]=Send_Data.Sensor_Str.Gyroscope.X_data;
	Send_Data.buffer[16]=Send_Data.Sensor_Str.Gyroscope.Y_data>>8;
	Send_Data.buffer[17]=Send_Data.Sensor_Str.Gyroscope.Y_data;
	Send_Data.buffer[18]=Send_Data.Sensor_Str.Gyroscope.Z_data>>8;
	Send_Data.buffer[19]=Send_Data.Sensor_Str.Gyroscope.Z_data;
	
	//Battery voltage, split into two 8 digit Numbers
	//电池电压,拆分为两个8位数据发送
	Send_Data.buffer[20]=Send_Data.Sensor_Str.Power_Voltage >>8; 
	Send_Data.buffer[21]=Send_Data.Sensor_Str.Power_Voltage; 

  //Data check digit calculation, Pattern 1 is a data check
  //数据校验位计算，模式1是发送数据校验
	Send_Data.buffer[22]=Check_Sum(22,1); 
	Send_Data.buffer[23]=Send_Data.Sensor_Str.Frame_Tail; //Frame_tail //帧尾
}


/**************************************************************************
函数功能：串口6发送数据
入口参数：无
返回  值：无
**************************************************************************/
void USART6_SEND(void)
{
  unsigned char i = 0;
//  USART_SendData(USART6,Send_Data.buffer[14]);	
	for(i=0; i<24; i++)
	{
		usart6_send(Send_Data.buffer[i]);
	}	 
}


/**************************************************************************
函数功能：串口3发送数据
入口参数：要发送的数据
返回  值：无
**************************************************************************/
void usart6_send(u8 data)
{
	USART6->DR = data;
	while((USART6->SR&0x40)==0);	
}


void SLAM_AutoContral(void)
{
		if(Receive_Data.buffer[9] ==Check_Sum(9,0))	 
		{							
//		float Vz,Move_X;
			//从串口数据求三轴目标速度， 单位m/s
				Move_X=XYZ_Target_Speed_transition(Receive_Data.buffer[3],Receive_Data.buffer[4]);
				Vz    =XYZ_Target_Speed_transition(Receive_Data.buffer[7],Receive_Data.buffer[8]);						
							
				VehiclePar=Vehicle_count_Usart(Move_X,Vz);						//速度控制输出

				Steering_contral(VehiclePar.PwmWheel);			
				//TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
			
				Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);				//车轮速度控制
			}	
		AGVStatus.SLAMAutoMark=0;
}
//#endif



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


#if dacai

void usart6_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟
 
	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOG14复用为USART6
	
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
	USART_ClearFlag(USART6, USART_FLAG_TC);	
	
	
#if EN_USART6_RX
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif
	
}



void USART6_IRQHandler(void)
{
	u8 Res;
   if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET)//接收中断有效,若接收数据寄存器满
   {													
			uint8_t data = USART_ReceiveData(USART6);
			queue_push(data);
		 
			if((USART6_RX_STA&0x8000)==0)//接收未完成
			{
				if(USART6_RX_STA&0x4000)//接收到了0x0d
				{
					if(Res!=0x0a)USART6_RX_STA=0;//接收错误,重新开始
					else USART6_RX_STA|=0x8000;	//接收完成了 
				}
				else //还没收到0X0D
				{	
					if(Res==0x0d)USART6_RX_STA|=0x4000;
					else
					{
						USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res ;
						USART6_RX_STA++;
						if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			} 		 	 
	  }
}


#endif
void SendChar(uchar t)
{
    USART_SendData(USART6,t);
    while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET));//等待串口发送完毕
}