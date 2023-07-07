#ifndef __MOTOR_H_
#define __MOTOR_H_
#include  "sys.h"
#include  "HEAD.h"



//电机控制相关写数据结构体
//当为两组电机		0:左电机   1：右电机
//当为四组电机   0:左前电机   1：右前电机   2：左后电机   3：右后电机
//当为八组电机   0:左前转向电机   1：右前转向电机   2：左后转向电机   3：右后转向电机   4:左前行走电机   5：右前行走电机   6：左后行走电机   7：右后行走电机
typedef struct 
{
	u16 WtStatus;			//电机写控制字
	int WtSpd;				//写速度值				r/min
	u16 WtFAcc;				//写加速度值			r/s^2
	u16 WtRAcc;				//写减速度值     r/s^2
	u16 RdStatus;			//读取电机状态字     
	int RdSpd;				//读取电机实时速度		r/min
	float RdCurrent;		//读取电机实时电流   A
	float RdSpdTrans;				//读取电机实时速度转换值		m/s
	
	float Ratio;				//电机减速比计算
	u8 MotorReady;		//电机就绪信号
	u8 MotorErr;			//电机故障信号
	u8 MotorEn;       //电机使能状态
	
}MotorMsgDef[MotorCount];
extern MotorMsgDef MotorMsg;				//定义控制相关数据结构体


void MotorValue_Read(MotorMsgDef motorMsgIn,u8 * MotorReadyIO,u8 * En);


#endif		    

