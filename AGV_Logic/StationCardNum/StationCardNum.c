#include "HEAD.h"


//u16 firstID,lastID,cardvalue;					//站点卡号ID号


/**********************通过卡号获取站点号*****************************/
//void Station_CardNum(u32 cardValue)
//输入参数：u32 cardValue：RFID获取的原始卡号
//输出参数：
//返回参数：
//功能描述：当获取RFID卡号后，搜索当前参数中对应的站号功能,并输出
/*********************************************************************/
void Station_CardNum(u32 cardValue)
{
	u16 firstID,lastID,cardvalue;					//站点卡号ID号
	u16 i,j;
	static StationCardNumDef staCardNum;			
	cardvalue=(u16)cardValue;							//转换为16位数据	
	StationCardNum.cardNum=cardValue;									//卡号赋值
	for(j=Pg_StaCardNum18;j<=Pg_StaCardNum19;j++)						//页面18-19循环
	{
		firstID=1;
		lastID=59;
		for(i=firstID;i<=lastID;i=i+2)
		{
			if(Paramt_BUFF[j][i]==cardvalue)								//当找到相应卡号
			{
				StationCardNum.stationNum=(u16)Paramt_BUFF[j][i+1];			//站号赋值
//			staCardNum.cardNum=cardvalue;									//卡号赋值
				StationCardNum.error=0x01;												//找到相应的卡号功能
				return;
			}	
			else																					//当没有找到相应卡号
			{							
				StationCardNum.error=0x00;												//没有找到相应的卡号功能				
			}		
		}			
	}
}


/**********************卡号相互等于功能****************************/
//void CardNum_StationNum(u16 CurrentScreenId)
//输入参数：u16 CurrentScreenId：当前屏幕的ID号
//输出参数：
//返回参数：
//功能描述：当获取RFID卡号后，搜索当前参数中对应的站号功能,并输出
/*********************************************************************/
//void CardNum_StationNum(u16 CurrentScreenId)
//{
//	union MultiType multiType9;
//	switch(CurrentScreenId)	
//	{
//		case Pg_StaCardNum18:
//		case Pg_StaCardNum19:
//		{
//			multiType9.u8_=Paramt_BUFF[Pg_StaCardNum5][64];										//其它站点卡号临时赋值
//			if(Paramt_BUFF[CurrentScreenId][64]!=multiType9.u8_)						//缓冲区和存储数据不同，说明数据发生了改变
//			{
//				Paramt_BUFF[Pg_StaCardNum1][64]=Paramt_BUFF[CurrentScreenId][64];						//各参数缓冲区重新赋值
//				Paramt_BUFF[Pg_StaCardNum2][65]=Paramt_BUFF[CurrentScreenId][64];
//				Paramt_BUFF[Pg_StaCardNum3][65]=Paramt_BUFF[CurrentScreenId][64];
//				Paramt_BUFF[Pg_StaCardNum4][65]=Paramt_BUFF[CurrentScreenId][64];
//				Paramt_BUFF[Pg_StaCardNum5][64]=Paramt_BUFF[CurrentScreenId][64];				
//			}				
//		}break;
//		default:break;
//	}
//}
