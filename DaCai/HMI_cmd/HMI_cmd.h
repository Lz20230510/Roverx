#ifndef __HMI_CMD_H
#define __HMI_CMD_H
#include "HEAD.h"
#include "hmi_driver.h"

							//��ǰ�����
//extern u8 cmd_buffer[CMD_MAX_SIZE];//ָ���
extern u16 ScreenID;//��ǰ����IDS
extern u16 CurrtScreenID;	

static long test_value;//����ֵ
static u8 update_en;//���±��
extern uint16_t  motor_speed;

//�������1��  ���ݴ洢  
extern double lf_spd;                       //��ǰ��ת��				�������1
extern double	lb_spd;												//�����ת��				�������1
extern double	rf_spd;												//��ǰ��ת��				�������1
extern double	rb_spd;												//�Һ���ת��				�������1

extern u8	BD_num;												//��������							�������1
extern u8	GPS_num;											//GPS����								�������1	
extern double	longti;												//����							�������1
extern double	lati;													//ά��							�������1
extern double	OA_x;														//��ǰ����������ϵ������ֵ	�������1
extern double	OB_y;														//��ǰ����������ϵ������ֵ	�������1
extern double	Angle;												//С����ǰ������				�������1


//��������1��  ���ݴ洢          
extern double Auto_spd;										//�Զ�Ĭ���ٶ�  		��������1
extern double Jog_spd;										//�ֶ�Ĭ���ٶ�			��������1
extern double Jog_spdlmt;									//�ֶ��ٶ�����			��������1
extern double Auto_spdlmt;									//�Զ��ٶ�����		��������1
extern double Jog_cirR;           //�ֶ�Բ���뾶							��������1
extern double ultro_dis;          //�������������þ���				��������1



typedef	struct
{
	u8 Pg0_JOGSetup;							//������  ң��������ť
	u8 Pg0_AUTOmode;								//������  ���Զ�ģʽ��ť
	u8 Pg0_SONSetup;							//������  �豸ʹ�ܰ�ť
	u8 Pg0_Rest;									//������ ������λ��ť
	u8 Pg4_TeachMode;							//�켣�༭����  ʾ��ģʽ�л���ť
	u8 Pg4_MapDown;							//�켣�༭����  ��ͼ���ذ�ť
	u8 Pg4_MapClean;							//�켣�༭����  ��ͼ���尴ť
	u8 Pg17_ultral;								//���ϴ����������ر�
}DacaiButtonDef;
extern DacaiButtonDef DacaiButton;





void SetTextValueFloat(u16 screen_id, u16 control_id,double value,u8 type);
void SetTextValueu16(u16 screen_id, u16 control_id,u16 value);



#endif
