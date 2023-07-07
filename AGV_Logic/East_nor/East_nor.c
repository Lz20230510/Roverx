#include "delay.h" 
#include "myiic.h" 
#include "math.h" 
#include "East_nor.h" 
#include "map.h" 
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//数据转换到东北坐标系下并进行转换
//
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////

float deviat_value;       //目标角度与当前小车角度的偏差值
	
	u16 mark_num=0;
	u16 mark_num_gross=0;
	u16 hhhh;
	u8 error=0;
/*********************************实现函数*************************************/
//函数原型double abs_(double in);
//功能：求双精度数据绝对值
//输入/输出：   输入 需要转换数据
//返回：返回绝对值
/*******************************************************************************/
double abs_(double in)
{
	if(in>=0) return in;
	else return -in;
}

/*********************************实现函数*************************************/
//函数原型：double four_quadrant(double first,double last);
//功能：拟合图形完成后对于单根直线的指向角方向的判断
//输入/输出：输入 线段上的第一个点，线段上的第二个点
//返回：返回角度
/*******************************************************************************/
float four_quadrant(double first_x,double first_y,double last_x,double last_y)
{

	double yl_yf,xl_xf,arcsin;
	if(last_y>=first_y&&last_x>=first_x)     //第一象限
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(xl_xf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//求出在第一象限的角度
		return 90.0-arcsin;																						//返回在东北坐标系下的角度
	}
	else if(last_y>=first_y&&last_x<first_x)		//第二象限
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(yl_yf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//求出在第二象限的角度
		return 270.0+arcsin;																					//返回在东北坐标系下的角度
	}
	else if(last_y<first_y&&last_x<first_x)			//第三象限
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(yl_yf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//求出在第三象限的角度
		return 270.0-arcsin;																					//返回在东北坐标系下的角度
	}
	else if(last_y<first_y&&last_x>=first_x)				//第四象限
	{
		yl_yf=abs_(last_y-first_y);   
		xl_xf=abs_(last_x-first_x);
		arcsin=asin(yl_yf/sqrt(yl_yf*yl_yf+xl_xf*xl_xf))*180/pi;						//求出在第二象限的角度
		return 90.0+arcsin;																					//返回在东北坐标系下的角度
	}
	return arcsin;
}


/*********************************实现函数**********************************/
//函数原型float JY901_eastnor(float JY901_angle);
//功能：将JY901输出角度值转换为统一的东北坐标系下的一圈360°的格式
//输入/输出：输入 JY901_angle:JY901的导航角数据,   error:传感器的测量值与实际东北坐标系角度的偏差值
//返回：修正后的数据
/*******************************************************************************/
float JY901_eastnor(float JY901_angle,float error)
{
	if(JY901_angle<=0.0f) return (-JY901_angle+error);
	else return (360.0f-JY901_angle+error);
}

/*********************************实现函数**********************************/
//函数原型void longti_lati_trans(u8 mode,double longti,double lati,double x0_y0[2],double lotlat_angle[2]);
//
//功能：将经纬度数据从度分格式-->度格式
//输入/输出：输入：经度，维度数据， 输出：x0_y0[0]  x0,x0_y0[0]  y0, lotlat_angle[0] 维度的度格式，lotlat_angle[1]  经维度的度格式
//u8 mode: mode=0 :输入格式为度分格式         mode=1：输入格式为度格式
//返回：void
/*******************************************************************************/
void longti_lati_trans(u8 mode,double longti,double lati,double x0_y0[2],double lotlat_du[2])
{
	double longti_H,longti_L,lati_H,lati_L;
	if(mode==0)
	{
			//求经度的度格式
		longti_H=(long int)(longti/100.0);					//数据左移两位，并提取整数部分
		longti_L=(longti/100.0-longti_H)*100.0/60.0;			//将分转换为度
		longti_H=longti_H+longti_L;          				//求出完整度格式数据

		//求维度的度格式
		lati_H=(long int)(lati/100.0);					//数据左移两位，并提取整数部分
		lati_L=(lati/100.0-lati_H)*100.0/60.0;			//将分转换为度
		lati_H=lati_H+lati_L;          				//求出完整度格式数据	
		
		x0_y0[0]=a*a/sqrt(a*a+b*b*tan(lati_H*pi/180)*tan(lati_H*pi/180));		//求出经度方向下X0的值
		x0_y0[1]=b*b/sqrt(b*b+a*a/tan(lati_H*pi/180)/tan(lati_H*pi/180));		//求出经度方向下y0的值
	
		lotlat_du[1]=lati_H;							//维度的度格式值
		lotlat_du[0]=longti_H;								//经度的度格式值
	}
	else if(mode==1)
	{
		longti_H=longti;
		lati_H=lati;
	
		x0_y0[0]=a*a/sqrt(a*a+b*b*tan(lati_H*pi/180)*tan(lati_H*pi/180));		//求出经度方向下X0的值
		x0_y0[1]=b*b/sqrt(b*b+a*a/tan(lati_H*pi/180)/tan(lati_H*pi/180));		//求出经度方向下y0的值		
	}
}

/***********************寻找电子地图上的某一个点**************************/
//函数形式：MapPotDef MapPot_Find(u16 potNum)
//输入参数：
//输出参数：
//返回参数：MapPotDef
//
/****************************************************************/
MapPotDef MapPot_Find(u16 potNum)
{
	static MapPotDef potDef;
	
	potDef=Map_ReadFromFLASH(potNum+1,4);				//读取存储的地图信息点
	
	return potDef;

}


/****************************************************************/
//函数形式：void longtiLatiTrans_xy(double lati,double * lati_xy)
//输入参数：double lati：纬度值，度格式
//输出参数：lat_xy[0]  经度方向x值，lat_xy[0]方向值
//返回参数：
//
/****************************************************************/
void longtiLati_xy(double lati,double * lati_xy)
{
	*lati_xy=a*a/sqrt(a*a+b*b*tan(lati*pi/180)*tan(lati*pi/180));		//求出经度方向下X0的值
	*(lati_xy+1)=b*b/sqrt(b*b+a*a/tan(lati*pi/180)/tan(lati*pi/180));		//求出经度方向下y0的值
}


/*********************************实现函数**********************************/
//函数原型void OAOB_caculate(double x0,double y0,double x,double y,doubel lati_O,doubel lati_xy,double double OAOB[2]);
//功能：在东北坐标系下计算某点(x,y)相对于原点O(x0,y0)的OAOB坐标值
//输入/输出：输入：输入 double x0,y0 原点O的经度方向下x,y值，double x,y  某一点（x,y）在经度方向下的x,y值，double lati_xy,lati_O分别是原点和某一点的经度值
//输出：double OAOB[2]：OA  OB值
//返回：void
/*******************************************************************************/
void OAOB_caculate(double x0,double y0,double x,double y,double longti_O,double longti_xy,double OAOB[2])
{

		//求OA
		if(y>=y0) OAOB[0]=sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0)); 
		else  OAOB[0]=-sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));
		
		//求OB
		OAOB[1]=x0*(longti_xy-longti_O)*pi/180;	//求OB
	
}


/*******************************实现函数*******************************/
//函数形式：LontLatiDef Origin_O_parmt_Init(void);
//输入参数：
//输出参数：
//返回参数：
//功能说明：读取GPS电子地图原点相关信息
//
/**********************************************************************/
LontLatiDef Origin_O_parmt_Init(void)
{
	static union  MultiType multiType;
	static LontLatiDef LontLati;
	double temp[2],temp1[2];
	
	multiType.u32Array_[0]=Paramt_BUFF[10][1];		
	multiType.u32Array_[1]=Paramt_BUFF[10][2];	
	LontLati.logti_O_dufen=multiType.double_;				//读取经度的度分格式
	
	multiType.u32Array_[0]=Paramt_BUFF[10][3];		
	multiType.u32Array_[1]=Paramt_BUFF[10][4];	
	LontLati.lati_O_dufen=multiType.double_;				//读取纬度度分格式

	LontLati.totalPotNum=(u16)Paramt_BUFF[10][10];		//电子地图总点数量
	LontLati.linePotNum=(u16)Paramt_BUFF[10][11];		//电子地图分线段上点的数量
	
	multiType.u32_=Paramt_BUFF[10][12];				//自动寻迹速度
	LontLati.spdTemp=multiType.float_;				//自动寻迹速度
	
	longti_lati_trans(0,LontLati.logti_O_dufen,LontLati.lati_O_dufen,temp,temp1);

	longtiLati_xy(LontLati.lati_O_du,temp);					//求出经度方向下x.y值	
	LontLati.lati_xO=temp[0];						//经度方向度x轴坐标
	LontLati.lati_yO=temp[1];						//经度方向度y轴坐标
	
	lati_x0=LontLati.lati_xO;
	lati_y0=LontLati.lati_yO;
	
	
	
	LontLati.logti_O_du=temp1[0];
	LontLati.lati_O_du=temp1[1];
	
	longti_O=LontLati.logti_O_du;
	lati_O=LontLati.lati_O_du;
		
	return LontLati;

}



/*********************************实现函数**********************************/
//函数原型: u8 Wakeup_position(double lati_OA,double longti_OB ,float cour_angle[2],u16 near_pot[2])
//功能:		刚上电启动下检查小车是否在路径范围内，当处于范围内是导航角等于最近点导航角，当不处于范围内时航向角等于与最近点航向角垂直的角度
//输入/输出：double lati_OA,double longti_OB:当前小车的实时东北坐标系下的坐标值，已经存储好的路径：double * OA,double * OB,float OAOB_angl,
//输出，float  cour_angl[2]   cour_angl[0] 用于存储获得的航向角        ：当return=1，最近点的航向角   当return=0，最近点航向角垂直角   u8 near_pot[0]: 当前距离最近的点指针
//返回u8 当返回等于0时  表示在范围内   当返回等于1时  不在范围内
/*******************************************************************************/
u8 Wakeup_position(double lati_OA,double longti_OB ,float cour_angle[2],u16 near_pot[2])
{
	u16 i;
	u8 re_back;
	float temp0,temp=10000000.0;
	static MapPotDef mapPot,mapPot1;
	mapPot=MapPot_Find(i*MapPot.numPot);				//寻找电子地图上对应点

	
	for(i=0;i<=MapPot.numFx;i++)					//求距离哪个直线线段上的点比较近
	{
		temp0=sqrt((mapPot.OA-lati_OA)*(mapPot.OA-lati_OA)+(mapPot.OB-longti_OB)*(mapPot.OB-longti_OB));              //路径上的点与小车之间的距离
		if(temp0<=temp)
		{
			temp=temp0;                            //当获得值小于上一个值时，存储数据
			mark_num=i;														//存储循环的数据
		}
			hhhh=i;
		error=1;
	}	
	mark_num=MapPot.numPot*mark_num;								//求出点的真实数组位置
	
	
	if(i==0)									//当是第一个路径点时
	{
		for(i=0;i<MapPot.numPot-5;i++)							//在这点后面15个点找最近点
		{
			mapPot1=MapPot_Find(i);				//寻找电子地图上对应点
			temp0=sqrt((mapPot1.OA-lati_OA)*(mapPot1.OA-lati_OA)+(mapPot1.OB-longti_OB)*(mapPot1.OB-longti_OB));              //路径上的点与小车之间的距离
			if(temp0<=temp)
			{
				temp=temp0;                            //当获得值小于上一个值时，存储数据
				mark_num_gross=i;														//存储循环的数据
			}
			else	mark_num_gross=0;		
		}
		error=2;
	}		
	else if(i==MapPot.grossNumPot)                //当是最后一个路径点时
	{
		for(i=MapPot.grossNumPot-MapPot.numPot+5;i<MapPot.grossNumPot;i++)							//在这点前面15个点找最近点
		{
			mapPot1=MapPot_Find(i);				//寻找电子地图上对应点
			temp0=sqrt((mapPot1.OA-lati_OA)*(mapPot1.OA-lati_OA)+(mapPot1.OB-longti_OB)*(mapPot1.OB-longti_OB));              //路径上的点与小车之间的距离
			if(temp0<=temp)
			{
				temp=temp0;                            //当获得值小于上一个值时，存储数据
				mark_num_gross=i;														//存储循环的数据
			}
			else	mark_num_gross=MapPot.grossNumPot-1;			//如果没有，存储最后一个点
		}
		error=3;
	
	}
	else																		//中间的点
	{
		for(i=mark_num-MapPot.numPot+5;i<mark_num+MapPot.numPot-5;i++)							//在这点前面15个点找最近点
		{
			mapPot1=MapPot_Find(i);				//寻找电子地图上对应点
			temp0=sqrt((mapPot1.OA-lati_OA)*(mapPot1.OA-lati_OA)+(mapPot1.OB-longti_OB)*(mapPot1.OB-longti_OB));              //路径上的点与小车之间的距离
			if(temp0<=temp)
			{
				temp=temp0;                            //当获得值小于上一个值时，存储数据
				mark_num_gross=i;														//存储循环的数据
			}
			else	mark_num_gross=mark_num;			//如果没有，存储最后一个点
		}	
		error=3;
	}
				
		if(temp<=(float)thresh_posi)					//当检测到当前点在阀值范围内 
		{
			cour_angle[0]=mapPot1.angle;			//小车的目标角度等于需要的角度
			re_back=0;				   
			
		}
		else																//当检测到小车不在阀值范围内
		{
			mapPot1=MapPot_Find(mark_num);				//寻找电子地图上对应点
			cour_angle[0]=four_quadrant(longti_OB,lati_OA,mapPot1.OB,mapPot1.OA);														//求当前位置与目标点的角度
			re_back=1;		
		}		
	near_pot[0]=mark_num;
	return re_back;		
}


/*********************************实现函数**********************************/
//函数原型: u8 direct_judge(doudble  longti_y,double  lati_x ,u16 neat_pot)
//功能:	判断当前小车的实时位置值处于最近路径点的左边还是右边
//输入/输出：doudble  longti_y,double lati_x:当前小车的实时东北坐标系下的坐标值，已经存储好的路径：double * OA,double * OB,float OAOB_angl, 距离最近点的指针：u16 neat_pot
//
//返回u8 当返回等于0时  处于路径点的左边   当返回等于1时 表示处于路径点的右边
//
/*******************************************************************************/
u8 direct_judge(double lati_y,double longti_x ,u16 neat_pot)
{
	u8 direct;
	static MapPotDef mapPot;
	mapPot=MapPot_Find(neat_pot);
	if((mapPot.angle>=0&&mapPot.angle<45)||(mapPot.angle>=315&&mapPot.angle<360)) 			//当路径点处于[0°,45°)||[315°,360°)
	{
		if(longti_x>=mapPot.OB) direct=1;																			//右边
		else direct=0;																									//左边
	}
	else if(mapPot.angle>=135&&mapPot.angle<225) 													//当路径点处于[135°,225°)
	{
		if(longti_x>=mapPot.OB) direct=0;																			//左边
		else direct=1;																									//右边
	}
	else if(mapPot.angle>=45&&mapPot.angle<135)														//当路径点处于[45°,135°)
	{
		if(lati_y>=mapPot.OA) direct=0;																			//左边
		else direct=1;																									//右边
	}	
	else if(mapPot.angle>=225&&mapPot.angle<315)														//当路径点处于[225°,315°)
	{
		if(lati_y>=mapPot.OA) direct=1;																			//右边
		else direct=0;																									//左边
	}
	return direct;
}



/*********************************实现函数**********************************/
//函数原型: int Scan_nearpot(doudble  longti_y,u16 neat_pot)
//功能:		寻找距离当前点距离最近的点,并求出距离当前的矢量距离
//输入/输出：double  longti_y,double lati_x:当前小车的实时东北坐标系下的坐标值，已经存储好的路径：double * OA,double * OB,float OAOB_angl,
//输入，u8 neat_pot[0]：距离当前的点距离最近的点  ,u8 neat_pot[1]:重新计算后距离最近点  u8 num：距离最近点前后扫描点的数量  默认10
//返回int 当值<0时，表示在路径点的左边   int 当值>=0时，表示在路径点的左边
//
//由于采样时标准阀值是以目标点为圆心画圆求得的距离，相邻几组采样点的距离很近，当前小车的实时位置与目标点的距离近似等于过目标点求垂线的侧方位距离
//只需要就出距离的法向量方向即可
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
		mapPot=MapPot_Find(i);				//求出多对应点OAOB值
		direct_2=sqrt((lati_y-mapPot.OA)*(lati_y-mapPot.OA)+(longti_x-mapPot.OB)*(longti_x-mapPot.OB));                 //计算近点的距离
		if(direct_2<=direct_1)
		{
			direct_1=direct_2;																					///将最小值赋予反馈的值
			near_pot[0]=i;
		}			
	}

	zuoyou=direct_judge(lati_y,longti_x,near_pot[2]);								//判断当前的点在路径的坐标还是右边
	if(zuoyou==0) return -direct_1;																										//在左边，距离值用负数表示
	else return direct_1;		
																														//在右边，距离值用负数表示			
}


/*********************************实现函数**********************************/
//函数原型:float course_weighting(u16 near_pot[2],float d_diviation)
//功能:	对寻迹中的小车航向角度做加权处理
//输入/输出： 输入：寻找到的最近点：u16 near_pot[2],    移动机器人与最近的目标点之间的矢量距离 float d_diviation  
//
//返回： 小车纠正后的导航角
//说明：阀值分为9档：
//  1. 小车左偏<-2*阀值       					 	导航角右偏转：路径角度+30°   
//	2. -1.5*阀值>小车左偏 >=-2*阀值		   	导航角右偏转：路径角度+20° 
//	3. -1*阀值>小车左偏　>=-1.5*阀值      导航角右偏转：路径角度+10° 
//	4. -0.5*阀值>小车左偏>=-1*阀值     	  导航角右偏转：路径角度+5° 
//	5. 0.5*阀值>小车左偏>=-0.5*阀值       导航角不偏转：路径角度+0° 
//	6. 1*阀值>小车左偏>=0.5*阀值       		导航角左偏转：路径角度-5° 
//	7. 1.5*阀值>小车左偏>=1*阀值       		导航角左偏转：路径角度-10° 
//	8. 2*阀值>小车左偏>=1.5*阀值       		导航角左偏转：路径角度-20° 
//	9. 小车左偏>=2*阀值       						导航角左偏转：路径角度-35°
/*******************************************************************************/
float course_weighting(u16 near_pot[2],float d_diviation)
{
	float rate;
	float weighting_angle; 										//小车的偏航角
	static MapPotDef mapPot;

	mapPot=MapPot_Find(near_pot[0]);				//求出电子地图上对应点
	rate=d_diviation/(float)thresh_posi;     				//求出偏置距离与阀值之间的距离
	if(rate<-2.0f)															//1. 小车左偏<-2*阀值
	weighting_angle=mapPot.angle-30;
	
	else if(rate>=-2.0f&&rate<-1.5f)							//2. -1.5*阀值>小车左偏 >=-2*阀值	
	weighting_angle=mapPot.angle-20;
	
	else if(rate>=-1.5f&&rate<-1.0f)						//3. -1*阀值>小车左偏　>=-1.5*阀值
	weighting_angle=mapPot.angle-10;
	
	else if(rate>=-1.0f&&rate<-0.5f)						//4. -0.5*阀值>小车左偏>=-1*阀值 
	weighting_angle=mapPot.angle-5;
	
	else if(rate>=-0.5f&&rate<0.5f)							//5. 0.5*阀值>小车左偏>=-0.5*阀值
	weighting_angle=mapPot.angle;
	
	else if(rate>=0.5f&&rate<1.0f)							//6.1*阀值>小车左偏>=0.5*阀值
	weighting_angle=mapPot.angle+5;
	
	else if(rate>=1.0f&&rate<1.5f)							//7. 1.5*阀值>小车左偏>=1*阀值
	weighting_angle=mapPot.angle+10;
	
	else if(rate>=1.5f&&rate<2.0f)							//8. 2*阀值>小车左偏>=1.5*阀值
	weighting_angle=mapPot.angle+20;
	
	else 																			//9. 小车左偏>=2*阀值 
	weighting_angle=mapPot.angle+30;

	return weighting_angle;										//返回小车偏航角
}



/*********************************实现函数**********************************/
//函数原型: float angle_deviat_judge(float rencent_angle,float taget_angle)
//功能: 对360度范围内，两个角度的极小值进行判断
//输入/输出 rencent_angle：当前小车角度     taget_angle：小车目标角度
//返回角度差值
//
/*******************************************************************************/
float angle_deviat_judge(float rencent_angle,float taget_angle)
{
	float deviat_a,deviat_b;  //a存储正常相减   b为用360度减
	float angle;
	deviat_a=abs_(rencent_angle-taget_angle);
	deviat_b=360.0-abs_(rencent_angle-taget_angle);
	if(deviat_a<=deviat_b)	angle=taget_angle-rencent_angle;	//当标准值小于360度减去值时，使用标准值
	else 
	{
		angle=360.0-abs_(rencent_angle-taget_angle);    //反之使用360-的值		
		if(taget_angle>=rencent_angle)		angle=-abs_(angle);//当目标值大于当前值，取负数		
		else	angle=abs_(angle);//当目标值大于当前值，取正数
	}
	
	return angle;
}




/*********************************实现函数**********************************/
//函数原型: veichel_contr(float rencent_angle,float taget_angle)
//功能: 对小车运动进行控制
//输入/输出 rencent_angle：当前小车角度     taget_angle：小车目标角度
//返回控制代码
//
/*******************************************************************************/
u8 veichel_contr(float rencent_angle,float taget_angle)
{
	
//	float deviat_value;       //目标角度与当前小车角度的偏差值
//	u8 data;									//返回的控制值
//	deviat_value=deviat_value=angle_deviat_judge(rencent_angle,taget_angle);
//	if(deviat_value<=10&&deviat_value>=-10)			data=0x05;      //前进
//	else if(deviat_value>10&&deviat_value<=30)      data=0x06;              //右转小角度
//	else if(deviat_value>30)     data=0x07;                               //右转大角度
//	else if(deviat_value<-10&&deviat_value>=-30)  data=0x08;              //左转小角度
//	else if(deviat_value<-30) 	data=0x09; 															//左转大角度
//	else         data=0x05;                                             //前进		
//	return data;
	
//	float deviat_value;       //目标角度与当前小车角度的偏差值
	u8 data;									//返回的控制值
	deviat_value=angle_deviat_judge(rencent_angle,taget_angle);
	if(deviat_value<=8&&deviat_value>=-8)			data=0x05;      //前进
	else if(deviat_value>8&&deviat_value<=30)      data=0x06;              //右转小角度
	else if(deviat_value>30)     data=0x07;                               //右转大角度
	else if(deviat_value<-8&&deviat_value>=-30)  data=0x08;              //左转小角度
	else if(deviat_value<-30) 	data=0x09; 															//左转大角度
	else         data=0x05;                                             //前进		
	return data;	
}


//u8 veichel_contr(float rencent_angle,float taget_angle)
//{
//	
//	float deviat_value;       //目标角度与当前小车角度的偏差值
//	u8 data;									//返回的控制值
//	deviat_value=deviat_value=angle_deviat_judge(rencent_angle,taget_angle);
//	if(deviat_value<=10&&deviat_value>=-10)			data=0x05;      //前进
//	else if(deviat_value>10&&deviat_value<=30)      data=0x08;              //右转小角度
//	else if(deviat_value>30)     data=0x09;                               //右转大角度
//	else if(deviat_value<-10&&deviat_value>=-30)  data=0x06;              //左转小角度
//	else if(deviat_value<-30) 	data=0x07; 															//左转大角度
//	else         data=0x05;                                             //前进		
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


