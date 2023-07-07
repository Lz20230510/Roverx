#include "stmflash.h"
#include "delay.h"
#include "usart3.h" 
#include "string.h" 
#include "cmd_process.h" 
#include "HEAD.h"
#include "pwm.h" 
#include "led.h" 
#include "RAD.h" 
#include "usart6.h" 
#include "mpu9250.h"



TextFormat textFormat[100]={0};
//TextFormat textFormat1[100]={0};
//TextFormat textFormat2[5]={0};


u32 temp_mark;
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//STM32�ڲ�FLASH��д ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
u8 ParamtType[20][121];
u32 Paramt_BUFF[20][121];




//��ȡָ����ַ�İ���(16λ����) 
//faddr:����ַ 
//����ֵ:��Ӧ����.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}  
//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ر�ע��:��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
//         д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
//         д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
//         û����Ҫ����,��������������Ȳ�����,Ȼ����������д. 
//�ú�����OTP����Ҳ��Ч!��������дOTP��!
//OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
//pBuffer:����ָ��
//NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//�Ƿ���ַ
	FLASH_Unlock();									//���� 
  FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
 		
	addrx=WriteAddr;				//д�����ʼ��ַ
	endaddr=WriteAddr+NumToWrite*4;	//д��Ľ�����ַ

	if(addrx<0X1FFF0000)			//ֻ�����洢��,����Ҫִ�в�������!!
	{	
		while(addrx<endaddr)		//ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
			}else addrx+=4;
		}			
	}
	
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//д����
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//д������
			{ 
				break;	//д���쳣
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}  

/*************************�������洢������������******************/
//void Parameter_2Buff(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
//���������u32 FlashSectorNum�������洢�������׵�ַ
//
//
/****************************************************************************/
void Parameter_2Buff(u32 WriteAddr,u32* pBuffer)
{
	u16 scrnContID[2];
	
	scrnContID[0]=(WriteAddr-AGVPa_Addr)/Page_Capa;						//ҳ���
	scrnContID[1]=((WriteAddr-AGVPa_Addr)%Page_Capa)>>2;				//�ؼ���
	Paramt_BUFF[scrnContID[0]][scrnContID[1]]=pBuffer[0];	//�洢������������Ӧ��ַ	
}


/*************************�Ӳ�����������ȡ����******************/
//void STMFLASH_Buff2(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
//���������u32 FlashSectorNum�������洢�������׵�ַ
//
//
/****************************************************************************/
void Parameter_FromBuff(u32 WriteAddr,u32* pBuffer)
{
	u16 scrnContID[2];
	scrnContID[0]=(WriteAddr-AGVPa_Addr)/Page_Capa;							//ҳ���
	scrnContID[1]=((WriteAddr-AGVPa_Addr)%Page_Capa)>>2;				//�ؼ���
	pBuffer[0]=Paramt_BUFF[scrnContID[0]][scrnContID[1]];				//�����ݻ�������������
}



/*************************�����������洢�����в�������******************/
//void STMFLASH_Clear(u32 FlashSectorNum,u8 mode)
//���������u32 FlashSectorNum�������洢�������׵�ַ    u8 mode 0:����д0����1��������д0����
//void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
//
//
/**********************************************************************/
void STMFLASH_Clear(u32 FlashSectorNum,u8 mode)
{
	u32 addrx=FlashSectorNum;				//д�����ʼ��ַ
	u32 endaddr;
	FLASH_Status status = FLASH_COMPLETE;
	
	
	if(FlashSectorNum<STM32_FLASH_BASE||FlashSectorNum%4)return;	//�Ƿ���ַ
	FLASH_Unlock();									//���� 
  FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
	
	switch(FlashSectorNum)
	{
		case ADDR_FLASH_SECTOR_0:
		case ADDR_FLASH_SECTOR_1:
		case ADDR_FLASH_SECTOR_2:
		case ADDR_FLASH_SECTOR_3:
		{
			endaddr=FlashSectorNum+0x4000;	//д��Ľ�����ַ   16k�洢�ռ�
			while(addrx<endaddr)		//��16k�洢�ռ��ڲ����洢����
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
				addrx+=4;
			}
			addrx=FlashSectorNum;				//��ʼֵ��ʼ��
			if(status==FLASH_COMPLETE&&mode==0)			//д0����
			{
				while(addrx<endaddr)//д����
				{
					if(FLASH_ProgramWord(addrx,0x00000000)!=FLASH_COMPLETE)//д������0
					{ break;	//д���쳣	
					}
					addrx+=4;
				} 
			}
		}break;
		case ADDR_FLASH_SECTOR_4:
		{
			endaddr=FlashSectorNum+0x10000;	//д��Ľ�����ַ   64k�洢�ռ�
			while(addrx<endaddr)		//��64k�洢�ռ��ڲ����洢����
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)break;	//����������
				addrx+=4;
			}
			addrx=FlashSectorNum;				//��ʼֵ��ʼ��
			if(status==FLASH_COMPLETE&&mode==0)			//д0����
			{
				while(addrx<endaddr)//д����
				{
					if(FLASH_ProgramWord(addrx,0x00000000)!=FLASH_COMPLETE)//д������0
					{ break;	//д���쳣				
					}
					addrx+=4;
				} 
			}		
		}break;
		case ADDR_FLASH_SECTOR_5:
		case ADDR_FLASH_SECTOR_6:
		case ADDR_FLASH_SECTOR_7:
		case ADDR_FLASH_SECTOR_8:
		case ADDR_FLASH_SECTOR_9:
		case ADDR_FLASH_SECTOR_10:
		case ADDR_FLASH_SECTOR_11:
		{
			endaddr=FlashSectorNum+0x20000;	//д��Ľ�����ַ   128k�洢�ռ�
			while(addrx<endaddr)		//��128k�洢�ռ��ڲ����洢����
			{
				if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
				{   
					status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
					if(status!=FLASH_COMPLETE)break;	//����������
				}else addrx+=4;
			}
			
			addrx=FlashSectorNum;				//��ʼֵ��ʼ��
			if(status==FLASH_COMPLETE&&mode==0)			//д0����
			{
				while(addrx<endaddr)//д����
				{
					if(FLASH_ProgramWord(addrx,0x00000000)!=FLASH_COMPLETE)//д������0
					{ break;	//д���쳣		
					}
					addrx+=4;
				} 
			}					
		}break;	
		default:break;
	}
	
	
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();//����
}



////////////////////////////////////////////////////////////
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToRead:��(4λ)��
//
///////////////////////////////////////////////////////////
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//��ȡ4���ֽ�.
		ReadAddr+=4;//ƫ��4���ֽ�.	
	}
}



/****************************�����洢��BUFF��*********************/
//void BuffParamt_Write(u32 WriteAddr,union Multi_type multiType,u32 *pBuffer,u32 NumToWrite,u8 type)					
//���������
//u32 WriteAddr���洢��ַ 
//union MultiType multiType1���洢����
//u8 type:��������   0:u8   1:u32     2����������float��   3:�ַ���			  u8 u32 float ����ֻ�ܴ洢һ��
//u8 type:��������   0:u8   1:u16  2:u32   3����������float��  4:�ַ���			 5:˫���ȸ�����     u8 u16 u32 float ����ֻ�ܴ洢һ��    
//
//���������
//����ֵ����
/******************************************************************/
void BuffParamt_Write(u32 WriteAddr,union MultiType multiType1,u8 type)
{	
	u8 i;
	u8 temp[8];
	u32 lenth_temp;		
	memset(temp,0,sizeof(temp));
	
	switch(type)																								//
	{
		case 0:																										//u8
		{
			temp[0]=multiType1.u8_;
			Parameter_2Buff(WriteAddr,(u32*)temp);							//���ݴ洢������������		
		}break;			
		case 1:																										//u16
		{
			for(i=0;i<2;i++)temp[i]=multiType1.betys4_[i];					//16λ���ݲ��
			Parameter_2Buff(WriteAddr,(u32*)temp);							//���ݴ洢������������			
		}break;
		case 2:																										//u32
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//32λ���ݲ��	
			Parameter_2Buff(WriteAddr,(u32*)temp);							//���ݴ洢������������	
		}break;
		case 3:																										//������
		{	
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//���������	
			Parameter_2Buff(WriteAddr,(u32*)temp);							//���ݴ洢������������			
		}break;
		case 4:																										//�ַ���
		{																		
			lenth_temp=strlen(multiType1.string_);									//�������ݳ���		
			for(i=0;i<lenth_temp;i++)temp[i]=multiType1.string_[i];	//��ȡ�ַ���
			Parameter_2Buff(WriteAddr,(u32*)temp);							//���ݴ洢������������							(��ʱ����ʹ��)
		}break;	
		case 5:																									//˫���ȸ�����
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys8_[i];					//���������	
			Parameter_2Buff(WriteAddr,(u32*)temp);							//���ݴ洢������������
			for(i=4;i<8;i++)temp[i-4]=multiType1.betys8_[i];					//���������	
			Parameter_2Buff(WriteAddr+4,(u32*)temp);							//���ݴ洢������������				
			
		}break;		
		default: break;
	}
}



/****************************��BUFF�ж�ȡ����********************/
//void BuffParamt_Read(u32 ReadAddr,union MultiType multiType1,u8 type)				
//���������
//u32 WriteAddr���洢��ַ 
//union MultiType multiType1���洢����														//u8 ��  float ����ֻ�ܴ洢һ��
//u8 type:��������   0:u8   1:u32     2����������float��   3:�ַ���							//u8 u32 float ����ֻ�ܴ洢һ��
//u8 type:��������   0:u8   1:u16  2:u32   3����������float��  4:�ַ���			  u8 u16 u32 float ����ֻ�ܴ洢һ��
//���������
//
//���� 
/******************************************************************/
union MultiType BuffParamt_Read(u32 ReadAddr,u8 type)
{
	union MultiType multiType2;
	u8 i;
	static char char_;
	static u8 temp[8];
	u32 lenth_temp;	
	multiType2.string_=&char_;
	
	switch(type)
	{
		case 0:																							//u8
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//�Ӳ�����BUFF��HMI
			multiType2.u8_=temp[0];	
		}break;
		case 1:																							//u16
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//�Ӳ�����BUFF��HMI
			for(i=0;i<2;i++) multiType2.betys4_[i]=temp[i];	
		}break;
		case 2:																							//u32
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//�Ӳ�����BUFF��HMI
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];	
		}break;
			case 3:																						//������
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//�Ӳ�����BUFF��HMI
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];			
		}break;
		case 4:																							//�ַ���
		{	
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//�Ӳ�����BUFF��HMI			
			//STMFLASH_Read(ReadAddr,(u32*)temp,20);			//���ݶ�ȡ,ǿ�ƶ�ȡ20���ֽڵ��ַ���
			lenth_temp=strlen((char*)temp);							//�������ݳ���			
			for(i=0;i<lenth_temp;i++) multiType2.string_[i]=temp[i];							//��ȡ�ַ���			
		}break;
		case 5:																						//˫���ȸ�����
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//�Ӳ�����BUFF��HMI
			for(i=0;i<4;i++) multiType2.betys8_[i]=temp[i];
			Parameter_FromBuff(ReadAddr+4,(u32*)temp);							//�Ӳ�����BUFF��HMI
			for(i=4;i<8;i++) multiType2.betys8_[i]=temp[i-4];
		}break;
		default: break;
	}
	return multiType2;
}



/****************************��FLASH�ж�ȡ����*********************/
//union MultiType FlashParamt_Read(u32 ReadAddr,u8 type)				
//���������
//u32 WriteAddr���洢��ַ 
//union MultiType multiType1���洢����														//u8 ��  float ����ֻ�ܴ洢һ��
//u8 type:��������   0:u8   1:u32     2����������float��   3:�ַ���							//u8 u32 float ����ֻ�ܴ洢һ��
//u8 type:��������   0:u8   1:u16  2:u32   3����������float��  4:�ַ���			  u8 u16 u32 float ����ֻ�ܴ洢һ��   5��˫���ȸ�����,double
//���������
//���� 
/******************************************************************/
union MultiType FlashParamt_Read(u32 ReadAddr,u8 type)
{

	union MultiType multiType2;
	u8 i;
	static char char_;
	static u8 temp[8];
	u32 lenth_temp;	
	multiType2.string_=&char_;
	
	switch(type)
	{
		case 0:																							//u8
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//���ݶ�ȡ
			multiType2.u8_=temp[0];	
		}break;
		case 1:																							//u16
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//���ݶ�ȡ
			for(i=0;i<2;i++) multiType2.betys4_[i]=temp[i];	
		}break;
		case 2:																							//u32
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//���ݶ�ȡ
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];	
		}break;
			case 3:																						//������
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//���ݶ�ȡ
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];
				
		}break;
		case 4:																							//�ַ���
		{	
			STMFLASH_Read(ReadAddr,(u32*)temp,20);			//���ݶ�ȡ,ǿ�ƶ�ȡ20���ֽڵ��ַ���
			lenth_temp=strlen((char*)temp);							//�������ݳ���			
			for(i=0;i<lenth_temp;i++) multiType2.string_[i]=temp[i];							//��ȡ�ַ���			
		}break;
		case 5:
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//���ݶ�ȡ
			for(i=0;i<4;i++) multiType2.betys8_[i]=temp[i];
			STMFLASH_Read(ReadAddr+4,(u32*)temp,1);  						//���ݶ�ȡ
			for(i=0;i<4;i++) multiType2.betys8_[i+4]=temp[i];
		
		}break;
		default: break;
	}
	return multiType2;
}



/***************************�����洢��FLASH��*********************/
//void FlashParamt_Write(u32 WriteAddr,union MultiType multiType1,u8 type)				
//���������
//u32 WriteAddr���洢��ַ 
//union MultiType multiType1���洢����
//u8 type:��������   0:u8   1:u32     2����������float��   3:�ַ���			  u8 u32 float ����ֻ�ܴ洢һ��
//u8 type:��������   0:u8   1:u16  2:u32   3����������float��  4:�ַ���			  u8 u16 u32 float ����ֻ�ܴ洢һ�� 5:double ˫���ȸ�����
//
//���������
//����ֵ����
/******************************************************************/
void FlashParamt_Write(u32 WriteAddr,union MultiType multiType1,u8 type)
{	
	u8 i;
	u8 temp[8];
	u32 lenth_temp;		
	memset(temp,0,sizeof(temp));
	
	switch(type)																								//
	{
		case 0:																										//u8
		{
			temp[0]=multiType1.u8_;
			STMFLASH_Write(WriteAddr,(u32*)temp,1);								//���ݴ洢			
		}break;			
		case 1:																										//u16
		{
			for(i=0;i<2;i++)temp[i]=multiType1.betys4_[i];					//16λ���ݲ��
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//���ݴ洢
		}break;
		case 2:																										//u32
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//32λ���ݲ��	
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//���ݴ洢
		}break;
		case 3:																										//������
		{	
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//���������	
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//���ݴ洢				
		}break;
		case 4:																										//�ַ���
		{																		
			lenth_temp=strlen(multiType1.string_);									//�������ݳ���		
			for(i=0;i<lenth_temp;i++)temp[i]=multiType1.string_[i];	//��ȡ�ַ���
			STMFLASH_Write(WriteAddr,(u32*)temp,lenth_temp);				//���ݴ洢
		}break;	
		case 5:				//˫���ȸ�����
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys8_[i];					//64λ���ݲ��	
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//���ݴ洢
			for(i=0;i<4;i++)temp[i]=multiType1.betys8_[i+4];					//64λ���ݲ��	
			STMFLASH_Write(WriteAddr+4,(u32*)temp,1);									//���ݴ洢
		}break;
		default: break;
	}
}




/***************************��BUFF�е������������浽FLASH��*********************/
//void Buff2_Flash(u16 screenID,u16 controlID)
//���������   u16 controlID���ؼ���
//���������
//
//����ֵ
//˵��������ҳ�����ˢ�µĿؼ��Ŷ���120,�ҵ���ȡ��ĳһҳ������ı�ʱ��������ҳ��������治�䣬ֻ�޸ĵ�ǰҳ��
//�ҵ�ǰ�����洢ҳ��ֻ�� screenID��6-39�Ž���
/**************************************************************************/
void Buff2_Flash(u16 screenID,u16 controlID)
{
	u8 i,j;
	if(controlID==120)												//���ؼ���Ϊ120����ִ�в������������浽FLASH���� 
	{			
//		for(j=Pg_OutsideParmt;j<=Pg_Parmt22;j++)
//		{
//			if(j!=screenID) for(i=0;i<121;i++) STMFLASH_Read(AGVPa_Addr+j*Page_Capa+i*0x04,&ParamtTemp[j][i],1);	//��Flash���Ĳ����洢��temp
//			
//			else for(i=0;i<121;i++) ParamtTemp[j][i]=Paramt_BUFF[j][i];	//������Buff���Ĳ����洢��temp	
//			
//		}		
		
		for(j=Pg_OutsideParmt;j<=Pg_StaCardNum19;j++)																													
		{			
			for(i=0;i<121;i++)
			{	
				STMFLASH_Write(AGVPa_Addr+j*Page_Capa+i*0x04,&Paramt_BUFF[j][i],1);								//��temp�еĲ����ٴ洢��FLASH		
			}
	
		}			
	}	
}



/*********************�����в��ֲ�����ѯ*********************/
//union MultiType AGV_ParameterSeek(u16 pageID,u8 row,u8 column,u8 type)
//���������u8 pageID���˻������pageId ��  u8 row����������ҳ����   u8 column����������ҳ����   u8 type: ��������    
//���������
//����ֵ������������
//�����ַu8���͸��洢�ռ�ռ�Ϊ0x200��С   ���Դ洢512/4=128������
//ÿһ�������ڲ������ݰ��տؼ��ŵ�˳�������ۼ�
//��Ҫ�������¼������棺
//����ģʽ��������
//����·�����ã�11-15��
//����ģʽ��������
//�����������17
//վ�㿨��18-19
//�����������20-25
/**********************************************************************************/
union MultiType AGV_ParameterRead(u16 pageID,u16 controlID,u8 type)
{
	union MultiType multiType2;
	u32 addr=0; 
	switch(pageID)
	{
		case 0x0A:				//����ģʽ��������
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			multiType2=BuffParamt_Read(addr,type);			//��������	
		}break;
			
		case 0x0B:					//����·������11
		case 0x0C:					//����·������12	
		case 0x0D:					//����·������13
		case 0x0E:					//����·������14
		case 0x0F:					//����·������15
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			multiType2=BuffParamt_Read(addr,type);			//��������	
		}break;
		case 0x10:					//����ģʽ��������	
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			multiType2=BuffParamt_Read(addr,type);			//��������			
		}break;	
		case 0x11:					//�����������17
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;													//���ĳ���ݵĵ�ַƫ����
			multiType2=BuffParamt_Read(addr,type);					//��������			
		}break;
		case 0x12:					//վ�㿨��18
		case 0x13:					//վ�㿨��19
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			multiType2=BuffParamt_Read(addr,type);			//��������			
		}break;
//		case 0x14:					//�����������20
//		case 0x15:					//�����������21
//		case 0x16:					//�����������22	
//		case 0x17:					//�����������23
//		case 0x18:					//�����������24	
//		case 0x19:					//�����������25	
//		{
//			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
//			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
//			multiType2=BuffParamt_Read(addr,type);			//��������			
//		}break;
		default:break;
	}
	return multiType2;
}



/*********************�����в��ֲ����洢*********************/
//void AGV_ParameterWrite(u16 pageID,u16 controlID,union MultiType multiType1,u8 type)
//���������u8 pageID���˻������pageId ��  u8 row����������ҳ����   u8 column����������ҳ����    u8 type:��������
//union MultiType multiType1	�洢�Ĳ���
//���������
//����ֵ��
//AGV���ݴ洢��ʽ��
//1.���ַ������������ݶ�ռ���ĸ��ֽڣ�
//2.��ǰ���Դ洢���������Ͱ�����u8��u16��u32,float  ,�ַ�����ַ�Ѿ����䣬������س���ûд
//3.���ݰ����˻�����ҳ��˳����з��䣬�����ͷ�ļ���
//4.ÿ��ҳ���ṩ0x200�ֽڽ������ݴ洢�����Դ洢128�����ݣ�
//5.�����ַ����⣬������������ʲô���Ͷ��洢��ͬһ��ҳ��ռ��ڣ�
//6.���ݸ�������ҳ��Ŀؼ��ž���ֵ���б���
/**********************************************************************************/
void AGV_ParameterWrite(u16 pageID,u16 controlID,union MultiType multiType1,u8 type)
{
	u32 addr=0;

	switch(pageID)
	{	
		case 0x00:
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			BuffParamt_Write(addr,multiType1,type);			//����д��					
		}
		case 0x0A:															//����ģʽ��������
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			BuffParamt_Write(addr,multiType1,type);			//����д��		
		}break;
			
		case 0x0B:					//����·������11
		case 0x0C:					//����·������12	
		case 0x0D:					//����·������13
		case 0x0E:					//����·������14
		case 0x0F:					//����·������15
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			BuffParamt_Write(addr,multiType1,type);			//����д��		
		}break;
		case 0x10:					//����ģʽ��������	
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			BuffParamt_Write(addr,multiType1,type);			//����д��			
		}break;	
		case 0x11:					//�����������17
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			BuffParamt_Write(addr,multiType1,type);			//����д��				
		}break;
		case 0x12:					//վ�㿨��18
		case 0x13:					//վ�㿨��19
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
			BuffParamt_Write(addr,multiType1,type);			//����д��				
		}break;
//		case 0x14:					//�����������20
//		case 0x15:					//�����������21
//		case 0x16:					//�����������22	
//		case 0x17:					//�����������23
//		case 0x18:					//�����������24	
//		case 0x19:					//�����������25	
//		{
//			addr=AGVPa_Addr+pageID*Page_Capa;								//�����ǰҳ���׵�ַ		
//			addr=addr+controlID*4;								//���ĳ���ݵĵ�ַƫ����
//			BuffParamt_Write(addr,multiType1,type);			//����д��			
//		}break;
		default:break;
	}
}


/*********************************ҳ�����д��**************************/
//void Pg_ParameterInput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type)
//���������u16 screenId:ҳ��ID��   u16 controlID���ؼ�ID��    union MultiType multiType1���洢�Ĳ���   u8 type����������     
//���������
//���أ�
//ע��:screenID����pageID
/**********************************************************************/
void Pg_ParameterInput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type)
{
	AGV_ParameterWrite(screenId,controlID,multiType1,type);				//д�����
}


/*********************************ҳ��������**************************/
//void Pg_ParameterOutput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type)
//���������u16 screenId:ҳ��ID��   u16 controlID���ؼ�ID��    union MultiType multiType1���洢�Ĳ���   u8 type����������     
//���������
//���أ�
//ע��:screenID����pageID
/***********************************************************************/
union MultiType Pg_ParameterOutput(u16 screenId,u16 controlID,u8 type)
{
	union MultiType multiType2;
	multiType2=AGV_ParameterRead(screenId,controlID,type);			//��ȡ����		
	return multiType2;
}




/***************************ĳһҳ���ض�������ˢ��*********************/
//void Pg_ParameteRefresh(u16 screenId)
//���������u16 screenId:ҳ��ID��       
//���������
//���أ�
//ע��:screenID����pageID
/***********************************************************************/
void Pg_ParameteRefresh(u16 screenId)
{
	u8 iconDate[16],i;
	double temp[2],temp1[2];
	static u8 cardNumStor=0,JogModeStor=0,AutoModeStor=0;
	switch(screenId)
	{
		case 0x00:										//������0
		{	
			iconDate[5-5]=!AGVStatus.AJMode;
			if(AGVStatus.AUTOmode==1) iconDate[6-5]=1;
			else iconDate[6-5]=0; 
			if(AGVStatus.AUTOmode==2) iconDate[7-5]=1;
			else iconDate[7-5]=0; 
			iconDate[8-5]=!AGVStatus.AJMode;
			iconDate[9-5]=AGVStatus.AUTOmode;
			iconDate[10-5]=AGVStatus.SONSetup;
			//iconDate[11-5]=BatteryMsg.RSOC*43/100;	
			iconDate[12-5]=AGVStatus.SONSetup;
			iconDate[13-5]=!AGVStatus.EmerStop;	
			iconDate[14-5]=AGVStatus.SONSetup&&((!IN_SEV1)&&(!IN_SEV2)&&(!IN_SEV3)&&(!IN_SEV4));
			iconDate[15-5]=!SymaStatus.linkStatus;
			iconDate[16-5]=SymaStatus.RunMode;
							
			MultiIcon_Refresh(screenId,5,16,iconDate,0);						//����ͼ����ʾ
			
			//ң�����ݿ�
			if(AGVStatus.AJMode==0)				//�ֶ�ģʽ����
			{
				SetTextValueFloat(screenId,56,SymaStatus.Spd_Jog,_float);		//�ٶ�ֵ
				SetTextValueFloat(screenId,57,SymaStatus.Spd_cle[1],_float);		//ת��뾶
			}	
			//�����Զ�����		
			if(AGVStatus.AUTOmode==1)
			{
				SetTextValueFloat(screenId,58,(float)(AGVSpd_IN*AGVStatus.Wheel_R*AGVStatus.pi1/60/32),_float);		//�ٶ�ֵ
				SetTextValueFloat(screenId,59,Mag_date[2],_float);		//�ŵ���ֵ
				SetTextValueu16(screenId,60,BranchPath.cardNum);							//��ǰ����
				SetTextValueu16(screenId,61,BranchPath.selectPathNum);			//��ǰ·����
			}					
			//�����Զ�����
			if(AGVStatus.AUTOmode==2)
			{
				SetTextValueFloat(screenId,62,MapPot.AutoSpd,_float);		//�ٶ�ֵ
				SetTextValueFloat(screenId,63,Smc345.scm345Angle,_float);		//�����
				SetTextValueFloat(screenId,64,lonti_lati[0],_float);		//����ֵ
				SetTextValueFloat(screenId,65,lonti_lati[1],_float);		//ά��ֵ
				SetTextValueFloat(screenId,66,OAOB[0],_float);					//OAֵ
				SetTextValueFloat(screenId,67,OAOB[1],_float);					//OBֵ		
			}
			
			if(AutoModeStor!=AGVStatus.AUTOmode|| JogModeStor!=AGVStatus.AJMode)			//��ģʽ״̬�����仯ʱ������״̬������
			{
				SetTextValueFloat(screenId,56,0,_float);		//�ٶ�ֵ
				SetTextValueFloat(screenId,57,0,_float);		//ת��뾶
			
				SetTextValueFloat(screenId,58,0,_float);		//�ٶ�ֵ
				SetTextValueFloat(screenId,59,0,_float);		//�ŵ���ֵ
				SetTextValueu16(screenId,60,0);							//��ǰ����
				SetTextValueu16(screenId,61,0);							//��ǰ·����
				
				SetTextValueFloat(screenId,62,0,_float);		//�ٶ�ֵ
				SetTextValueFloat(screenId,63,0,_float);		//�����
				SetTextValueFloat(screenId,64,0,_float);		//����ֵ
				SetTextValueFloat(screenId,65,0,_float);		//ά��ֵ
				SetTextValueFloat(screenId,66,0,_float);		//OAֵ
				SetTextValueFloat(screenId,67,0,_float);		//OBֵ		
			}
						
			//��ص���
	//		SetTextValueu16(screenId,55,BatteryMsg.RSOC);						//��ص����ٷֱ�					
			JogModeStor=AGVStatus.AJMode;
			AutoModeStor=AGVStatus.AUTOmode;

		}break;
		
		case 0x02:											//IO��ؽ���
		{		
			iconDate[0]=IN_JOGSetup;								//ң������
			iconDate[1]=IN_AUTOSetup;								//�豸����
			iconDate[2]=IN_EmerStop;								//��ͣ
			iconDate[3]=!IN_SEV1;										//�ŷ�1�����ź�
			iconDate[4]=!IN_SEV2;										//�ŷ�2�����ź�
			iconDate[5]=!IN_SEV3;										//�ŷ�3�����ź�
			iconDate[6]=!IN_SEV4;										//�ŷ�4�����ź�
			iconDate[7]=OT_SON;											//ʹ�����
			iconDate[8]=OT_JD1;											//�̵���1
			iconDate[9]=OT_JD2;											//�̵���2
			iconDate[10]=OT_DIR1;										//�ŷ�1�����ź�
			iconDate[11]=OT_DIR2;										//�ŷ�2�����ź�
			iconDate[12]=OT_DIR3;										//�ŷ�3�����ź�
			iconDate[13]=OT_DIR4;										//�ŷ�4�����ź�	
			MultiIcon_Refresh(screenId,1,14,iconDate,0);						//��ť״̬��ʾ			
		}break;
		case 0x03:							//�켣��ʾ����3
		{
			MapPotDsiplay= AGV_OutSideValue(gpsx,Smc345.scm345Angle,LontLati);
			if(AGVStatus.AUTOmode==2||AGVStatus.TeachMode==1)											//�����������Զ�ģʽ����ʾ��ģʽʱ
			{
				SetTextValueFloat(screenId,3,MapPotDsiplay.longti_du,_float);				//���ȶȸ�ʽ
				SetTextValueFloat(screenId,4,MapPotDsiplay.lati_du,_float);					//γ�ȶȸ�ʽ
				SetTextValueFloat(screenId,5,MapPotDsiplay.longti_dufen,_float);		//���ȶȷָ�ʽ
				SetTextValueFloat(screenId,6,MapPotDsiplay.lati_dufen,_float);			//γ�ȶȷָ�ʽ
				SetTextValueFloat(screenId,7,MapPotDsiplay.starNum,_float);					//��ǰ����
				SetTextValueFloat(screenId,8,MapPotDsiplay.OA,_float);							//OAֵ
				SetTextValueFloat(screenId,9,MapPotDsiplay.OB,_float);							//OBֵ
				SetTextValueFloat(screenId,10,MapPotDsiplay.trueAngle,_float);			//�����
			}
			else
			{
				SetTextValueFloat(screenId,3,0,_float);		//���ȶȸ�ʽ
				SetTextValueFloat(screenId,4,0,_float);		//γ�ȶȸ�ʽ
				SetTextValueFloat(screenId,5,0,_float);		//���ȶȷָ�ʽ
				SetTextValueFloat(screenId,6,0,_float);		//γ�ȶȷָ�ʽ
				SetTextValueFloat(screenId,7,0,_float);		//��ǰ����
				SetTextValueFloat(screenId,8,0,_float);		//OAֵ
				SetTextValueFloat(screenId,9,0,_float);		//OBֵ
				SetTextValueFloat(screenId,10,0,_float);	//�����
			}			
		}break;
		case 0x04:										//�켣�༭ҳ��4
		{	
			if(AGVStatus.MapDownMode==1)
			{
				SetTextValueu16(screenId,6,MapPot.Num);			//���ؽ�������,�����ͼ���ش�
				SetTextValueu16(screenId,21,MapPot.errNum);			//���ؽ�������,�����ͼ���ش�
			}
			else
			{
				MapPot.Num=0;
				SetTextValueu16(screenId,6,0);			//���ؽ������ݣ��������ص�ͼ�ر�
				SetTextValueu16(screenId,21,0);			//���ؽ�������,�����ͼ���ش�
				MapPot.errNum=0;
			}				
			AnimationPlayFrame(screenId,10,AGVStatus.MapDownMode);		//���ؽ���ͼ��
				
			AnimationPlayFrame(screenId,11,AGVStatus.TeachMode);		//ʾ��״̬
			if(AGVStatus.TeachMode)					//��ʾ��ģʽ����ʱ
			{	
				SetTextValueFloat(screenId,7,lonti_lati[0],_float);		//����ֵ
				SetTextValueFloat(screenId,8,lonti_lati[1],_float);		//ά��ֵ
				SetTextValueFloat(screenId,9,Smc345.scm345Angle,_float);		//�����
//				SetTextValueFloat(screenId,20,XX);		//����		
			}
			else					//��ʾ��ģʽδ����ʱ
			{
				SetTextValueFloat(screenId,7,0,_float);		//�����
				SetTextValueFloat(screenId,8,0,_float);		//����ֵ
				SetTextValueFloat(screenId,9,0,_float);		//ά��ֵ		
			}	
			
		}break;	
	
		case 0x0A:					//����·����������
		{
//			if((u8)Paramt_BUFF[screenId][13]==0xFF||(u8)Paramt_BUFF[screenId][13]==0)AnimationPlayFrame(screenId,13,0);		//����ѡ��״̬��
//			else AnimationPlayFrame(screenId,13,1);		//����ѡ��״̬��
			longti_lati_trans(0,longti_O,lati_O,temp1,temp);	
			LontLati= Origin_O_parmt_Init();				//GPS ԭ����Ϣ��ȡ
			MapPot.AutoSpd=LontLati.spdTemp;				//�Զ������ٶ�
			LontLati= Origin_O_parmt_Init();				//GPS ԭ����Ϣ��ȡ
			
		}break;
		case 0x10:					//����ģʽ�������ٶ�����
		{
			front_pid=AGVPID_Change_FLASH(0);		//PID��1  ǰ����
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 0x11:										//�����������17
		{			
			AnimationPlayFrame(screenId,5,(u8)Paramt_BUFF[17][5]);														//����ѡ��״̬��				
			//for(i=0;i<8;i++) iconDate[i]=(unsigned int)Ultrosonic.channel[i];			//��ȡ������ʵʱ����
			//MultiIcon_Refresh(screenId,30,37,iconDate,1);						//��ť״̬��ʾ	
			for(i=0;i<8;i++) SetTextValueFloat(screenId,30+i,(float)Ultrosonic.channel[i],_float);		//�����
			Ultrosonic.deviceEn=(u8)Paramt_BUFF[Pg_CommonParmt][5];						//���ϴ������Ƿ��
			
		}break;
		default:break;
	}
	cardNumStor=RFID_value;	
}


/*********************************�Ե�ǰ��ʾ����Ļ����ʵʱˢ��************/
//u16 Dacai_Display( PCTRL_MSG msg,u8 mode)
//�������:�PCTRL_MSG msg:���������ͨ�Ż���ȡ   
//���������
//���أ�
//ע��:screenID����pageID
/***********************************************************************/
u16 Dacai_Display( PCTRL_MSG msg)
{
	u16 size;
	u16 screenID; 
	temp_mark=1;
	size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);	//�ӻ������л�ȡһ��ָ��    
	if(size>0)
	{
		screenID=PTR2U16(&msg->screen_id);						//������յ�����������ǰ��ҳ��IDֵ
		temp_mark=2;
		ProcessMessage((PCTRL_MSG)cmd_buffer, size);	//ָ���
	}
	temp_mark=3;
	Pg_ParameteRefresh(CurrtScreenID);							//ʵʱˢ��ָ������Ĳ���
	temp_mark=4;
	return screenID;
}

/***************************�����в����������ˢ��************************/
//void AllPg_Refresh(TextFormat *textFormat,TextFormat *textFormat1,TextFormat *textFormat2)
//�������:PTextFormat *textFormat:�������ݴ洢1   PTextFormat *textFormat2:�������ݴ洢2   PTextFormat *textFormat3:�������ݴ洢3
//���������
//���أ�
//ע��:screenID����pageID
/*****************************************************************************/
void AllPg_Refresh(TextFormat *textFormat,TextFormat *textFormat1,TextFormat *textFormat2)
{
	u16 i,j;
	union MultiType multiType2;
	u16 firstID,lastID;

	//����ģʽ��������

	j=Pg_OutsideParmt;	
	i=1;							//����ֵ�洢		
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_double);
	ParamtType[j][i]=_double;											//�������ʹ洢
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_double);//��FLASH�еĲ����洢���Ӳ���BUFF				
	//OnePg_Refresh(j,textFormat1,1,_double,0);
	SetTextValueFloat(j,i,multiType2.double_,_double);
	
	i=3;							//ά��ֵ�洢		
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_double);
	ParamtType[j][i]=_double;											//�������ʹ洢
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_double);//��FLASH�еĲ����洢���Ӳ���BUFF				
	//OnePg_Refresh(j,textFormat,1,_double,0);
	SetTextValueFloat(j,i,multiType2.double_,_double);
	
	firstID=10;				//��ͼ������
	lastID=11;				//�ֶε�����
	for(i=firstID;i<=lastID;i++)
	{
		multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u16);
		textFormat[i-firstID].u16Text=multiType2.u16_;
		textFormat[i-firstID].controlID=i;	
		ParamtType[j][i]=_u16;											//�������ʹ洢
		BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u16);//��FLASH�еĲ����洢���Ӳ���BUFF				
	}OnePg_Refresh(j,textFormat,2,_u16,0);
	
	i=12;							//�Զ�Ѱ���ٶ�			
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_float);
	textFormat1[0].floatText=multiType2.float_;
	textFormat1[0].controlID=i;	
	ParamtType[j][i]=_float;											//�������ʹ洢
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_float);//��FLASH�еĲ����洢���Ӳ���BUFF				
	OnePg_Refresh(j,textFormat1,1,_float,0);
	
	i=13;							//����Ѱ�����			
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
	textFormat[0].u8Text=multiType2.u8_;
	textFormat[0].controlID=i;	
	ParamtType[j][i]=_u8;											//�������ʹ洢
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//��FLASH�еĲ����洢���Ӳ���BUFF					
	OnePg_Refresh(j,textFormat,1,_u8,1);

	//����·������11-15
	firstID=1;
	lastID=90;
	for(j=Pg_InsideMap11;j<=Pg_InsideMap15;j++)
	{	
		for(i=firstID;i<=lastID;i++)
		{
			multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
			textFormat[i-firstID].u8Text=multiType2.u8_;
			textFormat[i-firstID].controlID=i;	
			ParamtType[j][i]=_u8;											//�������ʹ洢
			BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//��FLASH�еĲ����洢���Ӳ���BUFF				
		}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u8,0);
	}
	
	//����ģʽ��������  
	firstID=1;
	lastID=15;
	j=Pg_InsideParmt;
	for(i=firstID;i<=lastID;i++)
	{
		multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_float);
		textFormat[i-firstID].floatText=multiType2.float_;
		textFormat[i-firstID].controlID=i;	
		ParamtType[j][i]=_float;											//�������ʹ洢
		BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_float);//��FLASH�еĲ����洢���Ӳ���BUFF			
	}OnePg_Refresh(j,textFormat,lastID-firstID+1,_float,0);
	
	
	//����������ý���
	firstID=20;
	lastID=27;
	j=Pg_CommonParmt;
	for(i=firstID;i<=lastID;i++)
	{
		multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
		textFormat[i-firstID].u8Text=multiType2.u8_;
		textFormat[i-firstID].controlID=i;	
		ParamtType[j][i]=_u8;											//�������ʹ洢
		BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//��FLASH�еĲ����洢���Ӳ���BUFF		
	}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u8,0);	
	
	i=5;							//���ϴ���������		
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
	textFormat[0].u8Text=multiType2.u8_;
	textFormat[0].controlID=i;	
	ParamtType[j][i]=_u8;											//�������ʹ洢
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//��FLASH�еĲ����洢���Ӳ���BUFF					
	OnePg_Refresh(j,textFormat,1,_u8,1);
	
	//վ�㿨��18-19
	firstID=1;
	lastID=60;
	for(j=Pg_StaCardNum18;j<=Pg_StaCardNum19;j++)
	{
		for(i=firstID;i<=lastID;i++)
		{
			multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u16);
			textFormat[i-firstID].u16Text=multiType2.u16_;
			textFormat[i-firstID].controlID=i;	
			ParamtType[j][i]=_u16;																	//�������ʹ洢	
			BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u16);//��FLASH�еĲ����洢���Ӳ���BUFF				
		}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u16,0);
	}

//	//�в����洢����20-22
//	firstID=1;
//	lastID=90;
//	for(j=Pg_Parmt20;j<=Pg_Parmt22;j++)
//	{
//		for(i=firstID;i<=lastID;i++)
//		{
//			multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
//			textFormat[i-firstID].u8Text=multiType2.u8_;
//			textFormat[i-firstID].controlID=i;	
//			ParamtType[j][i]=_u8;											//�������ʹ洢
//			BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//��FLASH�еĲ����洢���Ӳ���BUFF				
//		}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u8,0);
//	}
}

/**********��ȡ�˻������������ݣ��洢��Flash������ˢ���Ǹ����������*************/
//void Dacai_InOut(u16 screenID,u16 controlID,u8* str,union MultiType multiType1)
//�������:PTextFormat *textFormat:�������ݴ洢1   PTextFormat *textFormat2:�������ݴ洢2   PTextFormat *textFormat3:�������ݴ洢3
//���������
//���أ�
//ע��:screenID����pageID
/*****************************************************************************/
void Dacai_InOut(u16 screenID,u16 controlID,u8* str,union MultiType multiType1)
{
	u8 type;
	union MultiType multiType2;
	type=ParamtType[screenID][controlID];
	switch(type)
	{
		case _u8:
		{
			sscanf(str,"%u",&multiType1.u8_);//���ַ���ת��Ϊ����
			AGV_ParameterWrite(screenID,controlID,multiType1,type);						//�洢HMI����Ĳ���
			multiType2=AGV_ParameterRead(screenID,controlID,_u8);					//��ȡ��������Ĳ���
			SetTextValueu16(screenID,controlID,(u16)multiType2.u8_);		//���ش洢���ڵĲ������˻�����	
		}break;
		case _u16:
		{
			sscanf(str,"%u",&multiType1.u16_);//���ַ���ת��Ϊ����
			AGV_ParameterWrite(screenID,controlID,multiType1,_u16);						//�洢HMI����Ĳ���
			multiType2=AGV_ParameterRead(screenID,controlID,_u16);						//���ش洢���ڵĲ������˻�����			
			SetTextValueu16(screenID,controlID,(u16)multiType2.u16_);		    //���ش洢���ڵĲ������˻�����		
		
		}break;
		case _u32:
		{
	/*		sscanf(str,"%ld",&multiType1.u32_);//���ַ���ת��Ϊ����
			AGV_ParameterWrite(screenID,controlID,multiType1,_u32);						//�洢HMI����Ĳ���
			multiType2=AGV_ParameterRead(screenID,controlID,_u32);						//���ش洢���ڵĲ������˻�����	
			SetTextValueu32(screenID,controlID,(u16)multiType1.u32_);		//���ش洢���ڵĲ������˻�����	*/		
		}break;
		case _float:
		{
			sscanf(str,"%f",&multiType1.float_);//���ַ���ת��Ϊ����
			AGV_ParameterWrite(screenID,controlID,multiType1,_float);						//�洢HMI����Ĳ���
			multiType2=AGV_ParameterRead(screenID,controlID,_float);						//���ش洢���ڵĲ������˻�����
			SetTextValueFloat(screenID,controlID,multiType2.float_,_float);		//���ش洢���ڵĲ������˻�����		
		}break;
		case _double:
		{
			sscanf(str,"%lf",&multiType1.double_);//���ַ���ת��Ϊ����
			AGV_ParameterWrite(screenID,controlID,multiType1,_double);						//�洢HMI����Ĳ���
			multiType2=AGV_ParameterRead(screenID,controlID,_double);						//���ش洢���ڵĲ������˻�����
			SetTextValueFloat(screenID,controlID,multiType2.double_,_double);		//���ش洢���ڵĲ������˻�����		
		}break;
		default:break;
	}
}


/************************AGV��ǰ�����ĸ������ж�********************************/
//u16 ScreenID_Judge(u16 screenId��u16 controlID)
//�������:u16 screenId: ҳ��ID��                   u16 controlID:�ؼ�ID��
//���������
//���أ����ص�ǰ������ҳ��ID��
//ע��:screenID����pageID
/*****************************************************************************/
u16 ScreenID_Judge(u16 screenID,u16 controlID)
{
	u16 currtScreenID;
	
	if(controlID==0x7D) currtScreenID=0x00;		//���з��������水ť���£���ǰ����ID�� =0x00
	switch(screenID)
	{	
			case 0x00:				//������
			{
				switch(controlID)
				{
					case 0x01:currtScreenID=0x01;break;			//���ܲ�������		
					default:break;
				}		
			}break;
		
			case 0x01:				//���ܲ�������
			{
				switch(controlID)
				{
					case 0x01:currtScreenID=0x03;break;			//����켣�������	
					case 0x02:currtScreenID=0x0A;break;			//����ģʽ��������	
//					case 0x03:currtScreenID=0x03;break;			//����1����	
//					case 0x04:currtScreenID=0x03;break;			//����2����	
					case 0x05:currtScreenID=0x0B;break;			//����·����������	
					case 0x06:currtScreenID=0x10;break;			//����ģʽ��������	
					case 0x07:currtScreenID=0x12;break;			//վ�㿨�Ž���	
//					case 0x08:currtScreenID=0x03;break;			//����4����	
					case 0x09:currtScreenID=0x02;break;			//IO��ؽ���	
					case 0x0A:currtScreenID=0x11;break;			//�������ý���	
//					case 0x0B:currtScreenID=0x03;break;			//����5����
//					case 0x0C:currtScreenID=0x03;break;			//����6����						
					default:break;
				}		
			}break;
			
			case 0x03:				//����켣Ԥ��
			{
				if(controlID==0x01) currtScreenID=0x04;		//����켣�༭����	
			}break;
			
			case 0x04:				//����켣�༭
			{
				if(controlID==0x01) currtScreenID=0x03;		//����켣Ԥ������	
			}break;
			
			case 0x0B:				//����·����������11
			case 0x0C:				//����·����������12
			case 0x0D:				//����·����������13
			case 0x0E:				//����·����������14
			case 0x0F:				//����·����������15
			case 0x12:				//վ�㿨�Ž���18
			case 0x13:				//վ�㿨�Ž���19		
			{
				switch(controlID)
				{
					case 0x79:						
					{
						currtScreenID=screenID-1;				//�Ϸ�һҳ
					}break;
					case 0x7A:
					{
						currtScreenID=screenID+1;				//�·�һҳ
					}break;		
					default:break;
				}		
			}break;		
	}
	return currtScreenID;
}


/******************************AGV��FLASH�����޸�PIDֵ*****************************/
//void AGVPID_Change_FLASH(u8 PIDGroup)
//���������u8 PIDGroup:PID���
//PIDGroup=0-8��Ӧ���в���1������ϵ���8��PID���������б������ڶ�ӦK�����ֵ��ڶ�ӦI��
//
//���������frontPID��ǰ����PID������rearPID������������
//����ֵ��
/***********************************************************************************/
PIDtypedef AGVPID_Change_FLASH(u8 PIDGroup)
{
	union MultiType multiType;
	PIDtypedef static pid;
	switch(PIDGroup)
	{
		case 0:				//��һ��PID��������
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];//1���ٶ�
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][2];
			pid.Proportion=multiType.float_;				//����PID K����		
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][3];
			pid.Integral=multiType.float_;					//����PID I����
		}break;
		case 1:		  	//��һ��PID��������
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];//1���ٶ�
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][2];
			pid.Proportion=multiType.float_;				//����PID K����
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][3];
			pid.Integral=multiType.float_;					//����PID I����
		}break;
		case 2:				//�ڶ���PID��������
		{	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][4];//2���ٶ�
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][5];
			pid.Proportion=multiType.float_;				//����PID K����				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][6];
			pid.Integral=multiType.float_;					//����PID I����
		}break;
		case 3:				//������PID��������
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][7];//3���ٶ�
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][8];
			pid.Proportion=multiType.float_;				//����PID K����				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][9];
			pid.Integral=multiType.float_;					//����PID I����
		}break;
		case 4:				//������PID��������
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][10];//ת���ٶ�
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][11];
			pid.Proportion=multiType.float_;				//����PID K����				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][12];
			pid.Integral=multiType.float_;					//����PID I����
		}break;
		case 5:				//������PID��������
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][10];//ͣ���ٶ�
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][14];
			pid.Proportion=multiType.float_;				//����PID K����				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][15];
			pid.Integral=multiType.float_;					//����PID I����
		}break;
		default:break;
	}
	pid.Derivative=0;   //΢��Ϊ0
	pid.SetPoint=0;     //�趨Ŀ��Ϊ0
	return pid;
}


/******************************AGV��FLASH�����޸��ٶ�ֵ*****************************/
//void AGVSPD_Change_FLASH(u8 spdGroup)
//���������u8 spdGroup:�ٶ���ŷֱ���HMI�����8���ٶ�
//spdGroup=0����=1��1���ٶ�   spdGroup=2��2���ٶ�       spdGroup=3��3���ٶ� 
//spdGroup=4��4���ٶ�   spdGroup=5��5���ٶ�       spdGroup=6��6���ٶ�
//spdGroup=7������ٶ�  spdGroup=8��Ѱ��ͣ���ٶ�
//
//���������
//���أ�u16:��õ��ٶ�ֵ
/********************************************************************************/
u16 AGVSPD_Change_FLASH(u8 spdGroup)
{
//	u8 radi;
//	u16 lenth;
  float spd;
	union MultiType multiType;
	switch(spdGroup)
	{
		case 0:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];break;				//1���ٶ�									
		case 1:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];break;				//1���ٶ�
		case 2:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][4];break;					//2���ٶ�	
		case 3:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][7];break;					//3���ٶ�	
		case 4:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][10];break;				//ת���ٶ�	
		case 5:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][13];break;				//Ѱ��ͣ���ٶ�	
		default:break;
	}

	spd=multiType.float_;							  //�ٶ�ֵ����
//	radi=(u8)Paramt_BUFF[39][5];		//���ٱ�
//	lenth=(u8)Paramt_BUFF[39][8];	//�����ܳ�
//	if(lenth==0) return 0;							//��ֹlength�ܳ�ֵû�����룬��ĸΪ0�����ֵ����
	 return (u16)spd*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;       //��ǰ���ٶ�ǿ��ת��		��λ��m/s-->r/mim				
}


/*****************************����ͼ��Ϣ�洢��FLASH��***************************/
//������ʽ��void Map_Save2FLASH(u32 mapAddr ,MapPotDef mapPot,u16 potNum,u8 size)
//���������u32 mapAddr�������洢�׵�ַ     mapPot����ͼ�ϵĵ㣨һ���㼯������OA OB angle��,u16 potNum:��ͼ�ϵ�N���� ,u8 size�����ݳߴ�(ռ�ֽ���)
//���������
//���ز�����
/*****************************************************************************/
void Map_Save2FLASH(MapPotDef mapPot,u8 size)
	
{
	union MultiType multiType;
	multiType.float_=mapPot.OA;
	STMFLASH_Write(MAPOA_Addr+size*mapPot.Num,&multiType.u32_,size/4);							//�洢OAֵ
	
	multiType.float_=mapPot.OB;	
	STMFLASH_Write(MAPOB_Addr+size*mapPot.Num,&multiType.u32_,size/4);							//�洢OBֵ
	
	multiType.float_=mapPot.angle;
	STMFLASH_Write(MAPAngle_Addr+size*mapPot.Num,&multiType.u32_,size/4);							//�洢angleֵ
}


/*****************************��FLASH�ж�ȡ��ͼ����************************************/
//������ʽ��MapPotDef Map_ReadFromFLASH(u32 mapAddr,u16 potNum,u8 size)
//���������u16 potNum:��ͼ�ϵ�N���㣬  ,u8 size�����ݳߴ�(ռ�ֽ���)
//���������
//���ز�����MapPotDef   .OA;OAֵ    .OB;OBֵ   .angle;angleֵ   
//
/*****************************************************************************/
MapPotDef Map_ReadFromFLASH(u16 potNum,u8 size)
{
	MapPotDef mapPot;
	union MultiType multiType;
	STMFLASH_Read(MAPOA_Addr+size*potNum,&multiType.u32_,size/4);					//����OAֵ
	mapPot.OA=multiType.float_;
	
	
	STMFLASH_Read(MAPOB_Addr+size*potNum,&multiType.u32_,size/4);					//����OBֵ
	mapPot.OB=multiType.float_;
	
	STMFLASH_Read(MAPAngle_Addr+size*potNum,&multiType.u32_,size/4);					//����angleֵ
	mapPot.angle=multiType.float_;	
	
	mapPot.Num=potNum;
	return mapPot;
}


/*************����ͼ�洢����ĳ���洢��Ԫ�Ƿ��쳣Ϊ0xfffffff****************************/
//������ʽ��u8 Map_StorageCheck(u16 potNum��u8 size)
//���������u16 potNum:�洢�ĵ�ͼ���
//���������
//���ز�����error : 0�Ѿ��洢������  1���洢��ַΪ0x000000000,˵������û�д�����ߵ�ͼ�洢����û��ˢ�� 
//
/*****************************************************************************/
u8 Map_StorageCheck(u16 potNum,u8 size)
{
	u8 error;
	u32 static temp1=0,temp2=0,temp3=0;
	union MultiType multiType;
	STMFLASH_Read(MAPOA_Addr+size*potNum,&multiType.u32_,size/4);					//����OAֵ
	temp1=multiType.u32_;
		
	STMFLASH_Read(MAPOB_Addr+size*potNum,&multiType.u32_,size/4);					//����OBֵ
	temp2=multiType.u32_;
	
	STMFLASH_Read(MAPAngle_Addr+size*potNum,&multiType.u32_,size/4);					//����angleֵ
	temp3=multiType.u32_;	
	
	if(temp1&temp2&temp3==0xFFFFFFFF) error=1;
	else error=0;
	return error;
}


