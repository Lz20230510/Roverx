#ifndef __MAGNETIC_H_
#define __MAGNETIC_H_


#include "sys.h"
#include "HEAD.h"


#define wheel_space  220        //驱动机构车轮间距  	单位 mm
#define wheel_R		152						//车轮直径						单位 mm		
#define mag_length 160          //磁导航传感器长度		单位 mm
#define	mag_d		300							//驱动机构前后磁导航传感器间距  单位 mm
#define	thresh_Ea	5							//偏转角的死区阈值			单位 mm
#define thresh_Ed 10						//横向偏移量的死区阈值	单位 mm
//#define pi 3.14159							//圆周率

typedef struct									//磁导航原始数据结构体
{
	u16 front1;					//前1磁导航
	u16 front2;					//前2磁导航
	u16 rear1;					//后1磁导航
	u16 rear2;					//后2磁导航
}InitMagDateDef;
extern InitMagDateDef InitMagDate;

typedef struct									//磁导航数据存储
{
	u8 front1[8];
	u8 front2[8];
	u8 rear1[8];
	u8 rear2[8];
	int value[2];    //[0]磁道航值  [1]是否读到词条  num>0 表示有磁条
}MagneticDef;
extern MagneticDef Magnetic;

extern float Mag_date[12];					//四组磁导航传感器分别读取到的数值存储
extern u8 Mag_alarm[4];								//四组磁导航传感器是否有数据报警	

extern u8 mag1[8];				//读取1号磁导航传感器数据
extern u8 mag2[8];				//读取2号磁导航传感器数据
extern u8 mag3[8];				//读取3号磁导航传感器数据
extern u8 mag4[8];				//读取4号磁导航传感器数据


//初始化
float deci_hex(u8 * magnetic_in);			
float deci_hex_single(u8 date_in);
void Devia_Calcul(int SW_STOR[16],int Sum[2]);
u16 magbetic_tran(u8 * magnetic_in,float mag_date[4],u8 mag_alarm[4]);
InitMagDateDef magnetic_recei(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4]);
u16 mag_calcul(u8 * magnetic_in,float * Sum);
void Ea_Ed_calcu(float * mag_date,float Ea_Ed[4]);
void MNSV_X8IO_Read(int * temp);

#endif

