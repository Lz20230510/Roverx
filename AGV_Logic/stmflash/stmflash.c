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
u8 ParamtType[20][121];
u32 Paramt_BUFF[20][121];




//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}  
//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
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
//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
	FLASH_Unlock();									//解锁 
  FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
 		
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址

	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
	{	
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)break;	//发生错误了
			}else addrx+=4;
		}			
	}
	
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
}  

/*************************将参数存储到参数缓存区******************/
//void Parameter_2Buff(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
//输入参数：u32 FlashSectorNum：参数存储扇区的首地址
//
//
/****************************************************************************/
void Parameter_2Buff(u32 WriteAddr,u32* pBuffer)
{
	u16 scrnContID[2];
	
	scrnContID[0]=(WriteAddr-AGVPa_Addr)/Page_Capa;						//页面号
	scrnContID[1]=((WriteAddr-AGVPa_Addr)%Page_Capa)>>2;				//控件号
	Paramt_BUFF[scrnContID[0]][scrnContID[1]]=pBuffer[0];	//存储参数到数组相应地址	
}


/*************************从参数缓存区读取参数******************/
//void STMFLASH_Buff2(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
//输入参数：u32 FlashSectorNum：参数存储扇区的首地址
//
//
/****************************************************************************/
void Parameter_FromBuff(u32 WriteAddr,u32* pBuffer)
{
	u16 scrnContID[2];
	scrnContID[0]=(WriteAddr-AGVPa_Addr)/Page_Capa;							//页面号
	scrnContID[1]=((WriteAddr-AGVPa_Addr)%Page_Capa)>>2;				//控件号
	pBuffer[0]=Paramt_BUFF[scrnContID[0]][scrnContID[1]];				//从数据缓冲区读出数据
}



/*************************对整个参数存储区进行擦除操作******************/
//void STMFLASH_Clear(u32 FlashSectorNum,u8 mode)
//输入参数：u32 FlashSectorNum：参数存储扇区的首地址    u8 mode 0:带有写0处理，1：不带有写0处理
//void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
//
//
/**********************************************************************/
void STMFLASH_Clear(u32 FlashSectorNum,u8 mode)
{
	u32 addrx=FlashSectorNum;				//写入的起始地址
	u32 endaddr;
	FLASH_Status status = FLASH_COMPLETE;
	
	
	if(FlashSectorNum<STM32_FLASH_BASE||FlashSectorNum%4)return;	//非法地址
	FLASH_Unlock();									//解锁 
  FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
	
	switch(FlashSectorNum)
	{
		case ADDR_FLASH_SECTOR_0:
		case ADDR_FLASH_SECTOR_1:
		case ADDR_FLASH_SECTOR_2:
		case ADDR_FLASH_SECTOR_3:
		{
			endaddr=FlashSectorNum+0x4000;	//写入的结束地址   16k存储空间
			while(addrx<endaddr)		//在16k存储空间内擦除存储数据
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)break;	//发生错误了
				addrx+=4;
			}
			addrx=FlashSectorNum;				//起始值初始化
			if(status==FLASH_COMPLETE&&mode==0)			//写0处理
			{
				while(addrx<endaddr)//写数据
				{
					if(FLASH_ProgramWord(addrx,0x00000000)!=FLASH_COMPLETE)//写入数据0
					{ break;	//写入异常	
					}
					addrx+=4;
				} 
			}
		}break;
		case ADDR_FLASH_SECTOR_4:
		{
			endaddr=FlashSectorNum+0x10000;	//写入的结束地址   64k存储空间
			while(addrx<endaddr)		//在64k存储空间内擦除存储数据
			{
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)break;	//发生错误了
				addrx+=4;
			}
			addrx=FlashSectorNum;				//起始值初始化
			if(status==FLASH_COMPLETE&&mode==0)			//写0处理
			{
				while(addrx<endaddr)//写数据
				{
					if(FLASH_ProgramWord(addrx,0x00000000)!=FLASH_COMPLETE)//写入数据0
					{ break;	//写入异常				
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
			endaddr=FlashSectorNum+0x20000;	//写入的结束地址   128k存储空间
			while(addrx<endaddr)		//在128k存储空间内擦除存储数据
			{
				if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
				{   
					status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
					if(status!=FLASH_COMPLETE)break;	//发生错误了
				}else addrx+=4;
			}
			
			addrx=FlashSectorNum;				//起始值初始化
			if(status==FLASH_COMPLETE&&mode==0)			//写0处理
			{
				while(addrx<endaddr)//写数据
				{
					if(FLASH_ProgramWord(addrx,0x00000000)!=FLASH_COMPLETE)//写入数据0
					{ break;	//写入异常		
					}
					addrx+=4;
				} 
			}					
		}break;	
		default:break;
	}
	
	
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
}



////////////////////////////////////////////////////////////
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
//
///////////////////////////////////////////////////////////
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}



/****************************参数存储到BUFF中*********************/
//void BuffParamt_Write(u32 WriteAddr,union Multi_type multiType,u32 *pBuffer,u32 NumToWrite,u8 type)					
//输入参数：
//u32 WriteAddr：存储地址 
//union MultiType multiType1：存储数据
//u8 type:数据类型   0:u8   1:u32     2：浮点数（float）   3:字符串			  u8 u32 float 类型只能存储一个
//u8 type:数据类型   0:u8   1:u16  2:u32   3：浮点数（float）  4:字符串			 5:双精度浮点数     u8 u16 u32 float 类型只能存储一个    
//
//输出参数：
//返回值：无
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
			Parameter_2Buff(WriteAddr,(u32*)temp);							//数据存储到参数缓冲区		
		}break;			
		case 1:																										//u16
		{
			for(i=0;i<2;i++)temp[i]=multiType1.betys4_[i];					//16位数据拆分
			Parameter_2Buff(WriteAddr,(u32*)temp);							//数据存储到参数缓冲区			
		}break;
		case 2:																										//u32
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//32位数据拆分	
			Parameter_2Buff(WriteAddr,(u32*)temp);							//数据存储到参数缓冲区	
		}break;
		case 3:																										//浮点型
		{	
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//浮点数拆分	
			Parameter_2Buff(WriteAddr,(u32*)temp);							//数据存储到参数缓冲区			
		}break;
		case 4:																										//字符串
		{																		
			lenth_temp=strlen(multiType1.string_);									//计算数据长度		
			for(i=0;i<lenth_temp;i++)temp[i]=multiType1.string_[i];	//读取字符串
			Parameter_2Buff(WriteAddr,(u32*)temp);							//数据存储到参数缓冲区							(暂时不能使用)
		}break;	
		case 5:																									//双精度浮点数
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys8_[i];					//浮点数拆分	
			Parameter_2Buff(WriteAddr,(u32*)temp);							//数据存储到参数缓冲区
			for(i=4;i<8;i++)temp[i-4]=multiType1.betys8_[i];					//浮点数拆分	
			Parameter_2Buff(WriteAddr+4,(u32*)temp);							//数据存储到参数缓冲区				
			
		}break;		
		default: break;
	}
}



/****************************从BUFF中读取参数********************/
//void BuffParamt_Read(u32 ReadAddr,union MultiType multiType1,u8 type)				
//输入参数：
//u32 WriteAddr：存储地址 
//union MultiType multiType1：存储数据														//u8 和  float 类型只能存储一个
//u8 type:数据类型   0:u8   1:u32     2：浮点数（float）   3:字符串							//u8 u32 float 类型只能存储一个
//u8 type:数据类型   0:u8   1:u16  2:u32   3：浮点数（float）  4:字符串			  u8 u16 u32 float 类型只能存储一个
//输出参数：
//
//返回 
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
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//从参数从BUFF到HMI
			multiType2.u8_=temp[0];	
		}break;
		case 1:																							//u16
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//从参数从BUFF到HMI
			for(i=0;i<2;i++) multiType2.betys4_[i]=temp[i];	
		}break;
		case 2:																							//u32
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//从参数从BUFF到HMI
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];	
		}break;
			case 3:																						//浮点型
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//从参数从BUFF到HMI
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];			
		}break;
		case 4:																							//字符串
		{	
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//从参数从BUFF到HMI			
			//STMFLASH_Read(ReadAddr,(u32*)temp,20);			//数据读取,强制读取20个字节的字符串
			lenth_temp=strlen((char*)temp);							//计算数据长度			
			for(i=0;i<lenth_temp;i++) multiType2.string_[i]=temp[i];							//读取字符串			
		}break;
		case 5:																						//双精度浮点数
		{
			Parameter_FromBuff(ReadAddr,(u32*)temp);							//从参数从BUFF到HMI
			for(i=0;i<4;i++) multiType2.betys8_[i]=temp[i];
			Parameter_FromBuff(ReadAddr+4,(u32*)temp);							//从参数从BUFF到HMI
			for(i=4;i<8;i++) multiType2.betys8_[i]=temp[i-4];
		}break;
		default: break;
	}
	return multiType2;
}



/****************************从FLASH中读取参数*********************/
//union MultiType FlashParamt_Read(u32 ReadAddr,u8 type)				
//输入参数：
//u32 WriteAddr：存储地址 
//union MultiType multiType1：存储数据														//u8 和  float 类型只能存储一个
//u8 type:数据类型   0:u8   1:u32     2：浮点数（float）   3:字符串							//u8 u32 float 类型只能存储一个
//u8 type:数据类型   0:u8   1:u16  2:u32   3：浮点数（float）  4:字符串			  u8 u16 u32 float 类型只能存储一个   5：双精度浮点数,double
//输出参数：
//返回 
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
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//数据读取
			multiType2.u8_=temp[0];	
		}break;
		case 1:																							//u16
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//数据读取
			for(i=0;i<2;i++) multiType2.betys4_[i]=temp[i];	
		}break;
		case 2:																							//u32
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//数据读取
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];	
		}break;
			case 3:																						//浮点型
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//数据读取
			for(i=0;i<4;i++) multiType2.betys4_[i]=temp[i];
				
		}break;
		case 4:																							//字符串
		{	
			STMFLASH_Read(ReadAddr,(u32*)temp,20);			//数据读取,强制读取20个字节的字符串
			lenth_temp=strlen((char*)temp);							//计算数据长度			
			for(i=0;i<lenth_temp;i++) multiType2.string_[i]=temp[i];							//读取字符串			
		}break;
		case 5:
		{
			STMFLASH_Read(ReadAddr,(u32*)temp,1);  						//数据读取
			for(i=0;i<4;i++) multiType2.betys8_[i]=temp[i];
			STMFLASH_Read(ReadAddr+4,(u32*)temp,1);  						//数据读取
			for(i=0;i<4;i++) multiType2.betys8_[i+4]=temp[i];
		
		}break;
		default: break;
	}
	return multiType2;
}



/***************************参数存储到FLASH中*********************/
//void FlashParamt_Write(u32 WriteAddr,union MultiType multiType1,u8 type)				
//输入参数：
//u32 WriteAddr：存储地址 
//union MultiType multiType1：存储数据
//u8 type:数据类型   0:u8   1:u32     2：浮点数（float）   3:字符串			  u8 u32 float 类型只能存储一个
//u8 type:数据类型   0:u8   1:u16  2:u32   3：浮点数（float）  4:字符串			  u8 u16 u32 float 类型只能存储一个 5:double 双精度浮点数
//
//输出参数：
//返回值：无
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
			STMFLASH_Write(WriteAddr,(u32*)temp,1);								//数据存储			
		}break;			
		case 1:																										//u16
		{
			for(i=0;i<2;i++)temp[i]=multiType1.betys4_[i];					//16位数据拆分
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//数据存储
		}break;
		case 2:																										//u32
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//32位数据拆分	
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//数据存储
		}break;
		case 3:																										//浮点型
		{	
			for(i=0;i<4;i++)temp[i]=multiType1.betys4_[i];					//浮点数拆分	
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//数据存储				
		}break;
		case 4:																										//字符串
		{																		
			lenth_temp=strlen(multiType1.string_);									//计算数据长度		
			for(i=0;i<lenth_temp;i++)temp[i]=multiType1.string_[i];	//读取字符串
			STMFLASH_Write(WriteAddr,(u32*)temp,lenth_temp);				//数据存储
		}break;	
		case 5:				//双精度浮点数
		{
			for(i=0;i<4;i++)temp[i]=multiType1.betys8_[i];					//64位数据拆分	
			STMFLASH_Write(WriteAddr,(u32*)temp,1);									//数据存储
			for(i=0;i<4;i++)temp[i]=multiType1.betys8_[i+4];					//64位数据拆分	
			STMFLASH_Write(WriteAddr+4,(u32*)temp,1);									//数据存储
		}break;
		default: break;
	}
}




/***************************将BUFF中的数据批量保存到FLASH中*********************/
//void Buff2_Flash(u16 screenID,u16 controlID)
//输入参数：   u16 controlID：控件号
//输出参数：
//
//返回值
//说明：所有页面参数刷新的控件号都是120,且当读取到某一页面参数改变时，则其它页面参数保存不变，只修改当前页面
//且当前参数存储页面只有 screenID：6-39号界面
/**************************************************************************/
void Buff2_Flash(u16 screenID,u16 controlID)
{
	u8 i,j;
	if(controlID==120)												//当控件号为120，则执行参数的批量保存到FLASH工作 
	{			
//		for(j=Pg_OutsideParmt;j<=Pg_Parmt22;j++)
//		{
//			if(j!=screenID) for(i=0;i<121;i++) STMFLASH_Read(AGVPa_Addr+j*Page_Capa+i*0x04,&ParamtTemp[j][i],1);	//将Flash区的参数存储到temp
//			
//			else for(i=0;i<121;i++) ParamtTemp[j][i]=Paramt_BUFF[j][i];	//将参数Buff区的参数存储到temp	
//			
//		}		
		
		for(j=Pg_OutsideParmt;j<=Pg_StaCardNum19;j++)																													
		{			
			for(i=0;i<121;i++)
			{	
				STMFLASH_Write(AGVPa_Addr+j*Page_Capa+i*0x04,&Paramt_BUFF[j][i],1);								//将temp中的参数再存储回FLASH		
			}
	
		}			
	}	
}



/*********************界面中部分参数查询*********************/
//union MultiType AGV_ParameterSeek(u16 pageID,u8 row,u8 column,u8 type)
//输入参数：u8 pageID：人机界面的pageId 号  u8 row：数据所在页面行   u8 column：数据所在页面列   u8 type: 数据类型    
//输出参数：
//返回值：多类型数据
//分配地址u8类型各存储空间空间为0x200大小   可以存储512/4=128个数据
//每一个界面内部的数据按照控件号的顺序向上累加
//主要包括以下几个界面：
//室外模式参数界面
//室内路径设置（11-15）
//室内模式参数界面
//常规参数设置17
//站点卡号18-19
//常规参数设置20-25
/**********************************************************************************/
union MultiType AGV_ParameterRead(u16 pageID,u16 controlID,u8 type)
{
	union MultiType multiType2;
	u32 addr=0; 
	switch(pageID)
	{
		case 0x0A:				//室外模式参数界面
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			multiType2=BuffParamt_Read(addr,type);			//数据索引	
		}break;
			
		case 0x0B:					//室内路径设置11
		case 0x0C:					//室内路径设置12	
		case 0x0D:					//室内路径设置13
		case 0x0E:					//室内路径设置14
		case 0x0F:					//室内路径设置15
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			multiType2=BuffParamt_Read(addr,type);			//数据索引	
		}break;
		case 0x10:					//室内模式参数界面	
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			multiType2=BuffParamt_Read(addr,type);			//数据索引			
		}break;	
		case 0x11:					//常规参数设置17
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;													//求出某数据的地址偏移量
			multiType2=BuffParamt_Read(addr,type);					//数据索引			
		}break;
		case 0x12:					//站点卡号18
		case 0x13:					//站点卡号19
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			multiType2=BuffParamt_Read(addr,type);			//数据索引			
		}break;
//		case 0x14:					//常规参数设置20
//		case 0x15:					//常规参数设置21
//		case 0x16:					//常规参数设置22	
//		case 0x17:					//常规参数设置23
//		case 0x18:					//常规参数设置24	
//		case 0x19:					//常规参数设置25	
//		{
//			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
//			addr=addr+controlID*4;								//求出某数据的地址偏移量
//			multiType2=BuffParamt_Read(addr,type);			//数据索引			
//		}break;
		default:break;
	}
	return multiType2;
}



/*********************界面中部分参数存储*********************/
//void AGV_ParameterWrite(u16 pageID,u16 controlID,union MultiType multiType1,u8 type)
//输入参数：u8 pageID：人机界面的pageId 号  u8 row：数据所在页面行   u8 column：数据所在页面列    u8 type:数据类型
//union MultiType multiType1	存储的参数
//输出参数：
//返回值：
//AGV数据存储方式：
//1.除字符串外其它数据都占用四个字节；
//2.当前可以存储的数据类型包括：u8，u16，u32,float  ,字符串地址已经分配，但是相关程序还没写
//3.数据按照人机界面页面顺序进行分配，具体见头文件；
//4.每个页面提供0x200字节进行数据存储，可以存储128个数据；
//5.除了字符串外，其它数据无论什么类型都存储在同一个页面空间内；
//6.数据根据所在页面的控件号绝对值进行便宜
/**********************************************************************************/
void AGV_ParameterWrite(u16 pageID,u16 controlID,union MultiType multiType1,u8 type)
{
	u32 addr=0;

	switch(pageID)
	{	
		case 0x00:
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			BuffParamt_Write(addr,multiType1,type);			//数据写入					
		}
		case 0x0A:															//室外模式参数界面
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			BuffParamt_Write(addr,multiType1,type);			//数据写入		
		}break;
			
		case 0x0B:					//室内路径设置11
		case 0x0C:					//室内路径设置12	
		case 0x0D:					//室内路径设置13
		case 0x0E:					//室内路径设置14
		case 0x0F:					//室内路径设置15
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			BuffParamt_Write(addr,multiType1,type);			//数据写入		
		}break;
		case 0x10:					//室内模式参数界面	
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			BuffParamt_Write(addr,multiType1,type);			//数据写入			
		}break;	
		case 0x11:					//常规参数设置17
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			BuffParamt_Write(addr,multiType1,type);			//数据写入				
		}break;
		case 0x12:					//站点卡号18
		case 0x13:					//站点卡号19
		{
			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
			addr=addr+controlID*4;								//求出某数据的地址偏移量
			BuffParamt_Write(addr,multiType1,type);			//数据写入				
		}break;
//		case 0x14:					//常规参数设置20
//		case 0x15:					//常规参数设置21
//		case 0x16:					//常规参数设置22	
//		case 0x17:					//常规参数设置23
//		case 0x18:					//常规参数设置24	
//		case 0x19:					//常规参数设置25	
//		{
//			addr=AGVPa_Addr+pageID*Page_Capa;								//求出当前页面首地址		
//			addr=addr+controlID*4;								//求出某数据的地址偏移量
//			BuffParamt_Write(addr,multiType1,type);			//数据写入			
//		}break;
		default:break;
	}
}


/*********************************页面参数写入**************************/
//void Pg_ParameterInput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type)
//输入参数：u16 screenId:页面ID号   u16 controlID：控件ID号    union MultiType multiType1：存储的参数   u8 type：数据类型     
//输出参数：
//返回：
//注意:screenID就是pageID
/**********************************************************************/
void Pg_ParameterInput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type)
{
	AGV_ParameterWrite(screenId,controlID,multiType1,type);				//写入参数
}


/*********************************页面参数输出**************************/
//void Pg_ParameterOutput(u16 screenId,u16 controlID,union MultiType multiType1,u8 type)
//输入参数：u16 screenId:页面ID号   u16 controlID：控件ID号    union MultiType multiType1：存储的参数   u8 type：数据类型     
//输出参数：
//返回：
//注意:screenID就是pageID
/***********************************************************************/
union MultiType Pg_ParameterOutput(u16 screenId,u16 controlID,u8 type)
{
	union MultiType multiType2;
	multiType2=AGV_ParameterRead(screenId,controlID,type);			//读取参数		
	return multiType2;
}




/***************************某一页面特定参数的刷新*********************/
//void Pg_ParameteRefresh(u16 screenId)
//输入参数：u16 screenId:页面ID号       
//输出参数：
//返回：
//注意:screenID就是pageID
/***********************************************************************/
void Pg_ParameteRefresh(u16 screenId)
{
	u8 iconDate[16],i;
	double temp[2],temp1[2];
	static u8 cardNumStor=0,JogModeStor=0,AutoModeStor=0;
	switch(screenId)
	{
		case 0x00:										//主界面0
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
							
			MultiIcon_Refresh(screenId,5,16,iconDate,0);						//批量图标显示
			
			//遥控数据框
			if(AGVStatus.AJMode==0)				//手动模式启动
			{
				SetTextValueFloat(screenId,56,SymaStatus.Spd_Jog,_float);		//速度值
				SetTextValueFloat(screenId,57,SymaStatus.Spd_cle[1],_float);		//转弯半径
			}	
			//室内自动数据		
			if(AGVStatus.AUTOmode==1)
			{
				SetTextValueFloat(screenId,58,(float)(AGVSpd_IN*AGVStatus.Wheel_R*AGVStatus.pi1/60/32),_float);		//速度值
				SetTextValueFloat(screenId,59,Mag_date[2],_float);		//磁导航值
				SetTextValueu16(screenId,60,BranchPath.cardNum);							//当前卡号
				SetTextValueu16(screenId,61,BranchPath.selectPathNum);			//当前路径号
			}					
			//室外自动数据
			if(AGVStatus.AUTOmode==2)
			{
				SetTextValueFloat(screenId,62,MapPot.AutoSpd,_float);		//速度值
				SetTextValueFloat(screenId,63,Smc345.scm345Angle,_float);		//航向角
				SetTextValueFloat(screenId,64,lonti_lati[0],_float);		//经度值
				SetTextValueFloat(screenId,65,lonti_lati[1],_float);		//维度值
				SetTextValueFloat(screenId,66,OAOB[0],_float);					//OA值
				SetTextValueFloat(screenId,67,OAOB[1],_float);					//OB值		
			}
			
			if(AutoModeStor!=AGVStatus.AUTOmode|| JogModeStor!=AGVStatus.AJMode)			//当模式状态发生变化时，数据状态框清零
			{
				SetTextValueFloat(screenId,56,0,_float);		//速度值
				SetTextValueFloat(screenId,57,0,_float);		//转弯半径
			
				SetTextValueFloat(screenId,58,0,_float);		//速度值
				SetTextValueFloat(screenId,59,0,_float);		//磁导航值
				SetTextValueu16(screenId,60,0);							//当前卡号
				SetTextValueu16(screenId,61,0);							//当前路径号
				
				SetTextValueFloat(screenId,62,0,_float);		//速度值
				SetTextValueFloat(screenId,63,0,_float);		//航向角
				SetTextValueFloat(screenId,64,0,_float);		//经度值
				SetTextValueFloat(screenId,65,0,_float);		//维度值
				SetTextValueFloat(screenId,66,0,_float);		//OA值
				SetTextValueFloat(screenId,67,0,_float);		//OB值		
			}
						
			//电池电量
	//		SetTextValueu16(screenId,55,BatteryMsg.RSOC);						//电池电量百分比					
			JogModeStor=AGVStatus.AJMode;
			AutoModeStor=AGVStatus.AUTOmode;

		}break;
		
		case 0x02:											//IO监控界面
		{		
			iconDate[0]=IN_JOGSetup;								//遥控启动
			iconDate[1]=IN_AUTOSetup;								//设备启动
			iconDate[2]=IN_EmerStop;								//急停
			iconDate[3]=!IN_SEV1;										//伺服1就绪信号
			iconDate[4]=!IN_SEV2;										//伺服2就绪信号
			iconDate[5]=!IN_SEV3;										//伺服3就绪信号
			iconDate[6]=!IN_SEV4;										//伺服4就绪信号
			iconDate[7]=OT_SON;											//使能输出
			iconDate[8]=OT_JD1;											//继电器1
			iconDate[9]=OT_JD2;											//继电器2
			iconDate[10]=OT_DIR1;										//伺服1方向信号
			iconDate[11]=OT_DIR2;										//伺服2方向信号
			iconDate[12]=OT_DIR3;										//伺服3方向信号
			iconDate[13]=OT_DIR4;										//伺服4方向信号	
			MultiIcon_Refresh(screenId,1,14,iconDate,0);						//按钮状态显示			
		}break;
		case 0x03:							//轨迹显示界面3
		{
			MapPotDsiplay= AGV_OutSideValue(gpsx,Smc345.scm345Angle,LontLati);
			if(AGVStatus.AUTOmode==2||AGVStatus.TeachMode==1)											//当处于室外自动模式或者示教模式时
			{
				SetTextValueFloat(screenId,3,MapPotDsiplay.longti_du,_float);				//经度度格式
				SetTextValueFloat(screenId,4,MapPotDsiplay.lati_du,_float);					//纬度度格式
				SetTextValueFloat(screenId,5,MapPotDsiplay.longti_dufen,_float);		//经度度分格式
				SetTextValueFloat(screenId,6,MapPotDsiplay.lati_dufen,_float);			//纬度度分格式
				SetTextValueFloat(screenId,7,MapPotDsiplay.starNum,_float);					//当前星数
				SetTextValueFloat(screenId,8,MapPotDsiplay.OA,_float);							//OA值
				SetTextValueFloat(screenId,9,MapPotDsiplay.OB,_float);							//OB值
				SetTextValueFloat(screenId,10,MapPotDsiplay.trueAngle,_float);			//航向角
			}
			else
			{
				SetTextValueFloat(screenId,3,0,_float);		//经度度格式
				SetTextValueFloat(screenId,4,0,_float);		//纬度度格式
				SetTextValueFloat(screenId,5,0,_float);		//经度度分格式
				SetTextValueFloat(screenId,6,0,_float);		//纬度度分格式
				SetTextValueFloat(screenId,7,0,_float);		//当前星数
				SetTextValueFloat(screenId,8,0,_float);		//OA值
				SetTextValueFloat(screenId,9,0,_float);		//OB值
				SetTextValueFloat(screenId,10,0,_float);	//航向角
			}			
		}break;
		case 0x04:										//轨迹编辑页面4
		{	
			if(AGVStatus.MapDownMode==1)
			{
				SetTextValueu16(screenId,6,MapPot.Num);			//下载进度数据,允许地图下载打开
				SetTextValueu16(screenId,21,MapPot.errNum);			//下载进度数据,允许地图下载打开
			}
			else
			{
				MapPot.Num=0;
				SetTextValueu16(screenId,6,0);			//下载进度数据，允许下载地图关闭
				SetTextValueu16(screenId,21,0);			//下载进度数据,允许地图下载打开
				MapPot.errNum=0;
			}				
			AnimationPlayFrame(screenId,10,AGVStatus.MapDownMode);		//下载进度图标
				
			AnimationPlayFrame(screenId,11,AGVStatus.TeachMode);		//示教状态
			if(AGVStatus.TeachMode)					//当示教模式启动时
			{	
				SetTextValueFloat(screenId,7,lonti_lati[0],_float);		//经度值
				SetTextValueFloat(screenId,8,lonti_lati[1],_float);		//维度值
				SetTextValueFloat(screenId,9,Smc345.scm345Angle,_float);		//航向角
//				SetTextValueFloat(screenId,20,XX);		//星数		
			}
			else					//当示教模式未启动时
			{
				SetTextValueFloat(screenId,7,0,_float);		//航向角
				SetTextValueFloat(screenId,8,0,_float);		//经度值
				SetTextValueFloat(screenId,9,0,_float);		//维度值		
			}	
			
		}break;	
	
		case 0x0A:					//室外路径参数设置
		{
//			if((u8)Paramt_BUFF[screenId][13]==0xFF||(u8)Paramt_BUFF[screenId][13]==0)AnimationPlayFrame(screenId,13,0);		//反向选择状态框
//			else AnimationPlayFrame(screenId,13,1);		//反向选择状态框
			longti_lati_trans(0,longti_O,lati_O,temp1,temp);	
			LontLati= Origin_O_parmt_Init();				//GPS 原点信息读取
			MapPot.AutoSpd=LontLati.spdTemp;				//自动运行速度
			LontLati= Origin_O_parmt_Init();				//GPS 原点信息读取
			
		}break;
		case 0x10:					//室内模式参数，速度设置
		{
			front_pid=AGVPID_Change_FLASH(0);		//PID组1  前驱动
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 0x11:										//常规参数设置17
		{			
			AnimationPlayFrame(screenId,5,(u8)Paramt_BUFF[17][5]);														//避障选择状态框				
			//for(i=0;i<8;i++) iconDate[i]=(unsigned int)Ultrosonic.channel[i];			//获取超声波实时数据
			//MultiIcon_Refresh(screenId,30,37,iconDate,1);						//按钮状态显示	
			for(i=0;i<8;i++) SetTextValueFloat(screenId,30+i,(float)Ultrosonic.channel[i],_float);		//航向角
			Ultrosonic.deviceEn=(u8)Paramt_BUFF[Pg_CommonParmt][5];						//避障传感器是否打开
			
		}break;
		default:break;
	}
	cardNumStor=RFID_value;	
}


/*********************************对当前显示的屏幕进行实时刷新************/
//u16 Dacai_Display( PCTRL_MSG msg,u8 mode)
//输入参数:CTRL_MSG msg:大彩屏数据通信缓冲取   
//输出参数：
//返回：
//注意:screenID就是pageID
/***********************************************************************/
u16 Dacai_Display( PCTRL_MSG msg)
{
	u16 size;
	u16 screenID; 
	temp_mark=1;
	size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);	//从缓冲区中获取一条指令    
	if(size>0)
	{
		screenID=PTR2U16(&msg->screen_id);						//如果接收到命令，则输出当前的页面ID值
		temp_mark=2;
		ProcessMessage((PCTRL_MSG)cmd_buffer, size);	//指令处理
	}
	temp_mark=3;
	Pg_ParameteRefresh(CurrtScreenID);							//实时刷新指定界面的参数
	temp_mark=4;
	return screenID;
}

/***************************对所有参数界面进行刷新************************/
//void AllPg_Refresh(TextFormat *textFormat,TextFormat *textFormat1,TextFormat *textFormat2)
//输入参数:PTextFormat *textFormat:参数数据存储1   PTextFormat *textFormat2:参数数据存储2   PTextFormat *textFormat3:参数数据存储3
//输出参数：
//返回：
//注意:screenID就是pageID
/*****************************************************************************/
void AllPg_Refresh(TextFormat *textFormat,TextFormat *textFormat1,TextFormat *textFormat2)
{
	u16 i,j;
	union MultiType multiType2;
	u16 firstID,lastID;

	//室外模式参数界面

	j=Pg_OutsideParmt;	
	i=1;							//经度值存储		
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_double);
	ParamtType[j][i]=_double;											//参数类型存储
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_double);//将FLASH中的参数存储到从参数BUFF				
	//OnePg_Refresh(j,textFormat1,1,_double,0);
	SetTextValueFloat(j,i,multiType2.double_,_double);
	
	i=3;							//维度值存储		
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_double);
	ParamtType[j][i]=_double;											//参数类型存储
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_double);//将FLASH中的参数存储到从参数BUFF				
	//OnePg_Refresh(j,textFormat,1,_double,0);
	SetTextValueFloat(j,i,multiType2.double_,_double);
	
	firstID=10;				//地图点数量
	lastID=11;				//分段点数量
	for(i=firstID;i<=lastID;i++)
	{
		multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u16);
		textFormat[i-firstID].u16Text=multiType2.u16_;
		textFormat[i-firstID].controlID=i;	
		ParamtType[j][i]=_u16;											//参数类型存储
		BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u16);//将FLASH中的参数存储到从参数BUFF				
	}OnePg_Refresh(j,textFormat,2,_u16,0);
	
	i=12;							//自动寻迹速度			
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_float);
	textFormat1[0].floatText=multiType2.float_;
	textFormat1[0].controlID=i;	
	ParamtType[j][i]=_float;											//参数类型存储
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_float);//将FLASH中的参数存储到从参数BUFF				
	OnePg_Refresh(j,textFormat1,1,_float,0);
	
	i=13;							//反向寻迹标记			
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
	textFormat[0].u8Text=multiType2.u8_;
	textFormat[0].controlID=i;	
	ParamtType[j][i]=_u8;											//参数类型存储
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//将FLASH中的参数存储到从参数BUFF					
	OnePg_Refresh(j,textFormat,1,_u8,1);

	//室内路径设置11-15
	firstID=1;
	lastID=90;
	for(j=Pg_InsideMap11;j<=Pg_InsideMap15;j++)
	{	
		for(i=firstID;i<=lastID;i++)
		{
			multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
			textFormat[i-firstID].u8Text=multiType2.u8_;
			textFormat[i-firstID].controlID=i;	
			ParamtType[j][i]=_u8;											//参数类型存储
			BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//将FLASH中的参数存储到从参数BUFF				
		}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u8,0);
	}
	
	//室内模式参数界面  
	firstID=1;
	lastID=15;
	j=Pg_InsideParmt;
	for(i=firstID;i<=lastID;i++)
	{
		multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_float);
		textFormat[i-firstID].floatText=multiType2.float_;
		textFormat[i-firstID].controlID=i;	
		ParamtType[j][i]=_float;											//参数类型存储
		BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_float);//将FLASH中的参数存储到从参数BUFF			
	}OnePg_Refresh(j,textFormat,lastID-firstID+1,_float,0);
	
	
	//常规参数设置界面
	firstID=20;
	lastID=27;
	j=Pg_CommonParmt;
	for(i=firstID;i<=lastID;i++)
	{
		multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
		textFormat[i-firstID].u8Text=multiType2.u8_;
		textFormat[i-firstID].controlID=i;	
		ParamtType[j][i]=_u8;											//参数类型存储
		BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//将FLASH中的参数存储到从参数BUFF		
	}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u8,0);	
	
	i=5;							//避障传感器开关		
	multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
	textFormat[0].u8Text=multiType2.u8_;
	textFormat[0].controlID=i;	
	ParamtType[j][i]=_u8;											//参数类型存储
	BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//将FLASH中的参数存储到从参数BUFF					
	OnePg_Refresh(j,textFormat,1,_u8,1);
	
	//站点卡号18-19
	firstID=1;
	lastID=60;
	for(j=Pg_StaCardNum18;j<=Pg_StaCardNum19;j++)
	{
		for(i=firstID;i<=lastID;i++)
		{
			multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u16);
			textFormat[i-firstID].u16Text=multiType2.u16_;
			textFormat[i-firstID].controlID=i;	
			ParamtType[j][i]=_u16;																	//参数类型存储	
			BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u16);//将FLASH中的参数存储到从参数BUFF				
		}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u16,0);
	}

//	//有参数存储界面20-22
//	firstID=1;
//	lastID=90;
//	for(j=Pg_Parmt20;j<=Pg_Parmt22;j++)
//	{
//		for(i=firstID;i<=lastID;i++)
//		{
//			multiType2=FlashParamt_Read(AGVPa_Addr+j*Page_Capa+i*0x04,_u8);
//			textFormat[i-firstID].u8Text=multiType2.u8_;
//			textFormat[i-firstID].controlID=i;	
//			ParamtType[j][i]=_u8;											//参数类型存储
//			BuffParamt_Write(AGVPa_Addr+j*Page_Capa+i*0x04,multiType2,_u8);//将FLASH中的参数存储到从参数BUFF				
//		}OnePg_Refresh(j,textFormat,lastID-firstID+1,_u8,0);
//	}
}

/**********获取人机界面输入数据，存储到Flash并单独刷新那个输入的数据*************/
//void Dacai_InOut(u16 screenID,u16 controlID,u8* str,union MultiType multiType1)
//输入参数:PTextFormat *textFormat:参数数据存储1   PTextFormat *textFormat2:参数数据存储2   PTextFormat *textFormat3:参数数据存储3
//输出参数：
//返回：
//注意:screenID就是pageID
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
			sscanf(str,"%u",&multiType1.u8_);//把字符串转换为整数
			AGV_ParameterWrite(screenID,controlID,multiType1,type);						//存储HMI输入的参数
			multiType2=AGV_ParameterRead(screenID,controlID,_u8);					//读取重新输入的参数
			SetTextValueu16(screenID,controlID,(u16)multiType2.u8_);		//返回存储器内的参数到人机界面	
		}break;
		case _u16:
		{
			sscanf(str,"%u",&multiType1.u16_);//把字符串转换为整数
			AGV_ParameterWrite(screenID,controlID,multiType1,_u16);						//存储HMI输入的参数
			multiType2=AGV_ParameterRead(screenID,controlID,_u16);						//返回存储器内的参数到人机界面			
			SetTextValueu16(screenID,controlID,(u16)multiType2.u16_);		    //返回存储器内的参数到人机界面		
		
		}break;
		case _u32:
		{
	/*		sscanf(str,"%ld",&multiType1.u32_);//把字符串转换为整数
			AGV_ParameterWrite(screenID,controlID,multiType1,_u32);						//存储HMI输入的参数
			multiType2=AGV_ParameterRead(screenID,controlID,_u32);						//返回存储器内的参数到人机界面	
			SetTextValueu32(screenID,controlID,(u16)multiType1.u32_);		//返回存储器内的参数到人机界面	*/		
		}break;
		case _float:
		{
			sscanf(str,"%f",&multiType1.float_);//把字符串转换为整数
			AGV_ParameterWrite(screenID,controlID,multiType1,_float);						//存储HMI输入的参数
			multiType2=AGV_ParameterRead(screenID,controlID,_float);						//返回存储器内的参数到人机界面
			SetTextValueFloat(screenID,controlID,multiType2.float_,_float);		//返回存储器内的参数到人机界面		
		}break;
		case _double:
		{
			sscanf(str,"%lf",&multiType1.double_);//把字符串转换为整数
			AGV_ParameterWrite(screenID,controlID,multiType1,_double);						//存储HMI输入的参数
			multiType2=AGV_ParameterRead(screenID,controlID,_double);						//返回存储器内的参数到人机界面
			SetTextValueFloat(screenID,controlID,multiType2.double_,_double);		//返回存储器内的参数到人机界面		
		}break;
		default:break;
	}
}


/************************AGV当前处于哪个界面判断********************************/
//u16 ScreenID_Judge(u16 screenId，u16 controlID)
//输入参数:u16 screenId: 页面ID号                   u16 controlID:控件ID号
//输出参数：
//返回：返回当前所处的页面ID号
//注意:screenID就是pageID
/*****************************************************************************/
u16 ScreenID_Judge(u16 screenID,u16 controlID)
{
	u16 currtScreenID;
	
	if(controlID==0x7D) currtScreenID=0x00;		//当有返回主界面按钮按下，则当前界面ID号 =0x00
	switch(screenID)
	{	
			case 0x00:				//主界面
			{
				switch(controlID)
				{
					case 0x01:currtScreenID=0x01;break;			//功能参数界面		
					default:break;
				}		
			}break;
		
			case 0x01:				//功能参数界面
			{
				switch(controlID)
				{
					case 0x01:currtScreenID=0x03;break;			//室外轨迹处理界面	
					case 0x02:currtScreenID=0x0A;break;			//室外模式参数界面	
//					case 0x03:currtScreenID=0x03;break;			//备用1界面	
//					case 0x04:currtScreenID=0x03;break;			//备用2界面	
					case 0x05:currtScreenID=0x0B;break;			//室内路径参数界面	
					case 0x06:currtScreenID=0x10;break;			//室内模式参数界面	
					case 0x07:currtScreenID=0x12;break;			//站点卡号界面	
//					case 0x08:currtScreenID=0x03;break;			//备用4界面	
					case 0x09:currtScreenID=0x02;break;			//IO监控界面	
					case 0x0A:currtScreenID=0x11;break;			//常规设置界面	
//					case 0x0B:currtScreenID=0x03;break;			//备用5界面
//					case 0x0C:currtScreenID=0x03;break;			//备用6界面						
					default:break;
				}		
			}break;
			
			case 0x03:				//室外轨迹预览
			{
				if(controlID==0x01) currtScreenID=0x04;		//室外轨迹编辑界面	
			}break;
			
			case 0x04:				//室外轨迹编辑
			{
				if(controlID==0x01) currtScreenID=0x03;		//室外轨迹预览界面	
			}break;
			
			case 0x0B:				//室内路径参数界面11
			case 0x0C:				//室内路径参数界面12
			case 0x0D:				//室内路径参数界面13
			case 0x0E:				//室内路径参数界面14
			case 0x0F:				//室内路径参数界面15
			case 0x12:				//站点卡号界面18
			case 0x13:				//站点卡号界面19		
			{
				switch(controlID)
				{
					case 0x79:						
					{
						currtScreenID=screenID-1;				//上翻一页
					}break;
					case 0x7A:
					{
						currtScreenID=screenID+1;				//下翻一页
					}break;		
					default:break;
				}		
			}break;		
	}
	return currtScreenID;
}


/******************************AGV从FLASH在线修改PID值*****************************/
//void AGVPID_Change_FLASH(u8 PIDGroup)
//输入参数：u8 PIDGroup:PID组号
//PIDGroup=0-8对应运行参数1界面从上到下8组PID参数，其中比例调节对应K，积分调节对应I；
//
//输出参数：frontPID：前驱动PID参数，rearPID：后驱动参数
//返回值：
/***********************************************************************************/
PIDtypedef AGVPID_Change_FLASH(u8 PIDGroup)
{
	union MultiType multiType;
	PIDtypedef static pid;
	switch(PIDGroup)
	{
		case 0:				//第一组PID参数设置
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];//1级速度
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][2];
			pid.Proportion=multiType.float_;				//驱动PID K参数		
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][3];
			pid.Integral=multiType.float_;					//驱动PID I参数
		}break;
		case 1:		  	//第一组PID参数设置
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];//1级速度
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][2];
			pid.Proportion=multiType.float_;				//驱动PID K参数
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][3];
			pid.Integral=multiType.float_;					//驱动PID I参数
		}break;
		case 2:				//第二组PID参数设置
		{	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][4];//2级速度
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][5];
			pid.Proportion=multiType.float_;				//驱动PID K参数				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][6];
			pid.Integral=multiType.float_;					//驱动PID I参数
		}break;
		case 3:				//第三组PID参数设置
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][7];//3级速度
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][8];
			pid.Proportion=multiType.float_;				//驱动PID K参数				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][9];
			pid.Integral=multiType.float_;					//驱动PID I参数
		}break;
		case 4:				//第四组PID参数设置
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][10];//转弯速度
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;	
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][11];
			pid.Proportion=multiType.float_;				//驱动PID K参数				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][12];
			pid.Integral=multiType.float_;					//驱动PID I参数
		}break;
		case 5:				//第四组PID参数设置
		{
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][10];//停车速度
			pid.AGPspd=multiType.float_*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][14];
			pid.Proportion=multiType.float_;				//驱动PID K参数				
			multiType.u32_=Paramt_BUFF[Pg_InsideParmt][15];
			pid.Integral=multiType.float_;					//驱动PID I参数
		}break;
		default:break;
	}
	pid.Derivative=0;   //微分为0
	pid.SetPoint=0;     //设定目标为0
	return pid;
}


/******************************AGV从FLASH在线修改速度值*****************************/
//void AGVSPD_Change_FLASH(u8 spdGroup)
//输入参数：u8 spdGroup:速度组号分别定义HMI上面的8个速度
//spdGroup=0或者=1：1级速度   spdGroup=2：2级速度       spdGroup=3：3级速度 
//spdGroup=4：4级速度   spdGroup=5：5级速度       spdGroup=6：6级速度
//spdGroup=7：岔道速度  spdGroup=8：寻找停车速度
//
//输出参数：
//返回：u16:获得的速度值
/********************************************************************************/
u16 AGVSPD_Change_FLASH(u8 spdGroup)
{
//	u8 radi;
//	u16 lenth;
  float spd;
	union MultiType multiType;
	switch(spdGroup)
	{
		case 0:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];break;				//1级速度									
		case 1:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][1];break;				//1级速度
		case 2:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][4];break;					//2级速度	
		case 3:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][7];break;					//3级速度	
		case 4:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][10];break;				//转弯速度	
		case 5:multiType.u32_=Paramt_BUFF[Pg_InsideParmt][13];break;				//寻找停车速度	
		default:break;
	}

	spd=multiType.float_;							  //速度值给定
//	radi=(u8)Paramt_BUFF[39][5];		//减速比
//	lenth=(u8)Paramt_BUFF[39][8];	//轮子周长
//	if(lenth==0) return 0;							//防止length周长值没有输入，分母为0，输出值极大
	 return (u16)spd*60*32/AGVStatus.pi1/AGVStatus.Wheel_R;       //左前轮速度强制转换		单位：m/s-->r/mim				
}


/*****************************将地图信息存储到FLASH中***************************/
//函数形式：void Map_Save2FLASH(u32 mapAddr ,MapPotDef mapPot,u16 potNum,u8 size)
//输入参数：u32 mapAddr：参数存储首地址     mapPot：地图上的点（一个点集，包括OA OB angle）,u16 potNum:地图上第N个点 ,u8 size：数据尺寸(占字节数)
//输出参数：
//返回参数：
/*****************************************************************************/
void Map_Save2FLASH(MapPotDef mapPot,u8 size)
	
{
	union MultiType multiType;
	multiType.float_=mapPot.OA;
	STMFLASH_Write(MAPOA_Addr+size*mapPot.Num,&multiType.u32_,size/4);							//存储OA值
	
	multiType.float_=mapPot.OB;	
	STMFLASH_Write(MAPOB_Addr+size*mapPot.Num,&multiType.u32_,size/4);							//存储OB值
	
	multiType.float_=mapPot.angle;
	STMFLASH_Write(MAPAngle_Addr+size*mapPot.Num,&multiType.u32_,size/4);							//存储angle值
}


/*****************************从FLASH中读取地图数据************************************/
//函数形式：MapPotDef Map_ReadFromFLASH(u32 mapAddr,u16 potNum,u8 size)
//输入参数：u16 potNum:地图上第N个点，  ,u8 size：数据尺寸(占字节数)
//输出参数：
//返回参数：MapPotDef   .OA;OA值    .OB;OB值   .angle;angle值   
//
/*****************************************************************************/
MapPotDef Map_ReadFromFLASH(u16 potNum,u8 size)
{
	MapPotDef mapPot;
	union MultiType multiType;
	STMFLASH_Read(MAPOA_Addr+size*potNum,&multiType.u32_,size/4);					//返回OA值
	mapPot.OA=multiType.float_;
	
	
	STMFLASH_Read(MAPOB_Addr+size*potNum,&multiType.u32_,size/4);					//返回OB值
	mapPot.OB=multiType.float_;
	
	STMFLASH_Read(MAPAngle_Addr+size*potNum,&multiType.u32_,size/4);					//返回angle值
	mapPot.angle=multiType.float_;	
	
	mapPot.Num=potNum;
	return mapPot;
}


/*************检查地图存储区的某个存储单元是否异常为0xfffffff****************************/
//函数形式：u8 Map_StorageCheck(u16 potNum，u8 size)
//输入参数：u16 potNum:存储的地图点号
//输出参数：
//返回参数：error : 0已经存储了数据  1：存储地址为0x000000000,说明数据没有存入或者地图存储区还没有刷新 
//
/*****************************************************************************/
u8 Map_StorageCheck(u16 potNum,u8 size)
{
	u8 error;
	u32 static temp1=0,temp2=0,temp3=0;
	union MultiType multiType;
	STMFLASH_Read(MAPOA_Addr+size*potNum,&multiType.u32_,size/4);					//返回OA值
	temp1=multiType.u32_;
		
	STMFLASH_Read(MAPOB_Addr+size*potNum,&multiType.u32_,size/4);					//返回OB值
	temp2=multiType.u32_;
	
	STMFLASH_Read(MAPAngle_Addr+size*potNum,&multiType.u32_,size/4);					//返回angle值
	temp3=multiType.u32_;	
	
	if(temp1&temp2&temp3==0xFFFFFFFF) error=1;
	else error=0;
	return error;
}


