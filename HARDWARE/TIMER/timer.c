#include "timer.h"
#include "led.h"
//#include "HMI_cmd.h"
#include "myiic.h"
#include "24l01.h"
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


extern u16 USART3_RX_STA;


//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	
	u8 i;
	LED0=!LED0;
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//是更新中断
	{	 			   		
		if(Tim7Add.tiAdd<255)Tim7Add.tiAdd++;//溢出中断		time7  1s定时器定时
		else Tim7Add.tiAdd=0;						//清零	
		
		for(i=0;i<6;i++)												//定时器7_0--	定时器7_5			
		{
			Tim7Add.mark[i]=Time7_Timekeep(Tim7Add.time[i],Tim7Add.tiAdd,Tim7Add.start[i],i,0);						//定时器7_0
			if(Tim7Add.mark[i]==1&&Tim7Add.start[i]==1)
			{
				Tim7Add.time[i]=0;															//起始时间清零
				Tim7Add.start[i]=0;															//起始时间清零
			}			
		}		
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIM7更新中断标志  	
}
 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7时钟使能    
	
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时器7
	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}
	 


void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //允许定时器7更新中断
	TIM_Cmd(TIM6,ENABLE); //使能定时器6
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器6中断服务函数
void TIM6_DAC_IRQHandler(void)
{
	u8 static Tim6_mark;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //溢出中断
	{
		LCD_mark=!LCD_mark;
		if(Tim6_mark>=5)
		{
			Tim6_mark=0;
			if(AGVStatus.AUTOmode!=2&&AGVStatus.TeachMode==0&&AGVStatus.MapDownMode==0)  Battery_Send(7,Ask_Battery);							//向电池发送数据  当非室外模式和示教模式工作					
		}		
		Tim6_mark++;		

	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //清除中断标志位

}

//**********************Time7定时器**************************/
//u8 Time7_Timekeep(u8 time,u8 timekeep,u8 start,u8 reset)
//输入参数：u8 time：计时时间    u8 timekeep：Tim7Add,累加器全局变量   u8 reset:计时器复位  =1：复位  =0：不做复位处理
//u8 start：定时器开始
//输出参数：
//返回：
/******************************************************/
u8 Time7_Timekeep(u8 time,u8 timekeep,u8 start,u8 Num,u8 reset)
{	
	static u16 timeStart[8];						//计时器起始时间
	static u16 timeStop[8];						//计时器停止时间
	static u8 lastUpmark[8];								//上升沿标记保存
	u8 temp=0;
	if(reset!=1)
	{
		if(start==1&&lastUpmark[Num]!=start)	//
		{
			timeStart[Num]=timekeep;				//当计时开始，存储当前的time7累加器值
			if(timeStart[Num]+time>255) timeStop[Num]=timeStart[Num]+time-255;
			else timeStop[Num]=timeStart[Num]+time;
		}
		else
		{
			if(timekeep!=timeStop[Num])									//当没有到计时时间
			{
				temp=0;			
			}
			else //当到了计时时间
			{
				lastUpmark[Num]=0;									        //上升沿存储标记清零
				timeStart[Num]=0;													//起始值清零
				timeStop[Num]=0;														//停止值清零
				temp=1;								
			}	
		}	
		lastUpmark[Num]=start;							    //保存上一次状态
	}
	else
	{
		lastUpmark[Num]=0;									        //上升沿存储标记清零
		timeStart[Num]=0;													//起始值清零
		timeStop[Num]=0;														//停止值清零
		temp=0;
		Tim7Add.mark[Num]=0;
		Tim7Add.start[Num]=0;
	}	
	return temp;
}

