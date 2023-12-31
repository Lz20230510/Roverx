#include "pwm.h"
#include "HEAD.h"
#include "usart.h"
//#include "pstwo.h" 
#include "pwm.h" 
#include "AGV_contr.h" 
#include "vehicle_key.h" 
#include "RAD.h" 
#include "24l01.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器PWM，TIM2_CH1~4驱动4个轮毂电机，TIM3_CH1驱动转向电机   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

float spdTemp=0;
u8 markTemp=0;
/********************************************************************************************************************************************
函数名: TIM4_CH1_PWM_Init(u32 arr,u32 psc)
描述:   TIM4_CH1_PWM控制左前轮毂电机	 
输入参数:自动重装载参数，预分频参数	 
返回值:
*********************************************************************************************************************************************/
void TIM4_CH1_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //使能PORTC时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);            //GPIOC6复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                        //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //初始化PC6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                            //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //初始化定时器4
	
	//初始化TIM3 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //使能TIMB在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);                                           //使能TIM4	
}



/********************************************************************************************************************************************
函数名: TIM4_CH2_PWM_Init(u32 arr,u32 psc)
描述:TIM4_CH4_PWM控制左后轮毂电机	 
输入参数:自动重装载参数，预分频参数	 
返回值:
*********************************************************************************************************************************************/
void TIM4_CH2_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);            //GPIOB7复用为定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                        //GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //初始化PB7
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                            //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //初始化定时器4
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //使能TIM3在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);                                           //使能TIM4	
}


/********************************************************************************************************************************************
函数名: TIM4_CH3_PWM_Init(u32 arr,u32 psc)
描述:TIM4_CH3_PWM控制右前轮毂电机	 
输入参数:自动重装载参数，预分频参数	 
返回值:
*********************************************************************************************************************************************/
void TIM4_CH3_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);            //GPIOB8复用为定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                        //GPIOB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //初始化PC8
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                            //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //初始化定时器4
	
	//初始化TIM4 Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //使能TIM4在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);                                           //使能TIM4	
}

/********************************************************************************************************************************************
函数名: TIM4_CH4_PWM_Init(u32 arr,u32 psc)
描述:TIM4_CH4_PWM控制右后轮毂电机	 
输入参数:自动重装载参数，预分频参数	 
返回值:
*********************************************************************************************************************************************/
void TIM4_CH4_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);            //GPIOC9复用为定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                        //GPIOC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //初始化PC9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;                            //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //初始化定时器4
	
	//初始化TIM3 Channel4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);  

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //使能TIM4在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPE使能 
	
	TIM_Cmd(TIM4,ENABLE);                                           //使能TIM4	
}


/********************************************************************************************************************************************
函数名: TIM3_CH4_PWM_Init(u32 arr,u32 psc)
描述:TIM3_CH4_PWM控制右后轮毂电机	 
输入参数:自动重装载参数，预分频参数	 
返回值:
*********************************************************************************************************************************************/

void DIR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟

  //GPIOB6,B7初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);//GPIOF9,F10设置高，灯灭
}


/********************************************************************************************************************************************
函数名: u16 Speed_trans(u16 spped_in)
描述:   u16 Speed_trans(u16 spped_in):电机实际转速转换
输入参数:给定的电机转速 
返回值: 校正后的arr值                       转换公式：X=(y+0.6255)/0.9986
*********************************************************************************************************************************************/
u16 Speed_trans(u16 spped_in)
{
	u16 y;
	y=spped_in;
	return (y+0.6255)/0.9986;
}

/********************************************************************************************************************************************
函数名: void Time_CH1234_Init(void)
描述:    void Time_CH1234_Init(void):四个电机PWM初始化


*********************************************************************************************************************************************/
void Time_CH1234_Init(void)
{
	TIM4_CH1_PWM_Init(5000-1,84-1); //初始化TIM2 CH1，84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
	TIM4_CH2_PWM_Init(5000-1,84-1); //初始化TIM2 CH2，84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 	 	
	TIM4_CH3_PWM_Init(5000-1,84-1); //初始化TIM2 CH3，84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
	TIM4_CH4_PWM_Init(5000-1,84-1); //初始化TIM2 CH4，84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz. 
}


/********************************************************************************************************************************************
函数名: void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode)
描述:    Time_CH1234_Init(float spd_lf,float spd_lb,float spd_rf,float spd_rb):小车四轮转速设定   单位：米/秒   m/s
输入参数:spd_lf：左前轮转速   spd_lb：左后轮转速   spd_rf：右前轮转速    spd_rb：右后轮转速 ,u8 mode:
输入参数:,u8 mode: =0  输入速度值为单位 m/s              =1：输入速度值为 r/min
*********************************************************************************************************************************************/
void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode)
{

	float static spd_lf,spd_lb,spd_rf,spd_rb;
	u16 static spd_lf1,spd_lb1,spd_rf1,spd_rb1;
	u16 static temp[4]={0,0,0,0};
	
	spd_lf= spd_lf2;
	spd_lb= spd_lb2;
	spd_rf= spd_rf2;
	spd_rb= spd_rb2;
		
	if(spd_lf<0) spd_lf=-spd_lf;
	if(spd_lb<0) spd_lb=-spd_lb;
	if(spd_rf<0) spd_rf=-spd_rf;
	if(spd_rb<0) spd_rb=-spd_rb;
	
	if(!mode)					//当控制模式为0，及输入值为m/s
	{
		spd_lf1=spd_lf*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;       //左前轮速度强制转换		单位：m/s-->r/mim
		spd_lb1=spd_lb*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//左后轮速度强制转换		单位：m/s-->r/mim
		spd_rf1=spd_rf*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//右前轮速度强制转换		单位：m/s-->r/mim
		spd_rb1=spd_rb*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//右后轮速度强制转换		单位：m/s-->r/mim
	}			
	else		//当控制模式为1，及输入值为r/mim
	{
		spd_lf1=spd_lf;
		spd_lb1=spd_lb;
		spd_rf1=spd_rf;
		spd_rb1=spd_rb;
	}
	
	temp[0]=spd_lf1;
	temp[1]=spd_lb1;
	temp[2]=spd_rf1;
	temp[3]=spd_rb1;
	
	if(spd_lf1<10) spd_lf2=0;         //死区设置
	if(spd_lb1<10) spd_lb2=0;         //死区设置
	if(spd_rf1<10) spd_rf2=0;         //死区设置
	if(spd_rb1<10) spd_rb2=0;         //死区设置
	
	if(spd_lf2>=0)				//左前轮转速为正
	{
		OT_DIR1=1;
		TIM_SetCompare1(TIM4,spd_lf1);
	}
	else								//左前轮转速为负
	{
		OT_DIR1=0;
		TIM_SetCompare1(TIM4,spd_lf1);
	}
	
	if(spd_lb2>=0)				//左后轮转速为正
	{
		OT_DIR2=1;
		TIM_SetCompare2(TIM4,spd_lb1);
	}
	else								//左后轮转速为负
	{
		OT_DIR2=0;
		TIM_SetCompare2(TIM4,spd_lb1);
	}

	if(spd_rf2>=0)				//右前轮转速为正
	{
		OT_DIR3=0;
		TIM_SetCompare3(TIM4,spd_rf1);
	}
	else								//右前轮转速为负
	{
		OT_DIR3=1;
		TIM_SetCompare3(TIM4,spd_rf1);
	}

	if(spd_rb2>=0)				//右后轮转速为正
	{
		OT_DIR4=0;
		TIM_SetCompare4(TIM4,spd_rb1);
	}
	else								//右后轮转速为负
	{
		OT_DIR4=1;
		TIM_SetCompare4(TIM4,spd_rb1);
	}
}


/********************************************************************************************************************************************
函数名: void Circle_contr(float speed_in,float R)
描述:     void Circle_contr(float speed_in,float R):小车圆弧运动
输入参数:float speed_in：小车移动速度给定    float R：小车的圆弧半径    u8 mode:小车左右圆弧模式  mode=2，左圆弧   mode=3,右圆弧

*********************************************************************************************************************************************/
void Circle_contr(float speed_in,float R,u8 mode)
{
	
	float spd_l;      //左侧轮子速度
	float spd_r;      //右侧轮子速度
	spdTemp=speed_in;
	if(mode==2)						//左圆弧
	{
		markTemp=2;
		spd_l=speed_in*(R-AGVStatus.vehicle_width)/R;						//计算小车左圆弧左侧轮子车速
		spd_r=speed_in*(R+AGVStatus.vehicle_width)/R;						//计算小车右圆弧右侧轮子车速	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//速度给定
	}
	else if(mode==3)			//右圆弧
	{
		spd_l=speed_in*(R+AGVStatus.vehicle_width)/R;						//计算小车左圆弧左侧轮子车速
		spd_r=speed_in*(R-AGVStatus.vehicle_width)/R;						//计算小车右圆弧右侧轮子车速	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//速度给定
		markTemp=3;
	}	
}


/********************************************************************************************************************************************
函数名: void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit)
描述:   void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit):小车连续运动
输入参数:float speed_in：小车移动速度给定    float R：小车运动的圆弧半径  ,u8 switch:小车连续运动定时开启或者关闭
u8 mode:小车运动模式
mode=0:连续前进
mode=1:连续后退
mode=2:连续左圆弧 
mode=3:连续右圆弧


*********************************************************************************************************************************************/
void Vehicle_contr_Auto(float speed_in,float R,u8 mode)
{		
	float static speed_in1;
	speed_in1=speed_in;
	if(mode==0)					//连续前进
	{
		Speed_contr(speed_in1,speed_in1,speed_in1,speed_in1,0);
	}
	else if(mode==1)		//连续后退
	{
		Speed_contr(-speed_in1,-speed_in1,-speed_in1,-speed_in1,0);
	}
	else if(mode==2)			//连续左圆弧
	{
		Circle_contr(speed_in1,R,2);
	}
	else if(mode==3)		//连续右圆弧
	{
		Circle_contr(speed_in1,R,3);
	}
}

/********************************************************************************************************************************************
函数名: void Vehicle_contr_Jog(float speed_in,u8 mode)
描述:   void Vehicle_contr_Jog(float speed_in,u8 mode):小车点动移动
输入参数:float speed_in：小车移动速度给定    �
u8 mode:小车运动模式
mode=0:点动前进
mode=1:点动后退
mode=2:原地左转
mode=3:原地右转
*********************************************************************************************************************************************/
void Vehicle_contr_Jog(float speed_in,u8 mode)
{
		if(mode==0)					//点动前进
	{
		Speed_contr(speed_in,speed_in,speed_in,speed_in,0); 
	}
	else if(mode==1)		//点动后退
	{
		Speed_contr(-speed_in,-speed_in,-speed_in,-speed_in,0);
	}
	else if(mode==2)			//原地左转
	{
		 Speed_contr(-speed_in/4,-speed_in/4,speed_in/4,speed_in/4,0);
	}
	else if(mode==3)		//原地右转
	{
		Speed_contr(speed_in/4,speed_in/4,-speed_in/4,-speed_in/4,0);
	}
}


/***********************************************小车遥控控制************************************************************************
函数名: void Syma_vehicle_contr(float speed_in,u16 cmd)
描述:   void Syma_vehicle_contr(float speed_in,u16 cmd):遥控控制小车
输入参数:u8 key：手柄指令数据   u8 Scope_RL[2]：存储摇杆数据  
float spd_Auto：自动速度
float R：圆弧半径
float spd_Jog：点动速度
*********************************************************************************************************************************************/
void Syma_vehicle_contr(u8 * syma_cmd,u8 * tmd_buf,float spd_Auto,float R,float spd_Jog)
{
	
	if(OT_SON==0||AGVStatus.EmerStop==1)            //当急停或者断使能时，小车停止
	{
		Vehicle_contr_Jog(0,0);           //手动速度给定停止
		Vehicle_contr_Auto(0,0,0);				//自动速度给定停止		
	}
	else if(OT_SON==1&&AGVStatus.EmerStop==0&&!AGVStatus.AJMode)																//当没有停止信号输入
	{	
		if(syma_cmd[0]==0)	Vehicle_contr_Jog(0,0);   			//左摇杆左右为0 手动速度给定停止		
		if(syma_cmd[2]==0&&syma_cmd[3]==0)	Vehicle_contr_Auto(0,0,0);		//右摇杆上下  左右为0  自动速度给定停止	
		if(syma_cmd[0]==1)	Vehicle_contr_Jog(spd_Jog,2);									//小车左自转
		else if(syma_cmd[0]==2)	Vehicle_contr_Jog(spd_Jog,3);						  //小车右自转													
		else 	Vehicle_contr_Jog(0,0);																			//小车停止

		if(syma_cmd[3]==1)	Vehicle_contr_Auto(spd_Auto,R,0);																//小车前进
		else if(syma_cmd[3]==2)	Vehicle_contr_Auto(spd_Auto,R,1);															//小车后退

		if(syma_cmd[2]==1)	Vehicle_contr_Auto(spd_Auto,R,2);																//小车左圆弧
		else if(syma_cmd[2]==2)	Vehicle_contr_Auto(spd_Auto,R,3);															//小车右圆弧							

	}

}



/***********************************************自动模式小车运动控制***********************************************************************
函数名: void auto_vehicle_contr(float speed_in,u8 cmd)
描述:   void auto_vehicle_contr(float speed_in,u8 cmd):遥控控制小车
输入参数:u8 cmd：移动指令   float speed_in：自动运行速度

*********************************************************************************************************************************************/
void auto_vehicle_contr(float speed_in,u8 cmd)
{
	if(OT_SON==0||AGVStatus.EmerStop==1)            //当急停或者断使能时，小车停止
	{
		Vehicle_contr_Jog(0,0);           //手动速度给定停止
		Vehicle_contr_Auto(0,0,0);				//自动速度给定停止		
	}
	else if(AGVStatus.SONSetup==1&&AGVStatus.EmerStop==0)																//当没有停止信号输入
	{	
		switch(cmd)
		{
			case 0x05: Vehicle_contr_Auto(speed_in,0,0);break;             		 //小车前进				
			case 0x06: Vehicle_contr_Auto(speed_in,3.0,3);break;               //右转小角度					
			case 0x07: Vehicle_contr_Auto(speed_in,2.0,3);break;               //右转大角度					
			case 0x08: Vehicle_contr_Auto(speed_in,2.5,2);break;               //左转小角度					
			case 0x09: Vehicle_contr_Auto(speed_in,1.4,2);break;               //左转大角度			
			default:break;
		}
	}
	else	
	{
		Vehicle_contr_Jog(0,0);           //手动速度给定停止
		Vehicle_contr_Auto(0,0,0);				//自动速度给定停止	
	}			
}

