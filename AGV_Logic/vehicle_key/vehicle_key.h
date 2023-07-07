#ifndef __VEHICLDE_KEY_H
#define __VEHICLDE_KEY_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 


/*下面的方式是通过直接操作库函数方式读取IO*/
#define IN_AUTOSetup 			!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12) 		//设备启动按钮信号
#define IN_JOGSetup				!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)			//遥控启动
#define IN_EmerStop 	    !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) 		//急停按钮信号


typedef	struct
{
	u8 SONSetup;				//小车使能就绪			1使能启动    0使能未启动
	u8 Setup;						//小车启动标志位
	u8 AJMode;					//小车手自动状态		1自动状态    0手动状态
	u8 AUTOmode;        //小车自动模式     0自动模式未启动  1室内自动模式   2室外自动模式 
	u8 TeachMode;        //小车示教模式	  0关闭示教模式  	1打开示教模式
	u8 MapDownMode;			//地图下载模式   0下载模式未启动  1正在下载过程中 2下载完成
	u8 EmerStop;				//小车急停状态  1不处于急停状态    0急停状态
	u8 SevAlm[4];			  //伺服驱动器故障 1驱动器故障   0驱动器无故障    [0]=1:伺服1报警  [1]=1:伺服2报警 [2]=1:伺服3报警   [3]=1:伺服4报警
	u8 AutoRun;					//自动模式运行   =0：自动运行状态未启动     =1：自动运行状态启动
	u8 BatteryCap;      //AGV电池容量
	u8 temp;						//临时测试
	u8 SLAMAutoMark;		//=1启动一次SLAM控制  =0，关闭
	float Wheel_R;               //车轮直径
	float pi1;                 	//圆周率
	float vehicle_width;					//小车左右轮间距
	float vehicle_longth;				//小车前后轮间距
	
}AGVStatusDef;

extern AGVStatusDef AGVStatus;

extern u8 AGVModeChange;


u8 Vehkey_scan(u8 mode1);
void AGVsta_Scan(u8 mode2);
void Vehicle_key_Init(void);
void Vehicle_key_Init(void);
void KEYEXTIX_Init(void);

#endif
