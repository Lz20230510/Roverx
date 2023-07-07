#include "ultrasonic.h"
#include "delay.h"

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 超声波硬件模块初始化
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultra_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =ULTRA_TRIG;       //发送电平引脚GPIO_Pin_5
 	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode=0x00;//推挽输出
 	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =ULTRA_ECHO;	   //返回电平引脚GPIO_Pin_6
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = 0x00;//浮空输入
 	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOB,ULTRA_TRIG);
}

/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 float *p:测距缓存变量
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i;
	u32 j;
	float Ultr_Temp;	
	for(i=0;i<5;i++)
	{
		TRIG_Send=1;
		delay_ms(10);
		TRIG_Send=0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			delay_us(10);
			j++;
		}
		Ultr_Temp+=340/2*j*10;//模块最大可测距3m 
		j=0;
		delay_ms(60);//防止发射信号对回响信号的影响
	}
	*p=Ultr_Temp/5/1000000; 	
}

/********************************************************************************************************************************************
@f_name: short Get_Distance(void)
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@return: short:距离值(扩大了100倍,单位:mm.)
*********************************************************************************************************************************************/
short Get_Distance(void)
{
	u8 i;
	u32 j;
	float Distance;
	short result;
	for(i=0;i<5;i++)
	{
		TRIG_Send = 1;
		delay_ms(15);
		TRIG_Send = 0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			delay_us(10);
			j++;
		}
		Distance+=340/2*j*10;//  模块最大可测距3m 
		j=0;
		delay_ms(100);//防止发射信号对回响信号的影响
	}
	result=Distance/5/100; 
	return result;
}
	 

