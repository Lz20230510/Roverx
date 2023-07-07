//����III�ſ�����
#include "24l01.h"
#include "delay.h"
#include "spi.h"
#include "led.h"
#include "math.h"
#include "AGV_contr.h"
#include "vehicle_key.h" 
#include "HEAD.h"
#include "Time.h" 
//NRF24L01 �������� 
			
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x01}; //���͵�ַ							    
//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOB,Gʱ��
	
	 //GPIOB14��ʼ������:�������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB14
			
//	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);           //ʹ��GPIO��ʱ��
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE;              //NRF24L01 ģ��Ƭѡ�ź�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CE, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);   //ʹ��GPIO��ʱ��
//	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);          
  GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN;      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CSN, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                           //��ʼ��ʱ������
  Set_NRF24L01_CSN;                                   //��ʼ��ʱ������

    //����NRF2401��IRQ
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_IRQ, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NRF24L01_IRQ,NRF24L01_IRQ);

	SPI1_Init();                                       //��ʼ��SPI
	Clr_NRF24L01_CE; 	                               //ʹ��24L01
	Set_NRF24L01_CSN;                                  //SPIƬѡȡ��
}
//�ϵ���NRF24L01�Ƿ���λ
//д5������Ȼ���ٶ��������бȽϣ���ͬʱ����ֵ:0����ʾ��λ;���򷵻�1����ʾ����λ	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);              //����д��ĵ�ַ  	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;					   
	if(i!=5)return 1;                               //NRF24L01����λ	
	return 0;		                                //NRF24L01��λ
}	 	 
//ͨ��SPIд�Ĵ���
u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;                    //ʹ��SPI����
  	status =SPI1_ReadWriteByte(regaddr); //���ͼĴ����� 
  	SPI1_ReadWriteByte(data);            //д��Ĵ�����ֵ
  	Set_NRF24L01_CSN;                    //��ֹSPI����	   
  	return(status);       		         //����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ ��regaddr:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
 	Clr_NRF24L01_CSN;                //ʹ��SPI����		
  	SPI1_ReadWriteByte(regaddr);     //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	Set_NRF24L01_CSN;                //��ֹSPI����		    
  	return(reg_val);                 //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;                     //ʹ��SPI����
  	status=SPI1_ReadWriteByte(regaddr);   //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
  	Set_NRF24L01_CSN;                     //�ر�SPI����
  	return status;                        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//*pBuf:����ָ��
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
 	Clr_NRF24L01_CSN;                                    //ʹ��SPI����
  	status = SPI1_ReadWriteByte(regaddr);                //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	Set_NRF24L01_CSN;                                    //�ر�SPI����
  	return status;                                       //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;   
	Clr_NRF24L01_CE;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	Set_NRF24L01_CE;                                     //��������	   
	while(READ_NRF24L01_IRQ!=0);                              //�ȴ��������
	state=NRF24L01_Read_Reg(STATUS);                     //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //���TX_DS��MAX_RT�жϱ�־
	if(state&MAX_TX)                                     //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //�������
	{
		return TX_OK;
	}
	return 0xff;                                         //����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //���TX_DS��MAX_RT�жϱ�־
	if(state&RX_OK)                                 //���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;                                      //û�յ��κ�����
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x3f);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x3f);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);	     //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07); //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x3f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x71);  
  	Set_NRF24L01_CE;                                //CEΪ��,�������ģʽ 
	
		Clr_NRF24L01_CE;
		NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,NRF24L01_Read_Reg(STATUS)); //���TX_DS��MAX_RT�жϱ��
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //���RX FIFO�Ĵ��� �
		Set_NRF24L01_CE;                                //CEΪ��,�������ģʽ 
	
	
	
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(void)
{														 
	Clr_NRF24L01_CE;	    
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	Set_NRF24L01_CE;                                  //CEΪ��,10us����������
}		  



//////////////////////////////////////////////ң����ʧ�����////////////////////////////////////
//�������ƣ�u8 Syma_check(float *yaogan,u8 *tmp_buf)
//���룺tmp_buf�����յ���ң������������    
//�����yaogan: ҡ��ģ��������
//���أ�1��ң����ʧ��     0��ң��������
//
////////////////////////////////////////////////////////////////////////////////////////////////
u8 Syma_check(float *yaogan,u8 *tmp_buf)
{
	u8 static Err_num=20;																				//ң����ʧ��ѭ��ɨ�����
	u8 static Err_mark=0;                                      //������־λ
	u8 static Err[2]={0,0};																				//ң����ʧЧ���
	float static Err_stor[2]={0};                           //ң����ʧЧ���Ĵ���
	
	u8 i;
	if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
	{			
		for(i=0;i<4;i++)
		{
			yaogan[i]=-(*(float *)(&tmp_buf[i*4]));  //����ҡ�˵�����											
		}								
	}
		
	if(Err_stor[0]!=yaogan[0]||Err_stor[1]!=yaogan[1])  Err[0]=0;																										//�����ݲ���ͬʱ���¼���
	if((Err_stor[0]==yaogan[0])&&(Err_stor[1]==yaogan[1])&&Err[0]<Err_num) 	Err[0]++;                                //��������ͬ��С�ڱ���ѭ�������ǿ�ʼ����
	
	if(Err[0]==0&&Err_mark==0)                                      //���ޱ����Ҽ���Ϊ0ʱ�����ݴ洢
	{
		Err_stor[0]=yaogan[0];
		Err_stor[1]=yaogan[1];
	}		

	if(Err[0]>Err_num-2)	Err_mark=1;													//�����ݳ���20�ζ���ȣ�˵��ң����ʧ��													
	else Err_mark=0;																		//ң����û��ʧ��
	

	if(Err_mark)															//��ң����ʧ����������ֵ��ʼ��
	{
		AGVStatus.Setup=0;									//����ֹͣ
		OT_SON=0;											//��ʹ��
		for(i=0;i<4;i++)
		{
			SymaStatus.Yaogan[i]=0;								//ҡ�������ģ����ֵ��0
			SymaStatus.Syma_cmd[i]=0;							//ң����ҡ�˷�����ָ����0	
		}
	}
	
	return Err_mark;
}


//////////////////////////////////////////////ң����ҡ�����ݽ��ռ�����/////////////////////////////////
//˵����ң����ҡ��ָ��Ķ�ȡ
////�������ƣ�Syma_yaogan_receive(float *yaogan,u8 *tmp_buf,u8 * Syma_cmd,float spd)
////���룺 float *yaogan��ҡ�˸�����  u8 *tmp_buf��ң������������  
////�����u8 * Syma_cmd��ң����ָ��    float spd�������ٶȣ��ٶ����ֵ��
////���أ�����ֵ
//Syma_cmd:
//[0]:��ҡ�ˣ����ң�  ��תָ�� =1������ת   =2������ת
//[1]:��ҡ�ˣ����£�  8����7������٣�0��ֹͣ
//[2]:��ҡ�ˣ����ң�  =1����ת      =2����ת
//[3]:��ҡ�ˣ����£�  =1��ǰ��        =2������
//////////////////////////////////////////////////////////////////////////////////////////////////

float Syma_yaogan_receive(float *yaogan,u8 *tmp_buf,u8 * syma_cmd,float spd)
{
	//////////////////////////��ҡ������///////////////////////			
	
	
	if(AGVStatus.Setup==1&&AGVStatus.EmerStop!=1)													//�����ҷǼ�ͣ״̬�£�ң�ط����ٶȹ�����Ч						
	{
		//////////////////////////��ҡ������//////////////////////	
		if(yaogan[3]<-0.3f) syma_cmd[3]=1;	                           //��ҡ�ˣ����£�ǰ��
		else if(yaogan[3]>0.75f)  syma_cmd[3]=2; 									 		//��ҡ�ˣ����£�����
		else syma_cmd[3]=0; 
			
		if(syma_cmd[3]==0)                    											  //��ǰ��û���������
		{
			if(yaogan[2]<-0.3f) syma_cmd[2]=1;	                           //��ҡ�ˣ����ң���ת
			else if(yaogan[2]>0.85f) 
			{
				if(syma_cmd[3]!=2) syma_cmd[2]=2; 									 //��ҡ�ˣ����ң���ת
				else syma_cmd[2]=0;                                  //ǰ�������ȼ�����ת��
			}	
		}	
		if(yaogan[2]>=-0.3f&&yaogan[2]<0.85f) syma_cmd[2]=0; 			//��ҡ�������ڳ�ʼλ�ã�������λ0		

		//////////////////////////��ҡ��������ת//////////////////////			
		
		if(syma_cmd[3]==0&&syma_cmd[2]==0)																			//��ҡ�����ȼ�������ҡ��������ת
		{
			if(yaogan[0]<-0.3f)  syma_cmd[0]=1;                              			 //��ҡ�����ݴ������ң�  ����ת
			else if(yaogan[0]>0.75f) 	syma_cmd[0]=2;															//����ת
			else	syma_cmd[0]=0;																										//ֹͣ		
		}
		else 	syma_cmd[0]=0;	
		
		//////////////////////////��ҡ������//////////////////////			
		if(yaogan[1]<-0.3f)   syma_cmd[1]=5 ;                            			  //��ҡ�����ݴ������£�6��  
		else if(yaogan[1]<0&& yaogan[1]>=-0.3f)  syma_cmd[1]=4;							  //��ҡ�����ݴ������£�4�� 
		else if(yaogan[1]<0.365f&& yaogan[1]>=0)  syma_cmd[1]=3;								//��ҡ�����ݴ������£�3�� 
		else if(yaogan[1]<0.6f&& yaogan[1]>=0.365f)  syma_cmd[1]=2;							//��ҡ�����ݴ������£�2�� 
		else if(yaogan[1]<0.9f&& yaogan[1]>=0.6f)  syma_cmd[1]=1;              //��ҡ�����ݴ������£�1�� 
		else if(yaogan[1]>=0.9f)  syma_cmd[1]=0;																  //��ҡ�����ݴ������£�0�� 	
		
//		if(yaogan[1]<-0.7)   syma_cmd[1]=9 ;                            			  //��ҡ�����ݴ������£�9��  
//		else if(yaogan[1]<-0.5&& yaogan[1]>=-0.7)  syma_cmd[1]=8;							  //��ҡ�����ݴ������£�8�� 
//		else if(yaogan[1]<-0.3&& yaogan[1]>=-0.5)  syma_cmd[1]=7;								//��ҡ�����ݴ������£�7�� 
//		else if(yaogan[1]<-0.15&& yaogan[1]>=-0.3)  syma_cmd[1]=6;							//��ҡ�����ݴ������£�6�� 
//		else if(yaogan[1]<0.1&& yaogan[1]>=-0.15)  syma_cmd[1]=5;              //��ҡ�����ݴ������£�8�� 
//		else if(yaogan[1]<0.3&& yaogan[1]>=0.1)  syma_cmd[1]=4;								//��ҡ�����ݴ������£�4�� 
//		else if(yaogan[1]<0.5&& yaogan[1]>=0.3)  syma_cmd[1]=3;							//��ҡ�����ݴ������£�3�� 
//		else if(yaogan[1]<0.7&& yaogan[1]>=0.5)  syma_cmd[1]=2;              //��ҡ�����ݴ������£�2�� 
//		else if(yaogan[1]<0.85&& yaogan[1]>=0.7)  syma_cmd[1]=1;              //��ҡ�����ݴ������£�1�� 
//		else if(yaogan[1]>=0.85)  syma_cmd[1]=0;																  //��ҡ�����ݴ������£�0�� 	
				
	}
	else
	{
		syma_cmd[0]=0;								
		syma_cmd[1]=0;
		syma_cmd[2]=0;
		syma_cmd[3]=0;
	}		
	
	//ң�ض���ģʽ�ж�     //���ȼ�   ǰ������>ת��>��ת
		if(syma_cmd[0]==0&&syma_cmd[2]==0&&syma_cmd[3]==0)				//��ҡ�����ݶ�Ϊ0����˵��С���ֶ��޶���
		{
			SymaStatus.RunMode=0;
		}
		else
		{
			if(syma_cmd[3]==1) SymaStatus.RunMode=1;				// С��ǰ��
			else if(syma_cmd[3]==2) SymaStatus.RunMode=2;  // С������
			else if(syma_cmd[2]==1)	SymaStatus.RunMode=3;  // С����ת							
			else if(syma_cmd[2]==2) SymaStatus.RunMode=4;  // С����ת	
			else if(syma_cmd[0]==1||syma_cmd[0]==2) SymaStatus.RunMode=5;  // С����ת
		
		}
	
	
	return spd/8*syma_cmd[1]; 																							//�����ٶ�ֵ
}



//////////////////////////////////////////////ҡ�˰�ťָ���ȡ�������/////////////////////////////////
//˵����ҡ�˰�ťָ���ȡ������
////�������ƣ�void Syma_button_receive(u8 *tmp_buf)
////���룺 u8 *tmp_buf��ң������������  
////�����u8 * syma_cmd��ң����ָ��   
////���أ���
//tmp_buf[]:��Ӧң�����Ĵ�������ֲ�
//////////////////////////////////////////////////////////////////////////////////////////////////
void Syma_button_receive(u8 *tmp_buf,float spd_cle[2])
{
	u8 static Temp_spd_on=0;								//�ٶ�ֵ�޸ı�־λ
	u8 static Temp_spd_down=0;							//�ٶ�ֵ�޸ı�־λ
	u8 static Temp_clc_on=0;								//ת��뾶ֵ�޸ı�־λ
	u8 static Temp_clc_down=0;							//ת��뾶ֵ�޸ı�־λ
	
	if(tmp_buf[19]==1) AGVStatus.Setup=1;	              //���յ������ź�С����ʹ��
	if(tmp_buf[17]==1) AGVStatus.Setup=0;								//���յ�ֹͣ�źţ�С����ʹ��	
	
	if(AGVStatus.Setup==1)  OT_SON=1;											//��С����������ʹ��
	else  OT_SON=0;																//����������ʹ��

	if(AGVStatus.Setup==1) AGVStatus.SONSetup=1;      //����״̬��λ
	else AGVStatus.SONSetup=0;;								//����״̬��λ
	
	if(!AGVStatus.AJMode)									//�ֶ�ģʽ����Ч
	{
		if(tmp_buf[22]==1&&tmp_buf[22]!=Temp_spd_on)						// ����ҡ���Ұ�ť��Ϊ�������ź�
		{
			if(spd_cle[0]<2.0f)																//������С��3.0��/�룬�����ۼ�
			{
				spd_cle[0]=spd_cle[0]+1.0f;		
			}	
			else spd_cle[0]=2.0f;
		}
		if(tmp_buf[23]==1&&tmp_buf[23]!=Temp_spd_down)		// ����ҡ���Ұ�ť��Ϊ�������ź�
		{
			if(spd_cle[0]>1.0)																//�����ݴ���1.0��/��  �����ۼ�
			{
				spd_cle[0]=	spd_cle[0]-1.0f;		
			}	
			else spd_cle[0]=1.0f;
		}

		if(tmp_buf[20]==1&&tmp_buf[20]!=Temp_clc_on)						// ����ҡ����ť��Ϊ�������ź�
		{
			if(spd_cle[1]<5.0f)																//������С5.0�ף������ۼ�
			{
				spd_cle[1]=spd_cle[1]+0.1f;		
			}	
			else spd_cle[1]=5.0f;
		}
		if(tmp_buf[21]==1&&tmp_buf[21]!=Temp_clc_down)		// ����ҡ����ť��Ϊ�������ź�
		{
			if(spd_cle[1]>0.5f)																//�����ݴ���0.5�� �����ۼ�
			{
				spd_cle[1]=	spd_cle[1]-0.1f;		
			}	
			else spd_cle[1]=0.5f;
		}
	}
	
	
	/////////////////////�����ƿ���//////////////////////////
	if(tmp_buf[18]==1)	OT_JD2=1;		//����������
	else OT_JD2=0;									//�����ƹر�
	
	Temp_spd_on=tmp_buf[20];						//���ݴ洢
	Temp_spd_down=tmp_buf[21];					//���ݴ洢
	Temp_clc_on=tmp_buf[26];						//���ݴ洢
	Temp_clc_down=tmp_buf[27];					//���ݴ洢
}
