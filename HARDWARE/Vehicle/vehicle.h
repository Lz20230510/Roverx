#ifndef __VEHICLE_H
#define __VEHICLE_H
#include "sys.h"
#include "HEAD.h"
#include "Motor.h"
 
#define Vehicle_W	0.450						//左右轮中心距
#define Vehicle_L	0.327						//前后轮中心距	
#define Min_Turn_Radius 1.233			//转弯半径最小值  是当转动角度极限limit=15





typedef struct
{
//	float VehicleSpd;						//小车速度控制						m/s
	float SpdRBms;								//右后电机给定速度值			m/s
	float SpdLBms;								//左后电机给定速度值			m/s
	int SpdRBrmp;									//右后电机给定速度值			r/min
	int SpdLBrmp;									//左后电机给定速度值			r/min
	float AngleDu;								//前面中心点角度  角度值	
	float MAngleDu;								//主动轮中间点  角度值
	float Ratio;									//小车减速比	
	float ReadSpdRBms;						//读取小车当前的车轮线速度右侧
	float ReadSpdLBms;						//读取小车当前的车轮线速度左侧
	float ReadPointBms;						//读取质心点线速度
	float ReadPointAngleSpd;			//读取质心点角速度
	float GivePointAngleSpd;			//给定质心点角速度
	u32 PwmWheel;									//转向的PWM打角给定值
	float GiveAngle;							//给定的质心点角度值
}VehicleParDef;
extern VehicleParDef VehiclePar;
extern float VehicleSpd;
float float_abs(float insert);


float DriveMotor_Ratio(float R,float ratio);
void Wheel_control(int left,int right,MotorMsgDef motorMsgIn);
VehicleParDef Vehicle_count(float spd,float angle);
VehicleParDef Vehicle_count_Usart(float spd,float angleSpd);
float XYZ_Target_Speed_transition(u8 High,u8 Low);

#endif
