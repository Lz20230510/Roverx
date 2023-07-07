#ifndef __BRANCHPATH_H
#define __BRANCHPATH_H
#include "sys.h"

////////////////////////////////分支路径功能///////////////////////////// 
//根据人机界面上的分支路径界面制作分支路径功能
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////



#define UniverVal 254         //路径中的通用参数值

//////////////////////////////////////////
//分支路径模式控制结构体
//
//
typedef struct
{
	u8 selectPathNum;   //系统选定的路径编号，用于指定小车路径
	u8 pathNum;					//路径号
	u8 cardNum;					//获取到的当前路径处于当前路径上的卡号
	u8 statnCardNum;		//站点卡号
	u8 stopMode;				//停车模式
	u8 stopModeStor;		//停车模式临时存储
	u8 restartDir;			//重启方向
	u8 restatrTime;			//重启时长
	u8 operationCmd;		//动作指令
	u8 operationCmdStor;		//动作指令临时存储
	u8 spdCmd;					//速度指令
	u8 turnoffCmd;			//岔道指令
	u8 turnoffCmdStor;			//岔道指令临时存储
	u8 obstacleCmd;				//避障指令
	u8 jumpCmd;						//路径跳转指令
	u8 runDir;						//执行方向
	u8 uart4Mark;						//uart4中断标志
	u8 error;							//分支路径数据获取错误					=0：站点号异常     =1：站点号非异常
}BranchPathDef;					
extern BranchPathDef BranchPath;




u8 AGVPathJump_Cmd(u8 JumpCmd,u8 selectPathNum);																									//AGV路径跳转指令
void AGVStop_mode(u8 stopmode,u8 timeMark,u8 timeStart);												//AGV停止模式
void AGVRestart_Director(u8 runDirector,u8 timeMark,u8 timeStart);							//AGV自启方向
u8 AGVOperat_Cmd(u8 OperatCmd);																								//AGV动作指令
void AGVSpd_Cmd(u8 SpdCmd);																											//AGV速度指令												
void AGVTurnoff_Cmd(u8 turnoffCmd);																							//AGV岔道指令
BranchPathDef  BranchPath2_struct(u16 screenID,u16 controlID,u16 cardNum,u8 selectPathNum);			//将获取到的分支路径数据转换为分支路径结构体
void StatnNum_Search(u16 StationCardNum_cardNum,u8 selectPathNum);						//搜索当前的卡号对应的分支路径数据
void SignalBranchPath_Station(void);									//单独的分支路径模式下一个站点对应的整套功能																		
#endif




