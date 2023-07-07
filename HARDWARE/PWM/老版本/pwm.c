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
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F407¿ª·¢°å
//¶¨Ê±Æ÷PWM£¬TIM2_CH1~4Çı¶¯4¸öÂÖì±µç»ú£¬TIM3_CH1Çı¶¯×ªÏòµç»ú   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//´´½¨ÈÕÆÚ:2014/5/4
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

float spdTemp=0;
u8 markTemp=0;
/********************************************************************************************************************************************
º¯ÊıÃû: TIM4_CH1_PWM_Init(u32 arr,u32 psc)
ÃèÊö:   TIM4_CH1_PWM¿ØÖÆ×óÇ°ÂÖì±µç»ú	 
ÊäÈë²ÎÊı:×Ô¶¯ÖØ×°ÔØ²ÎÊı£¬Ô¤·ÖÆµ²ÎÊı	 
·µ»ØÖµ:
*********************************************************************************************************************************************/
void TIM4_CH1_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTCÊ±ÖÓ	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);            //GPIOC6¸´ÓÃÎª¶¨Ê±Æ÷3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                        //GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PC6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
	
	//³õÊ¼»¯TIM3 Channel1 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔµÍ
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIMBÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
	
	TIM_Cmd(TIM4, ENABLE);                                           //Ê¹ÄÜTIM4	
}



/********************************************************************************************************************************************
º¯ÊıÃû: TIM4_CH2_PWM_Init(u32 arr,u32 psc)
ÃèÊö:TIM4_CH4_PWM¿ØÖÆ×óºóÂÖì±µç»ú	 
ÊäÈë²ÎÊı:×Ô¶¯ÖØ×°ÔØ²ÎÊı£¬Ô¤·ÖÆµ²ÎÊı	 
·µ»ØÖµ:
*********************************************************************************************************************************************/
void TIM4_CH2_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTBÊ±ÖÓ	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);            //GPIOB7¸´ÓÃÎª¶¨Ê±Æ÷4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                        //GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PB7
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
	
	//³õÊ¼»¯TIM3 Channel2 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔµÍ
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIM3ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
	
	TIM_Cmd(TIM4, ENABLE);                                           //Ê¹ÄÜTIM4	
}


/********************************************************************************************************************************************
º¯ÊıÃû: TIM4_CH3_PWM_Init(u32 arr,u32 psc)
ÃèÊö:TIM4_CH3_PWM¿ØÖÆÓÒÇ°ÂÖì±µç»ú	 
ÊäÈë²ÎÊı:×Ô¶¯ÖØ×°ÔØ²ÎÊı£¬Ô¤·ÖÆµ²ÎÊı	 
·µ»ØÖµ:
*********************************************************************************************************************************************/
void TIM4_CH3_PWM_Init(u32 arr,u32 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	         //TIM3Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 	         //Ê¹ÄÜPORTBÊ±ÖÓ	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);            //GPIOB8¸´ÓÃÎª¶¨Ê±Æ÷4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                        //GPIOB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	             //ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);                            //³õÊ¼»¯PC8
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;                         //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
	
	//³õÊ¼»¯TIM4 Channel3 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔµÍ
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIM4ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
	
	TIM_Cmd(TIM4, ENABLE);                                           //Ê¹ÄÜTIM4	
}

/********************************************************************************************************************************************
º¯ÊıÃû: TIM4_CH4_PWM_Init(u32 arr,u32 psc)
ÃèÊö:TIM4_CH4_PWM¿ØÖÆÓÒºóÂÖì±µç»ú	 
ÊäÈë²ÎÊı:×Ô¶¯ÖØ×°ÔØ²ÎÊı£¬Ô¤·ÖÆµ²ÎÊı	 
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
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;        //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;                            //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                   //³õÊ¼»¯¶¨Ê±Æ÷4
	
	//³õÊ¼»¯TIM3 Channel4 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔµÍ
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);  

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);                 //Ê¹ÄÜTIM4ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);                               //ARPEÊ¹ÄÜ 
	
	TIM_Cmd(TIM4,ENABLE);                                           //Ê¹ÄÜTIM4	
}


/********************************************************************************************************************************************
º¯ÊıÃû: TIM3_CH4_PWM_Init(u32 arr,u32 psc)
ÃèÊö:TIM3_CH4_PWM¿ØÖÆÓÒºóÂÖì±µç»ú	 
ÊäÈë²ÎÊı:×Ô¶¯ÖØ×°ÔØ²ÎÊı£¬Ô¤·ÖÆµ²ÎÊı	 
·µ»ØÖµ:
*********************************************************************************************************************************************/

void DIR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜGPIOFÊ±ÖÓ

  //GPIOB6,B7³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;//LED0ºÍLED1¶ÔÓ¦IO¿Ú
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);//GPIOF9,F10ÉèÖÃ¸ß£¬µÆÃğ
}


/********************************************************************************************************************************************
º¯ÊıÃû: u16 Speed_trans(u16 spped_in)
ÃèÊö:   u16 Speed_trans(u16 spped_in):µç»úÊµ¼Ê×ªËÙ×ª»»
ÊäÈë²ÎÊı:¸ø¶¨µÄµç»ú×ªËÙ 
·µ»ØÖµ: Ğ£ÕıºóµÄarrÖµ                       ×ª»»¹«Ê½£ºX=(y+0.6255)/0.9986
*********************************************************************************************************************************************/
u16 Speed_trans(u16 spped_in)
{
	u16 y;
	y=spped_in;
	return (y+0.6255)/0.9986;
}

/********************************************************************************************************************************************
º¯ÊıÃû: void Time_CH1234_Init(void)
ÃèÊö:    void Time_CH1234_Init(void):ËÄ¸öµç»úPWM³õÊ¼»¯


*********************************************************************************************************************************************/
void Time_CH1234_Init(void)
{
	TIM4_CH1_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH1£¬84M/84=1MhzµÄ¼ÆÊıÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 
	TIM4_CH2_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH2£¬84M/84=1MhzµÄ¼ÆÊıÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 	 	
	TIM4_CH3_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH3£¬84M/84=1MhzµÄ¼ÆÊıÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 
	TIM4_CH4_PWM_Init(5000-1,84-1); //³õÊ¼»¯TIM2 CH4£¬84M/84=1MhzµÄ¼ÆÊıÆµÂÊ,ÖØ×°ÔØÖµ500£¬ËùÒÔPWMÆµÂÊÎª 1M/500=2Khz. 
}


/********************************************************************************************************************************************
º¯ÊıÃû: void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode)
ÃèÊö:    Time_CH1234_Init(float spd_lf,float spd_lb,float spd_rf,float spd_rb):Ğ¡³µËÄÂÖ×ªËÙÉè¶¨   µ¥Î»£ºÃ×/Ãë   m/s
ÊäÈë²ÎÊı:spd_lf£º×óÇ°ÂÖ×ªËÙ   spd_lb£º×óºóÂÖ×ªËÙ   spd_rf£ºÓÒÇ°ÂÖ×ªËÙ    spd_rb£ºÓÒºóÂÖ×ªËÙ ,u8 mode:
ÊäÈë²ÎÊı:,u8 mode: =0  ÊäÈëËÙ¶ÈÖµÎªµ¥Î» m/s              =1£ºÊäÈëËÙ¶ÈÖµÎª r/min
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
	
	if(spd_lf2>=0)				//×óÇ°ÂÖ×ªËÙÎªÕı
	{
		OT_DIR1=1;
		TIM_SetCompare1(TIM4,spd_lf1);
	}
	else								//×óÇ°ÂÖ×ªËÙÎª¸º
	{
		OT_DIR1=0;
		TIM_SetCompare1(TIM4,spd_lf1);
	}
	
	if(spd_lb2>=0)				//×óºóÂÖ×ªËÙÎªÕı
	{
		OT_DIR2=1;
		TIM_SetCompare2(TIM4,spd_lb1);
	}
	else								//×óºóÂÖ×ªËÙÎª¸º
	{
		OT_DIR2=0;
		TIM_SetCompare2(TIM4,spd_lb1);
	}

	if(spd_rf2>=0)				//ÓÒÇ°ÂÖ×ªËÙÎªÕı
	{
		OT_DIR3=0;
		TIM_SetCompare3(TIM4,spd_rf1);
	}
	else								//ÓÒÇ°ÂÖ×ªËÙÎª¸º
	{
		OT_DIR3=1;
		TIM_SetCompare3(TIM4,spd_rf1);
	}

	if(spd_rb2>=0)				//ÓÒºóÂÖ×ªËÙÎªÕı
	{
		OT_DIR4=0;
		TIM_SetCompare4(TIM4,spd_rb1);
	}
	else								//ÓÒºóÂÖ×ªËÙÎª¸º
	{
		OT_DIR4=1;
		TIM_SetCompare4(TIM4,spd_rb1);
	}
}


/********************************************************************************************************************************************
º¯ÊıÃû: void Circle_contr(float speed_in,float R)
ÃèÊö:     void Circle_contr(float speed_in,float R):Ğ¡³µÔ²»¡ÔË¶¯
ÊäÈë²ÎÊı:float speed_in£ºĞ¡³µÒÆ¶¯ËÙ¶È¸ø¶¨    float R£ºĞ¡³µµÄÔ²»¡°ë¾¶    u8 mode:Ğ¡³µ×óÓÒÔ²»¡Ä£Ê½  mode=2£¬×óÔ²»¡   mode=3,ÓÒÔ²»¡

*********************************************************************************************************************************************/
void Circle_contr(float speed_in,float R,u8 mode)
{
	
	float spd_l;      //×ó²àÂÖ×ÓËÙ¶È
	float spd_r;      //ÓÒ²àÂÖ×ÓËÙ¶È
	spdTemp=speed_in;
	if(mode==2)						//×óÔ²»¡
	{
		markTemp=2;
		spd_l=speed_in*(R-AGVStatus.vehicle_width)/R;						//¼ÆËãĞ¡³µ×óÔ²»¡×ó²àÂÖ×Ó³µËÙ
		spd_r=speed_in*(R+AGVStatus.vehicle_width)/R;						//¼ÆËãĞ¡³µÓÒÔ²»¡ÓÒ²àÂÖ×Ó³µËÙ	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//ËÙ¶È¸ø¶¨
	}
	else if(mode==3)			//ÓÒÔ²»¡
	{
		spd_l=speed_in*(R+AGVStatus.vehicle_width)/R;						//¼ÆËãĞ¡³µ×óÔ²»¡×ó²àÂÖ×Ó³µËÙ
		spd_r=speed_in*(R-AGVStatus.vehicle_width)/R;						//¼ÆËãĞ¡³µÓÒÔ²»¡ÓÒ²àÂÖ×Ó³µËÙ	
		Speed_contr(spd_l,spd_l,spd_r,spd_r,0);					//ËÙ¶È¸ø¶¨
		markTemp=3;
	}	
}


/********************************************************************************************************************************************
º¯ÊıÃû: void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit)
ÃèÊö:   void Vehicle_contr_Auto(float speed_in,float R,u8 mode,u8 swit):Ğ¡³µÁ¬ĞøÔË¶¯
ÊäÈë²ÎÊı:float speed_in£ºĞ¡³µÒÆ¶¯ËÙ¶È¸ø¶¨    float R£ºĞ¡³µÔË¶¯µÄÔ²»¡°ë¾¶  ,u8 switch:Ğ¡³µÁ¬ĞøÔË¶¯¶¨Ê±¿ªÆô»òÕß¹Ø±Õ
u8 mode:Ğ¡³µÔË¶¯Ä£Ê½
mode=0:Á¬ĞøÇ°½ø
mode=1:Á¬ĞøºóÍË
mode=2:Á¬Ğø×óÔ²»¡ 
mode=3:Á¬ĞøÓÒÔ²»¡


*********************************************************************************************************************************************/
void Vehicle_contr_Auto(float speed_in,float R,u8 mode)
{		
	float static speed_in1;
	speed_in1=speed_in;
	if(mode==0)					//Á¬ĞøÇ°½ø
	{
		Speed_contr(speed_in1,speed_in1,speed_in1,speed_in1,0);
	}
	else if(mode==1)		//Á¬ĞøºóÍË
	{
		Speed_contr(-speed_in1,-speed_in1,-speed_in1,-speed_in1,0);
	}
	else if(mode==2)			//Á¬Ğø×óÔ²»¡
	{
		Circle_contr(speed_in1,R,2);
	}
	else if(mode==3)		//Á¬ĞøÓÒÔ²»¡
	{
		Circle_contr(speed_in1,R,3);
	}
}

/********************************************************************************************************************************************
º¯ÊıÃû: void Vehicle_contr_Jog(float speed_in,u8 mode)
ÃèÊö:   void Vehicle_contr_Jog(float speed_in,u8 mode):Ğ¡³µµã¶¯ÒÆ¶¯
ÊäÈë²ÎÊı:float speed_in£ºĞ¡³µÒÆ¶¯ËÙ¶È¸ø¶¨    Õ
u8 mode:Ğ¡³µÔË¶¯Ä£Ê½
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


/***********************************************Ğ¡³µÒ£¿Ø¿ØÖÆ************************************************************************
º¯ÊıÃû: void Syma_vehicle_contr(float speed_in,u16 cmd)
ÃèÊö:   void Syma_vehicle_contr(float speed_in,u16 cmd):Ò£¿Ø¿ØÖÆĞ¡³µ
ÊäÈë²ÎÊı:u8 key£ºÊÖ±úÖ¸ÁîÊı¾İ   u8 Scope_RL[2]£º´æ´¢Ò¡¸ËÊı¾İ  
float spd_Auto£º×Ô¶¯ËÙ¶È
float R£ºÔ²»¡°ë¾¶
float spd_Jog£ºµã¶¯ËÙ¶È
*********************************************************************************************************************************************/
void Syma_vehicle_contr(u8 * syma_cmd,u8 * tmd_buf,float spd_Auto,float R,float spd_Jog)
{
	
	if(OT_SON==0||AGVStatus.EmerStop==1)            //µ±¼±Í£»òÕß¶ÏÊ¹ÄÜÊ±£¬Ğ¡³µÍ£Ö¹
	{
		Vehicle_contr_Jog(0,0);           //ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹
		Vehicle_contr_Auto(0,0,0);				//×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹		
	}
	else if(OT_SON==1&&AGVStatus.EmerStop==0&&!AGVStatus.AJMode)																//µ±Ã»ÓĞÍ£Ö¹ĞÅºÅÊäÈë
	{	
		if(syma_cmd[0]==0)	Vehicle_contr_Jog(0,0);   			//×óÒ¡¸Ë×óÓÒÎª0 ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹		
		if(syma_cmd[2]==0&&syma_cmd[3]==0)	Vehicle_contr_Auto(0,0,0);		//ÓÒÒ¡¸ËÉÏÏÂ  ×óÓÒÎª0  ×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹	
		if(syma_cmd[0]==1)	Vehicle_contr_Jog(spd_Jog,2);									//Ğ¡³µ×ó×Ô×ª
		else if(syma_cmd[0]==2)	Vehicle_contr_Jog(spd_Jog,3);						  //Ğ¡³µÓÒ×Ô×ª													
		else 	Vehicle_contr_Jog(0,0);																			//Ğ¡³µÍ£Ö¹

		if(syma_cmd[3]==1)	Vehicle_contr_Auto(spd_Auto,R,0);																//Ğ¡³µÇ°½ø
		else if(syma_cmd[3]==2)	Vehicle_contr_Auto(spd_Auto,R,1);															//Ğ¡³µºóÍË

		if(syma_cmd[2]==1)	Vehicle_contr_Auto(spd_Auto,R,2);																//Ğ¡³µ×óÔ²»¡
		else if(syma_cmd[2]==2)	Vehicle_contr_Auto(spd_Auto,R,3);															//Ğ¡³µÓÒÔ²»¡							

	}

}



/***********************************************×Ô¶¯Ä£Ê½Ğ¡³µÔË¶¯¿ØÖÆ***********************************************************************
º¯ÊıÃû: void auto_vehicle_contr(float speed_in,u8 cmd)
ÃèÊö:   void auto_vehicle_contr(float speed_in,u8 cmd):Ò£¿Ø¿ØÖÆĞ¡³µ
ÊäÈë²ÎÊı:u8 cmd£ºÒÆ¶¯Ö¸Áî   float speed_in£º×Ô¶¯ÔËĞĞËÙ¶È

*********************************************************************************************************************************************/
void auto_vehicle_contr(float speed_in,u8 cmd)
{
	if(OT_SON==0||AGVStatus.EmerStop==1)            //µ±¼±Í£»òÕß¶ÏÊ¹ÄÜÊ±£¬Ğ¡³µÍ£Ö¹
	{
		Vehicle_contr_Jog(0,0);           //ÊÖ¶¯ËÙ¶È¸ø¶¨Í£Ö¹
		Vehicle_contr_Auto(0,0,0);				//×Ô¶¯ËÙ¶È¸ø¶¨Í£Ö¹		
	}
	else if(AGVStatus.SONSetup==1&&AGVStatus.EmerStop==0)																//µ±Ã»ÓĞÍ£Ö¹ĞÅºÅÊäÈë
	{	
		switch(cmd)
		{
			case 0x05: Vehicle_contr_Auto(speed_in,0,0);break;             		 //Ğ¡³µÇ°½ø				
			case 0x06: Vehicle_contr_Auto(speed_in,3.0,3);break;               //ÓÒ×ªĞ¡½Ç¶È					
			case 0x07: Vehicle_contr_Auto(speed_in,2.0,3);break;               //ÓÒ×ª´ó½Ç¶È					
			case 0x08: Vehicle_contr_Auto(speed_in,2.5,2);break;               //×ó×ªĞ¡½Ç¶È					
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

