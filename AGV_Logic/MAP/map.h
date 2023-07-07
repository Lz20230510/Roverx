#ifndef __MAP_H
#define __MAP_H	

#include "sys.h"

//////////////////////////////////////////////////////////
//
//
//存储地图的途径信息
//thresh_posi 为标定值
////////////////////////////////////////////////////////////
extern float thresh_posi;



////////////////地图信息结构体/////////////////////////////////
typedef struct
{
	float OA;						//				地图信息OA值
	float OB;						//				地图信息OB值
	float angle;				//				地图信息航向角值
	float trueAngle;		//实际航向角
	u8 		starNum;					//星数
	
	unsigned int numFx;    //当前直线线段的数量 
	unsigned int numPot;    //单个直线上的点的数量
	unsigned int grossNumPot;   //总得点的数量
	double longti_du;							//度格式下的经度值
	double lati_du;								//度格式下的维度值
	u16 errNum;                   //错误点数量      
	double longti_dufen;							//度格式下的经度值 度分格式
	double lati_dufen;								//度格式下的维度值	度分格式
	unsigned int  Num;						//				数据组号
	float AutoSpd;								//自动运行速度
	u8 mode;
}MapPotDef;
extern MapPotDef MapPot,MapPotDsiplay;
#endif
