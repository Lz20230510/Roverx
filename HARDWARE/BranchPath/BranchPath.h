#ifndef __BRANCHPATH_H
#define __BRANCHPATH_H
#include "sys.h"

////////////////////////////////��֧·������///////////////////////////// 
//�����˻������ϵķ�֧·������������֧·������
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////



#define UniverVal 254         //·���е�ͨ�ò���ֵ

//////////////////////////////////////////
//��֧·��ģʽ���ƽṹ��
//
//
typedef struct
{
	u8 selectPathNum;   //ϵͳѡ����·����ţ�����ָ��С��·��
	u8 pathNum;					//·����
	u8 cardNum;					//��ȡ���ĵ�ǰ·�����ڵ�ǰ·���ϵĿ���
	u8 statnCardNum;		//վ�㿨��
	u8 stopMode;				//ͣ��ģʽ
	u8 stopModeStor;		//ͣ��ģʽ��ʱ�洢
	u8 restartDir;			//��������
	u8 restatrTime;			//����ʱ��
	u8 operationCmd;		//����ָ��
	u8 operationCmdStor;		//����ָ����ʱ�洢
	u8 spdCmd;					//�ٶ�ָ��
	u8 turnoffCmd;			//���ָ��
	u8 turnoffCmdStor;			//���ָ����ʱ�洢
	u8 obstacleCmd;				//����ָ��
	u8 jumpCmd;						//·����תָ��
	u8 runDir;						//ִ�з���
	u8 uart4Mark;						//uart4�жϱ�־
	u8 error;							//��֧·�����ݻ�ȡ����					=0��վ����쳣     =1��վ��ŷ��쳣
}BranchPathDef;					
extern BranchPathDef BranchPath;




u8 AGVPathJump_Cmd(u8 JumpCmd,u8 selectPathNum);																									//AGV·����תָ��
void AGVStop_mode(u8 stopmode,u8 timeMark,u8 timeStart);												//AGVֹͣģʽ
void AGVRestart_Director(u8 runDirector,u8 timeMark,u8 timeStart);							//AGV��������
u8 AGVOperat_Cmd(u8 OperatCmd);																								//AGV����ָ��
void AGVSpd_Cmd(u8 SpdCmd);																											//AGV�ٶ�ָ��												
void AGVTurnoff_Cmd(u8 turnoffCmd);																							//AGV���ָ��
BranchPathDef  BranchPath2_struct(u16 screenID,u16 controlID,u16 cardNum,u8 selectPathNum);			//����ȡ���ķ�֧·������ת��Ϊ��֧·���ṹ��
void StatnNum_Search(u16 StationCardNum_cardNum,u8 selectPathNum);						//������ǰ�Ŀ��Ŷ�Ӧ�ķ�֧·������
void SignalBranchPath_Station(void);									//�����ķ�֧·��ģʽ��һ��վ���Ӧ�����׹���																		
#endif




