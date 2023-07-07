#include "AGV_contr.h" 
#include "HEAD.h"
#include "gps.h" 
#include "East_nor.h" 
#include "map.h" 
#include "HEAD.h"
#include "RAD.h" 
#include "usart6.h"
#include "usart3.h"
#include "mpu9250.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化

u8 test4=0;

void SON1234_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//使能  GPIOE时钟
	
	 //GPIOE8初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz	`
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化	
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);//GPIOE8设置高，灯灭
}


void Sov_on_1234_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOD,GPIOE时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_15; //驱动0 1 15引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD0,1,15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //驱动7引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE7

}



/************************移动机器人室内自动模式相关控制*********************/
//函数形式：void Inside_Auto(void)			//
//功能描述：室内自动模式相关函数
//输入参数：
//输出参数：
//返回参数：
//
//
/*************************************************************************/
void Inside_Auto(void)	
{
	u8 static add;
//	InitMagDate=magnetic_recei(USART3_RX_BUF,Mag_date,Mag_alarm);	//磁导航数据读取
	 
	Station_CardNum(RFID_value);
	if(BranchPath.error==1)	SignalBranchPath_Station();																		//AGV通过RFID卡完成部分功能	
	
			
	if(AGVStatus.AutoRun==1 && AGVStatus.AUTOmode==1 && add>=20 )
	{
//		EV_calcu(&front_pid,&rear_pid,Ea_Ed,EV,Mag_date,1,AGVSpd_IN);	//允许自动寻迹
		add=0;
	}		
	
	if(AGVStatus.AutoRun==0||Mag_date[1]==0) Speed_contr(0.0,0.0,0.0,0.0,0);   //当启动运行关闭时，或者无磁条信号时，小车停止
	delay_ms(10);
		add++;
}
/*************************移动机器人室外自动模式相关控制*********************/
//函数形式：void Outside_Auto(void)			//
//功能描述：室外自动模式相关函数
//输入参数：
//输出参数：
//返回参数：
//
/*************************************************************************/

void Outside_Auto(void)	
{	
	u8 static firstMark=0;
			if(!firstMark) First_GPS_Msg();						//上电后首次开启GPS室外导航，读取数据
test4=1;
			SCM_345_contralCmd(5,Smc345.scm345_cmd);
			delay_ms(50);
test4=2;
			if(USART3_RX_STA&0x8000)																//GPS数据读取
			{							
				GPS_Analysis(&gpsx,(u8*)USART3_RX_BUF);    						//分析数据			
				Gps_Msg_Show(gpsx,lonti_lati);            						 //输出数据（输出当前移动机器人的实时位置值度格式	
				USART3_RX_STA=0; 
			}
test4=2;
			longti_lati_trans(0,lonti_lati[0],lonti_lati[1],x0_y0,lontlat_du); 					//求出经度方向的x,y值			
			MapPot.longti_du=lontlat_du[0];			//获取经度度格式数据
			MapPot.lati_du=lontlat_du[1];				//获取维度度格式数据
test4=3;
			OAOB_caculate(lati_x0,lati_y0,x0_y0[0],x0_y0[1],longti_O,lontlat_du[0],OAOB);		//东北坐标系下计算某点相对于原点O的坐标值	
						
			if(OAOB[0]<400&&OAOB[0]>-400&&OAOB[1]<400&&OAOB[1]>-400)    //防止数据异常
			{
//			Wakeup_position(OAOB[0],OAOB[1],OA,OB,OAOB_angle,cour_angle,near_pot);       //检测小车在与不在路径内，并采取不同的控制策略			
				Scan_nearpot_re=Scan_nearpot(OAOB[0],OAOB[1],near_pot,45);         //运行中寻找路径上最近的点，并求出矢量距离
								
				MapPot=MapPot_Find(near_pot[0]);					//寻找电子地图的对应点
				recent_near_pot[0]=MapPot.OA;
				recent_near_pot[1]=MapPot.OB;

				course_weighting_re=course_weighting(near_pot,Scan_nearpot_re);					//求出移动机器人的偏转角度				
				cmd_vehicle=veichel_contr(Smc345.scm345Angle,course_weighting_re);     //小车控制													
			}	
			if(AGVStatus.AutoRun==1 /*&& MapPot.starNum>5*/)
			{
				auto_vehicle_contr(MapPot.AutoSpd,cmd_vehicle);						//自动模式小车控制
			}
			else Speed_contr(0.0,0.0,0.0,0.0,0);   		//当启动运行关闭时，小车停止
			
			vehicle_upload(OAOB[0],OAOB[1],Smc345.scm345Angle,lonti_lati[0],lonti_lati[1],spd_1,MapPot.OA,MapPot.OB,near_pot[0]);		//上传室外导航数据lonti_lati：GPS度格式
			JOG_mark=0;		

			
			firstMark=1;	//首次读取标志位置1
}

/*************************移动机器人手动模式相关控制*********************/
//函数形式：void JogMode_control(void)	//
//功能描述：室外自动模式相关函数
//输入参数：
//输出参数：
//返回参数：
/*************************************************************************/
void JogMode_control(void)	
{
	
			if(JOG_mark==0)											//初次进如手动模式停车
			{
				Vehicle_contr_Jog(0,0);           //手动速度给定停止
				Vehicle_contr_Auto(0,0,0);				//自动速度给定停止	
				JOG_mark=1;
			}	
			if(USART3_RX_STA&0x8000)																//GPS数据读取
			{			
				GPS_Analysis(&gpsx,(u8*)USART3_RX_BUF);    						//分析数据			
				Gps_Msg_Show(gpsx,lonti_lati);            						 //输出数据（输出当前移动机器人的实时位置值度格式
				USART3_RX_STA=0;  
			}
			
			longti_lati_trans(0,lonti_lati[0],lonti_lati[1],x0_y0,lontlat_du); //求出经度方向的x,y值
			OAOB_caculate(lati_x0,lati_y0,x0_y0[0],x0_y0[1],longti_O,lontlat_du[0],OAOB);		//东北坐标系下计算某点相对于原点O的坐标值	
//			True_angle=MPU9250_data_receive(a1,w1,h1,Angle1,a2);    							//MPU9250数据读取		
//			vehicle_upload(OAOB[0],OAOB[1],Smc345.scm345Angle,lonti_lati[0],lonti_lati[1],spd_1,mapPot.OA,mapPot.OB,near_pot[0]);		//上传室外导航数据lonti_lati：GPS度格式	


}


/*******************************************************************/
//函数形式：void TeachMode_control(void)	
//输入参数：
//输出参数：
//返回参数：
//功能描述：AGV手动示教模式
/*******************************************************************/
void TeachMode_control(void)											//示教模式
{
	u8 static timstor;
	static MapPotDef mapPot;
	if(AGVStatus.TeachMode==1&&AGVStatus.AJMode==0)				//AGV示教模式打开，且处于手动模式
	{
		SCM_345_contralCmd(5,Smc345.scm345_cmd);
		delay_ms(100);
		if(USART3_RX_STA&0x8000)																//GPS数据读取
		{							
			GPS_Analysis(&gpsx,(u8*)USART3_RX_BUF);    						//分析数据			
			Gps_Msg_Show(gpsx,lonti_lati);            						 //输出数据（输出当前移动机器人的实时位置值度格式	
			USART3_RX_STA=0; 
		}
		longti_lati_trans(0,lonti_lati[0],lonti_lati[1],x0_y0,lontlat_du); 					//求出经度方向的x,y值
		mapPot=MapPot_Find(near_pot[0]);														//寻找对应点
		
		if(AGVStatus.TeachMode==1&&timstor!=Tim1Add.reMark) vehicle_upload(OAOB[0],OAOB[1],Smc345.scm345Angle,lonti_lati[0],lonti_lati[1],spd_1,mapPot.OA,mapPot.OB,near_pot[0]);		//上传室外导航数据lonti_lati：GPS度格式	,每0.5秒上传一次

		timstor=Tim1Add.reMark;
		
	}
}

/*******************************************************************/
//函数形式：void MapDownLoad_control(void)	
//输入参数：
//输出参数：
//返回参数：
//功能描述：电子地图下载处理控制
/*******************************************************************/
void MapDownLoad_control(void)	
{
	union MultiType multiType;
	switch(MapPot.mode)
	{
		case 0: break;					//
		case 1:									//电子地图下载		
		{
					//Map_Save2FLASH(MapPot,FloatSize);		//地图数据存储	
			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//当存储区数据为 0xFFFFFFFF,才能存储地图数据
			MapPot.mode=0;						//模式关闭，不在存储
		}break;
		case 2:									//电子地图参数存储区格式化
		{
			SetScreen(25);														//初始化完成，切换画面初始化
			STMFLASH_Clear(MAPOA_Addr,1);								//OA存储区总清
			STMFLASH_Clear(MAPOB_Addr,1);								//OB存储区总清
			STMFLASH_Clear(MAPAngle_Addr,1);								//angle存储区总清
			SetScreen(4);														//人机界面初始化完成，切换回参数运行3
			MapPot.mode=0;						//模式关闭，不在存储
		}break;
		case 3:									//电子地图初始数据存储
		{	
			multiType.u32Array_[0]=Paramt_BUFF[Pg_OutsideParmt][1];				//获取参数缓冲区经度数据低8位
			multiType.u32Array_[1]=Paramt_BUFF[Pg_OutsideParmt][2];				//获取参数缓冲区经度数据高8位
			SetTextValueFloat(Pg_OutsideParmt,1,multiType.double_,_double);	//发送数据到HMI
				
			multiType.u32Array_[0]=Paramt_BUFF[Pg_OutsideParmt][3];				//获取参数缓冲区经度数据低8位
			multiType.u32Array_[1]=Paramt_BUFF[Pg_OutsideParmt][4];				//获取参数缓冲区经度数据高8位
			SetTextValueFloat(Pg_OutsideParmt,3,multiType.double_,_double);	//发送数据到HMI
				
			SetTextValueu16(Pg_OutsideParmt, 10,(u16)Paramt_BUFF[Pg_OutsideParmt][10]);    //发送电子地图总的点数量
			SetTextValueu16(Pg_OutsideParmt, 11,(u16)Paramt_BUFF[Pg_OutsideParmt][11]);    //发送电子地图每个线段上点的数量
				
			SetScreen(25);														//初始化完成，切换画面初始化
			Buff2_Flash(Pg_OutsideParmt,120);															//批量存储某一页的参数到FLASH
			SetScreen(CurrtScreenID);														//人机界面初始化完成，切换回当前页面

			MapPot.mode=0;						//模式关闭，不在存储
		}break;
		default:break;
		}

}


/*********************************移动机器人参数初始化*********************/
//函数形式：void AGVParamter_Init()
//输入参数：
//输出参数：
//返回参数：
//
//
/*************************************************************************/
void AGVParamter_Init(void)
{
	double temp[2],temp1[2];
	union MultiType multiType;
	
	//遥控参数初始化
	SymaStatus.Spd_cle[0]=1.0;											//速度设定初始值为1米/秒 
	SymaStatus.Spd_cle[1]=2.5;											//转弯半径初始值设定为2.5米

	AGVStatus.Wheel_R=0.318;                //车轮直径
	AGVStatus.pi1=3.1415926;								//你懂得
	AGVStatus.vehicle_width=0.587/2;						//小车左右轮间距一半
	AGVStatus.vehicle_longth=0.412;						//小车前后轮间距
	OT_SON=0;											//启动时强制断使能
	
	//陀螺仪参数初始值
	Smc345.scm345_cmd[0]=0x68;
	Smc345.scm345_cmd[1]=0x04;
	Smc345.scm345_cmd[2]=0x00;
	Smc345.scm345_cmd[3]=0x03;
	Smc345.scm345_cmd[4]=0x07;
	
//	TIM_SetCompare1(TIM3,0);     //TIM2通道1占空比为100/500    	初始状态电机转速为0
//	TIM_SetCompare2(TIM3,0);     //TIM2通道2占空比为200/500			初始状态电机转速为0
//	TIM_SetCompare3(TIM3,0);     //TIM2通道3占空比为300/500			初始状态电机转速为0
//	TIM_SetCompare4(TIM3,0);     //TIM2通道4占空比为400/500			初始状态电机转速为0
		
	Battery_Send(7,Ask_Battery);							//向电池发送数据
	front_pid=AGVPID_Change_FLASH(0);		//PID组1  前驱动
	AGVSpd_IN=front_pid.AGPspd;
	
	Ultrosonic.deviceEn=Paramt_BUFF[17][5];					//避障状态赋值
	
	MapPot.numFx= LontLati.totalPotNum/LontLati.linePotNum;					//拟合线段数量赋值
	MapPot.numPot=LontLati.linePotNum;															//每个拟合线段上点的数量
	multiType.u32_=Paramt_BUFF[Pg_OutsideParmt][1];				//获取参数缓冲区经度数据低8位

	
	
	longti_lati_trans(0,longti_O,lati_O,temp1,temp);	
	LontLati= Origin_O_parmt_Init();				//GPS 原点信息读取
	MapPot.AutoSpd=LontLati.spdTemp;				//自动运行速度


	Ultrosonic.deviceEn=(u8)Paramt_BUFF[Pg_CommonParmt][5];						//避障传感器是否打开


	VehicleParamt.fir_fram="FR";			//数据上传首帧
	VehicleParamt.last_fram="OR";			//数据上传尾朕

}