#ifndef __STATIONCARDNUM_H
#define __STATIONCARDNUM_H
#include "sys.h"


typedef struct						//վ�㿨�Žṹ��
{
	u16 stationNum;					//վ��
	u16 StationCardNumstor;		//վ�㿨����ʱ�洢
	u16 cardNum;							//����
	u8 fucStor;						//���ܴ洢���洢վ�㼰���ŵ�ͳһ���ܣ��������ͳһ
	u8 error;								//����Ŀ���û�ж�Ӧ��վ�Ż��߿����쳣   error=0x00,�쳣    error=0x01, ����
}StationCardNumDef;

extern StationCardNumDef  StationCardNum;




void CardNum_StationNum(u16 CurrentScreenId);						//վ�㿨��5������Ŀ��ż�վ�Ź��ܻ��ำֵ
void Station_CardNum(u32 cardValue);							//��ȡ�Ŀ��ŵĶ�Ӧ��վ���
#endif
