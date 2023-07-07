#ifndef __HEAD_H_
#define __HEAD_H_
#include  "sys.h"
#include "TestMaster.h"
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

//#include "usart2.h"
#include "usart6.h"

#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lan8720.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "udp_demo.h" 
#include "stdio.h"

#include "stm32f4xx.h"
#include  <stdio.h>
#include  <math.h>
#include "string.h"	


#include "HMI_Cmd.h"
#include "cmd_process.h"
#include "vehicle_key.h"
#include "AGV_contr.h" 
#include "gps.h"
#include "map.h"
//#include "myiic.h" 
//#include "Motor.h"
#include "pwm.h"
#include "24l01.h" 
//#include "exti.h"
#include "magnetic.h"
#include "East_nor.h" 
#include "pid.h"
//#include "spi.h"
#include "RFID.h"
#include "timer1.h"
//#include "timer3.h"
#include "usart.h"	
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#include "hmi_driver.h"

#include "stmflash.h"

#include "AGV_contr.h" 

#include "BranchPath.h"
#include "StationCardNum.h"
//#include "vehicle.h" 









#define MotorCount 2             	//定义电机的数量
#define MotorMode 0              	//定义电机的工作模式    0：全部速度   1：全部位置   3:全部电流    4:8轮，4组位置，四组速度模式
#define Pi 3.14159								//圆周率


#define OT_JD1  PDout(6)    		 	//继电器1
#define OT_JD2  PDout(7)     			//继电器2
#define OT_JD3  PDout(11)    			//继电器3
#define OT_JD4  PDout(12)     		//继电器4
	








extern u8 cmd_vehicle;                //上位机发送给机器人的控制号
extern float spd_1;
extern double O_OA,O_OB;               			 //当前系统默认的东北坐标系下原点坐标值
extern double lati_x0,lati_y0;                //经度方向下原点的x,y值
extern double longti_O,lati_O;                //原点点经度，维度值度格式
//临时定义值

extern double longti_OA,lati_OB,x0_y0[2];
extern float cour_angle[2];							//获得的路径上最近点的航向角
extern double OAOB[2];           //某一点的OA，OB坐标值
extern double lontlat_du[2];       //存储经纬度度格式
extern double  recent_near_pot[2];          //距离小车最近的点
extern u16 near_pot[2];              //搜索到最近的点[0] 位存储
extern u8 Wakeup_position_re;		//初始位置计算反馈值
extern float Scan_nearpot_re;					//小车在路径坐标还是右边的反馈值
extern float course_weighting_re;
 
extern double lonti_lati[2];     //存储当前实时GPS位置

extern float angle_deviat;

extern float a1[3],w1[3],h1[3],Angle1[3];








//输出数据：float EV[2]：前后驱动速度偏差值存储  EV[0]：前  EV[1]：后   
//Ea_Ed[0]:前驱动输出的磁导航偏差角    Ea_Ed[1]:前驱动输出的横向偏移量
//Ea_Ed[2]:后驱动输出的磁导航偏差角    Ea_Ed[3]:后驱动输出的横向偏移量
extern float Ea_Ed[4],EV[2]; 		//PID数据处理偏转角等



extern u16 AGVSpd_IN;
extern u8 RFID_LeftRight;				//RFID左右转信号,=1左转，=2右转
extern u8 RFID_stor;					//RFID_stor临时存储值
extern u8 button;						//控制面板按键
extern u8 JOG_mark;					//首次进入手动控制
//extern PIDtypedef front_pid,rear_pid;//四个绝对式PID

/*****************************************************************/
extern u16 CurrtScreenID;					//当前获取的界面ID 号
/****************************************************************/
extern u8 PathTemp;
extern u8 LCD_mark;

void AGV_Init(u16 currntScreenID);							//AGV人机界面参数初始化刷新









#endif		    

