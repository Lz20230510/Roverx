#include "HEAD.h"

/**************************停车模式功能*************************/
//AGVStop_mode(u8 stopmode)
//输入参数：u8 stopmode：停车模式；=1：按时间停止     =2：按外部停车标志停止
//输入参数：TimeAddDef timeadd：定时器结构体
//返回参数：TimeAddDef
/***************************************************************/
void AGVStop_mode(u8 stopmode,u8 timeMark,u8 timeStart)
{
	u8 i;
	switch(stopmode)
	{
		case 0:break;							//不停止
		case 1:										//按时间停止
		{		
			if(Tim7Add.mark[1]==1/*&&Tim7Add.start[1]==1*/)					//当定时器计算时间完成
			{
				
			}		
		}break;	
		case 2:											//按外部停车标志停止
		{	
		}break;
		case 3:											//立即停止
		{			
			AGVStatus.AutoRun=0;					//自动运行关闭
			Speed_contr(0.0,0.0,0.0,0.0,0);   //小车运行速度置为零		
		}break;
		case 4:											//立即停止    休息点
		{

		}break;
		case 5:                     //询问是否允许小车进入   呼叫器范围内
		{
			
		}
		default:break;
	}
//		if(stopmode==4&&Tim7Add.start[2]!=1)							//当不处于呼叫器启动8秒内，且停止模式4(休息点停车)
//		{
//	
//		}			
//		else AGVWorkStatus.busy=0;			//AGV非限制状态状态，等待命令启动
		BranchPath.stopModeStor=BranchPath.stopMode;						//停车临时之存储
}



/**************************自启方向************************/
//void AGVRestart_Director(u8 runDirector,u8 restartMark)
//输入参数：u8 runDirector：自启动方向
//runDirector=0:不自启    runDirector=1: 前进启动                           
//runDirector=1: 后退启动 
//u8  timeMark:  重启标记  =0：不重启    =1：重启
//输出参数：
//返回参数：TimeAddDef
/***************************************************************/
void AGVRestart_Director(u8 runDirector,u8 timeMark,u8 timeStart)
{
	switch(runDirector)
	{
		case 0:break;					//不自启
		case 1:							//前向自启动
		{	
			if(/*timeStart==1&&*/timeMark==1)	
			{				
				AGVStatus.AutoRun=1;					//自动运行启动
				PID_ParometerRest(front_pid,rear_pid,EV);      //PID 强制复位
			}		
			
		}break;
		case 2:								//后向自启动
		{
//			if(/*timeStart==1&&*/timeMark==1)
//			{
//				Direction =	2;			//方向为后退
//				Back_start = 1;			//后退启动
//				For_start = 0;			//前进启动
//				PID_ParometerRest(front_pid,rear_pid,EV);      //PID 强制复位

//			}				
		}break;
		default:break;
	}
		
}	


/**************************动作指令***********************/
//u8 AGVOperat_Cmd(u8 OperatCmd)
//输入参数：u8 OperatCmd：动作指令
//OperatCmd=0:无动作  =1:AGV路径锁定开始  =2: 顶杆状态取反  =3：顶杆上升  =4：顶杆下降
//=5:等待启动信号   =6：AGV充电请求   =7：AGV下料请求   =8：AGV上料请求   99：驱动升降状态取反
//runDirector=1: 后退启动       
//输出参数：
//返回参数：
/***************************************************************/
u8 AGVOperat_Cmd(u8 OperatCmd)
{
	u8 date;
	switch(OperatCmd)
	{
		case 0:break;						//无动作指令
		case 1:									//AGV路径锁定开始 
		{			
		}break;
		case 2:									//顶杆状态取反
		{

		}break;
		case 3:									//顶杆上升
		{

		}break;
		case 4:									//顶杆下降
		{

		}break;
		case 5:									//AGV充电请求 
		{
		
		}break;
		case 6:									//AGV充电请求 
		{
		
		}break;
		case 7:									//AGV下料请求  
		{
																				//AGV已经到达上料位临时值
		}break;
		case 8:																																	//AGV上料请求 
		{
																				//AGV已经到达上料位临时值
		}break;
		case 9:																																	//AGV已经离开
		{

		}break;
		case 99:									//驱动升降状态取反
		{
		}break;
		default:break;
	}
	BranchPath.operationCmdStor=BranchPath.operationCmd;				//动作指令临时存储
	return date;
}

/**************************速度指令***********************/
//void AGVSpd_IN_Cmd(u8 SpdCmd)
//输入参数：u8 SpdCmd：速度指令
//u8 SpdCmd：无速度指令 =1-9：对应参数表上的速度1-9
//=10:AGV减速，为下一个站点停止做好提前准备					
//=11：AGV必停（AGV到达该站点后无论该站点是否是目标站点AGV都将停在此处）
//输出参数：
//返回参数：
/***************************************************************/
void AGVSpd_IN_Cmd(u8 SpdCmd)
{
	switch(SpdCmd)
	{
		case 0:break;						//无速度指令
		case 1:									//速度1指令		
		{	
			front_pid=AGVPID_Change_FLASH(1);		//PID组1  前驱动
			AGVSpd_IN=front_pid.AGPspd;
		}break;	
		case 2:									//速度2指令		
		{
			front_pid=AGVPID_Change_FLASH(2);		//PID组2  前驱动
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 3:									//速度3指令		
		{
			front_pid=AGVPID_Change_FLASH(3);		//PID组3 前驱动
		AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 4:									//速度4指令		
		{
			front_pid=AGVPID_Change_FLASH(4);		//PID组4  前驱动
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 5:									//速度5指令		
		{
			front_pid=AGVPID_Change_FLASH(5);		//PID组5  前驱动
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		default:break;
	}
}



/**************************岔道指令***********************/
//void AGVTurnoff_Cmd(u8 turnoffCmd)
//输入参数：u8 SpdCmd：速度指令
//u8 SpdCmd：无速度指令 =1-9：对应参数表上的速度1-9
//=10:AGV减速，为下一个站点停止做好提前准备					
//=11：AGV必停（AGV到达该站点后无论该站点是否是目标站点AGV都将停在此处）
//输出参数：
//返回参数：
/*******************************************************/
void AGVTurnoff_Cmd(u8 turnoffCmd)
{
	switch(turnoffCmd)
	{
		case 0:RFID_LeftRight=0;break;								//无岔道指复位
		case 1:RFID_LeftRight=0;break;		//前进		//岔道指复位
		case 2:break;											//岔道询问		
		case 3:break;											//岔道指令执行开始
		case 4:RFID_LeftRight=2;break;		//左转	//岔道左转执行
		case 5:RFID_LeftRight=1;break;		//右转										//岔道右转执行
		case 6:break;											//岔道询问动作执行
		case 7:break;											//十字路口顺时针转90度后前进
		case 8:break;											//十字路口顺时针转90度后后退
		case 9:break;											//十字路口逆时针转90度后前进
		case 10:break;										//十字路口逆时针转90度后后退
		case 11:break;										//低压左转执行
		case 12:break;										//低压右转执行
		case 13:											//前方十字路口指令
		{
//			AGVSpd_IN=AGVSPD_Change_FLASH(6);				//第六组速度参数
//			front_pid=AGVPID_Change_FLASH(6);			//前驱动PID   第六组PID参数
//			rear_pid=AGVPID_Change_FLASH(6);			//后驱动PID   第六组PID参数	
		}break;										
		default:break;
	}
	BranchPath.turnoffCmdStor=turnoffCmd;			//岔道临时值存储
}



/**************************跳转指令****************************/
//u8 AGVPathJump_Cmd(u8 JumpCmd,u8 selectPathNum)
//输入参数：u8 JumpCmd: 需要跳转的路径   u8 selectPathNum：之前选择的路径  
//输出参数：
//返回参数：
/***************************************************************/
u8 AGVPathJump_Cmd(u8 JumpCmd,u8 selectPathNum)
{
	if(JumpCmd!=0&&JumpCmd!=UniverVal)return JumpCmd;	
	else return selectPathNum;
}

/******将当前卡号获取到的分支路径数据赋值给分支路径结构体*******/
//BranchPathDef  BranchPath2_struct(u16 screenID,u16 controlID,u16 cardNum,u8 selectPathNum)
//输入参数：u16 screenID：页面号  u16 controlID:控件号  u16 cardNum:站点卡号结构体
//u8 selectPathNum:选择的路径号
//输出参数：
//返回：
//功能说明：
/***************************************************************/
BranchPathDef  BranchPath2_struct(u16 screenID,u16 controlID,u16 cardNum,u8 selectPathNum)
{
	BranchPathDef static branchPath;
	branchPath.selectPathNum=selectPathNum;				//选择的路径号赋值
	branchPath.cardNum=cardNum;																//当前所选择的处于路径上的卡号
	branchPath.pathNum=Paramt_BUFF[screenID][controlID-1];											//路径编号赋值
	branchPath.statnCardNum=Paramt_BUFF[screenID][controlID];										//站点编号
	branchPath.stopMode=Paramt_BUFF[screenID][controlID+1];											//停车模式
	branchPath.restartDir=Paramt_BUFF[screenID][controlID+2];										//自启方向
	branchPath.restatrTime=Paramt_BUFF[screenID][controlID+3];									//自启时间
	branchPath.operationCmd=Paramt_BUFF[screenID][controlID+4];									//动作命令
	branchPath.spdCmd=Paramt_BUFF[screenID][controlID+5];												//速度指令
	branchPath.turnoffCmd=Paramt_BUFF[screenID][controlID+6];										//岔道指令
//	branchPath.obstacleCmd=Paramt_BUFF[screenID][controlID+6];									//障碍指令
	branchPath.jumpCmd=Paramt_BUFF[screenID][controlID+7];											//跳转指令
//	branchPath.runDir=Paramt_BUFF[screenID][controlID+9];												//执行方向
	return branchPath;
}


/*********************搜索当前分支路径有无对应的卡号****************/
//void StatnNum_Search(u16 StationCardNum_cardNum,u8 pathNum,u8 selectPathNum)
//输入参数：u16 StationNum：从站点卡号界面获取的站点号
//u8 selectPathNum:选择的路径号
//输出参数：
//返回：当前卡号下对应的分支路径数据结构体
//功能说明：
/***************************************************************/
void StatnNum_Search(u16 StationCardNum_cardNum,u8 selectPathNum)
{
	u16 firstID,lastID;					//页面第一个控件号，页面最后一个控件号
	u16 i,j;										//

																													//重启时长
	if(StationCardNum.error!=0&&StationCardNum.StationCardNumstor!=StationCardNum.cardNum)																			//当索取到卡号无异常
	{
		for(j=Pg_InsideMap11;j<=Pg_InsideMap15;j++)								//搜索当前的RIFD号对应的站点编号
		{		

			firstID=2;						//第一个站点编号
			lastID=83;						//最后一个站点编号
			for(i=firstID;i<=lastID;i=i+9)
			{					
	//			if(Paramt_BUFF[j][i-1]==selectPathNum||Paramt_BUFF[j][i-1]==UniverVal)					//当路径编号正确
				if(Paramt_BUFF[j][i]==StationCardNum.stationNum)																	//当检测到有与当前RFID对应的站点号
				{
	//				if(Paramt_BUFF[j][i]==StationCardNum.stationNum)																	//当检测到有与当前RFID对应的站点号
					if(Paramt_BUFF[j][i-1]==selectPathNum||Paramt_BUFF[j][i-1]==UniverVal)					//当路径编号正确
					{							
						BranchPath=BranchPath2_struct(j,i,StationCardNum.cardNum,selectPathNum);													//获取当前卡号的分支路径数据
						BranchPath.error=1;																														//获取到的分支路径数据不异常							
						goto pos1;										//完成及跳转
					}
					else
					{
						BranchPath.error=0;																//返回0说明异常			
					}		
				}										
			}

		}		
	}	
	pos1:        //跳转点
	StationCardNum.StationCardNumstor=StationCardNum.cardNum;													//读取到的真事RFID卡号
}


/******单独的分支路径模式下一个站点对应的整套功能*******/
//void SignalBranchPath_Station(void)
//输入参数：StationNum：站点号      u8 SelectPathNum:指定的路径编号
//输出参数：
//返回：
//功能说明：
/***************************************************************/
void SignalBranchPath_Station(void)
{
	static u16 cardValueStor;															//上一次存储的RFID卡号
	union MultiType multitype;   	

	if(/*cardValue!=0*/1)																//当获取的卡号不为0时
	{
		if(/*BranchPath.cardNum!=cardValueStor||*/BranchPath.uart4Mark==1) 					//检查RFID是否有变化	
		{					
			switch(BranchPath.stopMode)
			{
				case 0:break;
				case 1:									//当停车模式为延时启动模式
				{
//					multitype.u32_=Paramt_BUFF[37][19];								//运行参数2   19号控件
//					if(multitype.float_<1.f) multitype.float_=1.f;					//当小于时，赋值为1
//					Tim7Add.time[1]=(u8)multitype.float_;							//AGV停车延时时间
//					Tim7Add.start[1]=1;	
				}break;
				case 2:	break;					//外部信号停止
				case 3:									//AGV停车模式为立即停车
				case 4:									//休息点停车指令
				case 5:									//询问是否停车
				{
					AGVStop_mode(BranchPath.stopMode,0,0);						//停车指令
					multitype.float_=0;			
				}break;
				default:break;	
			}	
			if(BranchPath.restartDir==1||BranchPath.restartDir==2)						//当重启运行方向为前进或者后退
			{
				Time7_Timekeep(Tim7Add.time[4],Tim7Add.tiAdd,Tim7Add.start[4],4,1);				//Tim7[4]计时器			计时器复位	
				if(BranchPath.restatrTime<1)	Tim7Add.time[4]=1;							//如果小于1 ，强制赋值为1
				else Tim7Add.time[4]=BranchPath.restatrTime+(u8)multitype.float_;			//AGV重启延时时间=停车时间+重启等待时间
				
				Tim7Add.start[4]=1;						//延时启动
			}																											//AGV重启延时计时器启动				
			AGVOperat_Cmd(BranchPath.operationCmd);								//AGV动作指令
			AGVSpd_IN_Cmd(BranchPath.spdCmd);												//AGV速度指令	
			AGVTurnoff_Cmd(BranchPath.turnoffCmd);										//岔道指令			
		}
//		if(BranchPath.stopMode==1)AGVStop_mode(BranchPath.stopMode,Tim7Add.mark[1],Tim7Add.start[1]);						//AGV停车模式为延时停车
		AGVRestart_Director(BranchPath.restartDir,Tim7Add.mark[4],Tim7Add.start[4]);			//AGV自启方向	
	}	
	cardValueStor=BranchPath.cardNum;
	BranchPath.uart4Mark=0;
}
