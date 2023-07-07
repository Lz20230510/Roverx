#ifndef __EAST_NOR_H
#define __EAST_NOR_H	
#include "sys.h" 
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//����ת������������ϵ�²�����ת��
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

#define a 6378136.49 //���򳤰��ᣬ  ��λ����
#define b 6356755.00 //����̰��ᣬ  ��λ����
#define pi 3.1415926 //Բ����
#define thresh_posi  65.0  //���ڷ�Χ�ķ�ֵ  �궨����ԭ��O֮������ı궨ֵΪ64.4691/1 m


typedef struct
{
	double logti_O_du;				//ԭ�㾭��
	double lati_O_du;					//ԭ��γ��	
	double logti_O_dufen;			//ԭ�㾭��
	double lati_O_dufen;			//ԭ��γ��	
	double lati_xO;						//ԭ�㾭��x��������
	double lati_yO;						//ԭ�㾭��y��������
	u8 StarNum;								//������
	u16 totalPotNum;					//�ܵ���
	u16 linePotNum;						//ÿ���߶��ϵ������
	float spdTemp;						//�Զ������ٶ���ʱ�洢
}LontLatiDef;
extern LontLatiDef LontLati;




double abs_(double in);//  ��˫���Ⱦ���ֵ
float four_quadrant(double first_x,double first_y,double last_x,double last_y);  //���ͼ����ɺ���ڵ���ֱ�ߵ�ָ��Ƿ�����ж�
float JY901_eastnor(float JY901_angle,float error);		//��JY901����Ƕ�ֵת��Ϊͳһ�Ķ�������ϵ�µ�һȦ360��ĸ�ʽ
void longti_lati_trans(u8 mode,double longti,double lati,double x0_y0[2],double lotlat_du[2]);
void OAOB_caculate(double x0,double y0,double x,double y,double longti_O,double longti_xy,double OAOB[2]);
//void Course_angle(double * OB,double * OA,float * cour_angl);
u8 Wakeup_position(double lati_OA,double longti_OB ,float cour_angle[2],u16 near_pot[2]);
u8 direct_judge(double lati_y,double longti_x ,u16 neat_pot);
float Scan_nearpot(double lati_y,double longti_x ,u16 near_pot[2],u8 num);
float course_weighting(u16 near_pot[2],float d_diviation);
u8 veichel_contr(float rencent_angle,float taget_angle);
float angle_deviat_judge(float rencent_angle,float taget_angle);
void longtiLati_xy(double lati,double * lati_xy);
LontLatiDef Origin_O_parmt_Init(void);
MapPotDef MapPot_Find(u16 potNum);
MapPotDef AGV_OutSideValue(nmea_msg gpsx,float Scm345,LontLatiDef lontLati_O);
#endif
