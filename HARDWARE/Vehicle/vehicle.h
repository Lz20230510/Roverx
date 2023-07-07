#ifndef __VEHICLE_H
#define __VEHICLE_H
#include "sys.h"
#include "HEAD.h"
#include "Motor.h"
 
#define Vehicle_W	0.450						//���������ľ�
#define Vehicle_L	0.327						//ǰ�������ľ�	
#define Min_Turn_Radius 1.233			//ת��뾶��Сֵ  �ǵ�ת���Ƕȼ���limit=15





typedef struct
{
//	float VehicleSpd;						//С���ٶȿ���						m/s
	float SpdRBms;								//�Һ��������ٶ�ֵ			m/s
	float SpdLBms;								//����������ٶ�ֵ			m/s
	int SpdRBrmp;									//�Һ��������ٶ�ֵ			r/min
	int SpdLBrmp;									//����������ٶ�ֵ			r/min
	float AngleDu;								//ǰ�����ĵ�Ƕ�  �Ƕ�ֵ	
	float MAngleDu;								//�������м��  �Ƕ�ֵ
	float Ratio;									//С�����ٱ�	
	float ReadSpdRBms;						//��ȡС����ǰ�ĳ������ٶ��Ҳ�
	float ReadSpdLBms;						//��ȡС����ǰ�ĳ������ٶ����
	float ReadPointBms;						//��ȡ���ĵ����ٶ�
	float ReadPointAngleSpd;			//��ȡ���ĵ���ٶ�
	float GivePointAngleSpd;			//�������ĵ���ٶ�
	u32 PwmWheel;									//ת���PWM��Ǹ���ֵ
	float GiveAngle;							//���������ĵ�Ƕ�ֵ
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
