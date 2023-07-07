/******************************************************************************
* 文件名：Motor.c
* 版  本：
* 作  者：
* 日  期：
* 库版本：   
* 文件描述：电机控制相关程序
*******************************************************************************/
#include "HEAD.h"
#include "Motor.h"




/*************************当前的电机实时数据读写（原始数据）****************************/
//函数名形式：void MotorValue_Read(MotorMsgDef motorMsgIn,u8 * MotorReadyIO,u8 En)
//输入传参：MotorWtMsgDef motorWtMsgIn：电机写状态结构体，u8 * MotorReadyIO：电机就绪信号 u8 * En：电机使能信号
//输出传参：
//返回值：MotorMsgDef  电机读状态结构体
/************************************************************************/
void MotorValue_Read(MotorMsgDef motorMsgIn,u8 * MotorReadyIO,u8 * En)
{	
//	float k;			//比例系数
//	k=DriveMotor_Ratio(0.18,30.0);    //减速比计算
	int SignL,SignR;
	
	if(AGVStatus.EmerStop == 1)
	{
		
		motorMsgIn[0].WtStatus = 5;
		motorMsgIn[0].WtSpd = 0;
		
		WtStatus_1=5;									//写左轮控制字
		WtSpd_1=0;
		
		motorMsgIn[1].WtStatus = 5;
		motorMsgIn[1].WtSpd = 0;
		
		WtStatus_2=5;									//写左轮控制字
		WtSpd_2=0;
	}
	else if(MotorCount==2 &&MotorMode==0)										//两轮差速车
	{	
		//左侧电机	
		if(RdStatus_1==0x01) SignL=-1;		
		else SignL=1;
		motorMsgIn[0].RdStatus=RdStatus_1;									//左轮状态读取
		motorMsgIn[0].RdSpd=SignL*(int)RdSpd_1;												//左轮速度读取    r/mim
		motorMsgIn[0].RdSpdTrans=motorMsgIn[0].RdSpd/MotorMsg[0].Ratio;			//电机实时速度    m/s		
		motorMsgIn[0].RdCurrent=(float)RdCurrent_1/100;			//左轮电流读取		A	
		motorMsgIn[0].MotorErr=RdAlarm_1;										//获取报警数据
		WtStatus_2=motorMsgIn[0].WtStatus;									//写左轮控制字
		WtSpd_2=motorMsgIn[0].WtSpd/2;												//写左轮速度      r/mim
		
		//右侧电机	
		if(RdStatus_2==0x02) SignR=-1;		
		else SignR=1;
		motorMsgIn[1].RdStatus=RdStatus_2;									//右轮状态读取
		motorMsgIn[1].RdSpd=SignR*RdSpd_2;												//右轮速度读取    r/mim
		motorMsgIn[1].RdSpdTrans=motorMsgIn[1].RdSpd/MotorMsg[0].Ratio;			//电机实时速度    m/s
		motorMsgIn[1].RdCurrent=(float)RdCurrent_2/100;			//右轮电流读取		A
		motorMsgIn[1].MotorErr=RdAlarm_2;										//获取报警数据
		WtStatus_1=motorMsgIn[1].WtStatus;									//写右轮控制字
		WtSpd_1=motorMsgIn[1].WtSpd/2;												//写右轮速度   		r/mim
	}
}




