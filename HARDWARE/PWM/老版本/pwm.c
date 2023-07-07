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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ��PWM��TIM2_CH1~4����4����챵����TIM3_CH1����ת����   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

float spdTemp=0;
u8 markTemp=0;
/********************************************************************************************************************************************
������: TIM4_CH1_PWM_Init(u32 arr,u32 psc)
����:   TIM4_CH1_PWM������ǰ��챵��	 
�������:�Զ���װ�ز�����Ԥ��Ƶ����	 
����ֵ:
*********************************************************************************************************************************************/
void TIM4_CH1_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //ʹ��PORTCʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);            //GPIOC6����Ϊ��ʱ��3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                        //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //��ʼ��PC6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                            //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //��ʼ����ʱ��4
	
	//��ʼ��TIM3 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //ʹ��TIMB��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEʹ�� 
	
	TIM_Cmd(TIM4, ENABLE);                                           //ʹ��TIM4	
}



/********************************************************************************************************************************************
������: TIM4_CH2_PWM_Init(u32 arr,u32 psc)
����:TIM4_CH4_PWM���������챵��	 
�������:�Զ���װ�ز�����Ԥ��Ƶ����	 
����ֵ:
*********************************************************************************************************************************************/
void TIM4_CH2_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //ʹ��PORTBʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);            //GPIOB7����Ϊ��ʱ��4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                        //GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //��ʼ��PB7
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                            //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //��ʼ����ʱ��4
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //�������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEʹ�� 
	
	TIM_Cmd(TIM4, ENABLE);                                           //ʹ��TIM4	
}


/********************************************************************************************************************************************
������: TIM4_CH3_PWM_Init(u32 arr,u32 psc)
����:TIM4_CH3_PWM������ǰ��챵��	 
�������:�Զ���װ�ز�����Ԥ��Ƶ����	 
����ֵ:
*********************************************************************************************************************************************/
void TIM4_CH3_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //ʹ��PORTBʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);            //GPIOB8����Ϊ��ʱ��4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                        //GPIOB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //��ʼ��PC8
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                            //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //��ʼ����ʱ��4
	
	//��ʼ��TIM4 Channel3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //�������:TIM����Ƚϼ��Ե�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEʹ�� 
	
	TIM_Cmd(TIM4, ENABLE);                                           //ʹ��TIM4	
}

/********************************************************************************************************************************************
������: TIM4_CH4_PWM_Init(u32 arr,u32 psc)
����:TIM4_CH4_PWM�����Һ���챵��	 
�������:�Զ���װ�ز�����Ԥ��Ƶ����	 
����ֵ:
*********************************************************************************************************************************************/
void TIM4_CH4_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM4ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //ʹ��PORTBʱ��	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);            //GPIOC9����Ϊ��ʱ��4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                        //GPIOC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //��ʼ��PC9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;                            //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //��ʼ����ʱ��4
	
	//��ʼ��TIM3 Channel4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //�������:TIM����Ƚϼ��Ե�
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);  

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEʹ�� 
	
	TIM_Cmd(TIM4,ENABLE);                                           //ʹ��TIM4	
}


/********************************************************************************************************************************************
������: TIM3_CH4_PWM_Init(u32 arr,u32 psc)
����:TIM3_CH4_PWM�����Һ���챵��	 
�������:�Զ���װ�ز�����Ԥ��Ƶ����	 
����ֵ:
*********************************************************************************************************************************************/

void DIR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOFʱ��

  //GPIOB6,B7��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);//GPIOF9,F10���øߣ�����
}


/********************************************************************************************************************************************
������: u16 Speed_trans(u16 spped_in)
����:   u16 Speed_trans(u16 spped_in):���ʵ��ת��ת��
�������:�����ĵ��ת�� 
����ֵ: У�����arrֵ                       ת����ʽ��X=(y+0.6255)/0.9986
*********************************************************************************************************************************************/
u16 Speed_trans(u16 spped_in)
{
	u16 y;
	y=spped_in;
	return (y+0.6255)/0.9986;
}

/********************************************************************************************************************************************
������: void Time_CH1234_Init(void)
����:    void Time_CH1234_Init(void):�ĸ����PWM��ʼ��


*********************************************************************************************************************************************/
void Time_CH1234_Init(void)
{
	TIM4_CH1_PWM_Init(5000-1,84-1); //��ʼ��TIM2 CH1��84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 
	TIM4_CH2_PWM_Init(5000-1,84-1); //��ʼ��TIM2 CH2��84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 	 	
	TIM4_CH3_PWM_Init(5000-1,84-1); //��ʼ��TIM2 CH3��84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 
	TIM4_CH4_PWM_Init(5000-1,84-1); //��ʼ��TIM2 CH4��84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz. 
}


/********************************************************************************************************************************************
������: void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode)
����:    Time_CH1234_Init(float spd_lf,float spd_lb,float spd_rf,float spd_rb):С������ת���趨   ��λ����/��   m/s
�������:spd_lf����ǰ��ת��   spd_lb�������ת��   spd_rf����ǰ��ת��    spd_rb���Һ���ת�� ,u8 mode:
�������:,u8 mode: =0  �����ٶ�ֵΪ��λ m/s              =1�������ٶ�ֵΪ r/min
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
	
	if(!mode)					//������ģʽΪ0��������ֵΪm/s
	{
		spd_lf1=spd_lf*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;       //��ǰ���ٶ�ǿ��ת��		��λ��m/s-->r/mim
		spd_lb1=spd_lb*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//������ٶ�ǿ��ת��		��λ��m/s-->r/mim
		spd_rf1=spd_rf*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//��ǰ���ٶ�ǿ��ת��		��λ��m/s-->r/mim
		spd_rb1=spd_rb*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//�Һ����ٶ�ǿ��ת��		��λ��m/s-->r/mim
	}			
	else		//������ģʽΪ1��������ֵΪr/mim
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
	
	if(spd_lf1<10) spd_lf2=0;         //��������
	if(spd_lb1<10) spd_lb2=0;         //��������
	if(spd_rf1<10) spd_rf2=0;         //��������
	if(spd_rb1<10) spd_rb2=0;         //��������
	
	if(spd_lf2>=0)				//��ǰ��ת��Ϊ��
	{
		OT_DIR1=1;
		TIM_SetCompare1(TIM4,spd_lf1);
	}
	else								//��ǰ��ת��Ϊ��
	{
		OT_DIR1=0;
		TIM_SetCompare1(TIM4,spd_lf1);
	}
	
	if(spd_lb2>=0)				//�����ת��Ϊ��
	{
		OT_DIR2=1;
		TIM_SetCompare2(TIM4,spd_lb1);
	}
	else								//�����ת��Ϊ��
	{
		OT_DIR2=0;
		TIM_SetCompare2(TIM4,spd_lb1);
	}

	if(spd_rf2>=0)				//��ǰ��ת��Ϊ��
	{
		OT_DIR3=0;
		TIM_SetCompare3(TIM4,spd_rf1);
	}
	else								//��ǰ��ת��Ϊ��
	{
		OT_DIR3=1;
		TIM_SetCompare3(TIM4,spd_rf1);
	}

	if(spd_rb2>=0)				//�Һ���ת��Ϊ��
	{
		OT_DIR4=0;
		TIM_SetCompare4(TIM4,spd_rb1);
	}
	else								//�Һ���ת��Ϊ��
	{
		OT_DIR4=1;
		TIM_SetCompare4(TIM4,spd_rb1);
	}
}


/********************************************************************************************************************************************
������: void Circle_contr(float speed_in,float R)
����:     void Circle_contr(float speed_in,float R):С��Բ���˶�
�������:float speed_in��С���ƶ��ٶȸ���    float R��С����Բ���뾶    u8 mode:С������Բ��ģʽ  mode=2����Բ��   mode=3,��Բ��

*********************************************************************************************************************************************/
void Circle_contr(float speed_in,float R,u8 mode)
{
	
	float spd_l;      //��������ٶ�
	float spd_r;      //�Ҳ������ٶ�
	spdTemp=speed_in;
	if(mode==2)						//��Բ��
	{
		markTemp=2;
		spd_l=speed_in*(R-AGVStatus.vehicle_width)/R;						//����С����Բ��������ӳ���
		spd_r=speed_in*(R+AGVStatus.vehicle_width)/R;						//����С����Բ���Ҳ����ӳ���	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//�ٶȸ���
	}
	else if(mode==3)			//��Բ��
	{
		spd_l=speed_in*(R+AGVStatus.vehicle_width)/R;						//����С����Բ��������ӳ���
		spd_r=speed_in*(R-AGVStatus.vehicle_width)/R;						//����С����Բ���Ҳ����ӳ���	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//�ٶȸ���
		markTemp=3;
	}	
}


/********************************************************************************************************************************************
������: void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit)
����:   void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit):С�������˶�
�������:float speed_in��С���ƶ��ٶȸ���    float R��С���˶���Բ���뾶  ,u8 switch:С�������˶���ʱ�������߹ر�
u8 mode:С���˶�ģʽ
mode=0:����ǰ��
mode=1:��������
mode=2:������Բ�� 
mode=3:������Բ��


*********************************************************************************************************************************************/
void Vehicle_contr_Auto(float speed_in,float R,u8 mode)
{		
	float static speed_in1;
	speed_in1=speed_in;
	if(mode==0)					//����ǰ��
	{
		Speed_contr(speed_in1,speed_in1,speed_in1,speed_in1,0);
	}
	else if(mode==1)		//��������
	{
		Speed_contr(-speed_in1,-speed_in1,-speed_in1,-speed_in1,0);
	}
	else if(mode==2)			//������Բ��
	{
		Circle_contr(speed_in1,R,2);
	}
	else if(mode==3)		//������Բ��
	{
		Circle_contr(speed_in1,R,3);
	}
}

/********************************************************************************************************************************************
������: void Vehicle_contr_Jog(float speed_in,u8 mode)
����:   void Vehicle_contr_Jog(float speed_in,u8 mode):С���㶯�ƶ�
�������:float speed_in��С���ƶ��ٶȸ���    �
u8 mode:С���˶�ģʽ
mode=0:�㶯ǰ��
mode=1:�㶯����
mode=2:ԭ����ת
mode=3:ԭ����ת
*********************************************************************************************************************************************/
void Vehicle_contr_Jog(float speed_in,u8 mode)
{
		if(mode==0)					//�㶯ǰ��
	{
		Speed_contr(speed_in,speed_in,speed_in,speed_in,0); 
	}
	else if(mode==1)		//�㶯����
	{
		Speed_contr(-speed_in,-speed_in,-speed_in,-speed_in,0);
	}
	else if(mode==2)			//ԭ����ת
	{
		 Speed_contr(-speed_in/4,-speed_in/4,speed_in/4,speed_in/4,0);
	}
	else if(mode==3)		//ԭ����ת
	{
		Speed_contr(speed_in/4,speed_in/4,-speed_in/4,-speed_in/4,0);
	}
}


/***********************************************С��ң�ؿ���************************************************************************
������: void Syma_vehicle_contr(float speed_in,u16 cmd)
����:   void Syma_vehicle_contr(float speed_in,u16 cmd):ң�ؿ���С��
�������:u8 key���ֱ�ָ������   u8 Scope_RL[2]���洢ҡ������  
float spd_Auto���Զ��ٶ�
float R��Բ���뾶
float spd_Jog���㶯�ٶ�
*********************************************************************************************************************************************/
void Syma_vehicle_contr(u8 * syma_cmd,u8 * tmd_buf,float spd_Auto,float R,float spd_Jog)
{
	
	if(OT_SON==0||AGVStatus.EmerStop==1)            //����ͣ���߶�ʹ��ʱ��С��ֹͣ
	{
		Vehicle_contr_Jog(0,0);           //�ֶ��ٶȸ���ֹͣ
		Vehicle_contr_Auto(0,0,0);				//�Զ��ٶȸ���ֹͣ		
	}
	else if(OT_SON==1&&AGVStatus.EmerStop==0&&!AGVStatus.AJMode)																//��û��ֹͣ�ź�����
	{	
		if(syma_cmd[0]==0)	Vehicle_contr_Jog(0,0);   			//��ҡ������Ϊ0 �ֶ��ٶȸ���ֹͣ		
		if(syma_cmd[2]==0&&syma_cmd[3]==0)	Vehicle_contr_Auto(0,0,0);		//��ҡ������  ����Ϊ0  �Զ��ٶȸ���ֹͣ	
		if(syma_cmd[0]==1)	Vehicle_contr_Jog(spd_Jog,2);									//С������ת
		else if(syma_cmd[0]==2)	Vehicle_contr_Jog(spd_Jog,3);						  //С������ת													
		else 	Vehicle_contr_Jog(0,0);																			//С��ֹͣ

		if(syma_cmd[3]==1)	Vehicle_contr_Auto(spd_Auto,R,0);																//С��ǰ��
		else if(syma_cmd[3]==2)	Vehicle_contr_Auto(spd_Auto,R,1);															//С������

		if(syma_cmd[2]==1)	Vehicle_contr_Auto(spd_Auto,R,2);																//С����Բ��
		else if(syma_cmd[2]==2)	Vehicle_contr_Auto(spd_Auto,R,3);															//С����Բ��							

	}

}



/***********************************************�Զ�ģʽС���˶�����***********************************************************************
������: void auto_vehicle_contr(float speed_in,u8 cmd)
����:   void auto_vehicle_contr(float speed_in,u8 cmd):ң�ؿ���С��
�������:u8 cmd���ƶ�ָ��   float speed_in���Զ������ٶ�

*********************************************************************************************************************************************/
void auto_vehicle_contr(float speed_in,u8 cmd)
{
	if(OT_SON==0||AGVStatus.EmerStop==1)            //����ͣ���߶�ʹ��ʱ��С��ֹͣ
	{
		Vehicle_contr_Jog(0,0);           //�ֶ��ٶȸ���ֹͣ
		Vehicle_contr_Auto(0,0,0);				//�Զ��ٶȸ���ֹͣ		
	}
	else if(AGVStatus.SONSetup==1&&AGVStatus.EmerStop==0)																//��û��ֹͣ�ź�����
	{	
		switch(cmd)
		{
			case 0x05: Vehicle_contr_Auto(speed_in,0,0);break;             		 //С��ǰ��				
			case 0x06: Vehicle_contr_Auto(speed_in,3.0,3);break;               //��תС�Ƕ�					
			case 0x07: Vehicle_contr_Auto(speed_in,2.0,3);break;               //��ת��Ƕ�					
			case 0x08: Vehicle_contr_Auto(speed_in,2.5,2);break;               //��תС�Ƕ�					
			case 0x09: Vehicle_contr_Auto(speed_in,1.4,2);break;               //��ת��Ƕ�			
			default:break;
		}
	}
	else	
	{
		Vehicle_contr_Jog(0,0);           //�ֶ��ٶȸ���ֹͣ
		Vehicle_contr_Auto(0,0,0);				//�Զ��ٶȸ���ֹͣ	
	}			
}

