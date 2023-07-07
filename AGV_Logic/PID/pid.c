#include "HEAD.h"
/*---------------------------------------------------
  * �� �� ����float PID_Paro_Init(PIDtypedef*PP)
  * ��    �ã�PID��ʼ��
  * ���������
  * ��    ����
  * ���÷�Χ��
  * ע�����
  ---------------------------------------------------*/	
float PID_Paro_Init(PIDtypedef*PP)
{
	memset(&PP,0,sizeof(PP));
	return 0;
}


/*---------------------------------------------------
  * �� �� ����void PID_Init(float K,float I,float D,float taget,PIDtypedef PP)
  * ��    �ã�PID������ʼ��
  * ���������
  * ��    ����
  * ���÷�Χ��
  * ע�����
  ---------------------------------------------------*/	
void PID_Init(float K,float I,float D,float taget,PIDtypedef PP)
{
	
	PP.Proportion=K;     //����ϵ��
	PP.Integral=I;       //����ϵ��
	PP.Derivative=D;     //΢��ϵ��
	PP.SetPoint=taget;      //�趨Ŀ��	
}

/***********************PID����************************/
//	float PID_Calcul(PID * PP,float NextPoint,u8 num)   
//  PID * PP PID�Ľṹ�� ��float NextPoint��һ�����ڵ���������    u8 num:ʹ��PID���豸��
//	u8 set PID���ֵ��ʼ�� 0����ʼ��Ϊ��     !0��������ʼ��
//
//
/******************************************************/
float PID_Calcul(PIDtypedef*PP,float NextPoint,u8 num)
{
	float dError,Error;
	u8 mark;
		
	Error=PP->SetPoint-NextPoint;             //��ƫ��ֵ
	PP->SumError+=Error;											//����
	dError=PP->LastError-PP->PrevError;      //��ǰ΢��
	PP->PrevError=PP->LastError; 							//���ϴ��������ϴ����
	PP->LastError=Error;                      //��ǰ������һ�����
	
	mark=PID_Righ_Left(NextPoint,num);
	
	if(NextPoint==0||mark==1) 													//���ŵ���������û�н����ź�ʱ��PID��λΪ��ʼ״̬
	{
		Error=0;
		dError=0;
		PP->PrevError=0;
		PP->SumError=0;
	}

	return 
		(PP->Proportion*Error                    //��������
		+PP->Integral*PP->SumError              //���ֻ���
		+PP->Derivative*dError)/1;                //΢�ֻ���		
}


/***********************PID����ִ��?**********************/
//	PID_Go(PID * PP,int magnetic_in,u8 num)
//  int magnetic_in  �ŵ����������ź�����
//  PID * PP   PID�ṹ��ָ��			u8 num��ʹ��PID���豸��
//u8 set PID���ֵ��ʼ�� 1����ʼ��Ϊ��     0��������ʼ��
/******************************************************/
float PID_RUN(PIDtypedef * PP,float magnetic_in,u8 num)
{
	float DAout;
  DAout=PID_Calcul(PP,magnetic_in,num);
	return DAout;
}

/***********�Դŵ�������������ƫִ�������PID��λ����*******************/
//˵��:��PID���յĴŵ����ź��������л�ʱ����PID���и�λ������������ƫִ����
//������ƫִ˲��PID�޷�������Ӧ������
//
//PID_Righ_Left(float next_point)   
//float next_point:PID��������  u8 num:ʹ��PID���豸��
/********************************************************/
u8 PID_Righ_Left(float next_point,u8 num)
{	
	u8 pre,next;
	static float Last_Point_Stor[2]={0,0};
	if(Last_Point_Stor[num]!=0)						
	{
		if(Last_Point_Stor[num]>0) pre=1; 				//����һ�ε�����������0ʱ����ǵ�Ϊ1
		else pre=-1;												//����Ϊ-1
	}
	
	if(next_point!=0)											
	{
		if(next_point>0) next=1; 						//��ǰ������������0ʱ����ǵ�Ϊ1
		else next=-1;												//����Ϊ0
	}
	Last_Point_Stor[num]=next_point;           //�����ݽ��д洢
	if(pre!=next) return 1;								//�������źŵı�ǵ���бȶԣ���ͬ����1
	else return 0;												//��ͬ����0
}





///************************�ŵ����������������������������ٶȲ�EV�ļ���*********************/
//void EV_calcu(PID * pp_front,PID * pp_behind,float Ea_Ed[4],float EV[2],float * mag_date)
//�������ݣ�PID * pp_front��ǰ����PID����  PID * pp_behind��ǰ����PID����  
//float Ea_Ed[4]:ǰ�����������Ƕ�ƫ���������ƫ�ƴ洢
// float * mag_date����ȡ���Ĵŵ������� 
//������ݣ�float EV[2]��ǰ�������ٶ�ƫ��ֵ�洢  EV[0]��ǰ  EV[1]����   
//Ea_Ed[0]:ǰ��������Ĵŵ���ƫ���    Ea_Ed[1]:ǰ��������ĺ���ƫ����
//Ea_Ed[2]:����������Ĵŵ���ƫ���    Ea_Ed[3]:����������ĺ���ƫ����
//u8 Direction:С��ǰ�������  1��ǰ����2������
/**************************************************************/
float EV_calcu(PIDtypedef * pp_front,float  mag_date,u16 spd)
{	
		static float angle;
		angle=PID_RUN(pp_front,mag_date,1);
	
		return angle;	
}



///**************************************************************/
////void AGV_pid_contral(u16 speed,float EV)
////˵����
////�����źţ�u8 speed������С���Ļ�������ת��
////����źţ�float EV�����ת�ٲ�
////
///**************************************************************/
//void AGV_pid_contral(u16 speed,float EV[2])
//{
//	u8 F_K=1;		//���ù�ϵϵ��	
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
//////		RLMotorSpeed(speed+temp_EV[1]/F_K);	//����
//////		RRMotorSpeed(speed-temp_EV[1]/F_K);	//����	
////	}
////	else
////	{
////		temp_EV[1]=(unsigned short)(-EV[1]);
//////		RLMotorSpeed(speed-temp_EV[1]/F_K);	//����
//////		RRMotorSpeed(speed+temp_EV[1]/F_K);	//����	
////	}	

//}





///*********************************PID����ǿ�Ƹ�λ*****************************/
////void PID_ParometerRest(PIDtypedef pidType,float evTn[2])
////���������PIDtypedef front:ǰ����PID      PIDtypedef behind:������PID   ��evIn��EV
////�������
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