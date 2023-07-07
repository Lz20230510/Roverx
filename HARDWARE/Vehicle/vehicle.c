#include "vehicle.h" 
#include "HEAD.h"
#include "math.h"

float VehicleSpd;
VehicleParDef VehiclePar;



	float Wheel_spacing=0.327;
//	static VehicleParDef vehiclePar;	
	float R,spdTemp1;

//	float angleH; 		//角度弧度
//	float limit=25; 			//质心点角度限位
//	float tempAngle;	//临时
//	float temp,Length1,Length2,LengthL,LengthR,LengthM,angle2,tan1;				//临时


/**************************************************************************
Function: Floating-point data calculates the absolute value
Input   : float
Output  : The absolute value of the input number
函数功能：浮点型数据计算绝对值
入口参数：浮点数
返回  值：输入数的绝对值
**************************************************************************/
float float_abs(float insert)
{
	if(insert>=0) return insert;
	else return -insert;
}






/*********************行走（驱动）电机减速比计算给定m/s->r/min************************/
//函数名形式：float DriveMotor_Ratio(float R,float ratio)
//输入传参：float R：轮子直径，单位：米，float ratio：减速比  wheelSpd:车轮移动速度  m/s
//输出传参：
//返回值：比例系数    行走速度=电机转速*比例系数     
/************************************************************************/
float DriveMotor_Ratio(float R,float ratio)
{
	return 60.f*ratio/(float)Pi/R;
}




/***************************车轮速度控制*********************************/
//函数名形式：void Wheel_control(float left,float right,MotorMsgDef motorMsgIn)
//输入传参：float left:左轮速度     float right：右轮速度    motorWtMsgDef motorWtMsgIn：电机写状态结构体
//输出传参：
//返回值：
/************************************************************************/
void Wheel_control(int left,int right,MotorMsgDef motorMsgIn)
{
	//左侧电机	
	if(left>0)
	{
		motorMsgIn[0].WtSpd=(u16)left;
		motorMsgIn[0].WtStatus=0x0001;
	}
	else if(left<0)
	{
		motorMsgIn[0].WtSpd=(u16)(-left);
		motorMsgIn[0].WtStatus=0x0002;
	}
	else
	{
		motorMsgIn[0].WtSpd=0;
		motorMsgIn[0].WtStatus=0x0005;
	}
	
	//右侧电机	
	if(right>0)
	{
		motorMsgIn[1].WtSpd=(u16)right;	
		motorMsgIn[1].WtStatus=0x0001;
	}
	else if(right<0)
	{
		motorMsgIn[1].WtSpd=(u16)(-right);
		motorMsgIn[1].WtStatus=0x0002;
	}
	else
	{
		motorMsgIn[1].WtSpd=0;
		motorMsgIn[1].WtStatus=0x05;
	}
}



/******************************小车速度控制**********************************/
//函数名形式：void Vehicle_control(float left,float right,MotorMsgDef motorMsgIn)
//输入传参：float spd:小车质心线性速度 m/s   float angle:转向轮角度  motorWtMsgDef motorWtMsgIn：电机写状态结构体
//输出传参：
//返回值：
/**************************************************************************/
void Vehicle_control(float spd,float angle,MotorMsgDef * motorMsgIn)
{
	
	





}


/******************************小车模型计算*********************************/
//函数名形式：VehicleParDef Vehicle_count(float spd,float angle)
//输入传参：float spd:小车质心线性速度 m/s   float angle:质心点角度 度格式  motorWtMsgDef motorWtMsgIn：电机写状态结构体
//输出传参：
//返回值：
/**************************************************************************/
/*********************************质心点在小车轴对称中心*****************************/
VehicleParDef Vehicle_count(float spd,float angle)
{
	float angleH; 		//角度弧度
	float limit=25; 			//质心点角度限位
	float tempAngle;	//临时
	float temp,Length1,Length2,LengthL,LengthR,LengthM,angle2,tan1;				//临时
	static VehicleParDef vehiclePar;
	float lengthL,lengthM,lengthR;
	
	vehiclePar.Ratio=MotorMsg[0].Ratio;						//减速比赋值
	tempAngle=angle;
	if(tempAngle<90-limit) tempAngle=90-limit;		//角度限位
	if(tempAngle>90+limit) tempAngle=90+limit;		//角度限位

	if(tempAngle<89)		
	{
		temp=tempAngle*(float)Pi/180.f;									//转换为弧度
		Length1=tan(temp)*((float)Vehicle_L/2.f);			//求后轮中心点到瞬心x方向距离
		LengthL=Length1+(float)Vehicle_W/2.f;							//左后轮转弯半径
		LengthR=Length1-(float)Vehicle_W/2.f;							//右后轮转弯半径	
		LengthM=Length1/sin(temp);									//求质心点转弯半径
		tan1=	Length1/(float)Vehicle_L;									//求前面中心点的正切值；		
		vehiclePar.AngleDu=atan(tan1)*180.f/(float)Pi;		//求前面中心点的角度；
		vehiclePar.SpdLBms=spd*LengthL/LengthM;		//求左后轮线速度
		vehiclePar.SpdRBms=spd*LengthR/LengthM;		//求右后轮线速度

	}
	else if(tempAngle>91)
	{
		temp=tempAngle*(float)Pi/180.f;									//转换为弧度
		Length1=tan((float)Pi-temp)*((float)Vehicle_L/2.f);	//求后轮中心点到瞬心x方向距离
		LengthL=Length1-(float)Vehicle_W/2.f;							//左后轮转弯半径
		LengthR=Length1+(float)Vehicle_W/2.f;							//右后轮转弯半径	
		LengthM=Length1/sin((float)Pi-temp);							//求质心点转弯半径
		tan1=	Length1/(float)Vehicle_L;									//求前面中心点的正切值；	
		vehiclePar.AngleDu=((float)Pi-atan(tan1))*180.f/(float)Pi;		//求前面中心点的角度；
		vehiclePar.SpdLBms=spd*LengthL/LengthM;		//求左后轮线速度
		vehiclePar.SpdRBms=spd*LengthR/LengthM;		//求右后轮线速度

	}
	else					//介于89°到91°算垂直角度
	{
		vehiclePar.AngleDu=90.f;		//求前面中心点的角度；
		vehiclePar.SpdLBms=spd;		//求左后轮线速度
		vehiclePar.SpdRBms=spd;		//求右后轮线速度
	}
	vehiclePar.SpdLBrmp=(int)(vehiclePar.SpdLBms*vehiclePar.Ratio);			//转换成  rmp
	vehiclePar.SpdRBrmp=(int)(vehiclePar.SpdRBms*vehiclePar.Ratio);
	
	vehiclePar.PwmWheel=Direction_Control(1,vehiclePar.AngleDu,15);
	return vehiclePar;
}




/*********************************质心点在两轮之间*****************************/

VehicleParDef Vehicle_count_Usart(float spd,float angleSpd)
{
//	float Wheel_spacing=0.327;
	static VehicleParDef vehiclePar;	
//	vehiclePar.Ratio=MotorMsg[0].Ratio;						//减速比赋值
//	float R,spdTemp;
	 	vehiclePar.Ratio=MotorMsg[0].Ratio;						//减速比赋值
		spdTemp1=angleSpd;
		if(angleSpd!=0 && spd!=0)
	{
		//If the target speed requires a turn radius less than the minimum turn radius
		//如果目标速度要求的转弯半径小于最小转弯半径
		if(float_abs(spd/angleSpd)<=Min_Turn_Radius)
		{
			//Reduce the target angular velocity and increase the turning radius to the minimum turning radius in conjunction with the forward speed
			//降低目标角速度，配合前进速度，提高转弯半径到最小转弯半径
			if(angleSpd>0)    //左转
				spdTemp1= float_abs(spd)/(Min_Turn_Radius);
			else							//右转
				spdTemp1=-float_abs(spd)/(Min_Turn_Radius);		
		}	
		
		R=spd/spdTemp1;
//		//AngleL=atan(Axle_spacing/(R-0.5f*Wheel_spacing));
//		AngleR=atan(Axle_spacing/(R+0.5f*Wheel_spacing));
		if(R>=0)				//左转
		{
			vehiclePar.AngleDu=atan(Vehicle_L/R)+pi/2;			//前中心点打角
			vehiclePar.SpdLBms=spd*(R-Vehicle_W/2)/R;				//左轮速度
			vehiclePar.SpdRBms=spd*(R+Vehicle_W/2)/R;				//右轮速度
		}
		else						//右转
		{
			
			vehiclePar.AngleDu=atan(float_abs(R)/Vehicle_L);			//前中心点打角
			vehiclePar.SpdLBms=spd*(float_abs(R)+Vehicle_W/2)/float_abs(R);				//左轮速度
			vehiclePar.SpdRBms=spd*(float_abs(R)-Vehicle_W/2)/float_abs(R);				//右轮速度	
		}
		
		
	}
	else						//当数据有0时候
	{
		if(angleSpd==0 && spd!=0)					//线速度不为0，角速度为0
		{
			vehiclePar.AngleDu=pi/2;
			vehiclePar.SpdLBms=spd;
			vehiclePar.SpdRBms=spd;
		}
		else															//数据都为0
		{
			vehiclePar.AngleDu=pi/2;
			vehiclePar.SpdLBms=0;
			vehiclePar.SpdRBms=0;	
		}
	}
	
	vehiclePar.SpdLBrmp=(int)(vehiclePar.SpdLBms*vehiclePar.Ratio);			//转换成  rmp
	vehiclePar.SpdRBrmp=(int)(vehiclePar.SpdRBms*vehiclePar.Ratio);	
	vehiclePar.PwmWheel=Direction_Control(1,vehiclePar.AngleDu*180/pi,15);   //舵机PWM输出
	return vehiclePar;
}






/**************************************************************************
Function: After the top 8 and low 8 figures are integrated into a short type data, the unit reduction is converted
Input   : 8 bits high, 8 bits low
Output  : The target velocity of the robot on the X/Y/Z axis
函数功能：将上位机发过来的高8位和低8位数据整合成一个short型数据后，再做单位还原换算
入口参数：高8位，低8位
返回  值：机器人X/Y/Z轴的目标速度
**************************************************************************/
float XYZ_Target_Speed_transition(u8 High,u8 Low)
{
	//Data conversion intermediate variable
	//数据转换的中间变量
	short transition; 
	
	//将高8位和低8位整合成一个16位的short型数据
	//The high 8 and low 8 bits are integrated into a 16-bit short data
	transition=((High<<8)+Low); 
	return 
		transition/1000+(transition%1000)*0.001; //Unit conversion, mm/s->m/s //单位转换, mm/s->m/s	
}




