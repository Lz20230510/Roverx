#ifndef __STATIONCARDNUM_H
#define __STATIONCARDNUM_H
#include "sys.h"


typedef struct						//站点卡号结构体
{
	u16 stationNum;					//站号
	u16 StationCardNumstor;		//站点卡号临时存储
	u16 cardNum;							//卡号
	u8 fucStor;						//功能存储，存储站点及卡号的统一功能，五个界面统一
	u8 error;								//输入的卡号没有对应的站号或者卡号异常   error=0x00,异常    error=0x01, 正常
}StationCardNumDef;

extern StationCardNumDef  StationCardNum;




void CardNum_StationNum(u16 CurrentScreenId);						//站点卡号5个界面的卡号及站号功能互相赋值
void Station_CardNum(u32 cardValue);							//获取的卡号的对应的站点号
#endif
