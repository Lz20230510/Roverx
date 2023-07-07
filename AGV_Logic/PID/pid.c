#include "HEAD.h"
/*---------------------------------------------------
  * 函 数 名：float PID_Paro_Init(PIDtypedef*PP)
  * 作    用：PID初始化
  * 输入参数：
  * 输    出：
  * 调用范围：
  * 注意事项：
  ---------------------------------------------------*/	
float PID_Paro_Init(PIDtypedef*PP)
{
	memset(&PP,0,sizeof(PP));
	return 0;
}


/*---------------------------------------------------
  * 函 数 名：void PID_Init(float K,float I,float D,float taget,PIDtypedef PP)
  * 作    用：PID参数初始化
  * 输入参数：
  * 输    出：
  * 调用范围：
  * 注意事项：
  ---------------------------------------------------*/	
void PID_Init(float K,float I,float D,float taget,PIDtypedef PP)
{
	
	PP.Proportion=K;     //比例系数
	PP.Integral=I;       //积分系数
	PP.Derivative=D;     //微分系数
	PP.SetPoint=taget;      //设定目标	
}

/***********************PID控制************************/
//	float PID_Calcul(PID * PP,float NextPoint,u8 num)   
//  PID * PP PID的结构体 ，float NextPoint下一采样节点数据输入    u8 num:使用PID的设备号
//	u8 set PID输出值初始化 0：初始化为零     !0：不做初始化
//
//
/******************************************************/
float PID_Calcul(PIDtypedef*PP,float NextPoint,u8 num)
{
	float dError,Error;
	u8 mark;
		
	Error=PP->SetPoint-NextPoint;             //求偏差值
	PP->SumError+=Error;											//积分
	dError=PP->LastError-PP->PrevError;      //当前微分
	PP->PrevError=PP->LastError; 							//把上次误差赋给上上次误差
	PP->LastError=Error;                      //当前误差赋给上一次误差
	
	mark=PID_Righ_Left(NextPoint,num);
	
	if(NextPoint==0||mark==1) 													//当磁导航传感器没有接受信号时，PID复位为初始状态
	{
		Error=0;
		dError=0;
		PP->PrevError=0;
		PP->SumError=0;
	}

	return 
		(PP->Proportion*Error                    //比例环节
		+PP->Integral*PP->SumError              //积分环节
		+PP->Derivative*dError)/1;                //微分环节		
}


/***********************PID函数执行?**********************/
//	PID_Go(PID * PP,int magnetic_in,u8 num)
//  int magnetic_in  磁导航传感器信号输入
//  PID * PP   PID结构体指针			u8 num：使用PID的设备号
//u8 set PID输出值初始化 1：初始化为零     0：不做初始化
/******************************************************/
float PID_RUN(PIDtypedef * PP,float magnetic_in,u8 num)
{
	float DAout;
  DAout=PID_Calcul(PP,magnetic_in,num);
	return DAout;
}

/***********对磁导航传感器左右偏执情况进行PID复位处理*******************/
//说明:当PID接收的磁导航信号在正负切换时，对PID进行复位处理，消除当左偏执或者
//或者右偏执瞬间PID无法快速响应的问题
//
//PID_Righ_Left(float next_point)   
//float next_point:PID的输入量  u8 num:使用PID的设备号
/********************************************************/
u8 PID_Righ_Left(float next_point,u8 num)
{	
	u8 pre,next;
	static float Last_Point_Stor[2]={0,0};
	if(Last_Point_Stor[num]!=0)						
	{
		if(Last_Point_Stor[num]>0) pre=1; 				//当上一次的输入量大于0时，标记点为1
		else pre=-1;												//否则为-1
	}
	
	if(next_point!=0)											
	{
		if(next_point>0) next=1; 						//当前的输入量大于0时，标记点为1
		else next=-1;												//否则为0
	}
	Last_Point_Stor[num]=next_point;           //将数据进行存储
	if(pre!=next) return 1;								//将采样信号的标记点进行比对，不同返回1
	else return 0;												//相同返回0
}





///************************磁导航传感器对驱动机构的左右轮速度差EV的计算*********************/
//void EV_calcu(PID * pp_front,PID * pp_behind,float Ea_Ed[4],float EV[2],float * mag_date)
//输入数据：PID * pp_front：前驱动PID参数  PID * pp_behind：前驱动PID参数  
//float Ea_Ed[4]:前后驱动机构角度偏差量与横向偏移存储
// float * mag_date：读取到的磁导航数据 
//输出数据：float EV[2]：前后驱动速度偏差值存储  EV[0]：前  EV[1]：后   
//Ea_Ed[0]:前驱动输出的磁导航偏差角    Ea_Ed[1]:前驱动输出的横向偏移量
//Ea_Ed[2]:后驱动输出的磁导航偏差角    Ea_Ed[3]:后驱动输出的横向偏移量
//u8 Direction:小车前进或后退  1：前进，2：后退
/**************************************************************/
float EV_calcu(PIDtypedef * pp_front,float  mag_date,u16 spd)
{	
		static float angle;
		angle=PID_RUN(pp_front,mag_date,1);
	
		return angle;	
}



///**************************************************************/
////void AGV_pid_contral(u16 speed,float EV)
////说明：
////输入信号：u8 speed：给定小车的基本轮子转速
////输出信号：float EV：电机转速差
////
///**************************************************************/
//void AGV_pid_contral(u16 speed,float EV[2])
//{
//	u8 F_K=1;		//设置关系系数	
//	u16 temp_EV[2];
//	if(EV[0]>=0)
//	{
//		temp_EV[0]=(u16)EV[0];
//		Speed_contr(speed+temp_EV[0]/F_K,speed+temp_EV[0]/F_K,speed-temp_EV[0]/F_K,speed-temp_EV[0]/F_K,1);
//	}
//	else
//	{
//		temp_EV[0]=(u16)-(EV[0]);
//		Speed_contr(speed-temp_EV[0]/F_K,speed-temp_EV[0]/F_K,speed+temp_EV[0]/F_K,speed+temp_EV[0]/F_K,1);
//	}		

////	if(EV[1]>=0)
////	{
////		temp_EV[1]=(unsigned short)EV[1];
//////		RLMotorSpeed(speed+temp_EV[1]/F_K);	//后左
//////		RRMotorSpeed(speed-temp_EV[1]/F_K);	//后右	
////	}
////	else
////	{
////		temp_EV[1]=(unsigned short)(-EV[1]);
//////		RLMotorSpeed(speed-temp_EV[1]/F_K);	//后左
//////		RRMotorSpeed(speed+temp_EV[1]/F_K);	//后右	
////	}	

//}





///*********************************PID参数强制复位*****************************/
////void PID_ParometerRest(PIDtypedef pidType,float evTn[2])
////输入参数：PIDtypedef front:前驱动PID      PIDtypedef behind:后驱动PID   ，evIn：EV
////输出参数
///***************************************************************************/

//void PID_ParometerRest(PIDtypedef front,PIDtypedef behind,float evIn[2])
//{
//	front.PrevError=0;
//	front.SumError=0;
//	front.LastError=0;			
//	behind.PrevError=0;
//	behind.SumError=0;
//	behind.LastError=0;
//						
//	evIn[0]=0;
//	evIn[1]=0;						
//}
//									