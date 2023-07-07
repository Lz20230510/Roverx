#include "vehicle.h" 
#include "HEAD.h"
#include "math.h"

float VehicleSpd;
VehicleParDef VehiclePar;



	float Wheel_spacing=0.327;
//	static VehicleParDef vehiclePar;	
	float R,spdTemp1;

//	float angleH; 		//�ǶȻ���
//	float limit=25; 			//���ĵ�Ƕ���λ
//	float tempAngle;	//��ʱ
//	float temp,Length1,Length2,LengthL,LengthR,LengthM,angle2,tan1;				//��ʱ


/**************************************************************************
Function: Floating-point data calculates the absolute value
Input   : float
Output  : The absolute value of the input number
�������ܣ����������ݼ������ֵ
��ڲ�����������
����  ֵ���������ľ���ֵ
**************************************************************************/
float float_abs(float insert)
{
	if(insert>=0) return insert;
	else return -insert;
}






/*********************���ߣ�������������ٱȼ������m/s->r/min************************/
//��������ʽ��float DriveMotor_Ratio(float R,float ratio)
//���봫�Σ�float R������ֱ������λ���ף�float ratio�����ٱ�  wheelSpd:�����ƶ��ٶ�  m/s
//������Σ�
//����ֵ������ϵ��    �����ٶ�=���ת��*����ϵ��     
/************************************************************************/
float DriveMotor_Ratio(float R,float ratio)
{
	return 60.f*ratio/(float)Pi/R;
}




/***************************�����ٶȿ���*********************************/
//��������ʽ��void Wheel_control(float left,float right,MotorMsgDef motorMsgIn)
//���봫�Σ�float left:�����ٶ�     float right�������ٶ�    motorWtMsgDef motorWtMsgIn�����д״̬�ṹ��
//������Σ�
//����ֵ��
/************************************************************************/
void Wheel_control(int left,int right,MotorMsgDef motorMsgIn)
{
	//�����	
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
	
	//�Ҳ���	
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



/******************************С���ٶȿ���**********************************/
//��������ʽ��void Vehicle_control(float left,float right,MotorMsgDef motorMsgIn)
//���봫�Σ�float spd:С�����������ٶ� m/s   float angle:ת���ֽǶ�  motorWtMsgDef motorWtMsgIn�����д״̬�ṹ��
//������Σ�
//����ֵ��
/**************************************************************************/
void Vehicle_control(float spd,float angle,MotorMsgDef * motorMsgIn)
{
	
	





}


/******************************С��ģ�ͼ���*********************************/
//��������ʽ��VehicleParDef Vehicle_count(float spd,float angle)
//���봫�Σ�float spd:С�����������ٶ� m/s   float angle:���ĵ�Ƕ� �ȸ�ʽ  motorWtMsgDef motorWtMsgIn�����д״̬�ṹ��
//������Σ�
//����ֵ��
/**************************************************************************/
/*********************************���ĵ���С����Գ�����*****************************/
VehicleParDef Vehicle_count(float spd,float angle)
{
	float angleH; 		//�ǶȻ���
	float limit=25; 			//���ĵ�Ƕ���λ
	float tempAngle;	//��ʱ
	float temp,Length1,Length2,LengthL,LengthR,LengthM,angle2,tan1;				//��ʱ
	static VehicleParDef vehiclePar;
	float lengthL,lengthM,lengthR;
	
	vehiclePar.Ratio=MotorMsg[0].Ratio;						//���ٱȸ�ֵ
	tempAngle=angle;
	if(tempAngle<90-limit) tempAngle=90-limit;		//�Ƕ���λ
	if(tempAngle>90+limit) tempAngle=90+limit;		//�Ƕ���λ

	if(tempAngle<89)		
	{
		temp=tempAngle*(float)Pi/180.f;									//ת��Ϊ����
		Length1=tan(temp)*((float)Vehicle_L/2.f);			//��������ĵ㵽˲��x�������
		LengthL=Length1+(float)Vehicle_W/2.f;							//�����ת��뾶
		LengthR=Length1-(float)Vehicle_W/2.f;							//�Һ���ת��뾶	
		LengthM=Length1/sin(temp);									//�����ĵ�ת��뾶
		tan1=	Length1/(float)Vehicle_L;									//��ǰ�����ĵ������ֵ��		
		vehiclePar.AngleDu=atan(tan1)*180.f/(float)Pi;		//��ǰ�����ĵ�ĽǶȣ�
		vehiclePar.SpdLBms=spd*LengthL/LengthM;		//����������ٶ�
		vehiclePar.SpdRBms=spd*LengthR/LengthM;		//���Һ������ٶ�

	}
	else if(tempAngle>91)
	{
		temp=tempAngle*(float)Pi/180.f;									//ת��Ϊ����
		Length1=tan((float)Pi-temp)*((float)Vehicle_L/2.f);	//��������ĵ㵽˲��x�������
		LengthL=Length1-(float)Vehicle_W/2.f;							//�����ת��뾶
		LengthR=Length1+(float)Vehicle_W/2.f;							//�Һ���ת��뾶	
		LengthM=Length1/sin((float)Pi-temp);							//�����ĵ�ת��뾶
		tan1=	Length1/(float)Vehicle_L;									//��ǰ�����ĵ������ֵ��	
		vehiclePar.AngleDu=((float)Pi-atan(tan1))*180.f/(float)Pi;		//��ǰ�����ĵ�ĽǶȣ�
		vehiclePar.SpdLBms=spd*LengthL/LengthM;		//����������ٶ�
		vehiclePar.SpdRBms=spd*LengthR/LengthM;		//���Һ������ٶ�

	}
	else					//����89�㵽91���㴹ֱ�Ƕ�
	{
		vehiclePar.AngleDu=90.f;		//��ǰ�����ĵ�ĽǶȣ�
		vehiclePar.SpdLBms=spd;		//����������ٶ�
		vehiclePar.SpdRBms=spd;		//���Һ������ٶ�
	}
	vehiclePar.SpdLBrmp=(int)(vehiclePar.SpdLBms*vehiclePar.Ratio);			//ת����  rmp
	vehiclePar.SpdRBrmp=(int)(vehiclePar.SpdRBms*vehiclePar.Ratio);
	
	vehiclePar.PwmWheel=Direction_Control(1,vehiclePar.AngleDu,15);
	return vehiclePar;
}




/*********************************���ĵ�������֮��*****************************/

VehicleParDef Vehicle_count_Usart(float spd,float angleSpd)
{
//	float Wheel_spacing=0.327;
	static VehicleParDef vehiclePar;	
//	vehiclePar.Ratio=MotorMsg[0].Ratio;						//���ٱȸ�ֵ
//	float R,spdTemp;
	 	vehiclePar.Ratio=MotorMsg[0].Ratio;						//���ٱȸ�ֵ
		spdTemp1=angleSpd;
		if(angleSpd!=0 && spd!=0)
	{
		//If the target speed requires a turn radius less than the minimum turn radius
		//���Ŀ���ٶ�Ҫ���ת��뾶С����Сת��뾶
		if(float_abs(spd/angleSpd)<=Min_Turn_Radius)
		{
			//Reduce the target angular velocity and increase the turning radius to the minimum turning radius in conjunction with the forward speed
			//����Ŀ����ٶȣ����ǰ���ٶȣ����ת��뾶����Сת��뾶
			if(angleSpd>0)    //��ת
				spdTemp1= float_abs(spd)/(Min_Turn_Radius);
			else							//��ת
				spdTemp1=-float_abs(spd)/(Min_Turn_Radius);		
		}	
		
		R=spd/spdTemp1;
//		//AngleL=atan(Axle_spacing/(R-0.5f*Wheel_spacing));
//		AngleR=atan(Axle_spacing/(R+0.5f*Wheel_spacing));
		if(R>=0)				//��ת
		{
			vehiclePar.AngleDu=atan(Vehicle_L/R)+pi/2;			//ǰ���ĵ���
			vehiclePar.SpdLBms=spd*(R-Vehicle_W/2)/R;				//�����ٶ�
			vehiclePar.SpdRBms=spd*(R+Vehicle_W/2)/R;				//�����ٶ�
		}
		else						//��ת
		{
			
			vehiclePar.AngleDu=atan(float_abs(R)/Vehicle_L);			//ǰ���ĵ���
			vehiclePar.SpdLBms=spd*(float_abs(R)+Vehicle_W/2)/float_abs(R);				//�����ٶ�
			vehiclePar.SpdRBms=spd*(float_abs(R)-Vehicle_W/2)/float_abs(R);				//�����ٶ�	
		}
		
		
	}
	else						//��������0ʱ��
	{
		if(angleSpd==0 && spd!=0)					//���ٶȲ�Ϊ0�����ٶ�Ϊ0
		{
			vehiclePar.AngleDu=pi/2;
			vehiclePar.SpdLBms=spd;
			vehiclePar.SpdRBms=spd;
		}
		else															//���ݶ�Ϊ0
		{
			vehiclePar.AngleDu=pi/2;
			vehiclePar.SpdLBms=0;
			vehiclePar.SpdRBms=0;	
		}
	}
	
	vehiclePar.SpdLBrmp=(int)(vehiclePar.SpdLBms*vehiclePar.Ratio);			//ת����  rmp
	vehiclePar.SpdRBrmp=(int)(vehiclePar.SpdRBms*vehiclePar.Ratio);	
	vehiclePar.PwmWheel=Direction_Control(1,vehiclePar.AngleDu*180/pi,15);   //���PWM���
	return vehiclePar;
}






/**************************************************************************
Function: After the top 8 and low 8 figures are integrated into a short type data, the unit reduction is converted
Input   : 8 bits high, 8 bits low
Output  : The target velocity of the robot on the X/Y/Z axis
�������ܣ�����λ���������ĸ�8λ�͵�8λ�������ϳ�һ��short�����ݺ�������λ��ԭ����
��ڲ�������8λ����8λ
����  ֵ��������X/Y/Z���Ŀ���ٶ�
**************************************************************************/
float XYZ_Target_Speed_transition(u8 High,u8 Low)
{
	//Data conversion intermediate variable
	//����ת�����м����
	short transition; 
	
	//����8λ�͵�8λ���ϳ�һ��16λ��short������
	//The high 8 and low 8 bits are integrated into a 16-bit short data
	transition=((High<<8)+Low); 
	return 
		transition/1000+(transition%1000)*0.001; //Unit conversion, mm/s->m/s //��λת��, mm/s->m/s	
}




