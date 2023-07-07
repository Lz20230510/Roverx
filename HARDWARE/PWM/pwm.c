#include "pwm.h"
#include "HEAD.h"
#include "usart.h"
//#include "pstwo.h" 
#include "pwm.h" 
#include "AGV_contr.h" 
#include "vehicle_key.h" 
#include "RAD.h" 
#include "24l01.h" 
#include "math.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//¶¨Ê±Æ÷PWM£¬TIM2_CH1~4Çý¶¯4¸öÂÖì±µç»ú£¬TIM3_CH1Çý¶¯×ªÏòµç»ú   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2014/5/4
//°æ±¾£ºV1.0
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

float spdTemp=0;
u8 markTemp=0;


///********************************************************************************************************************************************
//º¯ÊýÃû: TIM4_CH1_PWM_Init(u32 arr,u32 psc)
//ÃèÊö:   TIM4_CH1_PWM¿ØÖÆ×óÇ°ÂÖì±µç»ú	 
//ÊäÈë²ÎÊý:×Ô¶¯ÖØ×°ÔØ²ÎÊý£¬Ô¤·ÖÆµ²ÎÊý	 
//·µ»ØÖµ:
//*********************************************************************************************************************************************/
//void TIM4_CH1_PWM_Init(u32 arr,u32 psc)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3Ê±ÖÓÊ¹ÄÜ    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTCÊ±ÖÓ	
//	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);            //GPIOC6¸´ÓÃÎª¶¨Ê±Æ÷3
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                        //GPIOC6
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
//	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PC6
//	  
//	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊýÄ£Ê½
//	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
//	
//	//³õÊ¼»¯TIM3 Channel1 PWMÄ£Ê½	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
//	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  

//	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIMBÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
// 
//  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
//	
//	TIM_Cmd(TIM4, ENABLE);                                           //Ê¹ÄÜTIM4	
//}



///********************************************************************************************************************************************
//º¯ÊýÃû: TIM4_CH2_PWM_Init(u32 arr,u32 psc)
//ÃèÊö:TIM4_CH4_PWM¿ØÖÆ×óºóÂÖì±µç»ú	 
//ÊäÈë²ÎÊý:×Ô¶¯ÖØ×°ÔØ²ÎÊý£¬Ô¤·ÖÆµ²ÎÊý	 
//·µ»ØÖµ:
//*********************************************************************************************************************************************/
//void TIM4_CH2_PWM_Init(u32 arr,u32 psc)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3Ê±ÖÓÊ¹ÄÜ    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTBÊ±ÖÓ	
//	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);            //GPIOB7¸´ÓÃÎª¶¨Ê±Æ÷4
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                        //GPIOB7
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
//	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PB7
//	  
//	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊýÄ£Ê½
//	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
//	
//	//³õÊ¼»¯TIM3 Channel2 PWMÄ£Ê½	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
//	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  

//	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIM3ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
// 
//  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
//	
//	TIM_Cmd(TIM4, ENABLE);                                           //Ê¹ÄÜTIM4	
//}


///********************************************************************************************************************************************
//º¯ÊýÃû: TIM4_CH3_PWM_Init(u32 arr,u32 psc)
//ÃèÊö:TIM4_CH3_PWM¿ØÖÆÓÒÇ°ÂÖì±µç»ú	 
//ÊäÈë²ÎÊý:×Ô¶¯ÖØ×°ÔØ²ÎÊý£¬Ô¤·ÖÆµ²ÎÊý	 
//·µ»ØÖµ:
//*********************************************************************************************************************************************/
//void TIM4_CH3_PWM_Init(u32 arr,u32 psc)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3Ê±ÖÓÊ¹ÄÜ    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTBÊ±ÖÓ	
//	
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);            //GPIOB8¸´ÓÃÎª¶¨Ê±Æ÷4
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                        //GPIOB8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
//	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PC8
//	  
//	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊýÄ£Ê½
//	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
//	
//	//³õÊ¼»¯TIM4 Channel3 PWMÄ£Ê½	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  

//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIM4ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
// 
//  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
//	
//	TIM_Cmd(TIM4, ENABLE);                                           //Ê¹ÄÜTIM4	
//}

/********************************************************************************************************************************************
º¯ÊýÃû: TIM4_CH4_PWM_Init(u32 arr,u32 psc)
ÃèÊö:TIM4_CH4_PWM¿ØÖÆÓÒºóÂÖì±µç»ú	 
ÊäÈë²ÎÊý:×Ô¶¯ÖØ×°ÔØ²ÎÊý£¬Ô¤·ÖÆµ²ÎÊý	 
·µ»ØÖµ:
*********************************************************************************************************************************************/
void TIM4_CH4_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM4Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTBÊ±ÖÓ	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);            //GPIOC9¸´ÓÃÎª¶¨Ê±Æ÷4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                        //GPIOC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PC9
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
	
	//³õÊ¼»¯TIM3 Channel4 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);  

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIM4ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
	
	TIM_Cmd(TIM4,ENABLE);                                           //Ê¹ÄÜTIM4	
}


/********************************************************************************************************************************************
º¯ÊýÃû: void Time_CH1234_Init(void)
ÃèÊö:    void Time_CH1234_Init(void):ËÄ¸öµç»úPWM³õÊ¼»¯


*********************************************************************************************************************************************/
void Time_CH1234_Init(void)
{
//	TIM4_CH1_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH1£¬84M/84=1MhzµÄ¼ÆÊýÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 
//	TIM4_CH2_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH2£¬84M/84=1MhzµÄ¼ÆÊýÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 	 	
//	TIM4_CH3_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH3£¬84M/84=1MhzµÄ¼ÆÊýÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 
	TIM4_CH4_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH4£¬84M/84=1MhzµÄ¼ÆÊýÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 
	
	
}

/*****************************************************************************************************************
º¯ÊýÃû: void  Direction_Control(u8 num,float angle,float limit);   //
ÃèÊö:    ×ªÏòµç»úPWM¿ØÖÆ,ÔÝ¶¨Ê¹ÓÃÖ±ÏßÄâºÏ½Ç¶ÈÓëPWMÖ®¼ä¹ØÏµ£¬ºóÆÚ¸ù¾ÝÄ£ÐÍ²ÉÓÃÆäËû·½·¨ÄâºÏ
ÊäÈë²ÎÊý:u8 num:Í¨µÀºÅ1-4·Ö±ð¶ÔÓ¦CH1-CH4£¬ float angle¸ø¶¨Ð¡³µ·½Ïò½Ç¶È £¬float limit£º¶æÂÖ×óÓÒ¼«ÏÞÀÛ¼Ó½Ç¶È£¬´¹Ö±·½ÏòÎª90¶È
·µ»ØÖµ:ÎÞ

******************************************************************************************************************/
//u32 Direction_Control(u8 num,float angle,float limit)
//{
//	u32 pwm;
//	float k1,b1;   //y=kx+b;

//	switch(num)
//	{
//		case 1:				//CH1
//		{
//			if(angle>=90&&angle<=90+limit)					//ÍùÓÒ´ò
//			{
//				k1=(float)(RLimit-Median)/limit;
//				b1=(float)Median-90.0*k1;
//				pwm=(u32)angle*k1+b1;				
//			}
//			else if(angle<90&&angle>=90-limit)			//Íù×ó´ò
//			{
//				k1=(float)(Median-LLimit)/limit;
//				b1=(float)Median-90.0*k1;
//				pwm=(u32)angle*k1+b1;		
//			}
//			else if(angle>90+limit)	pwm=(u32)RLimit;		//³¬¹ýÓÒ¼«ÏÞ
//			else	pwm=(u32)LLimit;											//³¬¹ý×ó¼«ÏÞ
//			TIM_SetCompare1(TIM4,pwm);									//·¢ËÍ
//		}break;
//		case 2:break;				//CH2
//		case 3:break;				//CH3
//		case 4:break;				//CH4
//		default:break;
//	}
//	
//	return pwm;
//}
//u32 Direction_Control(u8 num,float angle,float limit)
//{
//	u32 pwm;
//	u32 Left,Right;
//	float temp;
//	Left=LLimit-Median;
//	Right=Median-RLimit;
//	if(angle>91)
//	{
//		temp=(float)Left/limit;
//		pwm=1700+(u32)((angle-90)*temp);
//	}
//	else if(angle<89)
//	{
//		temp=(float)Left/limit;
//		pwm=1700-(u32)((90-angle)*temp);
//		
//	}
//	else			//   89<=angle<=91
//	{
//		pwm=1700;		
//	}
//	
//	return pwm;
//}

u32 Direction_Control(u8 num,float angle,float limit)
{
	u32 pwm;
	u32 Left,Right;
	double SteeringAngle,temp5;
	temp5=(double)angle; 
	
	if(angle>=91&&angle<=angle+limit)				//µ±¸ø¶¨Ç°ÂÖÖÐÐÄµã½Ç¶È>=91
	{
		SteeringAngle=-0.0000817118780645862*pow(temp5,4)+0.0273998472907235*pow(temp5,3)-3.44767474694936*pow(temp5,2)+195.415130362405*temp5-4184.65501685534;
		
	 // SteeringAngle=0.00164478118527388*pow(temp5,3)-0.49225866632692*pow(temp5,2)+50.3308066290308*temp5-1650.79446553645   //sheet2ÄâºÏ
		
		//SteeringAngle=-0.00213183340258496*pow(temp5,3)+0.621761417933322*pow(temp5,2)-62.8571016965843*temp5+2265.28884481911;	 //sheet1ÄâºÏ
		SteeringAngle=0.00117938394111583*pow(temp5,4)-0.457052254963715*pow(temp5,3)+66.4033112026839*pow(temp5,2)-4283.90738504014*temp5+103599.980948377;	//ËÄ´Î·½ÄâºÏ Ô­±¾Êý¾Ý
	}
	else if(angle<=89&&angle>=angle-limit)		//µ±¸ø¶¨Ç°ÂÖÖÐÐÄµã½Ç¶È<=89
	{
		
		SteeringAngle=-0.0000319160171698469*pow(temp5,4)+0.0147015020205581*pow(temp5,3)-2.49099773885429*pow(temp5,2)+186.022280077304*temp5-5097.71651812848;
		
		//SteeringAngle=0.0021318334025861*pow(temp5,3)-0.529428619462817*pow(temp5,2)+46.2371979718661*temp5-1336.77192340241   //sheet2ÄâºÏ
		
		//SteeringAngle=-0.00213183340258608*pow(temp5,3)+0.529428619462813*pow(temp5,2)-46.2371979718658*temp5+1516.7719234024;	  //sheet1ÄâºÏ
		//SteeringAngle=-0.0000210216458836684*pow(temp5,4)+0.00908854329253429*pow(temp5,3)-1.4193742355538*pow(temp5,2)+98.6047923473897*temp5-2521.73377892326;	
	}
	else if(angle>angle+limit) SteeringAngle=angle+limit;			//Õý¼«ÏÞ
	else if(angle<angle-limit) SteeringAngle=angle-limit;			//¸º¼«ÏÞ
	else	SteeringAngle=85;									//µ±¸ø¶¨Ç°ÂÖÖÐÐÄµã½Ç¶È  <91&&>89     102.16   107.16

	pwm=Steering_Engine_Caculate((float)SteeringAngle);

	if(pwm>LLimit) pwm=LLimit;			//ÏÞ·ù
	if(pwm<RLimit) pwm=RLimit;
	
	return pwm;
}



/********************************************************************************************************************************************
º¯ÊýÃû:  u32 Steering_Engine_Caculate(float angle)
ÃèÊö:¶æ»ú¸ø¶¨½Ç¶ÈÓëÊä³öPWMÖµ×ª»»	 
ÊäÈë²ÎÊý:float angle£º¸ø¶¨¶æ»ú½Ç¶È 
·µ»ØÖµ:Êä³öµÄPWMÖµ    500£ºÍùÓÒ±ß´òËÀ     2500£ºÍù×ó±ß´òËÀ    1500£ºÖÐ¼ä
¸ø¶¨¶æ»ú½Ç¶ÈÓëÊä³öPWMÏßÐÔ¹ØÏµ£ºy=11.1111x+500
*********************************************************************************************************************************************/
u32 Steering_Engine_Caculate(float angle)
{
	u32 pwmOut;
	pwmOut=(u32)(11.1111*angle+500);

	return pwmOut;
}






/********************************************************************************************************************************************
º¯ÊýÃû: TIM3_CH4_PWM_Init(u32 arr,u32 psc)
ÃèÊö:TIM3_CH4_PWM¿ØÖÆÓÒºóÂÖì±µç»ú	 
ÊäÈë²ÎÊý:×Ô¶¯ÖØ×°ÔØ²ÎÊý£¬Ô¤·ÖÆµ²ÎÊý	 
·µ»ØÖµ:
*********************************************************************************************************************************************/

void DIR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜGPIOFÊ±ÖÓ

  //GPIOB6,B7³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;// GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//LED0ºÍLED1¶ÔÓ¦IO¿Ú
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	
	GPIO_SetBits(GPIOB,GPIO_Pin_9);//GPIOF9,F10ÉèÖÃ¸ß£¬µÆÃð
}


/********************************************************************************************************************************************
º¯ÊýÃû: u16 Speed_trans(u16 spped_in)
ÃèÊö:   u16 Speed_trans(u16 spped_in):µç»úÊµ¼Ê×ªËÙ×ª»»
ÊäÈë²ÎÊý:¸ø¶¨µÄµç»ú×ªËÙ 
·µ»ØÖµ: Ð£ÕýºóµÄarrÖµ                       ×ª»»¹«Ê½£ºX=(y+0.6255)/0.9986
*********************************************************************************************************************************************/
u16 Speed_trans(u16 spped_in)
{
	u16 y;
	y=spped_in;
	return (y+0.6255)/0.9986;
}

/********************************************************************************************************************************************
º¯ÊýÃû: void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode)
ÃèÊö:    Time_CH1234_Init(float spd_lf,float spd_lb,float spd_rf,float spd_rb):Ð¡³µËÄÂÖ×ªËÙÉè¶¨   µ¥Î»£ºÃ×/Ãë   m/s
ÊäÈë²ÎÊý:spd_lf£º×óÇ°ÂÖ×ªËÙ   spd_lb£º×óºóÂÖ×ªËÙ   spd_rf£ºÓÒÇ°ÂÖ×ªËÙ    spd_rb£ºÓÒºóÂÖ×ªËÙ ,u8 mode:
ÊäÈë²ÎÊý:,u8 mode: =0  ÊäÈëËÙ¶ÈÖµÎªµ¥Î» m/s              =1£ºÊäÈëËÙ¶ÈÖµÎª r/min
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
	
	if(!mode)					//µ±¿ØÖÆÄ£Ê½Îª0£¬¼°ÊäÈëÖµÎªm/s
	{
		spd_lf1=spd_lf*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;       //×óÇ°ÂÖËÙ¶ÈÇ¿ÖÆ×ª»»		µ¥Î»£ºm/s-->r/mim
		spd_lb1=spd_lb*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//×óºóÂÖËÙ¶ÈÇ¿ÖÆ×ª»»		µ¥Î»£ºm/s-->r/mim
		spd_rf1=spd_rf*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//ÓÒÇ°ÂÖËÙ¶ÈÇ¿ÖÆ×ª»»		µ¥Î»£ºm/s-->r/mim
		spd_rb1=spd_rb*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;				//ÓÒºóÂÖËÙ¶ÈÇ¿ÖÆ×ª»»		µ¥Î»£ºm/s-->r/mim
	}			
	else		//µ±¿ØÖÆÄ£Ê½Îª1£¬¼°ÊäÈëÖµÎªr/mim
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
	
	if(spd_lf1<10) spd_lf2=0;         //ËÀÇøÉèÖÃ
	if(spd_lb1<10) spd_lb2=0;         //ËÀÇøÉèÖÃ
	if(spd_rf1<10) spd_rf2=0;         //ËÀÇøÉèÖÃ
	if(spd_rb1<10) spd_rb2=0;         //ËÀÇøÉèÖÃ
	
	if(spd_lf2>=0)				//×óÇ°ÂÖ×ªËÙÎªÕý
	{
		OT_DIR1=1;
		TIM_SetCompare4(TIM4,5000-spd_lf1);      //TIM_SetCompare1(TIM4,spd_lf1)
	}
	else								//×óÇ°ÂÖ×ªËÙÎª¸º
	{
		OT_DIR1=0;
		TIM_SetCompare4(TIM4,5000-spd_lf1);                //TIM_SetCompare1(TIM4,spd_lf1);
	}
	
	if(spd_lb2>=0)				//×óºóÂÖ×ªËÙÎªÕý
	{
		OT_DIR2=1; 
		TIM_SetCompare4(TIM4,5000-spd_lb1);        //    TIM_SetCompare2(TIM4,spd_lb1);
	}
	else								//×óºóÂÖ×ªËÙÎª¸º
	{
		OT_DIR2=0;
		TIM_SetCompare4(TIM4,5000-spd_lb1);         // TIM_SetCompare2(TIM4,spd_lb1);
	}

	if(spd_rf2>=0)				//ÓÒÇ°ÂÖ×ªËÙÎªÕý
	{
		OT_DIR3=0;
		TIM_SetCompare4(TIM4,5000-spd_rf1);            //  TIM_SetCompare3(TIM4,spd_rf1);
	}
	else								//ÓÒÇ°ÂÖ×ªËÙÎª¸º
	{
		OT_DIR3=1;
		TIM_SetCompare4(TIM4,5000-spd_rf1);               //TIM_SetCompare3(TIM4,spd_rf1);
	}

	if(spd_rb2>=0)				//ÓÒºóÂÖ×ªËÙÎªÕý
	{
		OT_DIR4=0;
		TIM_SetCompare4(TIM4,5000-spd_rb1);
	}
	else								//ÓÒºóÂÖ×ªËÙÎª¸º
	{
		OT_DIR4=1;
		TIM_SetCompare4(TIM4,5000-spd_rb1);
	}
}


/********************************************************************************************************************************************
º¯ÊýÃû: void Circle_contr(float speed_in,float R)
ÃèÊö:     void Circle_contr(float speed_in,float R):Ð¡³µÔ²»¡ÔË¶¯
ÊäÈë²ÎÊý:float speed_in£ºÐ¡³µÒÆ¶¯ËÙ¶È¸ø¶¨    float R£ºÐ¡³µµÄÔ²»¡°ë¾¶    u8 mode:Ð¡³µ×óÓÒÔ²»¡Ä£Ê½  mode=2£¬×óÔ²»¡   mode=3,ÓÒÔ²»¡

*********************************************************************************************************************************************/
void Circle_contr(float speed_in,float R,u8 mode)
{
	
	float spd_l;      //×ó²àÂÖ×ÓËÙ¶È
	float spd_r;      //ÓÒ²àÂÖ×ÓËÙ¶È
	spdTemp=speed_in;
	if(mode==2)						//×óÔ²»¡
	{
		markTemp=2;
		spd_l=speed_in*(R-AGVStatus.vehicle_width)/R;						//¼ÆËãÐ¡³µ×óÔ²»¡×ó²àÂÖ×Ó³µËÙ
		spd_r=speed_in*(R+AGVStatus.vehicle_width)/R;						//¼ÆËãÐ¡³µÓÒÔ²»¡ÓÒ²àÂÖ×Ó³µËÙ	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//ËÙ¶È¸ø¶¨
	}
	else if(mode==3)			//ÓÒÔ²»¡
	{
		spd_l=speed_in*(R+AGVStatus.vehicle_width)/R;						//¼ÆËãÐ¡³µ×óÔ²»¡×ó²àÂÖ×Ó³µËÙ
		spd_r=speed_in*(R-AGVStatus.vehicle_width)/R;						//¼ÆËãÐ¡³µÓÒÔ²»¡ÓÒ²àÂÖ×Ó³µËÙ	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//ËÙ¶È¸ø¶¨
		markTemp=3;
	}	
}


/********************************************************************************************************************************************
º¯ÊýÃû: void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit)
ÃèÊö:   void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit):Ð¡³µÁ¬ÐøÔË¶¯
ÊäÈë²ÎÊý:float speed_in£ºÐ¡³µÒÆ¶¯ËÙ¶È¸ø¶¨    float R£ºÐ¡³µÔË¶¯µÄÔ²»¡°ë¾¶  ,u8 switch:Ð¡³µÁ¬ÐøÔË¶¯¶¨Ê±¿ªÆô»òÕß¹Ø±Õ
u8 mode:Ð¡³µÔË¶¯Ä£Ê½
mode=0:Á¬ÐøÇ°½ø
mode=1:Á¬ÐøºóÍË
mode=2:Á¬Ðø×óÔ²»¡ 
mode=3:Á¬ÐøÓÒÔ²»¡


*********************************************************************************************************************************************/
void Vehicle_contr_Auto(float speed_in,float R,u8 mode)
{		
	float static speed_in1;
	speed_in1=speed_in;
	if(mode==0)					//Á¬ÐøÇ°½ø
	{
		Speed_contr(speed_in1,speed_in1,speed_in1,speed_in1,0);
	}
	else if(mode==1)		//Á¬ÐøºóÍË
	{
		Speed_contr(-speed_in1,-speed_in1,-speed_in1,-speed_in1,0);
	}
	else if(mode==2)			//Á¬Ðø×óÔ²»¡
	{
		Circle_contr(speed_in1,R,2);
	}
	else if(mode==3)		//Á¬ÐøÓÒÔ²»¡
	{
		Circle_contr(speed_in1,R,3);
	}
}

/********************************************************************************************************************************************
º¯ÊýÃû: void Vehicle_contr_Jog(float speed_in,u8 mode)
ÃèÊö:   void Vehicle_contr_Jog(float speed_in,u8 mode):Ð¡³µµã¶¯ÒÆ¶¯
ÊäÈë²ÎÊý:float speed_in£ºÐ¡³µÒÆ¶¯ËÙ¶È¸ø¶¨    Õ
u8 mode:Ð¡³µÔË¶¯Ä£Ê½
mode=0:µã¶¯Ç°½ø
mode=1:µã¶¯ºóÍË
mode=2:Ô­µØ×ó×ª
mode=3:Ô­µØÓÒ×ª
*********************************************************************************************************************************************/
void Vehicle_contr_Jog(float speed_in,u8 mode)
{
		if(mode==0)					//µã¶¯Ç°½ø
	{
		Speed_contr(speed_in,speed_in,speed_in,speed_in,0); 
	}
	else if(mode==1)		//µã¶¯ºóÍË
	{
		Speed_contr(-speed_in,-speed_in,-speed_in,-speed_in,0);
	}
	else if(mode==2)			//Ô­µØ×ó×ª
	{
		 Speed_contr(-speed_in/4,-speed_in/4,speed_in/4,speed_in/4,0);
	}
	else if(mode==3)		//Ô­µØÓÒ×ª
	{
		Speed_contr(speed_in/4,speed_in/4,-speed_in/4,-speed_in/4,0);
	}
}


/***********************************************Ð¡³µÒ£¿Ø¿ØÖÆ************************************************************************
º¯ÊýÃû: void Syma_vehicle_contr(float speed_in,u16 cmd)
ÃèÊö:   void Syma_vehicle_contr(float speed_in,u16 cmd):Ò£¿Ø¿ØÖÆÐ¡³µ
ÊäÈë²ÎÊý:u8 key£ºÊÖ±úÖ¸ÁîÊý¾Ý   u8 Scope_RL[2]£º´æ´¢Ò¡¸ËÊý¾Ý  
float spd_Auto£º×Ô¶¯ËÙ¶È
float R£ºÔ²»¡°ë¾¶
float spd_Jog£ºµã¶¯ËÙ¶È
*********************************************************************************************************************************************/
void Syma_vehicle_contr(u8 * syma_cmd,u8 * tmd_buf,float spd_Auto,float R,float spd_Jog)
{
	
	if(OT_SON==0||AGVStatus.EmerStop==1)            //µ±¼±Í£»òÕß¶ÏÊ¹ÄÜÊ±£¬Ð¡³µÍ£Ö¹
	{
		Vehicle_contr_Jog(0,0);           //ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹
		Vehicle_contr_Auto(0,0,0);				//×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹		
	}
	else if(OT_SON==1&&AGVStatus.EmerStop==0&&!AGVStatus.AJMode)																//µ±Ã»ÓÐÍ£Ö¹ÐÅºÅÊäÈë
	{	
		if(syma_cmd[0]==0)	Vehicle_contr_Jog(0,0);   			//×óÒ¡¸Ë×óÓÒÎª0 ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹		
		if(syma_cmd[2]==0&&syma_cmd[3]==0)	Vehicle_contr_Auto(0,0,0);		//ÓÒÒ¡¸ËÉÏÏÂ  ×óÓÒÎª0  ×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹	
		if(syma_cmd[0]==1)	Vehicle_contr_Jog(spd_Jog,2);									//Ð¡³µ×ó×Ô×ª
		else if(syma_cmd[0]==2)	Vehicle_contr_Jog(spd_Jog,3);						  //Ð¡³µÓÒ×Ô×ª													
		else 	Vehicle_contr_Jog(0,0);																			//Ð¡³µÍ£Ö¹

		if(syma_cmd[3]==1)	Vehicle_contr_Auto(spd_Auto,R,0);																//Ð¡³µÇ°½ø
		else if(syma_cmd[3]==2)	Vehicle_contr_Auto(spd_Auto,R,1);															//Ð¡³µºóÍË

		if(syma_cmd[2]==1)	Vehicle_contr_Auto(spd_Auto,R,2);																//Ð¡³µ×óÔ²»¡
		else if(syma_cmd[2]==2)	Vehicle_contr_Auto(spd_Auto,R,3);															//Ð¡³µÓÒÔ²»¡							

	}

}



/***********************************************×Ô¶¯Ä£Ê½Ð¡³µÔË¶¯¿ØÖÆ***********************************************************************
º¯ÊýÃû: void auto_vehicle_contr(float speed_in,u8 cmd)
ÃèÊö:   void auto_vehicle_contr(float speed_in,u8 cmd):Ò£¿Ø¿ØÖÆÐ¡³µ
ÊäÈë²ÎÊý:u8 cmd£ºÒÆ¶¯Ö¸Áî   float speed_in£º×Ô¶¯ÔËÐÐËÙ¶È

*********************************************************************************************************************************************/
void auto_vehicle_contr(float speed_in,u8 cmd)
{
	if(OT_SON==0||AGVStatus.EmerStop==1)            //µ±¼±Í£»òÕß¶ÏÊ¹ÄÜÊ±£¬Ð¡³µÍ£Ö¹
	{
		Vehicle_contr_Jog(0,0);           //ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹
		Vehicle_contr_Auto(0,0,0);				//×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹		
	}
	else if(AGVStatus.SONSetup==1&&AGVStatus.EmerStop==0)																//µ±Ã»ÓÐÍ£Ö¹ÐÅºÅÊäÈë
	{	
		switch(cmd)
		{
			case 0x05: Vehicle_contr_Auto(speed_in,0,0);break;             		 //Ð¡³µÇ°½ø				
			case 0x06: Vehicle_contr_Auto(speed_in,3.0,3);break;               //ÓÒ×ªÐ¡½Ç¶È					
			case 0x07: Vehicle_contr_Auto(speed_in,2.0,3);break;               //ÓÒ×ª´ó½Ç¶È					
			case 0x08: Vehicle_contr_Auto(speed_in,2.5,2);break;               //×ó×ªÐ¡½Ç¶È					
			case 0x09: Vehicle_contr_Auto(speed_in,1.4,2);break;               //×ó×ª´ó½Ç¶È			
			default:break;
		}
	}
	else	
	{
		Vehicle_contr_Jog(0,0);           //ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹
		Vehicle_contr_Auto(0,0,0);				//×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹	
	}			
}

/**********************¶æ»ú·½Ïò¿ØÖÆ***********************************/
//Steering_contral(u32 pwmvalue)
//ÐÎ²Î£ºu32 pwmvalue ¸ø¶¨µÄPWMÖµ
//·µ»Ø£ºÎÞ
//
//
//
/************************************************************************/
void Steering_contral(u32 pwmvalue)
{
	
	u32 temp;
	temp=pwmvalue;
	if(temp>LLimit)	temp=LLimit;
	if(temp<RLimit)	temp=RLimit;
	TIM_SetCompare4(TIM4,5000-temp);                //TIM_SetCompare1(TIM4,spd_lf1);
}