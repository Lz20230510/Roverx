#ifndef __MAGNETIC_H_
#define __MAGNETIC_H_


#include "sys.h"
#include "HEAD.h"


#define wheel_space  220        //�����������ּ��  	��λ mm
#define wheel_R		152						//����ֱ��						��λ mm		
#define mag_length 160          //�ŵ�������������		��λ mm
#define	mag_d		300							//��������ǰ��ŵ������������  ��λ mm
#define	thresh_Ea	5							//ƫת�ǵ�������ֵ			��λ mm
#define thresh_Ed 10						//����ƫ������������ֵ	��λ mm
//#define pi 3.14159							//Բ����

typedef struct									//�ŵ���ԭʼ���ݽṹ��
{
	u16 front1;					//ǰ1�ŵ���
	u16 front2;					//ǰ2�ŵ���
	u16 rear1;					//��1�ŵ���
	u16 rear2;					//��2�ŵ���
}InitMagDateDef;
extern InitMagDateDef InitMagDate;

typedef struct									//�ŵ������ݴ洢
{
	u8 front1[8];
	u8 front2[8];
	u8 rear1[8];
	u8 rear2[8];
	int value[2];    //[0]�ŵ���ֵ  [1]�Ƿ��������  num>0 ��ʾ�д���
}MagneticDef;
extern MagneticDef Magnetic;

extern float Mag_date[12];					//����ŵ����������ֱ��ȡ������ֵ�洢
extern u8 Mag_alarm[4];								//����ŵ����������Ƿ������ݱ���	

extern u8 mag1[8];				//��ȡ1�Ŵŵ�������������
extern u8 mag2[8];				//��ȡ2�Ŵŵ�������������
extern u8 mag3[8];				//��ȡ3�Ŵŵ�������������
extern u8 mag4[8];				//��ȡ4�Ŵŵ�������������


//��ʼ��
float deci_hex(u8 * magnetic_in);			
float deci_hex_single(u8 date_in);
void Devia_Calcul(int SW_STOR[16],int Sum[2]);
u16 magbetic_tran(u8 * magnetic_in,float mag_date[4],u8 mag_alarm[4]);
InitMagDateDef magnetic_recei(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4]);
u16 mag_calcul(u8 * magnetic_in,float * Sum);
void Ea_Ed_calcu(float * mag_date,float Ea_Ed[4]);
void MNSV_X8IO_Read(int * temp);

#endif

