#include "HEAD.h"

//通用定时器1中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器1!
void TIM1_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //使能TIM1时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;;/*重复计数寄存器*/ //高级定时器1是用定时器功能配置这个才可以是正常的计数频率一开始的72mhz 值得注意的地方	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化TIM1
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);/*清更新标志位*/
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //允许定时器1更新中断
	TIM_Cmd(TIM1,ENABLE); //使能定时器1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //定时器1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


//定时器1中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{	
	u8 static tim1_mark;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
	{
		
		if(Tim1Add.tiAdd>250)	Tim1Add.tiAdd=0;					//time1计数器									
		Tim1Add.tiAdd++;
		Time1_Task();
		
		
		if(tim1_mark>=5)
		{
			tim1_mark=0;
			Tim1Add.reMark=!Tim1Add.reMark;					//每个0.5ms  取反一次状态			
		}		
		tim1_mark++;	
		
		
	}
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除中断标志位
}



////////////////////////定时器扫描工作///////////////////////////
//函数名称：void Time1_Task(void)
//函数功能：定时器1时间函数
//输入变量
//输出变量：
//返回值：
//////////////////////////////////////////////////////////////
void Time1_Task(void)
{

	SymaStatus.Spd_Jog=Syma_yaogan_receive(SymaStatus.Yaogan,SymaStatus.Tmp_buf,SymaStatus.Syma_cmd,SymaStatus.Spd_cle[0]);			//遥控器摇杆数据读取
	Syma_vehicle_contr(SymaStatus.Syma_cmd,SymaStatus.Tmp_buf,SymaStatus.Spd_Jog,SymaStatus.Spd_cle[1],0.5);					//遥控器对对小车的控制	
	Syma_button_receive(SymaStatus.Tmp_buf,SymaStatus.Spd_cle);																	//遥控器按钮数据读取	
//	AGVsta_Scan(1);	           //按钮状态检测	
//	InitMagDate=magnetic_recei(USART3_RX_BUF,Mag_date,Mag_alarm);	//磁导航数据读取
	



}








