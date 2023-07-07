//ÉñÖÛIIIºÅ¿ª·¢°å
#include "24l01.h"
#include "delay.h"
#include "spi.h"
#include "led.h"
#include "math.h"
#include "AGV_contr.h"
#include "vehicle_key.h" 
#include "HEAD.h"
#include "Time.h" 
//NRF24L01 Çı¶¯º¯Êı 
			
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x01}; //·¢ËÍµØÖ·							    
//³õÊ¼»¯24L01µÄIO¿Ú
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG, ENABLE);//Ê¹ÄÜGPIOB,GÊ±ÖÓ
	
	 //GPIOB14³õÊ¼»¯ÉèÖÃ:ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯PB14
			
//	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);           //Ê¹ÄÜGPIOµÄÊ±ÖÓ
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE;              //NRF24L01 Ä£¿éÆ¬Ñ¡ĞÅºÅ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CE, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);   //Ê¹ÄÜGPIOµÄÊ±ÖÓ
//	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);          
  GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN;      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CSN, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                           //³õÊ¼»¯Ê±ÏÈÀ­¸ß
  Set_NRF24L01_CSN;                                   //³õÊ¼»¯Ê±ÏÈÀ­¸ß

    //ÅäÖÃNRF2401µÄIRQ
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//ÊäÈë
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_IRQ, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NRF24L01_IRQ,NRF24L01_IRQ);

	SPI1_Init();                                       //³õÊ¼»¯SPI
	Clr_NRF24L01_CE; 	                               //Ê¹ÄÜ24L01
	Set_NRF24L01_CSN;                                  //SPIÆ¬Ñ¡È¡Ïû
}
//ÉÏµç¼ì²âNRF24L01ÊÇ·ñÔÚÎ»
//Ğ´5¸öÊı¾İÈ»ºóÔÙ¶Á»ØÀ´½øĞĞ±È½Ï£¬ÏàÍ¬Ê±·µ»ØÖµ:0£¬±íÊ¾ÔÚÎ»;·ñÔò·µ»Ø1£¬±íÊ¾²»ÔÚÎ»	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//Ğ´Èë5¸ö×Ö½ÚµÄµØÖ·.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);              //¶Á³öĞ´ÈëµÄµØÖ·  	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;					   
	if(i!=5)return 1;                               //NRF24L01²»ÔÚÎ»	
	return 0;		                                //NRF24L01ÔÚÎ»
}	 	 
//Í¨¹ıSPIĞ´¼Ä´æÆ÷
u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;                    //Ê¹ÄÜSPI´«Êä
  	status =SPI1_ReadWriteByte(regaddr); //·¢ËÍ¼Ä´æÆ÷ºÅ 
  	SPI1_ReadWriteByte(data);            //Ğ´Èë¼Ä´æÆ÷µÄÖµ
  	Set_NRF24L01_CSN;                    //½ûÖ¹SPI´«Êä	   
  	return(status);       		         //·µ»Ø×´Ì¬Öµ
}
//¶ÁÈ¡SPI¼Ä´æÆ÷Öµ £¬regaddr:Òª¶ÁµÄ¼Ä´æÆ÷
u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
 	Clr_NRF24L01_CSN;                //Ê¹ÄÜSPI´«Êä		
  	SPI1_ReadWriteByte(regaddr);     //·¢ËÍ¼Ä´æÆ÷ºÅ
  	reg_val=SPI1_ReadWriteByte(0XFF);//¶ÁÈ¡¼Ä´æÆ÷ÄÚÈİ
  	Set_NRF24L01_CSN;                //½ûÖ¹SPI´«Êä		    
  	return(reg_val);                 //·µ»Ø×´Ì¬Öµ
}	
//ÔÚÖ¸¶¨Î»ÖÃ¶Á³öÖ¸¶¨³¤¶ÈµÄÊı¾İ
//*pBuf:Êı¾İÖ¸Õë
//·µ»ØÖµ,´Ë´Î¶Áµ½µÄ×´Ì¬¼Ä´æÆ÷Öµ 
u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;                     //Ê¹ÄÜSPI´«Êä
  	status=SPI1_ReadWriteByte(regaddr);   //·¢ËÍ¼Ä´æÆ÷Öµ(Î»ÖÃ),²¢¶ÁÈ¡×´Ì¬Öµ   	   
 	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//¶Á³öÊı¾İ
  	Set_NRF24L01_CSN;                     //¹Ø±ÕSPI´«Êä
  	return status;                        //·µ»Ø¶Áµ½µÄ×´Ì¬Öµ
}
//ÔÚÖ¸¶¨Î»ÖÃĞ´Ö¸¶¨³¤¶ÈµÄÊı¾İ
//*pBuf:Êı¾İÖ¸Õë
//·µ»ØÖµ,´Ë´Î¶Áµ½µÄ×´Ì¬¼Ä´æÆ÷Öµ
u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
 	Clr_NRF24L01_CSN;                                    //Ê¹ÄÜSPI´«Êä
  	status = SPI1_ReadWriteByte(regaddr);                //·¢ËÍ¼Ä´æÆ÷Öµ(Î»ÖÃ),²¢¶ÁÈ¡×´Ì¬Öµ
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //Ğ´ÈëÊı¾İ	 
  	Set_NRF24L01_CSN;                                    //¹Ø±ÕSPI´«Êä
  	return status;                                       //·µ»Ø¶Áµ½µÄ×´Ì¬Öµ
}				   
//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊı¾İ
//txbuf:´ı·¢ËÍÊı¾İÊ×µØÖ·
//·µ»ØÖµ:·¢ËÍÍê³É×´¿ö
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;   
	Clr_NRF24L01_CE;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//Ğ´Êı¾İµ½TX BUF  32¸ö×Ö½Ú
 	Set_NRF24L01_CE;                                     //Æô¶¯·¢ËÍ	   
	while(READ_NRF24L01_IRQ!=0);                              //µÈ´ı·¢ËÍÍê³É
	state=NRF24L01_Read_Reg(STATUS);                     //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //Çå³ıTX_DS»òMAX_RTÖĞ¶Ï±êÖ¾
	if(state&MAX_TX)                                     //´ïµ½×î´óÖØ·¢´ÎÊı
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //Çå³ıTX FIFO¼Ä´æÆ÷ 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //·¢ËÍÍê³É
	{
		return TX_OK;
	}
	return 0xff;                                         //ÆäËûÔ­Òò·¢ËÍÊ§°Ü
}
//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊı¾İ
//txbuf:´ı·¢ËÍÊı¾İÊ×µØÖ·
//·µ»ØÖµ:0£¬½ÓÊÕÍê³É£»ÆäËû£¬´íÎó´úÂë
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //Çå³ıTX_DS»òMAX_RTÖĞ¶Ï±êÖ¾
	if(state&RX_OK)                                 //½ÓÊÕµ½Êı¾İ
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//¶ÁÈ¡Êı¾İ
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //Çå³ıRX FIFO¼Ä´æÆ÷ 
		return 0; 
	}	   
	return 1;                                      //Ã»ÊÕµ½ÈÎºÎÊı¾İ
}

//¸Ãº¯Êı³õÊ¼»¯NRF24L01µ½RXÄ£Ê½
//ÉèÖÃRXµØÖ·,Ğ´RXÊı¾İ¿í¶È,Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊı¾İÁË		   
void RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//Ğ´RX½ÚµãµØÖ·
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x3f);    //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ğ    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x3f);//Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);	     //ÉèÖÃRFÍ¨ĞÅÆµÂÊ		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//Ñ¡ÔñÍ¨µÀ0µÄÓĞĞ§Êı¾İ¿í¶È 	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07); //ÉèÖÃTX·¢Éä²ÎÊı,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x3f);  //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊı;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x71);  
  	Set_NRF24L01_CE;                                //CEÎª¸ß,½øÈë½ÓÊÕÄ£Ê½ 
	
		Clr_NRF24L01_CE;
		NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,NRF24L01_Read_Reg(STATUS)); //Çå³ıTX_DS»òMAX_RTÖĞ¶Ï±êÖ
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //Çå³ıRX FIFO¼Ä´æÆ÷ ¾
		Set_NRF24L01_CE;                                //CEÎª¸ß,½øÈë½ÓÊÕÄ£Ê½ 
	
	
	
}						 
//¸Ãº¯Êı³õÊ¼»¯NRF24L01µ½TXÄ£Ê½
//ÉèÖÃTXµØÖ·,Ğ´TXÊı¾İ¿í¶È,ÉèÖÃRX×Ô¶¯Ó¦´ğµÄµØÖ·,Ìî³äTX·¢ËÍÊı¾İ,Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR
//PWR_UP,CRCÊ¹ÄÜ
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊı¾İÁË		   
//CEÎª¸ß´óÓÚ10us,ÔòÆô¶¯·¢ËÍ.	 
void TX_Mode(void)
{														 
	Clr_NRF24L01_CE;	    
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //Ğ´TX½ÚµãµØÖ· 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //ÉèÖÃTX½ÚµãµØÖ·,Ö÷ÒªÎªÁËÊ¹ÄÜACK	  

  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ğ    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);//ÉèÖÃ×Ô¶¯ÖØ·¢¼ä¸ôÊ±¼ä:500us + 86us;×î´ó×Ô¶¯ÖØ·¢´ÎÊı:10´Î
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);       //ÉèÖÃRFÍ¨µÀÎª40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //ÉèÖÃTX·¢Éä²ÎÊı,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊı;PWR_UP,EN_CRC,16BIT_CRC,½ÓÊÕÄ£Ê½,¿ªÆôËùÓĞÖĞ¶Ï
	Set_NRF24L01_CE;                                  //CEÎª¸ß,10usºóÆô¶¯·¢ËÍ
}		  



//////////////////////////////////////////////Ò£¿ØÆ÷Ê§Áª¼ì²é////////////////////////////////////
//º¯ÊıÃû³Æ£ºu8 Syma_check(float *yaogan,u8 *tmp_buf)
//ÊäÈë£ºtmp_buf£º½ÓÊÕµ½µÄÒ£¿ØÆ÷»º´æÊı¾İ    
//Êä³ö£ºyaogan: Ò¡¸ËÄ£ÄâÁ¿Êı¾İ
//·µ»Ø£º1£ºÒ£¿ØÆ÷Ê§Áª     0£ºÒ£¿ØÆ÷Õı³£
//
////////////////////////////////////////////////////////////////////////////////////////////////
u8 Syma_check(float *yaogan,u8 *tmp_buf)
{
	u8 static Err_num=20;																				//Ò£¿ØÆ÷Ê§ÁªÑ­»·É¨Ãè´ÎÊı
	u8 static Err_mark=0;                                      //±¨¾¯±êÖ¾Î»
	u8 static Err[2]={0,0};																				//Ò£¿ØÆ÷Ê§Ğ§¼ì²â
	float static Err_stor[2]={0};                           //Ò£¿ØÆ÷Ê§Ğ§¼ì²â¼Ä´æÆ÷
	
	u8 i;
	if(NRF24L01_RxPacket(tmp_buf)==0)//Ò»µ©½ÓÊÕµ½ĞÅÏ¢,ÔòÏÔÊ¾³öÀ´.
	{			
		for(i=0;i<4;i++)
		{
			yaogan[i]=-(*(float *)(&tmp_buf[i*4]));  //Á½¸öÒ¡¸ËµÄÊı¾İ											
		}								
	}
		
	if(Err_stor[0]!=yaogan[0]||Err_stor[1]!=yaogan[1])  Err[0]=0;																										//µ±Êı¾İ²»ÏàÍ¬Ê±ÖØĞÂ¼ÆÊı
	if((Err_stor[0]==yaogan[0])&&(Err_stor[1]==yaogan[1])&&Err[0]<Err_num) 	Err[0]++;                                //µ±Êı¾İÏàÍ¬ÇÒĞ¡ÓÚ±¨¾¯Ñ­»·´ÎÊıÊÇ¿ªÊ¼¼ÆÊı
	
	if(Err[0]==0&&Err_mark==0)                                      //µ±ÎŞ±¨¾¯ÇÒ¼ÆÊıÎª0Ê±£¬Êı¾İ´æ´¢
	{
		Err_stor[0]=yaogan[0];
		Err_stor[1]=yaogan[1];
	}		

	if(Err[0]>Err_num-2)	Err_mark=1;													//µ±Êı¾İ³¬¹ı20´Î¶¼ÏàµÈ£¬ËµÃ÷Ò£¿ØÆ÷Ê§Áª													
	else Err_mark=0;																		//Ò£¿ØÆ÷Ã»ÓĞÊ§Áª
	

	if(Err_mark)															//µ±Ò£¿ØÆ÷Ê§Áª£¬ËùÓĞÊıÖµ³õÊ¼»¯
	{
		AGVStatus.Setup=0;									//Æô¶¯Í£Ö¹
		OT_SON=0;											//¶ÏÊ¹ÄÜ
		for(i=0;i<4;i++)
		{
			SymaStatus.Yaogan[i]=0;								//Ò¡¸ËÊä³öµÄÄ£ÄâÁ¿ÖµÖÃ0
			SymaStatus.Syma_cmd[i]=0;							//Ò£¿ØÆ÷Ò¡¸Ë·½·½ÏòÖ¸ÁîÖÃ0	
		}
	}
	
	return Err_mark;
}


//////////////////////////////////////////////Ò£¿ØÆ÷Ò¡¸ËÊı¾İ½ÓÊÕ¼°´¦Àí/////////////////////////////////
//ËµÃ÷£ºÒ£¿ØÆ÷Ò¡¸ËÖ¸ÁîµÄ¶ÁÈ¡
////º¯ÊıÃû³Æ£ºSyma_yaogan_receive(float *yaogan,u8 *tmp_buf,u8 * Syma_cmd,float spd)
////ÊäÈë£º float *yaogan£ºÒ¡¸Ë¸¡µãÊı  u8 *tmp_buf£ºÒ£¿ØÆ÷½ÓÊÜÊı¾İ  
////Êä³ö£ºu8 * Syma_cmd£ºÒ£¿ØÆ÷Ö¸Áî    float spd£ºÊäÈëËÙ¶È£¨ËÙ¶È×î´óÖµ£©
////·µ»Ø£ºµ÷ËÙÖµ
//Syma_cmd:
//[0]:×óÒ¡¸Ë£¨×óÓÒ£©  ×Ô×ªÖ¸Áî =1£º×ó×Ô×ª   =2£ºÓÒ×Ô×ª
//[1]:×óÒ¡¸Ë£¨ÉÏÏÂ£©  8¸öµµ7µµ×î¸ßËÙ£¬0µµÍ£Ö¹
//[2]:ÓÒÒ¡¸Ë£¨×óÓÒ£©  =1£º×ó×ª      =2£ºÓÒ×ª
//[3]:ÓÒÒ¡¸Ë£¨ÉÏÏÂ£©  =1£ºÇ°½ø        =2£ººóÍË
//////////////////////////////////////////////////////////////////////////////////////////////////

float Syma_yaogan_receive(float *yaogan,u8 *tmp_buf,u8 * syma_cmd,float spd)
{
	//////////////////////////×óÒ¡¸Ë×óÓÒ///////////////////////			
	
	
	if(AGVStatus.Setup==1&&AGVStatus.EmerStop!=1)													//Æô¶¯ÇÒ·Ç¼±Í£×´Ì¬ÏÂ£¬Ò£¿Ø·½Ïò¼°ËÙ¶È¹¦ÄÜÓĞĞ§						
	{
		//////////////////////////ÓÒÒ¡¸ËÉÏÏÂ//////////////////////	
		if(yaogan[3]<-0.3f) syma_cmd[3]=1;	                           //ÓÒÒ¡¸Ë£¨ÉÏÏÂ£©Ç°½ø
		else if(yaogan[3]>0.75f)  syma_cmd[3]=2; 									 		//ÓÒÒ¡¸Ë£¨ÉÏÏÂ£©ºóÍË
		else syma_cmd[3]=0; 
			
		if(syma_cmd[3]==0)                    											  //µ±Ç°ºóÃ»ÓĞÊı¾İÊä³ö
		{
			if(yaogan[2]<-0.3f) syma_cmd[2]=1;	                           //ÓÒÒ¡¸Ë£¨×óÓÒ£©×ó×ª
			else if(yaogan[2]>0.85f) 
			{
				if(syma_cmd[3]!=2) syma_cmd[2]=2; 									 //ÓÒÒ¡¸Ë£¨×óÓÒ£©ÓÒ×ª
				else syma_cmd[2]=0;                                  //Ç°½øµÄÓÅÏÈ¼¶¸ßÓÚ×ªÍä
			}	
		}	
		if(yaogan[2]>=-0.3f&&yaogan[2]<0.85f) syma_cmd[2]=0; 			//µ±Ò¡¸Ë×óÓÒÔÚ³õÊ¼Î»ÖÃ£¬ÔòÊı¾İÎ»0		

		//////////////////////////×óÒ¡¸Ë×óÓÒ×Ô×ª//////////////////////			
		
		if(syma_cmd[3]==0&&syma_cmd[2]==0)																			//ÓÒÒ¡¸ËÓÅÏÈ¼¶¸ßÓÚ×óÒ¡¸Ë×óÓÒ×Ô×ª
		{
			if(yaogan[0]<-0.3f)  syma_cmd[0]=1;                              			 //×óÒ¡¸ËÊı¾İ´¦Àí£¨×óÓÒ£©  ×ó×Ô×ª
			else if(yaogan[0]>0.75f) 	syma_cmd[0]=2;															//ÓÒ×Ô×ª
			else	syma_cmd[0]=0;																										//Í£Ö¹		
		}
		else 	syma_cmd[0]=0;	
		
		//////////////////////////×óÒ¡¸ËÉÏÏÂ//////////////////////			
		if(yaogan[1]<-0.3f)   syma_cmd[1]=5 ;                            			  //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©6µµ  
		else if(yaogan[1]<0&& yaogan[1]>=-0.3f)  syma_cmd[1]=4;							  //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©4µµ 
		else if(yaogan[1]<0.365f&& yaogan[1]>=0)  syma_cmd[1]=3;								//×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©3µµ 
		else if(yaogan[1]<0.6f&& yaogan[1]>=0.365f)  syma_cmd[1]=2;							//×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©2µµ 
		else if(yaogan[1]<0.9f&& yaogan[1]>=0.6f)  syma_cmd[1]=1;              //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©1µµ 
		else if(yaogan[1]>=0.9f)  syma_cmd[1]=0;																  //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©0µµ 	
		
//		if(yaogan[1]<-0.7)   syma_cmd[1]=9 ;                            			  //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©9µµ  
//		else if(yaogan[1]<-0.5&& yaogan[1]>=-0.7)  syma_cmd[1]=8;							  //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©8µµ 
//		else if(yaogan[1]<-0.3&& yaogan[1]>=-0.5)  syma_cmd[1]=7;								//×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©7µµ 
//		else if(yaogan[1]<-0.15&& yaogan[1]>=-0.3)  syma_cmd[1]=6;							//×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©6µµ 
//		else if(yaogan[1]<0.1&& yaogan[1]>=-0.15)  syma_cmd[1]=5;              //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©8µµ 
//		else if(yaogan[1]<0.3&& yaogan[1]>=0.1)  syma_cmd[1]=4;								//×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©4µµ 
//		else if(yaogan[1]<0.5&& yaogan[1]>=0.3)  syma_cmd[1]=3;							//×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©3µµ 
//		else if(yaogan[1]<0.7&& yaogan[1]>=0.5)  syma_cmd[1]=2;              //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©2µµ 
//		else if(yaogan[1]<0.85&& yaogan[1]>=0.7)  syma_cmd[1]=1;              //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©1µµ 
//		else if(yaogan[1]>=0.85)  syma_cmd[1]=0;																  //×óÒ¡¸ËÊı¾İ´¦Àí£¨ÉÏÏÂ£©0µµ 	
				
	}
	else
	{
		syma_cmd[0]=0;								
		syma_cmd[1]=0;
		syma_cmd[2]=0;
		syma_cmd[3]=0;
	}		
	
	//Ò£¿Ø¶¯×÷Ä£Ê½ÅĞ¶Ï     //ÓÅÏÈ¼¶   Ç°½øºóÍË>×ªÍä>×Ô×ª
		if(syma_cmd[0]==0&&syma_cmd[2]==0&&syma_cmd[3]==0)				//µ±Ò¡¸ËÊı¾İ¶¼Îª0£¬ÔòËµÃ÷Ğ¡³µÊÖ¶¯ÎŞ¶¯×÷
		{
			SymaStatus.RunMode=0;
		}
		else
		{
			if(syma_cmd[3]==1) SymaStatus.RunMode=1;				// Ğ¡³µÇ°½ø
			else if(syma_cmd[3]==2) SymaStatus.RunMode=2;  // Ğ¡³µºóÍË
			else if(syma_cmd[2]==1)	SymaStatus.RunMode=3;  // Ğ¡³µ×ó×ª							
			else if(syma_cmd[2]==2) SymaStatus.RunMode=4;  // Ğ¡³µÓÒ×ª	
			else if(syma_cmd[0]==1||syma_cmd[0]==2) SymaStatus.RunMode=5;  // Ğ¡³µ×Ô×ª
		
		}
	
	
	return spd/8*syma_cmd[1]; 																							//·µ»ØËÙ¶ÈÖµ
}



//////////////////////////////////////////////Ò¡¸Ë°´Å¥Ö¸Áî¶ÁÈ¡²¢´¦Àíí/////////////////////////////////
//ËµÃ÷£ºÒ¡¸Ë°´Å¥Ö¸Áî¶ÁÈ¡²¢´¦Àí
////º¯ÊıÃû³Æ£ºvoid Syma_button_receive(u8 *tmp_buf)
////ÊäÈë£º u8 *tmp_buf£ºÒ£¿ØÆ÷½ÓÊÜÊı¾İ  
////Êä³ö£ºu8 * syma_cmd£ºÒ£¿ØÆ÷Ö¸Áî   
////·µ»Ø£ºÎŞ
//tmp_buf[]:¶ÔÓ¦Ò£¿ØÆ÷¼Ä´æÆ÷Ïê¼ûÊÖ²á
//////////////////////////////////////////////////////////////////////////////////////////////////
void Syma_button_receive(u8 *tmp_buf,float spd_cle[2])
{
	u8 static Temp_spd_on=0;								//ËÙ¶ÈÖµĞŞ¸Ä±êÖ¾Î»
	u8 static Temp_spd_down=0;							//ËÙ¶ÈÖµĞŞ¸Ä±êÖ¾Î»
	u8 static Temp_clc_on=0;								//×ªÍä°ë¾¶ÖµĞŞ¸Ä±êÖ¾Î»
	u8 static Temp_clc_down=0;							//×ªÍä°ë¾¶ÖµĞŞ¸Ä±êÖ¾Î»
	
	if(tmp_buf[19]==1) AGVStatus.Setup=1;	              //½ÓÊÕµ½Æô¶¯ĞÅºÅĞ¡³µÉÏÊ¹ÄÜ
	if(tmp_buf[17]==1) AGVStatus.Setup=0;								//½ÓÊÕµ½Í£Ö¹ĞÅºÅ£¬Ğ¡³µ¶ÏÊ¹ÄÜ	
	
	if(AGVStatus.Setup==1)  OT_SON=1;											//µ±Ğ¡³µÆô¶¯£¬ÉÏÊ¹ÄÜ
	else  OT_SON=0;																//·ÇÆô¶¯£¬¶ÏÊ¹ÄÜ

	if(AGVStatus.Setup==1) AGVStatus.SONSetup=1;      //Æô¶¯×´Ì¬ÖÃÎ»
	else AGVStatus.SONSetup=0;;								//Æô¶¯×´Ì¬¸´Î»
	
	if(!AGVStatus.AJMode)									//ÊÖ¶¯Ä£Ê½ÏÂÉúĞ§
	{
		if(tmp_buf[22]==1&&tmp_buf[22]!=Temp_spd_on)						// µ±×óÒ¡¸ËÓÒ°´Å¥ÉÏÎªÉÏÉıÑØĞÅºÅ
		{
			if(spd_cle[0]<2.0f)																//µ±Êı¾İĞ¡ÓÚ3.0Ã×/Ãë£¬¿ÉÒÔÀÛ¼Ó
			{
				spd_cle[0]=spd_cle[0]+1.0f;		
			}	
			else spd_cle[0]=2.0f;
		}
		if(tmp_buf[23]==1&&tmp_buf[23]!=Temp_spd_down)		// µ±×óÒ¡¸ËÓÒ°´Å¥ÏÂÎªÉÏÉıÑØĞÅºÅ
		{
			if(spd_cle[0]>1.0)																//µ±Êı¾İ´óÓÚ1.0Ã×/Ãë  ¿ÉÒÔÀÛ¼õ
			{
				spd_cle[0]=	spd_cle[0]-1.0f;		
			}	
			else spd_cle[0]=1.0f;
		}

		if(tmp_buf[20]==1&&tmp_buf[20]!=Temp_clc_on)						// µ±ÓÒÒ¡¸Ë×ó°´Å¥ÉÏÎªÉÏÉıÑØĞÅºÅ
		{
			if(spd_cle[1]<5.0f)																//µ±Êı¾İĞ¡5.0Ã×£¬¿ÉÒÔÀÛ¼Ó
			{
				spd_cle[1]=spd_cle[1]+0.1f;		
			}	
			else spd_cle[1]=5.0f;
		}
		if(tmp_buf[21]==1&&tmp_buf[21]!=Temp_clc_down)		// µ±ÓÒÒ¡¸Ë×ó°´Å¥ÏÂÎªÉÏÉıÑØĞÅºÅ
		{
			if(spd_cle[1]>0.5f)																//µ±Êı¾İ´óÓÚ0.5Ã× ¿ÉÒÔÀÛ¼õ
			{
				spd_cle[1]=	spd_cle[1]-0.1f;		
			}	
			else spd_cle[1]=0.5f;
		}
	}
	
	
	/////////////////////ÕÕÃ÷µÆ¿ØÖÆ//////////////////////////
	if(tmp_buf[18]==1)	OT_JD2=1;		//ÕÕÃ÷µÆÆô¶¯
	else OT_JD2=0;									//ÕÕÃ÷µÆ¹Ø±Õ
	
	Temp_spd_on=tmp_buf[20];						//Êı¾İ´æ´¢
	Temp_spd_down=tmp_buf[21];					//Êı¾İ´æ´¢
	Temp_clc_on=tmp_buf[26];						//Êı¾İ´æ´¢
	Temp_clc_down=tmp_buf[27];					//Êı¾İ´æ´¢
}
