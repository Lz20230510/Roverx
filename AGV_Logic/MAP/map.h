#ifndef __MAP_H
#define __MAP_H	

#include "sys.h"

//////////////////////////////////////////////////////////
//
//
//�洢��ͼ��;����Ϣ
//thresh_posi Ϊ�궨ֵ
////////////////////////////////////////////////////////////
extern float thresh_posi;



////////////////��ͼ��Ϣ�ṹ��/////////////////////////////////
typedef struct
{
	float OA;						//				��ͼ��ϢOAֵ
	float OB;						//				��ͼ��ϢOBֵ
	float angle;				//				��ͼ��Ϣ�����ֵ
	float trueAngle;		//ʵ�ʺ����
	u8 		starNum;					//����
	
	unsigned int numFx;    //��ǰֱ���߶ε����� 
	unsigned int numPot;    //����ֱ���ϵĵ������
	unsigned int grossNumPot;   //�ܵõ������
	double longti_du;							//�ȸ�ʽ�µľ���ֵ
	double lati_du;								//�ȸ�ʽ�µ�ά��ֵ
	u16 errNum;                   //���������      
	double longti_dufen;							//�ȸ�ʽ�µľ���ֵ �ȷָ�ʽ
	double lati_dufen;								//�ȸ�ʽ�µ�ά��ֵ	�ȷָ�ʽ
	unsigned int  Num;						//				�������
	float AutoSpd;								//�Զ������ٶ�
	u8 mode;
}MapPotDef;
extern MapPotDef MapPot,MapPotDsiplay;
#endif
