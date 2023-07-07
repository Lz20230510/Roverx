//���Դ�ӡ/��λ��


#include "sys.h"
#include "usart.h"	
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "Head.h"
///// ////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
//#if SYSTEM_SUPPORT_OS
////#include "includes.h"					//ucos ʹ��	  
//#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F4̽���߿�����
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/6/10
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART1_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART1_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 
char temp_str1[10];

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8  Battery_RX_BUF[Battery_REC_LEN]; //���ջ���,���Battery_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:��ռ���ȼ���u8 SubPriority�����ȼ�    uart
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif

}


//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 Res;

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//		
//		if((USART1_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART1_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
//				else USART1_RX_STA|=0x8000;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART1_RX_STA|=0x4000;
//				else
//				{
//					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
//					USART1_RX_STA++;
//					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//  } 

//} 

void USART1_IRQHandler(void)                							//����6�жϷ������
{
	u8 Res;
	u16 lenth,char_len;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  	//�����ж�
	{
		Res =USART_ReceiveData(USART1);												//(USART1->DR);	//��ȡ���յ�������	
		if(AGVStatus.MapDownMode==0&&AGVStatus.AUTOmode!=2)						//�ǵ�ͼ����ģʽ
		{
			if((USART1_RX_STA&0x8000)==0)//����δ���
			{
				if(Res==0xDD||USART1_RX_BUF[0]==0xDD)						//����֡Ϊ0xDD,����
				{
					USART1_RX_BUF[USART1_RX_STA]=Res;
					USART1_RX_STA++;					
				}	
			}
			if(USART1_RX_STA>(Battery_REC_LEN -1))USART1_RX_STA=0;	//���ݽ�����ɣ�����	
			if(Res==0x77)	 USART1_RX_STA=0;	//���ݽ�����ɣ�����	
		}
		else		//��ͼ����ģʽ
		{


			if(Res==0x0d||Res==0x3b||Res==0x0A) USART1_RX_STA|=0x8000;				//�յ��س����߷ֺ�
			else
			{
				USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
				USART1_RX_STA++;
				if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
			}		 

		}
		if((USART1_RX_STA&0x8000)!=0)				//�����յ�0x0d
		{
			
			char_len=USART1_RX_STA&0x7FFF;		//��ȡ��ǰ���ݳ���
			if(USART1_RX_BUF[0]=='F'&&USART1_RX_BUF[1]=='R'&&USART1_RX_BUF[char_len-2]=='O'&&USART1_RX_BUF[char_len-1]=='R')				//�����ձ�ʶ����ȷ
			{
				MapPot=MapMsg_Propress(USART1_RX_BUF,strlen((char*)USART1_RX_BUF));							//�Խ��յ��ĵ�ͼ���ݽ��д���	
				if(MapPot.Num==(u16)Paramt_BUFF[10][10]) MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);			//��ͼ������
			}
			USART1_RX_STA=0;					//�������
		}		
	} 
} 


#endif	
	


///************************���Ƶ���ѯ�ʷ��ͳ���*********************/
////void Battery_Send(u8 len,u8 *cmd)
////���������u8 len�����ݳ��ȣ�      u8 *cmd�����͵�����
////
///*****************************************************************/
//void Battery_Send(u8 len,u8 *cmd)
//{
//	u8 t;
//	for(t=0;t<len;t++)
//	{
//		USART_SendData(USART1, cmd[t]);         //�򴮿�1��������
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//	}
//}


///************************��ȡ���״̬��Ϣ����************************/
////������ʽ��u8 Battery_Capacity(u8 * cmd)
////�����������������
////����ֵ��������Ϣ����
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
//	if(cmd[0]==0xDD && cmd[33]==0x77)			//����֡β֡��ȷ
//	{
//		for(i=2;i<31;i++)
//		{
//			checkStor +=(u16)cmd[i];				//���������ۼ�
//		}
//		checkStor=(checkStor^0xFFFF)+1;		//��У��
//		checkTemp=((u16)cmd[31]<<8)+(u16)cmd[32];			//�������ݵ�У��ֵ
//		
//		if(checkStor==checkTemp)			//��У����ȷ�������ݿ��Է��͸������洢�ṹ��
//		{
//			
//			
//			multiType.float_=((u16)cmd[4]<<8) + (u16)cmd[5];			//�ܵ�ѹ
//			batteryMsg.total_Vot=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[6]<<8) + (u16)cmd[7];				//����
//			batteryMsg.current=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[8]<<8) + (u16)cmd[9];						//ʣ������
//			batteryMsg.SOC=multiType.float_/100;	

//			
//			batteryMsg.cycle=((u16)cmd[12]<<8) + (u16)cmd[13];						//���ѭ������
//			batteryMsg.protectStatus=((u16)cmd[20]<<8) + (u16)cmd[21];						//����״̬
//			batteryMsg.RSOC=(int)cmd[23];					//���ʣ�������ٷֱ�
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


/******************* ������λ�����ݣ���������λ�����ģ�������ز���*******************************/
//������ʽ��MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
//���������u8 * usartBuff������BUFF��  u16 len:���ݳ�����Ϣ 
//���ز��������ݱ��
//�������ܣ�������λ�����ݣ���������λ�����ģ�������ز���    =0���޲���   =1���洢��ͼ   =2����ͼ�洢����ʽ��  =3�����͵��ӵ�ͼ������Ϣ  =254:���κ�����
/*********************************************************************/
MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
{

	u16 i,k,j=0;
	int mode;
	u8 lenth,stor[8];//���ݳ���,����λ�ô洢
	char static temp[20];
	char buff;	
	MapPotDef static mapPot,mapPotTemp;
	union MultiType multiType;
	
	buff=usartBuff[3];
	sscanf((char*)&buff,"%d",&mode);						//��ȡ��λ�����͵���������
	j=0;	
	
	for(i=0;i<len;i++)				//��ȡ����λ��
	{
		buff=usartBuff[i];			//��ȡ�����ַ�����
		if(buff==',')
		{
			stor[j]=i;
			j++;
		}		
	}		
	switch(mode)
	{
		case 0: break;
		case 1:											//���ͻ�������OA OB angle����   //���ӣ�FR,1,-33.6193923403310,0149.835464517472,-0289.516314197572,1,OR   ��ʽ:FR,������,�㣨OAֵ��,�㣨OBֵ��,�㣨����ǣ�,����,OR
		{
			k=0;
			j=1;
			memset(temp,0,strlen(temp));				
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
			sscanf(temp,"%f",&mapPot.OA);						//��ȡ��λ�����͵���������    ���OA
			
			j=2;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%f",&mapPot.OB);						//��ȡ��λ�����͵���������		���OB

			j=3;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%f",&mapPot.angle);						//��ȡ��λ�����͵���������	���angle

			j=4;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%d",&mapPot.Num);						//��ȡ��λ�����͵���������		��õ�ͼλ�õ���Ϣ		

			mapPot.mode=(u8)mode;				//����ģʽ
		}break;	
		case 2:											//��ͼ�洢����ʽ��   //��ʽ  :FR,������,OR
		{		
			mapPot.mode=(u8)mode;				//����ģʽ
		}break;
		case 3:											//���͵��ӵ�ͼ������Ϣ //���ӣ�FR,3,118.4652105558884,31.376517521623,2600,20,OR   ��ʽ��FR,������,����ֵ(�ȸ�ʽ),ά��ֵ(�ȸ�ʽ),��ͼ���ܵĵ�����,ÿ��ֱ�߶��ϵ�����,OR
		{
			k=0;
			j=1;
			memset(temp,0,strlen(temp));				
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
			sscanf(temp,"%lf",&mapPot.longti_du);						//��ȡ��λ�����͵���������    ��þ������ݣ��ȸ�ʽ��
			multiType.double_=mapPot.longti_du;							
			Paramt_BUFF[Pg_OutsideParmt][1]=multiType.u32Array_[0];				//double�������ݵ�32λ
			Paramt_BUFF[Pg_OutsideParmt][2]=multiType.u32Array_[1];				//double�������ݸ�32λ
			
			
			j=2;
			memset(temp,0,strlen(temp));				
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
			sscanf(temp,"%lf",&mapPot.lati_du);						//��ȡ��λ�����͵���������    ���ά�����ݣ��ȸ�ʽ��
			multiType.double_=mapPot.lati_du;		
			Paramt_BUFF[Pg_OutsideParmt][3]=multiType.u32Array_[0];				//double�������ݵ�32λ
			Paramt_BUFF[Pg_OutsideParmt][4]=multiType.u32Array_[1];				//double�������ݸ�32λ
			
			j=3;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%d",&mapPot.grossNumPot);						//��ȡ��λ�����͵���������		��õ�ͼ���ܵĵ�����,
			multiType.u32_=mapPot.grossNumPot;
			Paramt_BUFF[Pg_OutsideParmt][10]=multiType.u32_;			//�������������
			
			
			j=4;
			memset(temp,0,strlen(temp));
			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
			sscanf(temp,"%d",&mapPot.numPot);						//��ȡ��λ�����͵���������		���ÿ��ֱ�߶��ϵ�����,
			multiType.u32_=mapPot.numPot;
			Paramt_BUFF[Pg_OutsideParmt][11]=multiType.u32_;			//�������������

			mapPot.mode=(u8)mode;				//����ģʽ
		}break;
		default:
		{
			mapPot.mode=254;
		}break;
	}
	return 	mapPot;
}



/*******************************��ͼ���س���********************************/
//������ʽ��void Map_Download(void)
//���������
//���������
//���ز�����
//����˵������ͼ������غ���
/*************************************************************************/
void Map_Download(void)
{
	switch(MapPot.mode)
	{
		case 0: break;					//
		case 1:									//���ӵ�ͼ����		
		{
					//Map_Save2FLASH(MapPot,FloatSize);		//��ͼ���ݴ洢	
			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����	
			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
		}break;
		case 2:									//���ӵ�ͼ�����洢����ʽ��
		{
			SetScreen(25);														//��ʼ����ɣ��л������ʼ��
			STMFLASH_Clear(MAPOA_Addr,1);								//OA�洢������
			STMFLASH_Clear(MAPOB_Addr,1);								//OB�洢������
			STMFLASH_Clear(MAPAngle_Addr,1);								//angle�洢������
			SetScreen(4);														//�˻������ʼ����ɣ��л��ز�������3
			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
		}break;
		case 3:									//���ӵ�ͼ��ʼ���ݴ洢
		{			
			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
		}break;
		default:break;
	}	
}


/*******************************��ͼ������***************************/
//������ʽ��u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
//���������u16 lastPotNum:���ص����һ����ֵ��OAOB����ֵ
//���ز��������ش���������
//����˵�����Ե�ͼ�洢���ĵ�ͼ���д�����
//
//
/*********************************************************************/
u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
{
	MapPotDef mapPot;
	u16 errNum,i;			//���������
	for(i=1;i<lastPotNum+1;i++)
	{	
		mapPot=Map_ReadFromFLASH(i,4);				//��ȡ���ӵ�ͼ�洢������
		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) errNum++;					//�������д����ͼ��Ϣ�������ֵ�ۼ�	
	}
	return errNum;
}



/***************************���ӵ�ͼ������޸�*************************/
//������ʽ��void MapTable_repair(u16 lastPotNum,float OAOBLimt)
//���������u16 lastPotNum:���ص����һ����ֵ��OAOB����ֵ
//���ز�����
//����˵�����Ե�ͼ�洢���Ĵ����ͼ�����޸�
//
//
/*********************************************************************/
void MapTable_repair(u16 lastPotNum,float OAOBLimt)
{
	MapPotDef mapPot;
	u16 i;			
	for(i=1;i<lastPotNum+1;i++)
	{	
		mapPot=Map_ReadFromFLASH(i,4);			//��ȡ�洢����ͼ����
		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) 	//�������д����ͼ��Ϣ����ʼ�쳣��ͼ��Ϣ�޸�
		{
			if(i==1)				//���쳣��λ��һ������
			{
				for(i=2;i<lastPotNum+1;i++)   //��һ�����������黹�д����
				{
					mapPot=Map_ReadFromFLASH(i,4);			//��ȡ�洢����ͼ����
					if(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt) 	//�����ִ����ͼ���쳣���򽫸õ�ͼ�ź��鸳ֵ����һ������
					{
						mapPot.Num=mapPot.Num-1;
						Map_Save2FLASH(mapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����
						break;			//����ѭ��
					}
				}		
			}
			else
			{
				mapPot=Map_ReadFromFLASH(i-1,4);			//��ȡ�洢����ͼ����
				mapPot.Num=mapPot.Num+1;
				Map_Save2FLASH(mapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����							
			}				
		}
	}	
	MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);				//��ͼ���������
}
















//#include "sys.h"
//#include "usart.h"	
//#include "HEAD.h"
//#include "Motor.h"
//#include "Vehicle.h"
//////////////////////////////////////////////////////////////////////////////////// 	 
////���ʹ��ucos,����������ͷ�ļ�����.
//#if SYSTEM_SUPPORT_OS
//#include "FreeRTOS.h"					//FreeRTOSʹ��	 
//#include "mpu6050.h"
//#endif
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK STM32F4̽���߿�����
////����1��ʼ��		   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2014/6/10
////�汾��V1.5
////��Ȩ���У�����ؾ���
////Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
////All rights reserved
////********************************************************************************
////V1.3�޸�˵�� 
////֧����Ӧ��ͬƵ���µĴ��ڲ���������.
////�����˶�printf��֧��
////�����˴��ڽ��������.
////������printf��һ���ַ���ʧ��bug
////V1.4�޸�˵��
////1,�޸Ĵ��ڳ�ʼ��IO��bug
////2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
////3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
////4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////V1.5�޸�˵��
////1,�����˶�UCOSII��֧��
//////////////////////////////////////////////////////////////////////////////////// 	  
// 

////////////////////////////////////////////////////////////////////
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  

//BluetoothDef Bluetooth;
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//void _sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{ 	
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}
//#endif
// 
//#if EN_USART1_RX   //���ʹ���˽���
////����1�жϷ������
////ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//u8 USART_SD_BUF[USART_REC_LEN];			//���ͻ����� �����USART_REC_LEN_S���ֽ�.
////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	

////��ʼ��IO ����1 
////bound:������
////void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:��ռ���ȼ���u8 SubPriority�����ȼ�
////{
////  //GPIO�˿�����
////  GPIO_InitTypeDef GPIO_InitStructure;
////	USART_InitTypeDef USART_InitStructure;
////	NVIC_InitTypeDef NVIC_InitStructure;
////	
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
//// 
////	//����1��Ӧ���Ÿ���ӳ��
////	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
////	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
////	
////	//USART1�˿�����
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
////	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

////   //USART1 ��ʼ������
////	USART_InitStructure.USART_BaudRate = bound;//����������
////	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
////	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
////	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
////	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
////	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
////  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
////	
////  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
////	
////#if EN_USART1_RX	
////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

////	//Usart1 NVIC ����
////  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�3
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//�����ȼ�3
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
////	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

////#endif

////}
//void uart_init(u32 bound,u8 PreemptionPriority,u8 SubPriority )						//u8 PreemptionPriority:��ռ���ȼ���u8 SubPriority�����ȼ�
//{
//  //GPIO�˿�����
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
//	
//	
// 
//	//����1��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2); //GPIOA9����ΪUSART1
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); //GPIOA10����ΪUSART1
//	
//	//USART1�˿�����
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //GPIOA9��GPIOA10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10

//   //USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
//	
//  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
//	
//#if EN_USART1_RX	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =SubPriority;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

//#endif

//}

////void USART1_IRQHandler(void)                	//����1�жϷ������
////{
////	u8 Res;
////	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
////	{
////		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
////		
////		if((USART_RX_STA&0x8000)==0)//��� �δ���
////		{
////			if(USART_RX_STA&0x4000)//���յ���0x0d
////			{
////				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
////				else USART_RX_STA|=0x8000;	//��������� 
////			}
////			else //��û�յ�0X0D
////			{	
////				if(Res==0x0d)USART_RX_STA|=0x4000;
////				else
////				{
////					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
////					USART_RX_STA++;
////					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
////				}		 
////			}
////		}   		 
////	} 
////} 



////void USART1_IRQHandler(void)                	//����1�жϷ������
////{
////	u8 Res;
////	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
////	{
////		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
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

//void USART2_IRQHandler(void)                	//����2�жϷ������
//{
//	u8 Res;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
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
///**************************����ң��������********************* 
////��������u8 HC_05_Trans(u8 res)
////���������u8 res����������
////����ֵ��0
//---------------------------------------------------
//0x00	 ��   ��ָ�ͣ��			
//---------------------------------------------------
//0x41	A��		ǰ��
//---------------------------------------------------
//0x45	E��		����
//---------------------------------------------------
//0x47	G��		��ת
//---------------------------------------------------
//0x43	C��		��ת
//---------------------------------------------------
//0x48	H��		��ǰ
//---------------------------------------------------
//0x42	B��		��ǰ
//---------------------------------------------------
//0x46	F��		���
//---------------------------------------------------
//0x44	D��		�Һ�
//---------------------------------------------------
//0x4A 	J��		ң��ģʽ
//---------------------------------------------------
//0x58	X��		�ٶ�+
//---------------------------------------------------
//0x59	Y��		�ٶ�-
//---------------------------------------------------
//---------------------------------------------------
//****************************************************************/ 

//u8 HC_05_Trans(u8 res)
//{
//	switch(res)
//	{		
//		case 0x41:	//ǰ��
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,90);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x41;
//		}break;			
//		case 0x45:	//����
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,90);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x45;
//		}break;			
//		case 0x47:	//��ת
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,97.63);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x47;
//		}break;			
//		case 0x43:	//��ת
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,82.37);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x43;
//		}break;			
//		case 0x48:	//��ǰ
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,95.04);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x48;
//		}break;			
//		case 0x42:	//��ǰ
//		{
//			VehiclePar=Vehicle_count(VehicleSpd,85);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x42;
//		}break;			
//		case 0x46:	//���
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,97.63);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x46;
//		}break;			
//		case 0x44:	//�Һ�
//		{
//			VehiclePar=Vehicle_count(-VehicleSpd,82.37);
//			TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);
//			return 0x44;
//		}break;			
//		case 0x4A:	//ң��ģʽ
//		{
//		
//			return 0x4A;
//		}break;			
//		case 0x58:	//�ٶȼ�
//		{
//			
//			return 0x58;
//		}break;			
//		case 0x59:	//�ٶȼ�
//		{
//			
//			return 0x59;
//		}break;	
//		case 0x5A:	//ֹͣ
//		{
//			//TIM_SetCompare1(TIM4,Median);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			Wheel_control(0,0,MotorMsg);
//			return 0x59;
//		}break;	
//		default:			//��ָ��
//		{
//			TIM_SetCompare1(TIM4,Median);	//�޸ıȽ�ֵ���޸�ռ�ձ�
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
//�������ܣ�����1��������
//��ڲ�����Ҫ���͵�����
//����  ֵ����
//**************************************************************************/
//void usart1_send(u8 data)
//{
//	USART1->DR = data;
//	while((USART1->SR&0x40)==0);	
//}





///************************���Ƶ���ѯ�ʷ��ͳ���*********************/
////void Battery_Send(u8 len,u8 *cmd)
////���������u8 len�����ݳ��ȣ�      u8 *cmd�����͵�����
////
///*****************************************************************/
//void Battery_Send(u8 len,u8 *cmd)
//{
//	u8 t;
//	for(t=0;t<len;t++)
//	{
//		USART_SendData(USART1, cmd[t]);         //�򴮿�1��������
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//	}
//}


///************************��ȡ���״̬��Ϣ����************************/
////������ʽ��u8 Battery_Capacity(u8 * cmd)
////�����������������
////����ֵ��������Ϣ����
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
//	if(cmd[0]==0xDD && cmd[33]==0x77)			//����֡β֡��ȷ
//	{
//		for(i=2;i<31;i++)
//		{
//			checkStor +=(u16)cmd[i];				//���������ۼ�
//		}
//		checkStor=(checkStor^0xFFFF)+1;		//��У��
//		checkTemp=((u16)cmd[31]<<8)+(u16)cmd[32];			//�������ݵ�У��ֵ
//		
//		if(checkStor==checkTemp)			//��У����ȷ�������ݿ��Է��͸������洢�ṹ��
//		{
//			
//			
//			multiType.float_=((u16)cmd[4]<<8) + (u16)cmd[5];			//�ܵ�ѹ
//			batteryMsg.total_Vot=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[6]<<8) + (u16)cmd[7];				//����
//			batteryMsg.current=multiType.float_/100;
//			
//			multiType.float_=((u16)cmd[8]<<8) + (u16)cmd[9];						//ʣ������
//			batteryMsg.SOC=multiType.float_/100;	

//			
//			batteryMsg.cycle=((u16)cmd[12]<<8) + (u16)cmd[13];						//���ѭ������
//			batteryMsg.protectStatus=((u16)cmd[20]<<8) + (u16)cmd[21];						//����״̬
//			batteryMsg.RSOC=(int)cmd[23];					//���ʣ�������ٷֱ�
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


///******************* ������λ�����ݣ���������λ�����ģ�������ز���*******************************/
////������ʽ��MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
////���������u8 * usartBuff������BUFF��  u16 len:���ݳ�����Ϣ 
////���ز��������ݱ��
////�������ܣ�������λ�����ݣ���������λ�����ģ�������ز���    =0���޲���   =1���洢��ͼ   =2����ͼ�洢����ʽ��  =3�����͵��ӵ�ͼ������Ϣ  =254:���κ�����
///*********************************************************************/
//MapPotDef MapMsg_Propress(u8 * usartBuff,u16 len)
//{

//	u16 i,k,j=0;
//	int mode;
//	u8 lenth,stor[8];//���ݳ���,����λ�ô洢
//	char static temp[20];
//	char buff;	
//	MapPotDef static mapPot,mapPotTemp;
//	union MultiType multiType;
//	
//	buff=usartBuff[3];
//	sscanf((char*)&buff,"%d",&mode);						//��ȡ��λ�����͵���������
//	j=0;	
//	
//	for(i=0;i<len;i++)				//��ȡ����λ��
//	{
//		buff=usartBuff[i];			//��ȡ�����ַ�����
//		if(buff==',')
//		{
//			stor[j]=i;
//			j++;
//		}		
//	}		
//	switch(mode)
//	{
//		case 0: break;
//		case 1:											//���ͻ�������OA OB angle����   //���ӣ�FR,1,-33.6193923403310,0149.835464517472,-0289.516314197572,1,OR   ��ʽ:FR,������,�㣨OAֵ��,�㣨OBֵ��,�㣨����ǣ�,����,OR
//		{
//			k=0;
//			j=1;
//			memset(temp,0,strlen(temp));				
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
//			sscanf(temp,"%f",&mapPot.OA);						//��ȡ��λ�����͵���������    ���OA
//			
//			j=2;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%f",&mapPot.OB);						//��ȡ��λ�����͵���������		���OB

//			j=3;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%f",&mapPot.angle);						//��ȡ��λ�����͵���������	���angle

//			j=4;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%d",&mapPot.Num);						//��ȡ��λ�����͵���������		��õ�ͼλ�õ���Ϣ		

//			mapPot.mode=(u8)mode;				//����ģʽ
//		}break;	
//		case 2:											//��ͼ�洢����ʽ��   //��ʽ  :FR,������,OR
//		{		
//			mapPot.mode=(u8)mode;				//����ģʽ
//		}break;
//		case 3:											//���͵��ӵ�ͼ������Ϣ //���ӣ�FR,3,118.4652105558884,31.376517521623,2600,20,OR   ��ʽ��FR,������,����ֵ(�ȸ�ʽ),ά��ֵ(�ȸ�ʽ),��ͼ���ܵĵ�����,ÿ��ֱ�߶��ϵ�����,OR
//		{
//			k=0;
//			j=1;
//			memset(temp,0,strlen(temp));				
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
//			sscanf(temp,"%lf",&mapPot.longti_du);						//��ȡ��λ�����͵���������    ��þ������ݣ��ȸ�ʽ��
//			multiType.double_=mapPot.longti_du;							
//			Paramt_BUFF[Pg_OutsideParmt][1]=multiType.u32Array_[0];				//double�������ݵ�32λ
//			Paramt_BUFF[Pg_OutsideParmt][2]=multiType.u32Array_[1];				//double�������ݸ�32λ
//			
//			
//			j=2;
//			memset(temp,0,strlen(temp));				
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];		
//			sscanf(temp,"%lf",&mapPot.lati_du);						//��ȡ��λ�����͵���������    ���ά�����ݣ��ȸ�ʽ��
//			multiType.double_=mapPot.lati_du;		
//			Paramt_BUFF[Pg_OutsideParmt][3]=multiType.u32Array_[0];				//double�������ݵ�32λ
//			Paramt_BUFF[Pg_OutsideParmt][4]=multiType.u32Array_[1];				//double�������ݸ�32λ
//			
//			j=3;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%d",&mapPot.grossNumPot);						//��ȡ��λ�����͵���������		��õ�ͼ���ܵĵ�����,
//			multiType.u32_=mapPot.grossNumPot;
//			Paramt_BUFF[Pg_OutsideParmt][10]=multiType.u32_;			//�������������
//			
//			
//			j=4;
//			memset(temp,0,strlen(temp));
//			for(i=stor[j]+1,k=0;i<stor[j+1];i++,k++)	temp[k]=usartBuff[i];
//			sscanf(temp,"%d",&mapPot.numPot);						//��ȡ��λ�����͵���������		���ÿ��ֱ�߶��ϵ�����,
//			multiType.u32_=mapPot.numPot;
//			Paramt_BUFF[Pg_OutsideParmt][11]=multiType.u32_;			//�������������

//			mapPot.mode=(u8)mode;				//����ģʽ
//		}break;
//		default:
//		{
//			mapPot.mode=254;
//		}break;
//	}
//	return 	mapPot;
//}



///*******************************��ͼ���س���********************************/
////������ʽ��void Map_Download(void)
////���������
////���������
////���ز�����
////����˵������ͼ������غ���
///*************************************************************************/
//void Map_Download(void)
//{
//	switch(MapPot.mode)
//	{
//		case 0: break;					//
//		case 1:									//���ӵ�ͼ����		
//		{
//					//Map_Save2FLASH(MapPot,FloatSize);		//��ͼ���ݴ洢	
//			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����	
//			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
//		}break;
//		case 2:									//���ӵ�ͼ�����洢����ʽ��
//		{
//			SetScreen(25);														//��ʼ����ɣ��л������ʼ��
//			STMFLASH_Clear(MAPOA_Addr,1);								//OA�洢������
//			STMFLASH_Clear(MAPOB_Addr,1);								//OB�洢������
//			STMFLASH_Clear(MAPAngle_Addr,1);								//angle�洢������
//			SetScreen(4);														//�˻������ʼ����ɣ��л��ز�������3
//			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
//		}break;
//		case 3:									//���ӵ�ͼ��ʼ���ݴ洢
//		{			
//			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
//		}break;
//		default:break;
//	}	
//}


///*******************************��ͼ������***************************/
////������ʽ��u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
////���������u16 lastPotNum:���ص����һ����ֵ��OAOB����ֵ
////���ز��������ش���������
////����˵�����Ե�ͼ�洢���ĵ�ͼ���д�����
////
////
///*********************************************************************/
//u16 MapTable_check(u16 lastPotNum,float OAOBLimt)
//{
//	MapPotDef mapPot;
//	u16 errNum,i;			//���������
//	for(i=1;i<lastPotNum+1;i++)
//	{	
//		mapPot=Map_ReadFromFLASH(i,4);				//��ȡ���ӵ�ͼ�洢������
//		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) errNum++;					//�������д����ͼ��Ϣ�������ֵ�ۼ�	
//	}
//	return errNum;
//}



///***************************���ӵ�ͼ������޸�*************************/
////������ʽ��void MapTable_repair(u16 lastPotNum,float OAOBLimt)
////���������u16 lastPotNum:���ص����һ����ֵ��OAOB����ֵ
////���ز�����
////����˵�����Ե�ͼ�洢���Ĵ����ͼ�����޸�
////
////
///*********************************************************************/
//void MapTable_repair(u16 lastPotNum,float OAOBLimt)
//{
//	MapPotDef mapPot;
//	u16 i;			
//	for(i=1;i<lastPotNum+1;i++)
//	{	
//		mapPot=Map_ReadFromFLASH(i,4);			//��ȡ�洢����ͼ����
//		if(!(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt)) 	//�������д����ͼ��Ϣ����ʼ�쳣��ͼ��Ϣ�޸�
//		{
//			if(i==1)				//���쳣��λ��һ������
//			{
//				for(i=2;i<lastPotNum+1;i++)   //��һ�����������黹�д����
//				{
//					mapPot=Map_ReadFromFLASH(i,4);			//��ȡ�洢����ͼ����
//					if(mapPot.angle<=360.f&&mapPot.angle>=0.f&&mapPot.OA<OAOBLimt&&mapPot.OA>-OAOBLimt&&mapPot.OB<OAOBLimt&&mapPot.OB>-OAOBLimt) 	//�����ִ����ͼ���쳣���򽫸õ�ͼ�ź��鸳ֵ����һ������
//					{
//						mapPot.Num=mapPot.Num-1;
//						Map_Save2FLASH(mapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����
//						break;			//����ѭ��
//					}
//				}		
//			}
//			else
//			{
//				mapPot=Map_ReadFromFLASH(i-1,4);			//��ȡ�洢����ͼ����
//				mapPot.Num=mapPot.Num+1;
//				Map_Save2FLASH(mapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����							
//			}				
//		}
//	}	
//	MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);				//��ͼ���������
//}

