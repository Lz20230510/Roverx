//µçÄÔ´òÓ¡/ÉÏÎ»»ú


#include "sys.h"
#include "usart.h"	
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Head.h"
///// ////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼ş¼´¿É.
//#if SYSTEM_SUPPORT_OS
////#include "includes.h"					//ucos Ê¹ÓÃ	  
//#endif
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32F4Ì½Ë÷Õß¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ĞŞ¸ÄÈÕÆÚ:2014/6/10
//°æ±¾£ºV1.5
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ĞŞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ĞŞÕıÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ĞŞ¸ÄËµÃ÷
//1,ĞŞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ĞŞ¸ÄÁËUSART1_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊıÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART1_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊĞí½ÓÊÕµÄ×Ö½ÚÊı(²»´óÓÚ2µÄ14´Î·½)
//4,ĞŞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
//V1.5ĞŞ¸ÄËµÃ÷
//1,Ôö¼ÓÁË¶ÔUCOSIIµÄÖ§³Ö
////////////////////////////////////////////////////////////////////////////////// 	  
 
char temp_str1[10];

//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  
//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êı 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
u8  Battery_RX_BUF[Battery_REC_LEN]; //½ÓÊÕ»º³å,×î´óBattery_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û 
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
u16 USART1_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	

//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ
void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:ÇÀÕ¼ÓÅÏÈ¼¶£¬u8 SubPriority×ÓÓÅÏÈ¼¶    uart
{
  //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //Ê¹ÄÜGPIOAÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//Ê¹ÄÜUSART1Ê±ÖÓ
 
	//´®¿Ú1¶ÔÓ¦Òı½Å¸´ÓÃÓ³Éä
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9¸´ÓÃÎªUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10¸´ÓÃÎªUSART1
	
	//USART1¶Ë¿ÚÅäÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9ÓëGPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ËÙ¶È50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure); //³õÊ¼»¯PA9£¬PA10

   //USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
	
  USART_Cmd(USART1, ENABLE);  //Ê¹ÄÜ´®¿Ú1 
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖĞ¶Ï

	//Usart1 NVIC ÅäÖÃ
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//´®¿Ú1ÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

#endif

}


//void USART1_IRQHandler(void)                	//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//{
//	u8 Res;

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ
//		
//		if((USART1_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
//		{
//			if(USART1_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
//			{
//				if(Res!=0x0a)USART1_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
//				else USART1_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
//			}
//			else //»¹Ã»ÊÕµ½0X0D
//			{	
//				if(Res==0x0d)USART1_RX_STA|=0x4000;
//				else
//				{
//					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
//					USART1_RX_STA++;
//					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
//				}		 
//			}
//		}   		 
//  } 

//} 

void USART1_IRQHandler(void)                							//´®¿Ú6ÖĞ¶Ï·şÎñ³ÌĞò
{
	u8 Res;
	u16 lenth,char_len;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  	//½ÓÊÕÖĞ¶Ï
	{
		Res =USART_ReceiveData(USART1);												//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ	
		if(AGVStatus.MapDownMode==0&&AGVStatus.AUTOmode!=2)						//·ÇµØÍ¼ÏÂÔØÄ£Ê½
		{
			if((USART1_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
			{
				if(Res==0xDD||USART1_RX_BUF[0]==0xDD)						//µ±Ê×Ö¡Îª0xDD,¼ÌĞø
				{
					USART1_RX_BUF[USART1_RX_STA]=Res;
					USART1_RX_STA++;					
				}	
			}
			if(USART1_RX_STA>(Battery_REC_LEN -1))USART1_RX_STA=0;	//Êı¾İ½ÓÊÕÍê³É£¬ÇåÁã	
			if(Res==0x77)	 USART1_RX_STA=0;	//Êı¾İ½ÓÊÕÍê³É£¬ÇåÁã	
		}
		else		//µØÍ¼ÏÂÔØÄ£Ê½
		{


			if(Res==0x0d||Res==0x3b||Res==0x0A) USART1_RX_STA|=0x8000;				//ÊÕµ½»Ø³µ»òÕß·ÖºÅ
			else
			{
				USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
				USART1_RX_STA++;
				if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
			}		 

		}
		if((USART1_RX_STA&0x8000)!=0)				//µ±½ÓÊÕµ½0x0d
		{
			
			char_len=USART1_RX_STA&0x7FFF;		//»ñÈ¡µ±Ç°Êı¾İ³¤¶È
			if(USART1_RX_BUF[0]=='F'&&USART1_RX_BUF[1]=='R'&&USART1_RX_BUF[char_len-2]=='O'&&USART1_RX_BUF[char_len-1]=='R')				//µ±½ÓÊÕ±êÊ¶·ûÕıÈ·
			{
				MapPot=MapMsg_Propress(USART1_RX_BUF,strlen((char*)USART1_RX_BUF));							//¶Ô½ÓÊÕµ½µÄµØÍ¼Êı¾İ½øĞĞ´¦Àí	
				if(MapPot.Num==(u16)Paramt_BUFF[10][10]) MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);			//µØÍ¼´íÎó¼ì²é
			}
			USART1_RX_STA=0;					//±ê¼ÇÇåÁã
		}		
	} 
} 


#endif	
	


///************************Ãû³ÆµçÁ¿Ñ¯ÎÊ·¢ËÍ³ÌĞò*********************/
////void Battery_Send(u8 len,u8 *cmd)
////ÊäÈë²ÎÊı£ºu8 len£ºÊı¾İ³¤¶È£¬      u8 *cmd£º·¢ËÍµÄÊı¾İ
////
///*****************************************************************/
//void Battery_Send(u8 len,u8 *cmd)
//{
//	u8 t;
//	for(t=0;t<len;t++)
//	{
//		USART_SendData(USART1, cmd[t]);         //Ïò´®¿Ú1·¢ËÍÊı¾İ
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ı·¢ËÍ½áÊø
//	}
//}


///************************»ñÈ¡µç³Ø×´Ì¬ĞÅÏ¢Êı¾İ************************/
////º¯ÊıĞÎÊ½£ºu8 Battery_Capacity(u8 * cmd)
////ÊäÈë²ÎÊı£º´®¿ÚÊı¾İ
////·µ»ØÖµ£ºµçÁ¿ĞÅÏ¢²ÎÊı
////
///****************************************************************/
//BatteryMsgDef Battery_Capacity(u8 * cmd)
//{
//	u8 i;
//	u16 static checkStor=0;
//	u16 checkTemp;
//	BatteryMsgDef static batteryMsg;
//	union MultiType static multiType;
//	
//	checkStor=0;
//	if(cmd[0]==0xDD && cmd[33]==0x77)			//µ±Ê×Ö¡Î²Ö¡ÕıÈ·
//	{
//		for(i=2;i<31;i++)
//		{
//			checkStor +=(u16)cmd[i];				//Êı¾İÄÚÈİÀÛ¼Ó
//		}
//		checkStor=(checkStor^0xFFFF)+1;		//ÇóĞ£Ñé
//		checkTemp=((u16)cmd[31]<<8)+(u16)cmd[32];			//½ÓÊÕÊı¾İµÄĞ£ÑéÖµ
//		
//		if(checkStor==checkTemp)			//µ±Ğ£ÑéÕıÈ·£¬ÔòÊı¾İ¿ÉÒÔ·¢ËÍ¸ø²ÎÊı´æ´¢½á¹¹Ìå
//		{
//			
//			
//			multiType.float_=((u16)cmd[4]<<8) + (u16)cmd[5];			//×ÜµçÑ¹
//			batteryMsg.total_Vot=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[6]<<8) + (u16)cmd[7];				//µçÁ÷
//			batteryMsg.current=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[8]<<8) + (u16)cmd[9];						//Ê£ÓàÈİÁ¿
//			batteryMsg.SOC=multiType.float_/100;	

//			
//			batteryMsg.cycle=((u16)cmd[12]<<8) + (u16)cmd[13];						//³äµçÑ­»·´ÎÊı
//			batteryMsg.protectStatus=((u16)cmd[20]<<8) + (u16)cmd[21];						//±£»¤×´Ì¬
//			batteryMsg.RSOC=(int)cmd[23];					//µç³ØÊ£ÓàÈİÁ¿°Ù·Ö±È
//		}		
//	}
//	return batteryMsg;
//}


//u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num)
//{
//	u8 lenth=0;
//	char static temp_str[130];
//	sprintf(&temp_str[0],"%.1lf",OA);
//	VehicleParamt.OA_ve=&temp_str[0];
//	sprintf(&temp_str[15*1],"%.1lf",OB);
//	VehicleParamt.OB_ve=&temp_str[15*1];
//	sprintf(&temp_str[15*2],"%.2f",angle);
//	VehicleParamt.angle=&temp_str[15*2];
//	
//	sprintf(&temp_str[15*3],"%.5lf",lont_du);
//	VehicleParamt.longti=&temp_str[15*3];
//	sprintf(&temp_str[15*4],"%.5lf",lat_du);
//	VehicleParamt.lati=&temp_str[15*4];
//	sprintf(&temp_str[15*5],"%.1f",angle_near);
//	VehicleParamt.angle_near=&temp_str[15*5];
//	sprintf(&temp_str[15*6],"%.1lf",OA_near);
//	VehicleParamt.OA_near=&temp_str[15*6];
//	sprintf(&temp_str[15*7],"%.1lf",OB_near);
//	VehicleParamt.OB_near=&temp_str[15*7];
//	sprintf(&temp_str[15*8],"%d",num);
//	VehicleParamt.OAOB_num=&temp_str[15*8];
//	
//	
//	printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
//	VehicleParamt.fir_fram,VehicleParamt.OA_ve,VehicleParamt.OB_ve,
//	VehicleParamt.angle,VehicleParamt.longti,VehicleParamt.lati,
//	VehicleParamt.angle_near,VehicleParamt.OA_near,VehicleParamt.OB_near,VehicleParamt.OAOB_num,
//	VehicleParamt.last_fram);
//	printf("\r\n");
//	USART1_RX_STA=0;				
//	
//	return lenth;
//}


/******************* ½ÓÊÕÉÏÎ»»úÊı¾İ£¬²¢¸ù¾İÉÏÎ»»ú±¨ÎÄ£¬½øĞĞÏà¹Ø²Ù×÷*******************************/
//º¯ÊıĞÎÊ½£ºMapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
//ÊäÈë²ÎÊı£ºu8 * usartBuff£º´®¿ÚBUFF£¬  u16 len:Êı¾İ³¤¶ÈĞÅÏ¢ 
//·µ»Ø²ÎÊı£ºÊı¾İ±àºÅ
//º¯Êı¹¦ÄÜ£º½ÓÊÕÉÏÎ»»úÊı¾İ£¬²¢¸ù¾İÉÏÎ»»ú±¨ÎÄ£¬½øĞĞÏà¹Ø²Ù×÷    =0£ºÎŞ²Ù×÷   =1£º´æ´¢µØÍ¼   =2£ºµØÍ¼´æ´¢Çø¸ñÊ½»¯  =3£º·¢ËÍµç×ÓµØÍ¼»ù±¾ĞÅÏ¢  =254:ÎŞÈÎºÎÈÎÎñ
/*********************************************************************/
MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
{

	u16 i,k,j=0;
	int mode;
	u8 lenth,stor[8];//Êı¾İ³¤¶È,¶ººÅÎ»ÖÃ´æ´¢
	char static temp[20];
	char buff;	
	MapPotDef static mapPot,mapPotTemp;
	union MultiType multiType;
	
	buff=usartBuff[3];
	sscanf((char*)&buff,"%d",&mode);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ
	j=0;	
	
	for(i=0;i<len;i++)				//»ñÈ¡¶ººÅÎ»ÖÃ
	{
		buff=usartBuff[i];			//»ñÈ¡µ¥¸ö×Ö·ûÊı¾İ
		if(buff==',')
		{
			stor[j]=i;
			j++;
		}		
	}		
	switch(mode)
	{
		case 0: break;
		case 1:											//·¢ËÍ»ØÀ´µÄÊÇOA OB angleÊı¾İ   //Àı×Ó£ºFR,1,-33.6193923403310,0149.835464517472,-0289.516314197572,1,OR   ¸ñÊ½:FR,¹¦ÄÜÂë,µã£¨OAÖµ£©,µã£¨OBÖµ£©,µã£¨º½Ïò½Ç£©,µã±àºÅ,OR
		{
			k=0;
			j=1;
			memset(temp,0,strlen(temp));				
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
			sscanf(temp,"%f",&mapPot.OA);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ    »ñµÃOA
			
			j=2;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%f",&mapPot.OB);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃOB

			j=3;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%f",&mapPot.angle);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ	»ñµÃangle

			j=4;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%d",&mapPot.Num);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃµØÍ¼Î»ÖÃµãĞÅÏ¢		

			mapPot.mode=(u8)mode;				//ÈÎÎñÄ£Ê½
		}break;	
		case 2:											//µØÍ¼´æ´¢Çø¸ñÊ½»¯   //¸ñÊ½  :FR,¹¦ÄÜÂë,OR
		{		
			mapPot.mode=(u8)mode;				//ÈÎÎñÄ£Ê½
		}break;
		case 3:											//·¢ËÍµç×ÓµØÍ¼»ù±¾ĞÅÏ¢ //Àı×Ó£ºFR,3,118.4652105558884,31.376517521623,2600,20,OR   ¸ñÊ½£ºFR,¹¦ÄÜÂë,¾­¶ÈÖµ(¶È¸ñÊ½),Î¬¶ÈÖµ(¶È¸ñÊ½),µØÍ¼ÉÏ×ÜµÄµãÊıÁ¿,Ã¿¸öÖ±Ïß¶ÎÉÏµãÊıÁ¿,OR
		{
			k=0;
			j=1;
			memset(temp,0,strlen(temp));				
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
			sscanf(temp,"%lf",&mapPot.longti_du);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ    »ñµÃ¾­¶ÈÊı¾İ£¨¶È¸ñÊ½£©
			multiType.double_=mapPot.longti_du;							
			Paramt_BUFF[Pg_OutsideParmt][1]=multiType.u32Array_[0];				//doubleÀàĞÍÊı¾İµÍ32Î»
			Paramt_BUFF[Pg_OutsideParmt][2]=multiType.u32Array_[1];				//doubleÀàĞÍÊı¾İ¸ß32Î»
			
			
			j=2;
			memset(temp,0,strlen(temp));				
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
			sscanf(temp,"%lf",&mapPot.lati_du);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ    »ñµÃÎ¬¶ÈÊı¾İ£¨¶È¸ñÊ½£©
			multiType.double_=mapPot.lati_du;		
			Paramt_BUFF[Pg_OutsideParmt][3]=multiType.u32Array_[0];				//doubleÀàĞÍÊı¾İµÍ32Î»
			Paramt_BUFF[Pg_OutsideParmt][4]=multiType.u32Array_[1];				//doubleÀàĞÍÊı¾İ¸ß32Î»
			
			j=3;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%d",&mapPot.grossNumPot);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃµØÍ¼ÉÏ×ÜµÄµãÊıÁ¿,
			multiType.u32_=mapPot.grossNumPot;
			Paramt_BUFF[Pg_OutsideParmt][10]=multiType.u32_;			//µ¼Èë²ÎÊı»º³åÇø
			
			
			j=4;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%d",&mapPot.numPot);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃÃ¿¸öÖ±Ïß¶ÎÉÏµãÊıÁ¿,
			multiType.u32_=mapPot.numPot;
			Paramt_BUFF[Pg_OutsideParmt][11]=multiType.u32_;			//µ¼Èë²ÎÊı»º³åÇø

			mapPot.mode=(u8)mode;				//ÈÎÎñÄ£Ê½
		}break;
		default:
		{
			mapPot.mode=254;
		}break;
	}
	return 	mapPot;
}



/*******************************µØÍ¼ÏÂÔØ³ÌĞò********************************/
//º¯ÊıĞÎÊ½£ºvoid Map_Download(void)
//ÊäÈë²ÎÊı£º
//Êä³ö²ÎÊı£º
//·µ»Ø²ÎÊı£º
//º¯ÊıËµÃ÷£ºµØÍ¼ÏÂÔØÏà¹Øº¯Êı
/*************************************************************************/
void Map_Download(void)
{
	switch(MapPot.mode)
	{
		case 0: break;					//
		case 1:									//µç×ÓµØÍ¼ÏÂÔØ		
		{
					//Map_Save2FLASH(MapPot,FloatSize);		//µØÍ¼Êı¾İ´æ´¢	
			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//µ±´æ´¢ÇøÊı¾İÎª 0xFFFFFFFF,²ÅÄÜ´æ´¢µØÍ¼Êı¾İ	
			MapPot.mode=0;						//Ä£Ê½¹Ø±Õ£¬²»ÔÚ´æ´¢
		}break;
		case 2:									//µç×ÓµØÍ¼²ÎÊı´æ´¢Çø¸ñÊ½»¯
		{
			SetScreen(25);														//³õÊ¼»¯Íê³É£¬ÇĞ»»»­Ãæ³õÊ¼»¯
			STMFLASH_Clear(MAPOA_Addr,1);								//OA´æ´¢Çø×ÜÇå
			STMFLASH_Clear(MAPOB_Addr,1);								//OB´æ´¢Çø×ÜÇå
			STMFLASH_Clear(MAPAngle_Addr,1);								//angle´æ´¢Çø×ÜÇå
			SetScreen(4);														//ÈË»ú½çÃæ³õÊ¼»¯Íê³É£¬ÇĞ»»»Ø²ÎÊıÔËĞĞ3
			MapPot.mode=0;						//Ä£Ê½¹Ø±Õ£¬²»ÔÚ´æ´¢
		}break;
		case 3:									//µç×ÓµØÍ¼³õÊ¼Êı¾İ´æ´¢
		{			
			MapPot.mode=0;						//Ä£Ê½¹Ø±Õ£¬²»ÔÚ´æ´¢
		}break;
		default:break;
	}	
}


/*******************************µØÍ¼´íÎó¼ì²é***************************/
//º¯ÊıĞÎÊ½£ºu16 MapTable_check(u16 lastPotNum,float OAOBLimt)
//ÊäÈë²ÎÊı£ºu16 lastPotNum:ÏÂÔØµÄ×îºóÒ»¸öµãÖµ£¬OAOB¼«ÏŞÖµ
//·µ»Ø²ÎÊı£º·µ»Ø´íÎóµãµÄÊıÁ¿
//º¯ÊıËµÃ÷£º¶ÔµØÍ¼´æ´¢ÇøµÄµØÍ¼½øĞĞ´íÎó¼ì²é
//
//
/*********************************************************************/
u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
{
	MapPotDef mapPot;
	u16 errNum,i;			//´íÎóµãÊıÁ¿
	for(i=1;i<lastPotNum+1;i++)
	{	
		mapPot=Map_ReadFromFLASH(i,4);				//¶ÁÈ¡µç×ÓµØÍ¼´æ´¢ÇøÊı¾İ
		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) errNum++;					//µ±·¢ÏÖÓĞ´íÎóµØÍ¼ĞÅÏ¢£¬Ôò´íÎóÖµÀÛ¼Ó	
	}
	return errNum;
}



/***************************µç×ÓµØÍ¼´íÎóµãĞŞ¸´*************************/
//º¯ÊıĞÎÊ½£ºvoid MapTable_repair(u16 lastPotNum,float OAOBLimt)
//ÊäÈë²ÎÊı£ºu16 lastPotNum:ÏÂÔØµÄ×îºóÒ»¸öµãÖµ£¬OAOB¼«ÏŞÖµ
//·µ»Ø²ÎÊı£º
//º¯ÊıËµÃ÷£º¶ÔµØÍ¼´æ´¢ÇøµÄ´íÎóµØÍ¼½øĞĞĞŞ¸´
//
//
/*********************************************************************/
void MapTable_repair(u16 lastPotNum,float OAOBLimt)
{
	MapPotDef mapPot;
	u16 i;			
	for(i=1;i<lastPotNum+1;i++)
	{	
		mapPot=Map_ReadFromFLASH(i,4);			//¶ÁÈ¡´æ´¢ÇøµØÍ¼Êı¾İ
		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) 	//µ±·¢ÏÖÓĞ´íÎóµØÍ¼ĞÅÏ¢£¬¿ªÊ¼Òì³£µØÍ¼ĞÅÏ¢ĞŞ¸´
		{
			if(i==1)				//µ±Òì³£µãÎ»µÚÒ»×éÊı¾İ
			{
				for(i=2;i<lastPotNum+1;i++)   //µÚÒ»¸öÊı¾İÍùºó¼ì²é»¹ÓĞ´íÎóµã
				{
					mapPot=Map_ReadFromFLASH(i,4);			//¶ÁÈ¡´æ´¢ÇøµØÍ¼Êı¾İ
					if(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt) 	//µ±·¢ÏÖ´íÎóµØÍ¼ÎŞÒì³££¬Ôò½«¸ÃµØÍ¼ĞÅºÅ×é¸³Öµµ½µÚÒ»×éÊı¾İ
					{
						mapPot.Num=mapPot.Num-1;
						Map_Save2FLASH(mapPot,FloatSize);		//µ±´æ´¢ÇøÊı¾İÎª 0xFFFFFFFF,²ÅÄÜ´æ´¢µØÍ¼Êı¾İ
						break;			//Ìø³öÑ­»·
					}
				}		
			}
			else
			{
				mapPot=Map_ReadFromFLASH(i-1,4);			//¶ÁÈ¡´æ´¢ÇøµØÍ¼Êı¾İ
				mapPot.Num=mapPot.Num+1;
				Map_Save2FLASH(mapPot,FloatSize);		//µ±´æ´¢ÇøÊı¾İÎª 0xFFFFFFFF,²ÅÄÜ´æ´¢µØÍ¼Êı¾İ							
			}				
		}
	}	
	MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);				//µØÍ¼´íÎóµãÊıÁ¿
}
















//#include "sys.h"
//#include "usart.h"	
//#include "HEAD.h"
//#include "Motor.h"
//#include "Vehicle.h"
//////////////////////////////////////////////////////////////////////////////////// 	 
////Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼ş¼´¿É.
//#if SYSTEM_SUPPORT_OS
//#include "FreeRTOS.h"					//FreeRTOSÊ¹ÓÃ	 
//#include "mpu6050.h"
//#endif
////////////////////////////////////////////////////////////////////////////////////	 
////±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
////ALIENTEK STM32F4Ì½Ë÷Õß¿ª·¢°å
////´®¿Ú1³õÊ¼»¯		   
////ÕıµãÔ­×Ó@ALIENTEK
////¼¼ÊõÂÛÌ³:www.openedv.com
////ĞŞ¸ÄÈÕÆÚ:2014/6/10
////°æ±¾£ºV1.5
////°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
////Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
////All rights reserved
////********************************************************************************
////V1.3ĞŞ¸ÄËµÃ÷ 
////Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
////¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
////Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
////ĞŞÕıÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
////V1.4ĞŞ¸ÄËµÃ÷
////1,ĞŞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
////2,ĞŞ¸ÄÁËUSART_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊıÎª2µÄ14´Î·½
////3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊĞí½ÓÊÕµÄ×Ö½ÚÊı(²»´óÓÚ2µÄ14´Î·½)
////4,ĞŞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
////V1.5ĞŞ¸ÄËµÃ÷
////1,Ôö¼ÓÁË¶ÔUCOSIIµÄÖ§³Ö
//////////////////////////////////////////////////////////////////////////////////// 	  
// 

////////////////////////////////////////////////////////////////////
////¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  

//BluetoothDef Bluetooth;
//#if 1
//#pragma import(__use_no_semihosting)             
////±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı                 
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;       
////¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////ÖØ¶¨Òåfputcº¯Êı 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
//	USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif
// 
//#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
////´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
////×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
//u8 USART_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//u8 USART_SD_BUF[USART_REC_LEN];			//·¢ËÍ»º³åÇø £¬×î´óUSART_REC_LEN_S¸ö×Ö½Ú.
////½ÓÊÕ×´Ì¬
////bit15£¬	½ÓÊÕÍê³É±êÖ¾
////bit14£¬	½ÓÊÕµ½0x0d
////bit13~0£¬	½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
//u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	

////³õÊ¼»¯IO ´®¿Ú1 
////bound:²¨ÌØÂÊ
////void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:ÇÀÕ¼ÓÅÏÈ¼¶£¬u8 SubPriority×ÓÓÅÏÈ¼¶
////{
////  //GPIO¶Ë¿ÚÉèÖÃ
////  GPIO_InitTypeDef GPIO_InitStructure;
////	USART_InitTypeDef USART_InitStructure;
////	NVIC_InitTypeDef NVIC_InitStructure;
////	
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //Ê¹ÄÜGPIOAÊ±ÖÓ
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//Ê¹ÄÜUSART1Ê±ÖÓ
//// 
////	//´®¿Ú1¶ÔÓ¦Òı½Å¸´ÓÃÓ³Éä
////	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9¸´ÓÃÎªUSART1
////	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10¸´ÓÃÎªUSART1
////	
////	//USART1¶Ë¿ÚÅäÖÃ
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9ÓëGPIOA10
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ËÙ¶È50MHz
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
////	GPIO_Init(GPIOA,&GPIO_InitStructure); //³õÊ¼»¯PA9£¬PA10

////   //USART1 ³õÊ¼»¯ÉèÖÃ
////	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
////	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
////	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
////	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
////	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
////	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
////  USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
////	
////  USART_Cmd(USART1, ENABLE);  //Ê¹ÄÜ´®¿Ú1 
////	
////#if EN_USART1_RX	
////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖĞ¶Ï

////	//Usart1 NVIC ÅäÖÃ
////  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//´®¿Ú1ÖĞ¶ÏÍ¨µÀ
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//ÇÀÕ¼ÓÅÏÈ¼¶3
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//×ÓÓÅÏÈ¼¶3
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
////	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

////#endif

////}
//void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:ÇÀÕ¼ÓÅÏÈ¼¶£¬u8 SubPriority×ÓÓÅÏÈ¼¶
//{
//  //GPIO¶Ë¿ÚÉèÖÃ
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //Ê¹ÄÜGPIOAÊ±ÖÓ
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//Ê¹ÄÜUSART1Ê±ÖÓ
//	
//	
// 
//	//´®¿Ú1¶ÔÓ¦Òı½Å¸´ÓÃÓ³Éä
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOA9¸´ÓÃÎªUSART1
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOA10¸´ÓÃÎªUSART1
//	
//	//USART1¶Ë¿ÚÅäÖÃ
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOA9ÓëGPIOA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ËÙ¶È50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
//	GPIO_Init(GPIOD,&GPIO_InitStructure); //³õÊ¼»¯PA9£¬PA10

//   //USART1 ³õÊ¼»¯ÉèÖÃ
//	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
//	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
//  USART_Init(USART2, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
//	
//  USART_Cmd(USART2, ENABLE);  //Ê¹ÄÜ´®¿Ú1 
//	
//#if EN_USART1_RX	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖĞ¶Ï

//	//Usart1 NVIC ÅäÖÃ
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//´®¿Ú1ÖĞ¶ÏÍ¨µÀ
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//ÇÀÕ¼ÓÅÏÈ¼¶3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//×ÓÓÅÏÈ¼¶3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
//	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

//#endif

//}

////void USART1_IRQHandler(void)                	//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
////{
////	u8 Res;
////	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
////	{
////		Res =USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ
////		
////		if((USART_RX_STA&0x8000)==0)//½ÓÊ ÕÎ´Íê³É
////		{
////			if(USART_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
////			{
////				if(Res!=0x0a)USART_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
////				else USART_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
////			}
////			else //»¹Ã»ÊÕµ½0X0D
////			{	
////				if(Res==0x0d)USART_RX_STA|=0x4000;
////				else
////				{
////					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
////					USART_RX_STA++;
////					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
////				}		 
////			}
////		}   		 
////	} 
////} 



////void USART1_IRQHandler(void)                	//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
////{
////	u8 Res;
////	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
////	{
////		Res =USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ
////		if((Res>=0x41&&Res<=0x4A) || (Res>=0x58&&Res<=0x5A))	
////			 {Bluetooth.buff=Res;
////			 usart1_send(Res);
////			 } 
////		else Bluetooth.value=0x00;				
////		if(Res==0x58 && VehicleSpd<=0.5) VehicleSpd=VehicleSpd+0.1;
////		if(Res==0x59 && VehicleSpd>=0.0) VehicleSpd=VehicleSpd-0.1;
////			
////		
////	} 
////} 

//void USART2_IRQHandler(void)                	//´®¿Ú2ÖĞ¶Ï·şÎñ³ÌĞò
//{
//	u8 Res;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
//	{
//		Res =USART_ReceiveData(USART2);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ
////    USART_SendData(USART2,Res);
//		if((Res>=0x41&&Res<=0x4A) || (Res>=0x58&&Res<=0x5A))	Bluetooth.buff=Res;
//		else Bluetooth.value=0x00;			
//		
//		if(Res==0x58 && VehicleSpd<=0.5) VehicleSpd=VehicleSpd+0.1;
//		if(Res==0x59 && VehicleSpd>=0.0) VehicleSpd=VehicleSpd-0.1;
//		
//	} 
//} 
//#endif	
///**************************À¶ÑÀÒ£¿ØÆ÷¿ØÖÆ********************* 
////º¯ÊıÃû£ºu8 HC_05_Trans(u8 res)
////ÊäÈë²ÎÊı£ºu8 res£º´®¿ÚÊı¾İ
////·µ»ØÖµ£º0
//---------------------------------------------------
//0x00	 £º   ÎŞÖ¸Áî£¬Í£³µ			
//---------------------------------------------------
//0x41	A£º		Ç°½ø
//---------------------------------------------------
//0x45	E£º		ºóÍË
//---------------------------------------------------
//0x47	G£º		×ó×ª
//---------------------------------------------------
//0x43	C£º		ÓÒ×ª
//---------------------------------------------------
//0x48	H£º		×óÇ°
//---------------------------------------------------
//0x42	B£º		ÓÒÇ°
//---------------------------------------------------
//0x46	F£º		×óºó
//---------------------------------------------------
//0x44	D£º		ÓÒºó
//---------------------------------------------------
//0x4A 	J£º		Ò£¸ĞÄ£Ê½
//---------------------------------------------------
//0x58	X£º		ËÙ¶È+
//---------------------------------------------------
//0x59	Y£º		ËÙ¶È-
//---------------------------------------------------
//---------------------------------------------------
//****************************************************************/ 

//u8 HC_05_Trans(u8 res)
//{
//	switch(res)
//	{		
//		case 0x41:	//Ç°½ø
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,90);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x41;
//		}break;			
//		case 0x45:	//ºóÍË
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,90);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x45;
//		}break;			
//		case 0x47:	//×ó×ª
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,97.63);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x47;
//		}break;			
//		case 0x43:	//ÓÒ×ª
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,82.37);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x43;
//		}break;			
//		case 0x48:	//×óÇ°
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,95.04);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x48;
//		}break;			
//		case 0x42:	//ÓÒÇ°
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,85);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x42;
//		}break;			
//		case 0x46:	//×óºó
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,97.63);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x46;
//		}break;			
//		case 0x44:	//ÓÒºó
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,82.37);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x44;
//		}break;			
//		case 0x4A:	//Ò£¸ËÄ£Ê½
//		{
//		
//			return 0x4A;
//		}break;			
//		case 0x58:	//ËÙ¶È¼Ó
//		{
//			
//			return 0x58;
//		}break;			
//		case 0x59:	//ËÙ¶È¼õ
//		{
//			
//			return 0x59;
//		}break;	
//		case 0x5A:	//Í£Ö¹
//		{
//			//TIM_SetCompare1(TIM4,Median);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(0,0,MotorMsg);
//			return 0x59;
//		}break;	
//		default:			//¿ÕÖ¸Áî
//		{
//			TIM_SetCompare1(TIM4,Median);	//ĞŞ¸Ä±È½ÏÖµ£¬ĞŞ¸ÄÕ¼¿Õ±È
//			Wheel_control(0,0,MotorMsg);
//			return 0x00;
//		}break;				
//		
//	}
//}
///**************************************************************************
//Function: Serial port 1 sends data
//Input   : The data to send
//Output  : none
//º¯Êı¹¦ÄÜ£º´®¿Ú1·¢ËÍÊı¾İ
//Èë¿Ú²ÎÊı£ºÒª·¢ËÍµÄÊı¾İ
//·µ»Ø  Öµ£ºÎŞ
//**************************************************************************/
//void usart1_send(u8 data)
//{
//	USART1->DR = data;
//	while((USART1->SR&0x40)==0);	
//}





///************************Ãû³ÆµçÁ¿Ñ¯ÎÊ·¢ËÍ³ÌĞò*********************/
////void Battery_Send(u8 len,u8 *cmd)
////ÊäÈë²ÎÊı£ºu8 len£ºÊı¾İ³¤¶È£¬      u8 *cmd£º·¢ËÍµÄÊı¾İ
////
///*****************************************************************/
//void Battery_Send(u8 len,u8 *cmd)
//{
//	u8 t;
//	for(t=0;t<len;t++)
//	{
//		USART_SendData(USART1, cmd[t]);         //Ïò´®¿Ú1·¢ËÍÊı¾İ
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ı·¢ËÍ½áÊø
//	}
//}


///************************»ñÈ¡µç³Ø×´Ì¬ĞÅÏ¢Êı¾İ************************/
////º¯ÊıĞÎÊ½£ºu8 Battery_Capacity(u8 * cmd)
////ÊäÈë²ÎÊı£º´®¿ÚÊı¾İ
////·µ»ØÖµ£ºµçÁ¿ĞÅÏ¢²ÎÊı
////
///****************************************************************/
//BatteryMsgDef Battery_Capacity(u8 * cmd)
//{
//	u8 i;
//	u16 static checkStor=0;
//	u16 checkTemp;
//	BatteryMsgDef static batteryMsg;
//	union MultiType static multiType;
//	
//	checkStor=0;
//	if(cmd[0]==0xDD && cmd[33]==0x77)			//µ±Ê×Ö¡Î²Ö¡ÕıÈ·
//	{
//		for(i=2;i<31;i++)
//		{
//			checkStor +=(u16)cmd[i];				//Êı¾İÄÚÈİÀÛ¼Ó
//		}
//		checkStor=(checkStor^0xFFFF)+1;		//ÇóĞ£Ñé
//		checkTemp=((u16)cmd[31]<<8)+(u16)cmd[32];			//½ÓÊÕÊı¾İµÄĞ£ÑéÖµ
//		
//		if(checkStor==checkTemp)			//µ±Ğ£ÑéÕıÈ·£¬ÔòÊı¾İ¿ÉÒÔ·¢ËÍ¸ø²ÎÊı´æ´¢½á¹¹Ìå
//		{
//			
//			
//			multiType.float_=((u16)cmd[4]<<8) + (u16)cmd[5];			//×ÜµçÑ¹
//			batteryMsg.total_Vot=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[6]<<8) + (u16)cmd[7];				//µçÁ÷
//			batteryMsg.current=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[8]<<8) + (u16)cmd[9];						//Ê£ÓàÈİÁ¿
//			batteryMsg.SOC=multiType.float_/100;	

//			
//			batteryMsg.cycle=((u16)cmd[12]<<8) + (u16)cmd[13];						//³äµçÑ­»·´ÎÊı
//			batteryMsg.protectStatus=((u16)cmd[20]<<8) + (u16)cmd[21];						//±£»¤×´Ì¬
//			batteryMsg.RSOC=(int)cmd[23];					//µç³ØÊ£ÓàÈİÁ¿°Ù·Ö±È
//		}		
//	}
//	return batteryMsg;
//}


//u8 vehicle_upload(double OA,double OB,float angle,double lont_du,double lat_du,float angle_near,double OA_near,double OB_near,int num)
//{
//	u8 lenth=0;
//	char static temp_str[130];
//	sprintf(&temp_str[0],"%.1lf",OA);
//	VehicleParamt.OA_ve=&temp_str[0];
//	sprintf(&temp_str[15*1],"%.1lf",OB);
//	VehicleParamt.OB_ve=&temp_str[15*1];
//	sprintf(&temp_str[15*2],"%.2f",angle);
//	VehicleParamt.angle=&temp_str[15*2];
//	
//	sprintf(&temp_str[15*3],"%.5lf",lont_du);
//	VehicleParamt.longti=&temp_str[15*3];
//	sprintf(&temp_str[15*4],"%.5lf",lat_du);
//	VehicleParamt.lati=&temp_str[15*4];
//	sprintf(&temp_str[15*5],"%.1f",angle_near);
//	VehicleParamt.angle_near=&temp_str[15*5];
//	sprintf(&temp_str[15*6],"%.1lf",OA_near);
//	VehicleParamt.OA_near=&temp_str[15*6];
//	sprintf(&temp_str[15*7],"%.1lf",OB_near);
//	VehicleParamt.OB_near=&temp_str[15*7];
//	sprintf(&temp_str[15*8],"%d",num);
//	VehicleParamt.OAOB_num=&temp_str[15*8];
//	
//	
//	printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
//	VehicleParamt.fir_fram,VehicleParamt.OA_ve,VehicleParamt.OB_ve,
//	VehicleParamt.angle,VehicleParamt.longti,VehicleParamt.lati,
//	VehicleParamt.angle_near,VehicleParamt.OA_near,VehicleParamt.OB_near,VehicleParamt.OAOB_num,
//	VehicleParamt.last_fram);
//	printf("\r\n");
//	USART_RX_STA=0;				
//	
//	return lenth;
//}


///******************* ½ÓÊÕÉÏÎ»»úÊı¾İ£¬²¢¸ù¾İÉÏÎ»»ú±¨ÎÄ£¬½øĞĞÏà¹Ø²Ù×÷*******************************/
////º¯ÊıĞÎÊ½£ºMapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
////ÊäÈë²ÎÊı£ºu8 * usartBuff£º´®¿ÚBUFF£¬  u16 len:Êı¾İ³¤¶ÈĞÅÏ¢ 
////·µ»Ø²ÎÊı£ºÊı¾İ±àºÅ
////º¯Êı¹¦ÄÜ£º½ÓÊÕÉÏÎ»»úÊı¾İ£¬²¢¸ù¾İÉÏÎ»»ú±¨ÎÄ£¬½øĞĞÏà¹Ø²Ù×÷    =0£ºÎŞ²Ù×÷   =1£º´æ´¢µØÍ¼   =2£ºµØÍ¼´æ´¢Çø¸ñÊ½»¯  =3£º·¢ËÍµç×ÓµØÍ¼»ù±¾ĞÅÏ¢  =254:ÎŞÈÎºÎÈÎÎñ
///*********************************************************************/
//MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
//{

//	u16 i,k,j=0;
//	int mode;
//	u8 lenth,stor[8];//Êı¾İ³¤¶È,¶ººÅÎ»ÖÃ´æ´¢
//	char static temp[20];
//	char buff;	
//	MapPotDef static mapPot,mapPotTemp;
//	union MultiType multiType;
//	
//	buff=usartBuff[3];
//	sscanf((char*)&buff,"%d",&mode);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ
//	j=0;	
//	
//	for(i=0;i<len;i++)				//»ñÈ¡¶ººÅÎ»ÖÃ
//	{
//		buff=usartBuff[i];			//»ñÈ¡µ¥¸ö×Ö·ûÊı¾İ
//		if(buff==',')
//		{
//			stor[j]=i;
//			j++;
//		}		
//	}		
//	switch(mode)
//	{
//		case 0: break;
//		case 1:											//·¢ËÍ»ØÀ´µÄÊÇOA OB angleÊı¾İ   //Àı×Ó£ºFR,1,-33.6193923403310,0149.835464517472,-0289.516314197572,1,OR   ¸ñÊ½:FR,¹¦ÄÜÂë,µã£¨OAÖµ£©,µã£¨OBÖµ£©,µã£¨º½Ïò½Ç£©,µã±àºÅ,OR
//		{
//			k=0;
//			j=1;
//			memset(temp,0,strlen(temp));				
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
//			sscanf(temp,"%f",&mapPot.OA);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ    »ñµÃOA
//			
//			j=2;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%f",&mapPot.OB);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃOB

//			j=3;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%f",&mapPot.angle);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ	»ñµÃangle

//			j=4;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%d",&mapPot.Num);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃµØÍ¼Î»ÖÃµãĞÅÏ¢		

//			mapPot.mode=(u8)mode;				//ÈÎÎñÄ£Ê½
//		}break;	
//		case 2:											//µØÍ¼´æ´¢Çø¸ñÊ½»¯   //¸ñÊ½  :FR,¹¦ÄÜÂë,OR
//		{		
//			mapPot.mode=(u8)mode;				//ÈÎÎñÄ£Ê½
//		}break;
//		case 3:											//·¢ËÍµç×ÓµØÍ¼»ù±¾ĞÅÏ¢ //Àı×Ó£ºFR,3,118.4652105558884,31.376517521623,2600,20,OR   ¸ñÊ½£ºFR,¹¦ÄÜÂë,¾­¶ÈÖµ(¶È¸ñÊ½),Î¬¶ÈÖµ(¶È¸ñÊ½),µØÍ¼ÉÏ×ÜµÄµãÊıÁ¿,Ã¿¸öÖ±Ïß¶ÎÉÏµãÊıÁ¿,OR
//		{
//			k=0;
//			j=1;
//			memset(temp,0,strlen(temp));				
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
//			sscanf(temp,"%lf",&mapPot.longti_du);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ    »ñµÃ¾­¶ÈÊı¾İ£¨¶È¸ñÊ½£©
//			multiType.double_=mapPot.longti_du;							
//			Paramt_BUFF[Pg_OutsideParmt][1]=multiType.u32Array_[0];				//doubleÀàĞÍÊı¾İµÍ32Î»
//			Paramt_BUFF[Pg_OutsideParmt][2]=multiType.u32Array_[1];				//doubleÀàĞÍÊı¾İ¸ß32Î»
//			
//			
//			j=2;
//			memset(temp,0,strlen(temp));				
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
//			sscanf(temp,"%lf",&mapPot.lati_du);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ    »ñµÃÎ¬¶ÈÊı¾İ£¨¶È¸ñÊ½£©
//			multiType.double_=mapPot.lati_du;		
//			Paramt_BUFF[Pg_OutsideParmt][3]=multiType.u32Array_[0];				//doubleÀàĞÍÊı¾İµÍ32Î»
//			Paramt_BUFF[Pg_OutsideParmt][4]=multiType.u32Array_[1];				//doubleÀàĞÍÊı¾İ¸ß32Î»
//			
//			j=3;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%d",&mapPot.grossNumPot);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃµØÍ¼ÉÏ×ÜµÄµãÊıÁ¿,
//			multiType.u32_=mapPot.grossNumPot;
//			Paramt_BUFF[Pg_OutsideParmt][10]=multiType.u32_;			//µ¼Èë²ÎÊı»º³åÇø
//			
//			
//			j=4;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%d",&mapPot.numPot);						//»ñÈ¡ÉÏÎ»»ú·¢ËÍµÄÊı¾İÀàĞÍ		»ñµÃÃ¿¸öÖ±Ïß¶ÎÉÏµãÊıÁ¿,
//			multiType.u32_=mapPot.numPot;
//			Paramt_BUFF[Pg_OutsideParmt][11]=multiType.u32_;			//µ¼Èë²ÎÊı»º³åÇø

//			mapPot.mode=(u8)mode;				//ÈÎÎñÄ£Ê½
//		}break;
//		default:
//		{
//			mapPot.mode=254;
//		}break;
//	}
//	return 	mapPot;
//}



///*******************************µØÍ¼ÏÂÔØ³ÌĞò********************************/
////º¯ÊıĞÎÊ½£ºvoid Map_Download(void)
////ÊäÈë²ÎÊı£º
////Êä³ö²ÎÊı£º
////·µ»Ø²ÎÊı£º
////º¯ÊıËµÃ÷£ºµØÍ¼ÏÂÔØÏà¹Øº¯Êı
///*************************************************************************/
//void Map_Download(void)
//{
//	switch(MapPot.mode)
//	{
//		case 0: break;					//
//		case 1:									//µç×ÓµØÍ¼ÏÂÔØ		
//		{
//					//Map_Save2FLASH(MapPot,FloatSize);		//µØÍ¼Êı¾İ´æ´¢	
//			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//µ±´æ´¢ÇøÊı¾İÎª 0xFFFFFFFF,²ÅÄÜ´æ´¢µØÍ¼Êı¾İ	
//			MapPot.mode=0;						//Ä£Ê½¹Ø±Õ£¬²»ÔÚ´æ´¢
//		}break;
//		case 2:									//µç×ÓµØÍ¼²ÎÊı´æ´¢Çø¸ñÊ½»¯
//		{
//			SetScreen(25);														//³õÊ¼»¯Íê³É£¬ÇĞ»»»­Ãæ³õÊ¼»¯
//			STMFLASH_Clear(MAPOA_Addr,1);								//OA´æ´¢Çø×ÜÇå
//			STMFLASH_Clear(MAPOB_Addr,1);								//OB´æ´¢Çø×ÜÇå
//			STMFLASH_Clear(MAPAngle_Addr,1);								//angle´æ´¢Çø×ÜÇå
//			SetScreen(4);														//ÈË»ú½çÃæ³õÊ¼»¯Íê³É£¬ÇĞ»»»Ø²ÎÊıÔËĞĞ3
//			MapPot.mode=0;						//Ä£Ê½¹Ø±Õ£¬²»ÔÚ´æ´¢
//		}break;
//		case 3:									//µç×ÓµØÍ¼³õÊ¼Êı¾İ´æ´¢
//		{			
//			MapPot.mode=0;						//Ä£Ê½¹Ø±Õ£¬²»ÔÚ´æ´¢
//		}break;
//		default:break;
//	}	
//}


///*******************************µØÍ¼´íÎó¼ì²é***************************/
////º¯ÊıĞÎÊ½£ºu16 MapTable_check(u16 lastPotNum,float OAOBLimt)
////ÊäÈë²ÎÊı£ºu16 lastPotNum:ÏÂÔØµÄ×îºóÒ»¸öµãÖµ£¬OAOB¼«ÏŞÖµ
////·µ»Ø²ÎÊı£º·µ»Ø´íÎóµãµÄÊıÁ¿
////º¯ÊıËµÃ÷£º¶ÔµØÍ¼´æ´¢ÇøµÄµØÍ¼½øĞĞ´íÎó¼ì²é
////
////
///*********************************************************************/
//u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
//{
//	MapPotDef mapPot;
//	u16 errNum,i;			//´íÎóµãÊıÁ¿
//	for(i=1;i<lastPotNum+1;i++)
//	{	
//		mapPot=Map_ReadFromFLASH(i,4);				//¶ÁÈ¡µç×ÓµØÍ¼´æ´¢ÇøÊı¾İ
//		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) errNum++;					//µ±·¢ÏÖÓĞ´íÎóµØÍ¼ĞÅÏ¢£¬Ôò´íÎóÖµÀÛ¼Ó	
//	}
//	return errNum;
//}



///***************************µç×ÓµØÍ¼´íÎóµãĞŞ¸´*************************/
////º¯ÊıĞÎÊ½£ºvoid MapTable_repair(u16 lastPotNum,float OAOBLimt)
////ÊäÈë²ÎÊı£ºu16 lastPotNum:ÏÂÔØµÄ×îºóÒ»¸öµãÖµ£¬OAOB¼«ÏŞÖµ
////·µ»Ø²ÎÊı£º
////º¯ÊıËµÃ÷£º¶ÔµØÍ¼´æ´¢ÇøµÄ´íÎóµØÍ¼½øĞĞĞŞ¸´
////
////
///*********************************************************************/
//void MapTable_repair(u16 lastPotNum,float OAOBLimt)
//{
//	MapPotDef mapPot;
//	u16 i;			
//	for(i=1;i<lastPotNum+1;i++)
//	{	
//		mapPot=Map_ReadFromFLASH(i,4);			//¶ÁÈ¡´æ´¢ÇøµØÍ¼Êı¾İ
//		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) 	//µ±·¢ÏÖÓĞ´íÎóµØÍ¼ĞÅÏ¢£¬¿ªÊ¼Òì³£µØÍ¼ĞÅÏ¢ĞŞ¸´
//		{
//			if(i==1)				//µ±Òì³£µãÎ»µÚÒ»×éÊı¾İ
//			{
//				for(i=2;i<lastPotNum+1;i++)   //µÚÒ»¸öÊı¾İÍùºó¼ì²é»¹ÓĞ´íÎóµã
//				{
//					mapPot=Map_ReadFromFLASH(i,4);			//¶ÁÈ¡´æ´¢ÇøµØÍ¼Êı¾İ
//					if(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt) 	//µ±·¢ÏÖ´íÎóµØÍ¼ÎŞÒì³££¬Ôò½«¸ÃµØÍ¼ĞÅºÅ×é¸³Öµµ½µÚÒ»×éÊı¾İ
//					{
//						mapPot.Num=mapPot.Num-1;
//						Map_Save2FLASH(mapPot,FloatSize);		//µ±´æ´¢ÇøÊı¾İÎª 0xFFFFFFFF,²ÅÄÜ´æ´¢µØÍ¼Êı¾İ
//						break;			//Ìø³öÑ­»·
//					}
//				}		
//			}
//			else
//			{
//				mapPot=Map_ReadFromFLASH(i-1,4);			//¶ÁÈ¡´æ´¢ÇøµØÍ¼Êı¾İ
//				mapPot.Num=mapPot.Num+1;
//				Map_Save2FLASH(mapPot,FloatSize);		//µ±´æ´¢ÇøÊı¾İÎª 0xFFFFFFFF,²ÅÄÜ´æ´¢µØÍ¼Êı¾İ							
//			}				
//		}
//	}	
//	MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);				//µØÍ¼´íÎóµãÊıÁ¿
//}

