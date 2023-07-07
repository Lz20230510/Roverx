#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h" 
#include "cmd_queue.h"
#include "cmd_process.h"
#include "HEAD.h"
#include "usart6.h" 
#include "usart3.h" 
#include "hmi_driver.h" 
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


//�����������Ͷ�Ӧֵ
#define _u8       0
#define _u16      1
#define _u32      2
#define _float    3
#define _string   4
#define _double   5

//�����˻������и������洢�����PageID��
#define Pg_Start 0x00				//������0
#define Pg_FuncMenu 0x01				//���ܲ˵�1
#define Pg_IO_Monitor 0x02			//IO���2
#define Pg_MapPreview 0x03			//�켣Ԥ��3
#define Pg_MapEdit	0x04				//�켣�༭4
#define Pg_NoParmt5	0x05			//�޲����洢����5
#define Pg_NoParmt6	0x06			//�޲����洢����6
#define Pg_NoParmt7	0x07			//�޲����洢����7
#define Pg_NoParmt8	0x08			//�޲����洢����8
#define Pg_NoParmt9	0x09			//�޲����洢����9
#define Pg_OutsideParmt	0x0A			//����ģʽ��������10    �в����洢
#define Pg_InsideMap11	0x0B			//����·������11
#define Pg_InsideMap12	0x0C			//����·������12
#define Pg_InsideMap13	0x0D			//����·������13
#define Pg_InsideMap14	0x0E			//����·������14
#define Pg_InsideMap15	0x0F			//����·������15
#define Pg_InsideParmt	0x10			//����ģʽ��������16
#define Pg_CommonParmt	0x11			//�����������17
#define Pg_StaCardNum18	0x12			//վ�㿨��18
#define Pg_StaCardNum19	0x13			//վ�㿨��19
#define Pg_Parmt20	0x14			//�в����洢����20
#define Pg_Parmt21	0x15			//�в����洢����21
#define Pg_Parmt22	0x16			//�в����洢����22
#define Pg_XX23		0x17				//�в����洢����23
#define Pg_XX24		0x18				//�в����洢����24
#define Pg_Init		0x19				//�в����洢����25


//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
 

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes 		//�����Ӵ˴���ʼ�洢 
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  


//˫��AGV�Ĳ����洢��ַ
#define AGVPa_Addr ((u32)0x080E0000)				//AGV�����洢�׵�ַ
#define MAPOA_Addr ((u32)0x08080000)					//��ͼ���ݴ洢OA�׵�ַ
#define MAPOB_Addr ((u32)0x080A0000)					//��ͼ���ݴ洢OA�׵�ַ
#define MAPAngle_Addr ((u32)0x080C0000)					//��ͼ���ݴ洢angle�׵�ַ
#define Page_Capa    0x200									//ÿһҳ������Ϊ0x200�ֽ�
#define AGVString ((u32)0x080F0000)					//AGV�ַ��������洢�׵�ַ
#define FloatSize 4											//�����ȸ�����ռ�ռ�
#define DoubleSize 8											//˫���ȸ�����ռ�ռ�
///////////////////////////////////////////////////////////////////
extern union MultiType								//���������ʹ洢
{
	u8 betys4_[4];
	u8 betys8_[8];
	u32 u32Array_[2];
	u8 u8_;
	u16 u16_;
	u32 u32_;
	float float_;	
	double double_;
	char *string_;	
}MultiType;
extern union MultiType multitype;  


void Pg_ParameterInput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type);			//ҳ�����д��
union MultiType Pg_ParameterOutput(u16 screenId,u16 controlID,u8 type);											//ҳ�������ȡ

u32 STMFLASH_ReadWord(u32 faddr);		  	//������  
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void BuffParamt_Write(u32 WriteAddr,union MultiType multiType1/*,u32 NumToWrite*/,u8 type);    //�������洢��BUFF��
union MultiType BuffParamt_Read(u32 ReadAddr,u8 type);							//��Buff�ж�ȡ����������ȡ
void AGV_ParameterWrite(u16 pageID,u16 controlID,union MultiType multiType1,u8 type);					//AGV����д��
union MultiType AGV_ParameterRead(u16 pageID,u16 controlID,u8 type);													//AGV������ȡ

u16 Dacai_Display(PCTRL_MSG msg);																														//																														//
void AllPg_Refresh(TextFormat *textFormat,TextFormat *textFormat1,TextFormat *textFormat2);																									//
void Dacai_InOut(u16 screenID,u16 controlID,u8* str,union MultiType multiType1);											//��ʵ�����Ļ�����Ĳ�����д����
u16 ScreenID_Judge(u16 screenID,u16 controlID);															//��ǰ����ҳ��ż���
void STMFLASH_Clear(u32 FlashSectorNum,u8 mode);                                     //����ҳ�����в�������

void Parameter_2Buff(u32 WriteAddr,u32* pBuffer);													//������ֵ�洢�������洢BUFF			
void Parameter_FromBuff(u32 WriteAddr,u32* pBuffer);													//��������BUFF�ж�ȡ
void Pg_ParameteRefresh(u16 screenId);   																 //����ҳ��ˢ��
union MultiType FlashParamt_Read(u32 ReadAddr,u8 type);										//��FLASH�ж�ȡ����
void FlashParamt_Write(u32 WriteAddr,union MultiType multiType1,u8 type);		//�������FLASH��
void Buff2_Flash(u16 screenID,u16 controlID);																//��ĳһҳ�Ĳ������棬����ҳ�Ĳ�������FLASH�еĲ�

void Map_Save2FLASH(MapPotDef mapPot,u8 size);										//��FLASH�б����ͼ����
MapPotDef Map_ReadFromFLASH(u16 potNum,u8 size);									//��FLASH�ж�ȡ��ͼ����

PIDtypedef AGVPID_Change_FLASH(u8 PIDGroup);    //AGV��FLASH���߻�ȡPIDֵ
u16 AGVSPD_Change_FLASH(u8 spdGroup);							//AGV��FLASH���߻�ȡ�ٶ�ֵ
u8 Map_StorageCheck(u16 potNum,u8 size);				//��ͼ�洢��ĳһ���洢��Ԫ�����Ƿ�Ϊ0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////////
//FLASH�д洢��ÿ��������Ӧ���������� 
//�����б�ʾ���ݶ�Ӧ��ҳ��ţ������ж�Ӧ�ؼ�
extern u8 ParamtType[20][121];																														

//�������ݻ�����
extern u32 Paramt_BUFF[20][121];
//�ı���ʽ�洢��
extern TextFormat textFormat[100];
//extern TextFormat textFormat1[100];
//extern TextFormat textFormat2[100];




#endif














