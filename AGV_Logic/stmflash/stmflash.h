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
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//STM32内部FLASH读写 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//定义数据类型对应值
#define _u8       0
#define _u16      1
#define _u32      2
#define _float    3
#define _string   4
#define _double   5

//定义人机界面中各参数存储界面的PageID号
#define Pg_Start 0x00				//主界面0
#define Pg_FuncMenu 0x01				//功能菜单1
#define Pg_IO_Monitor 0x02			//IO监控2
#define Pg_MapPreview 0x03			//轨迹预览3
#define Pg_MapEdit	0x04				//轨迹编辑4
#define Pg_NoParmt5	0x05			//无参数存储界面5
#define Pg_NoParmt6	0x06			//无参数存储界面6
#define Pg_NoParmt7	0x07			//无参数存储界面7
#define Pg_NoParmt8	0x08			//无参数存储界面8
#define Pg_NoParmt9	0x09			//无参数存储界面9
#define Pg_OutsideParmt	0x0A			//室外模式参数界面10    有参数存储
#define Pg_InsideMap11	0x0B			//室内路径设置11
#define Pg_InsideMap12	0x0C			//室内路径设置12
#define Pg_InsideMap13	0x0D			//室内路径设置13
#define Pg_InsideMap14	0x0E			//室内路径设置14
#define Pg_InsideMap15	0x0F			//室内路径设置15
#define Pg_InsideParmt	0x10			//室内模式参数界面16
#define Pg_CommonParmt	0x11			//常规参数设置17
#define Pg_StaCardNum18	0x12			//站点卡号18
#define Pg_StaCardNum19	0x13			//站点卡号19
#define Pg_Parmt20	0x14			//有参数存储界面20
#define Pg_Parmt21	0x15			//有参数存储界面21
#define Pg_Parmt22	0x16			//有参数存储界面22
#define Pg_XX23		0x17				//有参数存储界面23
#define Pg_XX24		0x18				//有参数存储界面24
#define Pg_Init		0x19				//有参数存储界面25


//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes 		//参数从此处开始存储 
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  


//双向AGV的参数存储地址
#define AGVPa_Addr ((u32)0x080E0000)				//AGV参数存储首地址
#define MAPOA_Addr ((u32)0x08080000)					//地图数据存储OA首地址
#define MAPOB_Addr ((u32)0x080A0000)					//地图数据存储OA首地址
#define MAPAngle_Addr ((u32)0x080C0000)					//地图数据存储angle首地址
#define Page_Capa    0x200									//每一页的容量为0x200字节
#define AGVString ((u32)0x080F0000)					//AGV字符串参数存储首地址
#define FloatSize 4											//单精度浮点数占空间
#define DoubleSize 8											//双精度浮点数占空间
///////////////////////////////////////////////////////////////////
extern union MultiType								//多数据类型存储
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


void Pg_ParameterInput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type);			//页面参数写入
union MultiType Pg_ParameterOutput(u16 screenId,u16 controlID,u8 type);											//页面参数读取

u32 STMFLASH_ReadWord(u32 faddr);		  	//读出字  
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
void BuffParamt_Write(u32 WriteAddr,union MultiType multiType1/*,u32 NumToWrite*/,u8 type);    //将参数存储到BUFF中
union MultiType BuffParamt_Read(u32 ReadAddr,u8 type);							//从Buff中读取参数参数读取
void AGV_ParameterWrite(u16 pageID,u16 controlID,union MultiType multiType1,u8 type);					//AGV参数写入
union MultiType AGV_ParameterRead(u16 pageID,u16 controlID,u8 type);													//AGV参数读取

u16 Dacai_Display(PCTRL_MSG msg);																														//																														//
void AllPg_Refresh(TextFormat *textFormat,TextFormat *textFormat1,TextFormat *textFormat2);																									//
void Dacai_InOut(u16 screenID,u16 controlID,u8* str,union MultiType multiType1);											//大彩单独屏幕单独的参数读写操作
u16 ScreenID_Judge(u16 screenID,u16 controlID);															//当前所处页面号检索
void STMFLASH_Clear(u32 FlashSectorNum,u8 mode);                                     //参数页面所有参数清零

void Parameter_2Buff(u32 WriteAddr,u32* pBuffer);													//将参数值存储到参数存储BUFF			
void Parameter_FromBuff(u32 WriteAddr,u32* pBuffer);													//将参数从BUFF中读取
void Pg_ParameteRefresh(u16 screenId);   																 //单独页面刷新
union MultiType FlashParamt_Read(u32 ReadAddr,u8 type);										//从FLASH中读取参数
void FlashParamt_Write(u32 WriteAddr,union MultiType multiType1,u8 type);		//存参数到FLASH中
void Buff2_Flash(u16 screenID,u16 controlID);																//将某一页的参数保存，其它页的参数还是FLASH中的参

void Map_Save2FLASH(MapPotDef mapPot,u8 size);										//向FLASH中保存地图数据
MapPotDef Map_ReadFromFLASH(u16 potNum,u8 size);									//从FLASH中读取地图数据

PIDtypedef AGVPID_Change_FLASH(u8 PIDGroup);    //AGV从FLASH在线获取PID值
u16 AGVSPD_Change_FLASH(u8 spdGroup);							//AGV从FLASH在线获取速度值
u8 Map_StorageCheck(u16 potNum,u8 size);				//地图存储器某一个存储单元数据是否为0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////////
//FLASH中存储的每个参数对应的数据类型 
//数组行表示数据对应的页面号，数组列对应控件
extern u8 ParamtType[20][121];																														

//参数数据缓冲区
extern u32 Paramt_BUFF[20][121];
//文本格式存储器
extern TextFormat textFormat[100];
//extern TextFormat textFormat1[100];
//extern TextFormat textFormat2[100];




#endif














