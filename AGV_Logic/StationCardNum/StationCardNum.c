#include "HEAD.h"


//u16 firstID,lastID,cardvalue;					//վ�㿨��ID��


/**********************ͨ�����Ż�ȡվ���*****************************/
//void Station_CardNum(u32 cardValue)
//���������u32 cardValue��RFID��ȡ��ԭʼ����
//���������
//���ز�����
//��������������ȡRFID���ź�������ǰ�����ж�Ӧ��վ�Ź���,�����
/*********************************************************************/
void Station_CardNum(u32 cardValue)
{
	u16 firstID,lastID,cardvalue;					//վ�㿨��ID��
	u16 i,j;
	static StationCardNumDef staCardNum;			
	cardvalue=(u16)cardValue;							//ת��Ϊ16λ����	
	StationCardNum.cardNum=cardValue;									//���Ÿ�ֵ
	for(j=Pg_StaCardNum18;j<=Pg_StaCardNum19;j++)						//ҳ��18-19ѭ��
	{
		firstID=1;
		lastID=59;
		for(i=firstID;i<=lastID;i=i+2)
		{
			if(Paramt_BUFF[j][i]==cardvalue)								//���ҵ���Ӧ����
			{
				StationCardNum.stationNum=(u16)Paramt_BUFF[j][i+1];			//վ�Ÿ�ֵ
//			staCardNum.cardNum=cardvalue;									//���Ÿ�ֵ
				StationCardNum.error=0x01;												//�ҵ���Ӧ�Ŀ��Ź���
				return;
			}	
			else																					//��û���ҵ���Ӧ����
			{							
				StationCardNum.error=0x00;												//û���ҵ���Ӧ�Ŀ��Ź���				
			}		
		}			
	}
}


/**********************�����໥���ڹ���****************************/
//void CardNum_StationNum(u16 CurrentScreenId)
//���������u16 CurrentScreenId����ǰ��Ļ��ID��
//���������
//���ز�����
//��������������ȡRFID���ź�������ǰ�����ж�Ӧ��վ�Ź���,�����
/*********************************************************************/
//void CardNum_StationNum(u16 CurrentScreenId)
//{
//	union MultiType multiType9;
//	switch(CurrentScreenId)	
//	{
//		case Pg_StaCardNum18:
//		case Pg_StaCardNum19:
//		{
//			multiType9.u8_=Paramt_BUFF[Pg_StaCardNum5][64];										//����վ�㿨����ʱ��ֵ
//			if(Paramt_BUFF[CurrentScreenId][64]!=multiType9.u8_)						//�������ʹ洢���ݲ�ͬ��˵�����ݷ����˸ı�
//			{
//				Paramt_BUFF[Pg_StaCardNum1][64]=Paramt_BUFF[CurrentScreenId][64];						//���������������¸�ֵ
//				Paramt_BUFF[Pg_StaCardNum2][65]=Paramt_BUFF[CurrentScreenId][64];
//				Paramt_BUFF[Pg_StaCardNum3][65]=Paramt_BUFF[CurrentScreenId][64];
//				Paramt_BUFF[Pg_StaCardNum4][65]=Paramt_BUFF[CurrentScreenId][64];
//				Paramt_BUFF[Pg_StaCardNum5][64]=Paramt_BUFF[CurrentScreenId][64];				
//			}				
//		}break;
//		default:break;
//	}
//}
