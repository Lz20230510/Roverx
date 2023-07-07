#include "timer.h"
#include "led.h"
//#include "HMI_cmd.h"
#include "myiic.h"
#include "24l01.h"
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


extern u16 USART3_RX_STA;


//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	u8 i;
	LED0=!LED0;
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   		
		if(Tim7Add.tiAdd<255)Tim7Add.tiAdd++;//����ж�		time7  1s��ʱ����ʱ
		else Tim7Add.tiAdd=0;						//����	
		
		for(i=0;i<6;i++)												//��ʱ��7_0--	��ʱ��7_5			
		{
			Tim7Add.mark[i]=Time7_Timekeep(Tim7Add.time[i],Tim7Add.tiAdd,Tim7Add.start[i],i,0);						//��ʱ��7_0
			if(Tim7Add.mark[i]==1&&Tim7Add.start[i]==1)
			{
				Tim7Add.time[i]=0;															//��ʼʱ������
				Tim7Add.start[i]=0;															//��ʼʱ������
			}			
		}		
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIM7�����жϱ�־  	
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��7
	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
	 


void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///ʹ��TIM7ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��7�����ж�
	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��6
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//��ʱ��6�жϷ�����
void TIM6_DAC_IRQHandler(void)
{
	u8 static Tim6_mark;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET) //����ж�
	{
		LCD_mark=!LCD_mark;
		if(Tim6_mark>=5)
		{
			Tim6_mark=0;
			if(AGVStatus.AUTOmode!=2&&AGVStatus.TeachMode==0&&AGVStatus.MapDownMode==0)  Battery_Send(7,Ask_Battery);							//���ط�������  ��������ģʽ��ʾ��ģʽ����					
		}		
		Tim6_mark++;		

	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);  //����жϱ�־λ

}

//**********************Time7��ʱ��**************************/
//u8 Time7_Timekeep(u8 time,u8 timekeep,u8 start,u8 reset)
//���������u8 time����ʱʱ��    u8 timekeep��Tim7Add,�ۼ���ȫ�ֱ���   u8 reset:��ʱ����λ  =1����λ  =0��������λ����
//u8 start����ʱ����ʼ
//���������
//���أ�
/******************************************************/
u8 Time7_Timekeep(u8 time,u8 timekeep,u8 start,u8 Num,u8 reset)
{	
	static u16 timeStart[8];						//��ʱ����ʼʱ��
	static u16 timeStop[8];						//��ʱ��ֹͣʱ��
	static u8 lastUpmark[8];								//�����ر�Ǳ���
	u8 temp=0;
	if(reset!=1)
	{
		if(start==1&&lastUpmark[Num]!=start)	//
		{
			timeStart[Num]=timekeep;				//����ʱ��ʼ���洢��ǰ��time7�ۼ���ֵ
			if(timeStart[Num]+time>255) timeStop[Num]=timeStart[Num]+time-255;
			else timeStop[Num]=timeStart[Num]+time;
		}
		else
		{
			if(timekeep!=timeStop[Num])									//��û�е���ʱʱ��
			{
				temp=0;			
			}
			else //�����˼�ʱʱ��
			{
				lastUpmark[Num]=0;									        //�����ش洢�������
				timeStart[Num]=0;													//��ʼֵ����
				timeStop[Num]=0;														//ֵֹͣ����
				temp=1;								
			}	
		}	
		lastUpmark[Num]=start;							    //������һ��״̬
	}
	else
	{
		lastUpmark[Num]=0;									        //�����ش洢�������
		timeStart[Num]=0;													//��ʼֵ����
		timeStop[Num]=0;														//ֵֹͣ����
		temp=0;
		Tim7Add.mark[Num]=0;
		Tim7Add.start[Num]=0;
	}	
	return temp;
}

