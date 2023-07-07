#ifndef __EAST_NOR_H
#define __EAST_NOR_H	
#include "sys.h" 
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//数据转换到东北坐标系下并进行转换
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

#define a 6378136.49 //地球长半轴，  单位：米
#define b 6356755.00 //地球短半轴，  单位：米
#define pi 3.1415926 //圆周率
#define thresh_posi  65.0  //处于范围的阀值  标定点与原点O之间算出的标定值为64.4691/1 m


typedef struct
{
	double logti_O_du;				//原点经度
	double lati_O_du;					//原点纬度	
	double logti_O_dufen;			//原点经度
	double lati_O_dufen;			//原点纬度	
	double lati_xO;						//原点经度x方向坐标
	double lati_yO;						//原点经度y方向坐标
	u8 StarNum;								//星数量
	u16 totalPotNum;					//总点数
	u16 linePotNum;						//每个线段上点的数量
	float spdTemp;						//自动运行速度临时存储
}LontLatiDef;
extern LontLatiDef LontLati;




double abs_(double in);//  求双精度绝对值
float four_quadrant(double first_x,double first_y,double last_x,double last_y);  //拟合图形完成后对于单根直线的指向角方向的判断
float JY901_eastnor(float JY901_angle,float error);		//将JY901输出角度值转换为统一的东北坐标系下的一圈360°的格式
void longti_lati_trans(u8 mode,double longti,double lati,double x0_y0[2],double lotlat_du[2]);
void OAOB_caculate(double x0,double y0,double x,double y,double longti_O,double longti_xy,double OAOB[2]);
//void Course_angle(double * OB,double * OA,float * cour_angl);
u8 Wakeup_position(double lati_OA,double longti_OB ,float cour_angle[2],u16 near_pot[2]);
u8 direct_judge(double lati_y,double longti_x ,u16 neat_pot);
float Scan_nearpot(double lati_y,double longti_x ,u16 near_pot[2],u8 num);
float course_weighting(u16 near_pot[2],float d_diviation);
u8 veichel_contr(float rencent_angle,float taget_angle);
float angle_deviat_judge(float rencent_angle,float taget_angle);
void longtiLati_xy(double lati,double * lati_xy);
LontLatiDef Origin_O_parmt_Init(void);
MapPotDef MapPot_Find(u16 potNum);
MapPotDef AGV_OutSideValue(nmea_msg gpsx,float Scm345,LontLatiDef lontLati_O);
#endif
