#include "HEAD.h"

/**************************ͣ��ģʽ����*************************/
//AGVStop_mode(u8 stopmode)
//���������u8 stopmode��ͣ��ģʽ��=1����ʱ��ֹͣ     =2�����ⲿͣ����־ֹͣ
//���������TimeAddDef timeadd����ʱ���ṹ��
//���ز�����TimeAddDef
/***************************************************************/
void AGVStop_mode(u8 stopmode,u8 timeMark,u8 timeStart)
{
	u8 i;
	switch(stopmode)
	{
		case 0:break;							//��ֹͣ
		case 1:										//��ʱ��ֹͣ
		{		
			if(Tim7Add.mark[1]==1/*&&Tim7Add.start[1]==1*/)					//����ʱ������ʱ�����
			{
				
			}		
		}break;	
		case 2:											//���ⲿͣ����־ֹͣ
		{	
		}break;
		case 3:											//����ֹͣ
		{			
			AGVStatus.AutoRun=0;					//�Զ����йر�
			Speed_contr(0.0,0.0,0.0,0.0,0);   //С�������ٶ���Ϊ��		
		}break;
		case 4:											//����ֹͣ    ��Ϣ��
		{

		}break;
		case 5:                     //ѯ���Ƿ�����С������   ��������Χ��
		{
			
		}
		default:break;
	}
//		if(stopmode==4&&Tim7Add.start[2]!=1)							//�������ں���������8���ڣ���ֹͣģʽ4(��Ϣ��ͣ��)
//		{
//	
//		}			
//		else AGVWorkStatus.busy=0;			//AGV������״̬״̬���ȴ���������
		BranchPath.stopModeStor=BranchPath.stopMode;						//ͣ����ʱ֮�洢
}



/**************************��������************************/
//void AGVRestart_Director(u8 runDirector,u8 restartMark)
//���������u8 runDirector������������
//runDirector=0:������    runDirector=1: ǰ������                           
//runDirector=1: �������� 
//u8  timeMark:  �������  =0��������    =1������
//���������
//���ز�����TimeAddDef
/***************************************************************/
void AGVRestart_Director(u8 runDirector,u8 timeMark,u8 timeStart)
{
	switch(runDirector)
	{
		case 0:break;					//������
		case 1:							//ǰ��������
		{	
			if(/*timeStart==1&&*/timeMark==1)	
			{				
				AGVStatus.AutoRun=1;					//�Զ���������
				PID_ParometerRest(front_pid,rear_pid,EV);      //PID ǿ�Ƹ�λ
			}		
			
		}break;
		case 2:								//����������
		{
//			if(/*timeStart==1&&*/timeMark==1)
//			{
//				Direction =	2;			//����Ϊ����
//				Back_start = 1;			//��������
//				For_start = 0;			//ǰ������
//				PID_ParometerRest(front_pid,rear_pid,EV);      //PID ǿ�Ƹ�λ

//			}				
		}break;
		default:break;
	}
		
}	


/**************************����ָ��***********************/
//u8 AGVOperat_Cmd(u8 OperatCmd)
//���������u8 OperatCmd������ָ��
//OperatCmd=0:�޶���  =1:AGV·��������ʼ  =2: ����״̬ȡ��  =3����������  =4�������½�
//=5:�ȴ������ź�   =6��AGV�������   =7��AGV��������   =8��AGV��������   99����������״̬ȡ��
//runDirector=1: ��������       
//���������
//���ز�����
/***************************************************************/
u8 AGVOperat_Cmd(u8 OperatCmd)
{
	u8 date;
	switch(OperatCmd)
	{
		case 0:break;						//�޶���ָ��
		case 1:									//AGV·��������ʼ 
		{			
		}break;
		case 2:									//����״̬ȡ��
		{

		}break;
		case 3:									//��������
		{

		}break;
		case 4:									//�����½�
		{

		}break;
		case 5:									//AGV������� 
		{
		
		}break;
		case 6:									//AGV������� 
		{
		
		}break;
		case 7:									//AGV��������  
		{
																				//AGV�Ѿ���������λ��ʱֵ
		}break;
		case 8:																																	//AGV�������� 
		{
																				//AGV�Ѿ���������λ��ʱֵ
		}break;
		case 9:																																	//AGV�Ѿ��뿪
		{

		}break;
		case 99:									//��������״̬ȡ��
		{
		}break;
		default:break;
	}
	BranchPath.operationCmdStor=BranchPath.operationCmd;				//����ָ����ʱ�洢
	return date;
}

/**************************�ٶ�ָ��***********************/
//void AGVSpd_IN_Cmd(u8 SpdCmd)
//���������u8 SpdCmd���ٶ�ָ��
//u8 SpdCmd�����ٶ�ָ�� =1-9����Ӧ�������ϵ��ٶ�1-9
//=10:AGV���٣�Ϊ��һ��վ��ֹͣ������ǰ׼��					
//=11��AGV��ͣ��AGV�����վ������۸�վ���Ƿ���Ŀ��վ��AGV����ͣ�ڴ˴���
//���������
//���ز�����
/***************************************************************/
void AGVSpd_IN_Cmd(u8 SpdCmd)
{
	switch(SpdCmd)
	{
		case 0:break;						//���ٶ�ָ��
		case 1:									//�ٶ�1ָ��		
		{	
			front_pid=AGVPID_Change_FLASH(1);		//PID��1  ǰ����
			AGVSpd_IN=front_pid.AGPspd;
		}break;	
		case 2:									//�ٶ�2ָ��		
		{
			front_pid=AGVPID_Change_FLASH(2);		//PID��2  ǰ����
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 3:									//�ٶ�3ָ��		
		{
			front_pid=AGVPID_Change_FLASH(3);		//PID��3 ǰ����
		AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 4:									//�ٶ�4ָ��		
		{
			front_pid=AGVPID_Change_FLASH(4);		//PID��4  ǰ����
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		case 5:									//�ٶ�5ָ��		
		{
			front_pid=AGVPID_Change_FLASH(5);		//PID��5  ǰ����
			AGVSpd_IN=front_pid.AGPspd;
		}break;
		default:break;
	}
}



/**************************���ָ��***********************/
//void AGVTurnoff_Cmd(u8 turnoffCmd)
//���������u8 SpdCmd���ٶ�ָ��
//u8 SpdCmd�����ٶ�ָ�� =1-9����Ӧ�������ϵ��ٶ�1-9
//=10:AGV���٣�Ϊ��һ��վ��ֹͣ������ǰ׼��					
//=11��AGV��ͣ��AGV�����վ������۸�վ���Ƿ���Ŀ��վ��AGV����ͣ�ڴ˴���
//���������
//���ز�����
/*******************************************************/
void AGVTurnoff_Cmd(u8 turnoffCmd)
{
	switch(turnoffCmd)
	{
		case 0:RFID_LeftRight=0;break;								//�޲��ָ��λ
		case 1:RFID_LeftRight=0;break;		//ǰ��		//���ָ��λ
		case 2:break;											//���ѯ��		
		case 3:break;											//���ָ��ִ�п�ʼ
		case 4:RFID_LeftRight=2;break;		//��ת	//�����תִ��
		case 5:RFID_LeftRight=1;break;		//��ת										//�����תִ��
		case 6:break;											//���ѯ�ʶ���ִ��
		case 7:break;											//ʮ��·��˳ʱ��ת90�Ⱥ�ǰ��
		case 8:break;											//ʮ��·��˳ʱ��ת90�Ⱥ����
		case 9:break;											//ʮ��·����ʱ��ת90�Ⱥ�ǰ��
		case 10:break;										//ʮ��·����ʱ��ת90�Ⱥ����
		case 11:break;										//��ѹ��תִ��
		case 12:break;										//��ѹ��תִ��
		case 13:											//ǰ��ʮ��·��ָ��
		{
//			AGVSpd_IN=AGVSPD_Change_FLASH(6);				//�������ٶȲ���
//			front_pid=AGVPID_Change_FLASH(6);			//ǰ����PID   ������PID����
//			rear_pid=AGVPID_Change_FLASH(6);			//������PID   ������PID����	
		}break;										
		default:break;
	}
	BranchPath.turnoffCmdStor=turnoffCmd;			//�����ʱֵ�洢
}



/**************************��תָ��****************************/
//u8 AGVPathJump_Cmd(u8 JumpCmd,u8 selectPathNum)
//���������u8 JumpCmd: ��Ҫ��ת��·��   u8 selectPathNum��֮ǰѡ���·��  
//���������
//���ز�����
/***************************************************************/
u8 AGVPathJump_Cmd(u8 JumpCmd,u8 selectPathNum)
{
	if(JumpCmd!=0&&JumpCmd!=UniverVal)return JumpCmd;	
	else return selectPathNum;
}

/******����ǰ���Ż�ȡ���ķ�֧·�����ݸ�ֵ����֧·���ṹ��*******/
//BranchPathDef  BranchPath2_struct(u16 screenID,u16 controlID,u16 cardNum,u8 selectPathNum)
//���������u16 screenID��ҳ���  u16 controlID:�ؼ���  u16 cardNum:վ�㿨�Žṹ��
//u8 selectPathNum:ѡ���·����
//���������
//���أ�
//����˵����
/***************************************************************/
BranchPathDef  BranchPath2_struct(u16 screenID,u16 controlID,u16 cardNum,u8 selectPathNum)
{
	BranchPathDef static branchPath;
	branchPath.selectPathNum=selectPathNum;				//ѡ���·���Ÿ�ֵ
	branchPath.cardNum=cardNum;																//��ǰ��ѡ��Ĵ���·���ϵĿ���
	branchPath.pathNum=Paramt_BUFF[screenID][controlID-1];											//·����Ÿ�ֵ
	branchPath.statnCardNum=Paramt_BUFF[screenID][controlID];										//վ����
	branchPath.stopMode=Paramt_BUFF[screenID][controlID+1];											//ͣ��ģʽ
	branchPath.restartDir=Paramt_BUFF[screenID][controlID+2];										//��������
	branchPath.restatrTime=Paramt_BUFF[screenID][controlID+3];									//����ʱ��
	branchPath.operationCmd=Paramt_BUFF[screenID][controlID+4];									//��������
	branchPath.spdCmd=Paramt_BUFF[screenID][controlID+5];												//�ٶ�ָ��
	branchPath.turnoffCmd=Paramt_BUFF[screenID][controlID+6];										//���ָ��
//	branchPath.obstacleCmd=Paramt_BUFF[screenID][controlID+6];									//�ϰ�ָ��
	branchPath.jumpCmd=Paramt_BUFF[screenID][controlID+7];											//��תָ��
//	branchPath.runDir=Paramt_BUFF[screenID][controlID+9];												//ִ�з���
	return branchPath;
}


/*********************������ǰ��֧·�����޶�Ӧ�Ŀ���****************/
//void StatnNum_Search(u16 StationCardNum_cardNum,u8 pathNum,u8 selectPathNum)
//���������u16 StationNum����վ�㿨�Ž����ȡ��վ���
//u8 selectPathNum:ѡ���·����
//���������
//���أ���ǰ�����¶�Ӧ�ķ�֧·�����ݽṹ��
//����˵����
/***************************************************************/
void StatnNum_Search(u16 StationCardNum_cardNum,u8 selectPathNum)
{
	u16 firstID,lastID;					//ҳ���һ���ؼ��ţ�ҳ�����һ���ؼ���
	u16 i,j;										//

																													//����ʱ��
	if(StationCardNum.error!=0&&StationCardNum.StationCardNumstor!=StationCardNum.cardNum)																			//����ȡ���������쳣
	{
		for(j=Pg_InsideMap11;j<=Pg_InsideMap15;j++)								//������ǰ��RIFD�Ŷ�Ӧ��վ����
		{		

			firstID=2;						//��һ��վ����
			lastID=83;						//���һ��վ����
			for(i=firstID;i<=lastID;i=i+9)
			{					
	//			if(Paramt_BUFF[j][i-1]==selectPathNum||Paramt_BUFF[j][i-1]==UniverVal)					//��·�������ȷ
				if(Paramt_BUFF[j][i]==StationCardNum.stationNum)																	//����⵽���뵱ǰRFID��Ӧ��վ���
				{
	//				if(Paramt_BUFF[j][i]==StationCardNum.stationNum)																	//����⵽���뵱ǰRFID��Ӧ��վ���
					if(Paramt_BUFF[j][i-1]==selectPathNum||Paramt_BUFF[j][i-1]==UniverVal)					//��·�������ȷ
					{							
						BranchPath=BranchPath2_struct(j,i,StationCardNum.cardNum,selectPathNum);													//��ȡ��ǰ���ŵķ�֧·������
						BranchPath.error=1;																														//��ȡ���ķ�֧·�����ݲ��쳣							
						goto pos1;										//��ɼ���ת
					}
					else
					{
						BranchPath.error=0;																//����0˵���쳣			
					}		
				}										
			}

		}		
	}	
	pos1:        //��ת��
	StationCardNum.StationCardNumstor=StationCardNum.cardNum;													//��ȡ��������RFID����
}


/******�����ķ�֧·��ģʽ��һ��վ���Ӧ�����׹���*******/
//void SignalBranchPath_Station(void)
//���������StationNum��վ���      u8 SelectPathNum:ָ����·�����
//���������
//���أ�
//����˵����
/***************************************************************/
void SignalBranchPath_Station(void)
{
	static u16 cardValueStor;															//��һ�δ洢��RFID����
	union MultiType multitype;   	

	if(/*cardValue!=0*/1)																//����ȡ�Ŀ��Ų�Ϊ0ʱ
	{
		if(/*BranchPath.cardNum!=cardValueStor||*/BranchPath.uart4Mark==1) 					//���RFID�Ƿ��б仯	
		{					
			switch(BranchPath.stopMode)
			{
				case 0:break;
				case 1:									//��ͣ��ģʽΪ��ʱ����ģʽ
				{
//					multitype.u32_=Paramt_BUFF[37][19];								//���в���2   19�ſؼ�
//					if(multitype.float_<1.f) multitype.float_=1.f;					//��С��ʱ����ֵΪ1
//					Tim7Add.time[1]=(u8)multitype.float_;							//AGVͣ����ʱʱ��
//					Tim7Add.start[1]=1;	
				}break;
				case 2:	break;					//�ⲿ�ź�ֹͣ
				case 3:									//AGVͣ��ģʽΪ����ͣ��
				case 4:									//��Ϣ��ͣ��ָ��
				case 5:									//ѯ���Ƿ�ͣ��
				{
					AGVStop_mode(BranchPath.stopMode,0,0);						//ͣ��ָ��
					multitype.float_=0;			
				}break;
				default:break;	
			}	
			if(BranchPath.restartDir==1||BranchPath.restartDir==2)						//���������з���Ϊǰ�����ߺ���
			{
				Time7_Timekeep(Tim7Add.time[4],Tim7Add.tiAdd,Tim7Add.start[4],4,1);				//Tim7[4]��ʱ��			��ʱ����λ	
				if(BranchPath.restatrTime<1)	Tim7Add.time[4]=1;							//���С��1 ��ǿ�Ƹ�ֵΪ1
				else Tim7Add.time[4]=BranchPath.restatrTime+(u8)multitype.float_;			//AGV������ʱʱ��=ͣ��ʱ��+�����ȴ�ʱ��
				
				Tim7Add.start[4]=1;						//��ʱ����
			}																											//AGV������ʱ��ʱ������				
			AGVOperat_Cmd(BranchPath.operationCmd);								//AGV����ָ��
			AGVSpd_IN_Cmd(BranchPath.spdCmd);												//AGV�ٶ�ָ��	
			AGVTurnoff_Cmd(BranchPath.turnoffCmd);										//���ָ��			
		}
//		if(BranchPath.stopMode==1)AGVStop_mode(BranchPath.stopMode,Tim7Add.mark[1],Tim7Add.start[1]);						//AGVͣ��ģʽΪ��ʱͣ��
		AGVRestart_Director(BranchPath.restartDir,Tim7Add.mark[4],Tim7Add.start[4]);			//AGV��������	
	}	
	cardValueStor=BranchPath.cardNum;
	BranchPath.uart4Mark=0;
}
