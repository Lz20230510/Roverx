/******************************************************************************
* 文件名：HEAD.c
* 版  本：
* 作  者：
* 日  期：
* 库版本：   
* 文件描述：调用所有头文件
*******************************************************************************/
#include "HEAD.h"
#include "I_O.h"
#include "FreeRTOSDef.h"
#include "task.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "FreeRTOSDef.h"
#include "cantimer.h"
#include "cancan.h"
#include "dcf.h"
#include "TestMaster.h"
#include "Motor.h"


#include "vehicle_key.h" 
#include "AGV_contr.h" 
#include "HMI_cmd.h" 
#include "24l01.h" 
#include "usart.h" 
#include "usart6.h" 
#include "gps.h"
#include "East_nor.h" 
#include "myiic.h" 
#include "pid.h" 
#include "map.h" 
#include "RAD.h" 
#include "usart3.h" 
#include "BranchPath.h"
#include "StationCardNum.h"
#include "magnetic.h"
#include "RFID.h"
#include "timer1.h"
#include "vehicle.h" 
#include "mpu9250.h"


IOStatusDef IOStatus;     //离散量GPIO状态



u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP接收数据缓冲区
const u8 *udp_demo_sendbuf="1-------1------1\r\n";//UDP发送数据内容
u8 udp_flag;							//UDP数据发送标志位



//////////操作系统其它任务句柄定义///////////
TaskHandle_t StartTask_Handler;
TaskHandle_t LED0Task_Handler;
TaskHandle_t IOSCANTask_Handler;
TaskHandle_t CANOPENTask_Handler;
TaskHandle_t UDP_SENDTask_Handler;
TaskHandle_t HMI_FLASHTask_Handler;		//HMI FLASH
TaskHandle_t JOGAUTOMODETask_Handler;


AGVStatusDef AGVStatus;						//移动机器人状态信号		
DacaiButtonDef DacaiButton;				//大彩屏按钮信号
SymaStatusDef SymaStatus;					//遥控器状态信号
//BatteryMsgDef BatteryMsg;					//电池参数信息
TimeAddDef Tim1Add,Tim7Add,Tim6Add;												//time1定时器结构体存储
CRC16Def CRC16RFID,CRC16Wireless,CRC16Mag;				//CRC16校验
InitMagDateDef InitMagDate;								//磁导航原始数据结构体
Smc345Def Smc345;													//陀螺仪数据
UltrosonicDef Ultrosonic;									//超声波控制
MagneticDef Magnetic;

u8 cmd_vehicle=0;                //上位机发送给机器人的控制号
float spd_1=0;
double O_OA=0,O_OB=0;               			 //当前系统默认的东北坐标系下原点坐标值
double lati_x0=0,lati_y0=0;                //经度方向下原点的x,y值
double longti_O=0,lati_O=0;                		//原点点经度，维度值度格式

double longti_OA,lati_OB,x0_y0[2];
float cour_angle[2];							//获得的路径上最近点的航向角
double OAOB[2];           //某一点的OA，OB坐标值
double lontlat_du[2];       //存储经纬度度格式
double  recent_near_pot[2];          //距离小车最近的点
u16 near_pot[2];              //搜索到最近的点[0] 位存储
u8 Wakeup_position_re=0;		//初始位置计算反馈值
float Scan_nearpot_re=0;					//小车在路径坐标还是右边的反馈值
float course_weighting_re;

nmea_msg gpsx;   
double lonti_lati[2];     //存储当前实时GPS位置

float angle_deviat=293;             //导航角矫正值

float a1[3]={0,0,0};
float w1[3]={0,0,0};
float h1[3]={0,0,0};
float Angle1[3]={0,0,0};

float Ea_Ed[4],EV[2];				//PID数据处理偏转角等


u8 RFID_LeftRight=1;											//RFID左右转信号,=1左转，=2右转
u32 RFID_value=0;													//RFID数据值
u16 AGVSpd_IN;														//PID调节基础速度
float AGVSpd_OUT;													//室外自动AGV速度
PIDtypedef front_pid;											//front_pid设定:Kp,Ki,Kd,SetPoint,
PIDtypedef rear_pid;											//behind_pid设定:Kp,Ki,Kd,，SetPoint
BranchPathDef BranchPath;									//分支路劲模式结构体
StationCardNumDef StationCardNum;					//站点卡号结构体
VehicleParamtDef VehicleParamt;						//AGV向上位机发送的数据
MapPotDef MapPot,MapPotDsiplay;													//东北天坐标OAOB值
AGVStatusDef AGVStatus;											//AGV状态
LontLatiDef LontLati;											//GPS相关参数
MotorMsgDef MotorMsg;		//电机数据结构体

u8 JOG_mark=0;					//首次进入手动控制
u8 LCD_mark=0;
u8 Ask_Battery[7]={0xDD,0xA5,0x03,0x00,0xFF,0xFD,0x77};				//询问电池剩余电量

//u8 Ask_Battery[14]={0x3A,0x30,0x30,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x65,0x65,0x36,0x7E};
//Ask_Battery[14]={0x3A,0x30,0x30,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x65,0x65,0x36,0x7E};				//询问电池剩余电量
/****************************AGV所有参数初始化*****************************/
//void AGV_Init(u16 currntScreenID)
//输入参数：u16 currntScreenID： 当currntScreenID=0x00--0x28     当currntScreenID=0xFF,则说明是所有参数初始化装载
//输出参数：
//返回参数：
/************************************************************************/
void AGV_Init(u16 currntScreenID)
{
	switch(currntScreenID)
	{
		case 0x24:						//运行参数1
		case 0x27:						//AGV基本定义
		{
			if(currntScreenID==0x24)					
			{

			}
			else if(currntScreenID==0x27)
			{

			}		
		}break;
		case 0x25:						//运行参数2界面
		{

		}break;
		case 0xFF:						//AGV所有参数初始化装载
		{

					
		}break;		
		default:break;
	}
}

//无线串口通信,GPS,      USART1
//


