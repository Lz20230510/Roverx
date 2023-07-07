#include "I_O.h"
#include "delay.h" 
#include "FreeRTOS.h"
#include "task.h"






////GPIO 输入初始化  新板子输入输出
//void GPIO_IN_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|
//	RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG时钟
//	//输入的初始化操作
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11; //PORTB对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOD0,1,10,11
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3; //PORTC对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC0,2,3
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_12; //PORTD对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD11-15
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15; //PORTE对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 0-8,10,12,15
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|
//	GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_15; //PORTF对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF0,15
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15; //PORTG对应引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG0-5
//}

//GPIO 输入初始化  新板子输入输出
void GPIO_IN_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|
	RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG时钟
	//输入的初始化操作
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11; //PORTB对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOD0,1,10,11
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3; //PORTC对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC0,2,3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PORTD对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD11-15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15; //PORTE对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE 0-8,10,12,15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //PORTF对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF0,15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //PORTG对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG0-5
}

//GPIO 输出初始化
void GPIO_OUT_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOA,GPIOD,GPIOF,GPIOG,时钟

	//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOC,GPIOD,GPIOE,GPIOF,GPIOG时钟
	//输出的初始化操作
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4;			//PORTD对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOB3,4,10
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6| GPIO_Pin_7|
	GPIO_Pin_8;						//PORTF对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF1-10
	
	GPIO_SetBits(GPIOF,GPIO_Pin_11 | GPIO_Pin_12);//GPIOF11,F12设置高，灯灭
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_15;			//PORTG对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//GPIOG9,10,12,15
	


//LED 初始化操作
 //GPIOF11,F12初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO

////	

// //GPIOF9,F10初始化设置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//LED0和LED1对应IO口
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
//	
//	GPIO_SetBits(GPIOF,GPIO_Pin_11 | GPIO_Pin_12);//GPIOF9,F10设置高，灯灭
	
	
//	//继电器初始化操作与输出操作
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_3| GPIO_Pin_12|
//	GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	
//	GPIO_SetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);//GPIOD12,D13,D14设置高，灯灭
	
} 



  /**************************获取离散量GPIO状态值****************************/
//函数名形式：IOStatusDef GPIO_INOUT_Scan(void)
//输入传参：IOStatusDef ioStatus:	GPIOl离散量数据输入输出
//输出传参：IOStatusDef ioStatus:	GPIOl离散量数据输入输出
//返回值：VOID
//
//
/************************************************************************/
void GPIO_INOUT_Scan(void)
{
	u8 i;
	static u8 temp[4][9];				//IO输入状态临时量
	//IA离散量输入组数据读取
	IOStatus.IA[0]=0;
	temp[0][1]=!IA1;
	temp[0][2]=!IA2;
	temp[0][3]=!IA3;
	temp[0][4]=!IA4;
	temp[0][5]=!IA5;
	temp[0][6]=!IA6;
	temp[0][7]=!IA7;
	temp[0][8]=!IA8;
	
//	//IB离散量输入组数据读取
//	IOStatus.IB[0]=0;
//	temp[1][1]=!IB1;
//	temp[1][2]=!IB2;
//	temp[1][3]=!IB3;
//	temp[1][4]=!IB4;
//	temp[1][5]=!IB5;
//	temp[1][6]=!IB6;
//	temp[1][7]=!IB7;
//	temp[1][8]=!IB8;
	
	//IC离散量输入组数据读取
	IOStatus.IC[0]=0;
	temp[2][1]=!IC1;
	temp[2][2]=!IC2;
	temp[2][3]=!IC3;
	temp[2][4]=!IC4;
	temp[2][5]=!IC5;
	temp[2][6]=!IC6;
	temp[2][7]=!IC7;
	temp[2][8]=!IC8;
	
//	//ID离散量输入组数据读取
//	IOStatus.ID[0]=0;
//	temp[3][1]=!ID1;
//	temp[3][2]=!ID2;
//	temp[3][3]=!ID3;
//	temp[3][4]=!ID4;
//	temp[3][5]=!ID5;
//	temp[3][6]=!ID6;
//	temp[3][7]=!ID7;
//	temp[3][8]=!ID8;
	
	
	//IA组信号输入处理
	for(i=1;i<9;i++)					//信号消抖
	{
		if(temp[0][i]>0)				//当某个信号输入高电平时
		{
	//		vTaskDelay(5);				//等待5ms
			switch(i)
			{
				case 1: temp[0][i]=!IA1;break;
				case 2: temp[0][i]=!IA2;break;
				case 3: temp[0][i]=!IA3;break;
				case 4: temp[0][i]=!IA4;break;	
				case 5: temp[0][i]=!IA5;break;
				case 6: temp[0][i]=!IA6;break;
				case 7: temp[0][i]=!IA7;break;
				case 8: temp[0][i]=!IA8;break;
				default:break;
			}
			if(temp[0][i]>0) IOStatus.IA[i]=1;			
			else IOStatus.IA[i]=0;
		}
		else IOStatus.IA[i]=0;
	}
	
	
//	//IB组信号输入处理
//	for(i=1;i<9;i++)					//信号消抖
//	{
//		if(temp[1][i]>0)				//当某个信号输入高电平时
//		{
//	//		vTaskDelay(8);				//等待8ms
//			switch(i)
//			{
//				case 1: temp[1][i]=!IB1;break;
//				case 2: temp[1][i]=!IB2;break;
//				case 3: temp[1][i]=!IB3;break;
//				case 4: temp[1][i]=!IB4;break;
//				case 5: temp[1][i]=!IB5;break;
//				case 6: temp[1][i]=!IB6;break;
//				case 7: temp[1][i]=!IB7;break;
//				case 8: temp[1][i]=!IB8;break;                                              		
//				default:break;
//			}
//			if(temp[1][i]>0) IOStatus.IB[i]=1;			
//			else IOStatus.IB[i]=0;
//		}
//		else IOStatus.IB[i]=0;
//	}
	
		//IC组信号输入处理
	for(i=1;i<9;i++)					//信号消抖
	{
		if(temp[2][i]>0)				//当某个信号输入高电平时
		{
			vTaskDelay(8);				//等待8ms
			switch(i)
			{
				case 1: temp[2][i]=!IC1;break;
				case 2: temp[2][i]=!IC2;break;
				case 3: temp[2][i]=!IC3;break;
				case 4: temp[2][i]=!IC4;break;
				case 5: temp[2][i]=!IC5;break;
				case 6: temp[2][i]=!IC6;break;
				case 7: temp[2][i]=!IC7;break;
				case 8: temp[2][i]=!IC8;break;		
				default:break;
			}
			if(temp[2][i]>0) IOStatus.IC[i]=1;			
			else IOStatus.IC[i]=0;
		}
		else IOStatus.IC[i]=0; 
	}
	
//		//IC组信号输入处理
//	for(i=1;i<9;i++)					//信号消抖
//	{
//		if(temp[3][i]>0)				//当某个信号输入高电平时
//		{
//			vTaskDelay(8);				//等待8ms
//			switch(i)
//			{
//				case 1: temp[3][i]=!ID1;break;
//				case 2: temp[3][i]=!ID2;break;
//				case 3: temp[3][i]=!ID3;break;
//				case 4: temp[3][i]=!ID4;break;
//				case 5: temp[3][i]=!ID5;break;
//				case 6: temp[3][i]=!ID6;break;
//				case 7: temp[3][i]=!ID7;break;
//				case 8: temp[3][i]=!ID8;break;		
//				default:break;
//			}
//			if(temp[3][i]>0) IOStatus.ID[i]=1;			
//			else IOStatus.ID[i]=0;
//		}
//		else IOStatus.ID[i]=0; 
//	}

	//QE离散量输出
	 QE1=!IOStatus.QE[1];
	 QE2=!IOStatus.QE[2];
	 QE3=!IOStatus.QE[3];
	 QE4=!IOStatus.QE[4];
	 QE5=!IOStatus.QE[5];
	 QE6=!IOStatus.QE[6];
	 QE7=!IOStatus.QE[7];
	 QE8=!IOStatus.QE[8];
	
	//QF离散量输出
	 QF1=!IOStatus.QE[1];
	 QF2=!IOStatus.QE[2];
	 QF3=!IOStatus.QE[3];
	 QF4=!IOStatus.QE[4];
	 QF5=!IOStatus.QE[5];
	 QF6=!IOStatus.QE[6];
	 QF7=!IOStatus.QE[7];
	 QF8=!IOStatus.QE[8];
	 
//	 JD1=!IOStatus.JD[1];
//	 JD2=!IOStatus.JD[2];
//	 JD3=!IOStatus.JD[3];
	 
}









