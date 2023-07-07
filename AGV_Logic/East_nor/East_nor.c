#include "delay.h" 
#include "myiic.h" 
#include "math.h" 
#include "East_nor.h" 
#include "map.h" 
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//����ת������������ϵ�²�����ת��
//
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////

float deviat_value;       //Ŀ��Ƕ��뵱ǰС���Ƕȵ�ƫ��ֵ
	
	u16 mark_num=0;
	u16 mark_num_gross=0;
	u16 hhhh;
	u8 error=0;
/*********************************ʵ�ֺ���*************************************/
//����ԭ�ͣddouble abs_(double in);
//���ܣ���˫�������ݾ���ֵ
//����/�����   ���� ��Ҫת������
//���أ����ؾ���ֵ
/*******************************************************************************/
double abs_(double in)
{
	if(in>=0) return in;
	else return -in;
}

/*********************************ʵ�ֺ���*************************************/
//����ԭ�ͣ�double four_quadrant(double first,double last);
//���ܣ����ͼ����ɺ���ڵ���ֱ�ߵ�ָ��Ƿ�����ж�
//����/��������� �߶��ϵĵ�һ���㣬�߶��ϵĵڶ�����
//���أ����ؽǶ�
/*******************************************************************************/
float four_quadrant(double first_x,double first_y,double last_x,double last_y)
{

	double yl_yf,xl_xf,arcsin;
	if(last_y>=first_y&&last_x>=first_x)     //��һ����
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(xl_xf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//����ڵ�һ���޵ĽǶ�
		return 90.0-arcsin;																						//�����ڶ�������ϵ�µĽǶ�
	}
	else if(last_y>=first_y&&last_x<first_x)		//�ڶ�����
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(yl_yf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//����ڵڶ����޵ĽǶ�
		return 270.0+arcsin;																					//�����ڶ�������ϵ�µĽǶ�
	}
	else if(last_y<first_y&&last_x<first_x)			//��������
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(yl_yf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//����ڵ������޵ĽǶ�
		return 270.0-arcsin;																					//�����ڶ�������ϵ�µĽǶ�
	}
	else if(last_y<first_y&&last_x>=first_x)				//��������
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(yl_yf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//����ڵڶ����޵ĽǶ�
		return 90.0+arcsin;																					//�����ڶ�������ϵ�µĽǶ�
	}
	return arcsin;
}


/*********************************ʵ�ֺ���**********************************/
//����ԭ�ͣffloat JY901_eastnor(float JY901_angle);
//���ܣ���JY901����Ƕ�ֵת��Ϊͳһ�Ķ�������ϵ�µ�һȦ360��ĸ�ʽ
//����/��������� JY901_angle:JY901�ĵ���������,   error:�������Ĳ���ֵ��ʵ�ʶ�������ϵ�Ƕȵ�ƫ��ֵ
//���أ������������
/*******************************************************************************/
float JY901_eastnor(float JY901_angle,float error)
{
	if(JY901_angle<=0.0f) return (-JY901_angle+error);
	else return (360.0f-JY901_angle+error);
}

/*********************************ʵ�ֺ���**********************************/
//����ԭ�ͣfvoid longti_lati_trans(u8 mode,double longti,double lati,double x0_y0[2],double lotlat_angle[2]);
//
//���ܣ�����γ�����ݴӶȷָ�ʽ-->�ȸ�ʽ
//����/��������룺���ȣ�ά�����ݣ� �����x0_y0[0]  x0,x0_y0[0]  y0, lotlat_angle[0] ά�ȵĶȸ�ʽ��lotlat_angle[1]  ��ά�ȵĶȸ�ʽ
//u8 mode: mode=0 :�����ʽΪ�ȷָ�ʽ         mode=1�������ʽΪ�ȸ�ʽ
//���أ�void
/*******************************************************************************/
void longti_lati_trans(u8 mode,double longti,double lati,double x0_y0[2],double lotlat_du[2])
{
	double longti_H,longti_L,lati_H,lati_L;
	if(mode==0)
	{
			//�󾭶ȵĶȸ�ʽ
		longti_H=(long int)(longti/100.0);					//����������λ������ȡ��������
		longti_L=(longti/100.0-longti_H)*100.0/60.0;			//����ת��Ϊ��
		longti_H=longti_H+longti_L;          				//��������ȸ�ʽ����

		//��ά�ȵĶȸ�ʽ
		lati_H=(long int)(lati/100.0);					//����������λ������ȡ��������
		lati_L=(lati/100.0-lati_H)*100.0/60.0;			//����ת��Ϊ��
		lati_H=lati_H+lati_L;          				//��������ȸ�ʽ����	
		
		x0_y0[0]=a*a/sqrt(a*a+b*b*tan(lati_H*pi/180)*tan(lati_H*pi/180));		//������ȷ�����X0��ֵ
		x0_y0[1]=b*b/sqrt(b*b+a*a/tan(lati_H*pi/180)/tan(lati_H*pi/180));		//������ȷ�����y0��ֵ
	
		lotlat_du[1]=lati_H;							//ά�ȵĶȸ�ʽֵ
		lotlat_du[0]=longti_H;								//���ȵĶȸ�ʽֵ
	}
	else if(mode==1)
	{
		longti_H=longti;
		lati_H=lati;
	
		x0_y0[0]=a*a/sqrt(a*a+b*b*tan(lati_H*pi/180)*tan(lati_H*pi/180));		//������ȷ�����X0��ֵ
		x0_y0[1]=b*b/sqrt(b*b+a*a/tan(lati_H*pi/180)/tan(lati_H*pi/180));		//������ȷ�����y0��ֵ		
	}
}

/***********************Ѱ�ҵ��ӵ�ͼ�ϵ�ĳһ����**************************/
//������ʽ��MapPotDef MapPot_Find(u16 potNum)
//���������
//���������
//���ز�����MapPotDef
//
/****************************************************************/
MapPotDef MapPot_Find(u16 potNum)
{
	static MapPotDef potDef;
	
	potDef=Map_ReadFromFLASH(potNum+1,4);				//��ȡ�洢�ĵ�ͼ��Ϣ��
	
	return potDef;

}


/****************************************************************/
//������ʽ��void longtiLatiTrans_xy(double lati,double * lati_xy)
//���������double lati��γ��ֵ���ȸ�ʽ
//���������lat_xy[0]  ���ȷ���xֵ��lat_xy[0]����ֵ
//���ز�����
//
/****************************************************************/
void longtiLati_xy(double lati,double * lati_xy)
{
	*lati_xy=a*a/sqrt(a*a+b*b*tan(lati*pi/180)*tan(lati*pi/180));		//������ȷ�����X0��ֵ
	*(lati_xy+1)=b*b/sqrt(b*b+a*a/tan(lati*pi/180)/tan(lati*pi/180));		//������ȷ�����y0��ֵ
}


/*********************************ʵ�ֺ���**********************************/
//����ԭ�ͣfvoid OAOB_caculate(double x0,double y0,double x,double y,doubel lati_O,doubel lati_xy,double double OAOB[2]);
//���ܣ��ڶ�������ϵ�¼���ĳ��(x,y)�����ԭ��O(x0,y0)��OAOB����ֵ
//����/��������룺���� double x0,y0 ԭ��O�ľ��ȷ�����x,yֵ��double x,y  ĳһ�㣨x,y���ھ��ȷ����µ�x,yֵ��double lati_xy,lati_O�ֱ���ԭ���ĳһ��ľ���ֵ
//�����double OAOB[2]��OA  OBֵ
//���أ�void
/*******************************************************************************/
void OAOB_caculate(double x0,double y0,double x,double y,double longti_O,double longti_xy,double OAOB[2])
{

		//��OA
		if(y>=y0) OAOB[0]=sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0)); 
		else  OAOB[0]=-sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));
		
		//��OB
		OAOB[1]=x0*(longti_xy-longti_O)*pi/180;	//��OB
	
}


/*******************************ʵ�ֺ���*******************************/
//������ʽ��LontLatiDef Origin_O_parmt_Init(void);
//���������
//���������
//���ز�����
//����˵������ȡGPS���ӵ�ͼԭ�������Ϣ
//
/**********************************************************************/
LontLatiDef Origin_O_parmt_Init(void)
{
	static union  MultiType multiType;
	static LontLatiDef LontLati;
	double temp[2],temp1[2];
	
	multiType.u32Array_[0]=Paramt_BUFF[10][1];		
	multiType.u32Array_[1]=Paramt_BUFF[10][2];	
	LontLati.logti_O_dufen=multiType.double_;				//��ȡ���ȵĶȷָ�ʽ
	
	multiType.u32Array_[0]=Paramt_BUFF[10][3];		
	multiType.u32Array_[1]=Paramt_BUFF[10][4];	
	LontLati.lati_O_dufen=multiType.double_;				//��ȡγ�ȶȷָ�ʽ

	LontLati.totalPotNum=(u16)Paramt_BUFF[10][10];		//���ӵ�ͼ�ܵ�����
	LontLati.linePotNum=(u16)Paramt_BUFF[10][11];		//���ӵ�ͼ���߶��ϵ������
	
	multiType.u32_=Paramt_BUFF[10][12];				//�Զ�Ѱ���ٶ�
	LontLati.spdTemp=multiType.float_;				//�Զ�Ѱ���ٶ�
	
	longti_lati_trans(0,LontLati.logti_O_dufen,LontLati.lati_O_dufen,temp,temp1);

	longtiLati_xy(LontLati.lati_O_du,temp);					//������ȷ�����x.yֵ	
	LontLati.lati_xO=temp[0];						//���ȷ����x������
	LontLati.lati_yO=temp[1];						//���ȷ����y������
	
	lati_x0=LontLati.lati_xO;
	lati_y0=LontLati.lati_yO;
	
	
	
	LontLati.logti_O_du=temp1[0];
	LontLati.lati_O_du=temp1[1];
	
	longti_O=LontLati.logti_O_du;
	lati_O=LontLati.lati_O_du;
		
	return LontLati;

}



/*********************************ʵ�ֺ���**********************************/
//����ԭ��: u8 Wakeup_position(double lati_OA,double longti_OB ,float cour_angle[2],u16 near_pot[2])
//����:		���ϵ������¼��С���Ƿ���·����Χ�ڣ������ڷ�Χ���ǵ����ǵ�������㵼���ǣ��������ڷ�Χ��ʱ����ǵ���������㺽��Ǵ�ֱ�ĽǶ�
//����/�����double lati_OA,double longti_OB:��ǰС����ʵʱ��������ϵ�µ�����ֵ���Ѿ��洢�õ�·����double * OA,double * OB,float OAOB_angl,
//�����float  cour_angl[2]   cour_angl[0] ���ڴ洢��õĺ����        ����return=1�������ĺ����   ��return=0������㺽��Ǵ�ֱ��   u8 near_pot[0]: ��ǰ��������ĵ�ָ��
//���أvu8 �����ص���0ʱ  ��ʾ�ڷ�Χ��   �����ص���1ʱ  ���ڷ�Χ��
/*******************************************************************************/
u8 Wakeup_position(double lati_OA,double longti_OB ,float cour_angle[2],u16 near_pot[2])
{
	u16 i;
	u8 re_back;
	float temp0,temp=10000000.0;
	static MapPotDef mapPot,mapPot1;
	mapPot=MapPot_Find(i*MapPot.numPot);				//Ѱ�ҵ��ӵ�ͼ�϶�Ӧ��

	
	for(i=0;i<=MapPot.numFx;i++)					//������ĸ�ֱ���߶��ϵĵ�ȽϽ�
	{
		temp0=sqrt((mapPot.OA-lati_OA)*(mapPot.OA-lati_OA)+(mapPot.OB-longti_OB)*(mapPot.OB-longti_OB));              //·���ϵĵ���С��֮��ľ���
		if(temp0<=temp)
		{
			temp=temp0;                            //�����ֵС����һ��ֵʱ���洢����
			mark_num=i;														//�洢ѭ��������
		}
			hhhh=i;
		error=1;
	}	
	mark_num=MapPot.numPot*mark_num;								//��������ʵ����λ��
	
	
	if(i==0)									//���ǵ�һ��·����ʱ
	{
		for(i=0;i<MapPot.numPot-5;i++)							//��������15�����������
		{
			mapPot1=MapPot_Find(i);				//Ѱ�ҵ��ӵ�ͼ�϶�Ӧ��
			temp0=sqrt((mapPot1.OA-lati_OA)*(mapPot1.OA-lati_OA)+(mapPot1.OB-longti_OB)*(mapPot1.OB-longti_OB));              //·���ϵĵ���С��֮��ľ���
			if(temp0<=temp)
			{
				temp=temp0;                            //�����ֵС����һ��ֵʱ���洢����
				mark_num_gross=i;														//�洢ѭ��������
			}
			else	mark_num_gross=0;		
		}
		error=2;
	}		
	else if(i==MapPot.grossNumPot)                //�������һ��·����ʱ
	{
		for(i=MapPot.grossNumPot-MapPot.numPot+5;i<MapPot.grossNumPot;i++)							//�����ǰ��15�����������
		{
			mapPot1=MapPot_Find(i);				//Ѱ�ҵ��ӵ�ͼ�϶�Ӧ��
			temp0=sqrt((mapPot1.OA-lati_OA)*(mapPot1.OA-lati_OA)+(mapPot1.OB-longti_OB)*(mapPot1.OB-longti_OB));              //·���ϵĵ���С��֮��ľ���
			if(temp0<=temp)
			{
				temp=temp0;                            //�����ֵС����һ��ֵʱ���洢����
				mark_num_gross=i;														//�洢ѭ��������
			}
			else	mark_num_gross=MapPot.grossNumPot-1;			//���û�У��洢���һ����
		}
		error=3;
	
	}
	else																		//�м�ĵ�
	{
		for(i=mark_num-MapPot.numPot+5;i<mark_num+MapPot.numPot-5;i++)							//�����ǰ��15�����������
		{
			mapPot1=MapPot_Find(i);				//Ѱ�ҵ��ӵ�ͼ�϶�Ӧ��
			temp0=sqrt((mapPot1.OA-lati_OA)*(mapPot1.OA-lati_OA)+(mapPot1.OB-longti_OB)*(mapPot1.OB-longti_OB));              //·���ϵĵ���С��֮��ľ���
			if(temp0<=temp)
			{
				temp=temp0;                            //�����ֵС����һ��ֵʱ���洢����
				mark_num_gross=i;														//�洢ѭ��������
			}
			else	mark_num_gross=mark_num;			//���û�У��洢���һ����
		}	
		error=3;
	}
				
		if(temp<=(float)thresh_posi)					//����⵽��ǰ���ڷ�ֵ��Χ�� 
		{
			cour_angle[0]=mapPot1.angle;			//С����Ŀ��Ƕȵ�����Ҫ�ĽǶ�
			re_back=0;				   
			
		}
		else																//����⵽С�����ڷ�ֵ��Χ��
		{
			mapPot1=MapPot_Find(mark_num);				//Ѱ�ҵ��ӵ�ͼ�϶�Ӧ��
			cour_angle[0]=four_quadrant(longti_OB,lati_OA,mapPot1.OB,mapPot1.OA);														//��ǰλ����Ŀ���ĽǶ�
			re_back=1;		
		}		
	near_pot[0]=mark_num;
	return re_back;		
}


/*********************************ʵ�ֺ���**********************************/
//����ԭ��: u8 direct_judge(doudble  longti_y,double  lati_x ,u16 neat_pot)
//����:	�жϵ�ǰС����ʵʱλ��ֵ�������·�������߻����ұ�
//����/�����doudble  longti_y,double lati_x:��ǰС����ʵʱ��������ϵ�µ�����ֵ���Ѿ��洢�õ�·����double * OA,double * OB,float OAOB_angl, ����������ָ�룺u16 neat_pot
//
//���أvu8 �����ص���0ʱ  ����·��������   �����ص���1ʱ ��ʾ����·������ұ�
//
/*******************************************************************************/
u8 direct_judge(double lati_y,double longti_x ,u16 neat_pot)
{
	u8 direct;
	static MapPotDef mapPot;
	mapPot=MapPot_Find(neat_pot);
	if((mapPot.angle>=0&&mapPot.angle<45)||(mapPot.angle>=315&&mapPot.angle<360)) 			//��·���㴦��[0��,45��)||[315��,360��)
	{
		if(longti_x>=mapPot.OB) direct=1;																			//�ұ�
		else direct=0;																									//���
	}
	else if(mapPot.angle>=135&&mapPot.angle<225) 													//��·���㴦��[135��,225��)
	{
		if(longti_x>=mapPot.OB) direct=0;																			//���
		else direct=1;																									//�ұ�
	}
	else if(mapPot.angle>=45&&mapPot.angle<135)														//��·���㴦��[45��,135��)
	{
		if(lati_y>=mapPot.OA) direct=0;																			//���
		else direct=1;																									//�ұ�
	}	
	else if(mapPot.angle>=225&&mapPot.angle<315)														//��·���㴦��[225��,315��)
	{
		if(lati_y>=mapPot.OA) direct=1;																			//�ұ�
		else direct=0;																									//���
	}
	return direct;
}



/*********************************ʵ�ֺ���**********************************/
//����ԭ��: int Scan_nearpot(doudble  longti_y,u16 neat_pot)
//����:		Ѱ�Ҿ��뵱ǰ���������ĵ�,��������뵱ǰ��ʸ������
//����/�����double  longti_y,double lati_x:��ǰС����ʵʱ��������ϵ�µ�����ֵ���Ѿ��洢�õ�·����double * OA,double * OB,float OAOB_angl,
//���룬u8 neat_pot[0]�����뵱ǰ�ĵ��������ĵ�  ,u8 neat_pot[1]:���¼������������  u8 num�����������ǰ��ɨ��������  Ĭ��10
//���أvint ��ֵ<0ʱ����ʾ��·��������   int ��ֵ>=0ʱ����ʾ��·��������
//
//���ڲ���ʱ��׼��ֵ����Ŀ���ΪԲ�Ļ�Բ��õľ��룬���ڼ��������ľ���ܽ�����ǰС����ʵʱλ����Ŀ���ľ�����Ƶ��ڹ�Ŀ������ߵĲ෽λ����
//ֻ��Ҫ�ͳ�����ķ��������򼴿�
//
/*******************************************************************************/

float Scan_nearpot(double lati_y,double longti_x,u16 near_pot[2],u8 num)
{
	float direct_1=1000000.0,direct_2;
	static MapPotDef mapPot;
	u16 i;
	u8 zuoyou;
	for(i=0;i<MapPot.grossNumPot;i++)
	{
		mapPot=MapPot_Find(i);				//������Ӧ��OAOBֵ
		direct_2=sqrt((lati_y-mapPot.OA)*(lati_y-mapPot.OA)+(longti_x-mapPot.OB)*(longti_x-mapPot.OB));                 //�������ľ���
		if(direct_2<=direct_1)
		{
			direct_1=direct_2;																					///����Сֵ���跴����ֵ
			near_pot[0]=i;
		}			
	}

	zuoyou=direct_judge(lati_y,longti_x,near_pot[2]);								//�жϵ�ǰ�ĵ���·�������껹���ұ�
	if(zuoyou==0) return -direct_1;																										//����ߣ�����ֵ�ø�����ʾ
	else return direct_1;		
																														//���ұߣ�����ֵ�ø�����ʾ			
}


/*********************************ʵ�ֺ���**********************************/
//����ԭ��:float course_weighting(u16 near_pot[2],float d_diviation)
//����:	��Ѱ���е�С������Ƕ�����Ȩ����
//����/����� ���룺Ѱ�ҵ�������㣺u16 near_pot[2],    �ƶ��������������Ŀ���֮���ʸ������ float d_diviation  
//
//���أ� С��������ĵ�����
//˵������ֵ��Ϊ9����
//  1. С����ƫ<-2*��ֵ       					 	��������ƫת��·���Ƕ�+30��   
//	2. -1.5*��ֵ>С����ƫ >=-2*��ֵ		   	��������ƫת��·���Ƕ�+20�� 
//	3. -1*��ֵ>С����ƫ��>=-1.5*��ֵ      ��������ƫת��·���Ƕ�+10�� 
//	4. -0.5*��ֵ>С����ƫ>=-1*��ֵ     	  ��������ƫת��·���Ƕ�+5�� 
//	5. 0.5*��ֵ>С����ƫ>=-0.5*��ֵ       �����ǲ�ƫת��·���Ƕ�+0�� 
//	6. 1*��ֵ>С����ƫ>=0.5*��ֵ       		��������ƫת��·���Ƕ�-5�� 
//	7. 1.5*��ֵ>С����ƫ>=1*��ֵ       		��������ƫת��·���Ƕ�-10�� 
//	8. 2*��ֵ>С����ƫ>=1.5*��ֵ       		��������ƫת��·���Ƕ�-20�� 
//	9. С����ƫ>=2*��ֵ       						��������ƫת��·���Ƕ�-35��
/*******************************************************************************/
float course_weighting(u16 near_pot[2],float d_diviation)
{
	float rate;
	float weighting_angle; 										//С����ƫ����
	static MapPotDef mapPot;

	mapPot=MapPot_Find(near_pot[0]);				//������ӵ�ͼ�϶�Ӧ��
	rate=d_diviation/(float)thresh_posi;     				//���ƫ�þ����뷧ֵ֮��ľ���
	if(rate<-2.0f)															//1. С����ƫ<-2*��ֵ
	weighting_angle=mapPot.angle-30;
	
	else if(rate>=-2.0f&&rate<-1.5f)							//2. -1.5*��ֵ>С����ƫ >=-2*��ֵ	
	weighting_angle=mapPot.angle-20;
	
	else if(rate>=-1.5f&&rate<-1.0f)						//3. -1*��ֵ>С����ƫ��>=-1.5*��ֵ
	weighting_angle=mapPot.angle-10;
	
	else if(rate>=-1.0f&&rate<-0.5f)						//4. -0.5*��ֵ>С����ƫ>=-1*��ֵ 
	weighting_angle=mapPot.angle-5;
	
	else if(rate>=-0.5f&&rate<0.5f)							//5. 0.5*��ֵ>С����ƫ>=-0.5*��ֵ
	weighting_angle=mapPot.angle;
	
	else if(rate>=0.5f&&rate<1.0f)							//6.1*��ֵ>С����ƫ>=0.5*��ֵ
	weighting_angle=mapPot.angle+5;
	
	else if(rate>=1.0f&&rate<1.5f)							//7. 1.5*��ֵ>С����ƫ>=1*��ֵ
	weighting_angle=mapPot.angle+10;
	
	else if(rate>=1.5f&&rate<2.0f)							//8. 2*��ֵ>С����ƫ>=1.5*��ֵ
	weighting_angle=mapPot.angle+20;
	
	else 																			//9. С����ƫ>=2*��ֵ 
	weighting_angle=mapPot.angle+30;

	return weighting_angle;										//����С��ƫ����
}



/*********************************ʵ�ֺ���**********************************/
//����ԭ��: float angle_deviat_judge(float rencent_angle,float taget_angle)
//����: ��360�ȷ�Χ�ڣ������Ƕȵļ�Сֵ�����ж�
//����/����f rencent_angle����ǰС���Ƕ�     taget_angle��С��Ŀ��Ƕ�
//���أv�ǶȲ�ֵ
//
/*******************************************************************************/
float angle_deviat_judge(float rencent_angle,float taget_angle)
{
	float deviat_a,deviat_b;  //a�洢�������   bΪ��360�ȼ�
	float angle;
	deviat_a=abs_(rencent_angle-taget_angle);
	deviat_b=360.0-abs_(rencent_angle-taget_angle);
	if(deviat_a<=deviat_b)	angle=taget_angle-rencent_angle;	//����׼ֵС��360�ȼ�ȥֵʱ��ʹ�ñ�׼ֵ
	else 
	{
		angle=360.0-abs_(rencent_angle-taget_angle);    //��֮ʹ��360-��ֵ		
		if(taget_angle>=rencent_angle)		angle=-abs_(angle);//��Ŀ��ֵ���ڵ�ǰֵ��ȡ����		
		else	angle=abs_(angle);//��Ŀ��ֵ���ڵ�ǰֵ��ȡ����
	}
	
	return angle;
}




/*********************************ʵ�ֺ���**********************************/
//����ԭ��: veichel_contr(float rencent_angle,float taget_angle)
//����: ��С���˶����п���
//����/����f rencent_angle����ǰС���Ƕ�     taget_angle��С��Ŀ��Ƕ�
//���أv���ƴ���
//
/*******************************************************************************/
u8 veichel_contr(float rencent_angle,float taget_angle)
{
	
//	float deviat_value;       //Ŀ��Ƕ��뵱ǰС���Ƕȵ�ƫ��ֵ
//	u8 data;									//���صĿ���ֵ
//	deviat_value=deviat_value=angle_deviat_judge(rencent_angle,taget_angle);
//	if(deviat_value<=10&&deviat_value>=-10)			data=0x05;      //ǰ��
//	else if(deviat_value>10&&deviat_value<=30)      data=0x06;              //��תС�Ƕ�
//	else if(deviat_value>30)     data=0x07;                               //��ת��Ƕ�
//	else if(deviat_value<-10&&deviat_value>=-30)  data=0x08;              //��תС�Ƕ�
//	else if(deviat_value<-30) 	data=0x09; 															//��ת��Ƕ�
//	else         data=0x05;                                             //ǰ��		
//	return data;
	
//	float deviat_value;       //Ŀ��Ƕ��뵱ǰС���Ƕȵ�ƫ��ֵ
	u8 data;									//���صĿ���ֵ
	deviat_value=angle_deviat_judge(rencent_angle,taget_angle);
	if(deviat_value<=8&&deviat_value>=-8)			data=0x05;      //ǰ��
	else if(deviat_value>8&&deviat_value<=30)      data=0x06;              //��תС�Ƕ�
	else if(deviat_value>30)     data=0x07;                               //��ת��Ƕ�
	else if(deviat_value<-8&&deviat_value>=-30)  data=0x08;              //��תС�Ƕ�
	else if(deviat_value<-30) 	data=0x09; 															//��ת��Ƕ�
	else         data=0x05;                                             //ǰ��		
	return data;	
}


//u8 veichel_contr(float rencent_angle,float taget_angle)
//{
//	
//	float deviat_value;       //Ŀ��Ƕ��뵱ǰС���Ƕȵ�ƫ��ֵ
//	u8 data;									//���صĿ���ֵ
//	deviat_value=deviat_value=angle_deviat_judge(rencent_angle,taget_angle);
//	if(deviat_value<=10&&deviat_value>=-10)			data=0x05;      //ǰ��
//	else if(deviat_value>10&&deviat_value<=30)      data=0x08;              //��תС�Ƕ�
//	else if(deviat_value>30)     data=0x09;                               //��ת��Ƕ�
//	else if(deviat_value<-10&&deviat_value>=-30)  data=0x06;              //��תС�Ƕ�
//	else if(deviat_value<-30) 	data=0x07; 															//��ת��Ƕ�
//	else         data=0x05;                                             //ǰ��		
//	return data;
//	
//}




/*****************************************************************/
// 
//
//
//
//
/*****************************************************************/


