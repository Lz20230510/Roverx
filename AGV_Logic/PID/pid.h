#ifndef __PID_H__
#define __PID_H__
#include "sys.h"	 
#include "stdlib.h" 
#include "HEAD.h"



#define radio 0.5						//ƫת�ǵ���ƫ������ӵļ�Ȩϵ��

//PID�������ṹ��
typedef struct
{
	u16 AGPspd;				//AGV����Ѱ���ĳ�ʼ�ٶ�      r/min
	float Proportion;   //����
	float Integral;     //����    
	float Derivative;   //΢��
	float SetPoint;     //�趨Ŀ��	
	
	float LastError;     //Error[-1]  �ϴ����
	float PrevError;     //Error[-2]���ϴ����
	float SumError;      //ƫ���ۼ�
}PIDtypedef;
extern PIDtypedef front_pid,rear_pid;
extern float Last_Point_Stor[2];									//PID ���µ�洢
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
