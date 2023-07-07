#ifndef __PID_H__
#define __PID_H__
#include "sys.h"	 
#include "stdlib.h" 
#include "HEAD.h"



#define radio 0.5						//偏转角的与偏移量相加的加权系数

//PID各参数结构体
typedef struct
{
	u16 AGPspd;				//AGV室内寻迹的初始速度      r/min
	float Proportion;   //比例
	float Integral;     //积分    
	float Derivative;   //微分
	float SetPoint;     //设定目标	
	
	float LastError;     //Error[-1]  上次误差
	float PrevError;     //Error[-2]上上次误差
	float SumError;      //偏差累计
}PIDtypedef;
extern PIDtypedef front_pid,rear_pid;
extern float Last_Point_Stor[2];									//PID 最新点存储
u8 PID_Righ_Left(float next_point,u8 num);					//

float PID_Paro_Init(PIDtypedef* PP);
float PID_Calcul(PIDtypedef* PP,float NextPoint,u8 num);
float PID_RUN(PIDtypedef * PP,float magnetic_in,u8 num);
float EV_calcu(PIDtypedef * pp_front,float  mag_date,u16 spd);
//void AGV_pid_contral(u16 speed,float EV[2]);
//void PID_Init(float K,float I,float D,float taget,PIDtypedef PP);
//void EV_calcu(PIDtypedef* pp_front,PIDtypedef* pp_behind,float Ea_Ed[4],float EV[2],float * mag_date,u8 direction,u16 spd);
//void PID_ParometerRest(PIDtypedef front,PIDtypedef behind,float evIn[2]);



#endif
