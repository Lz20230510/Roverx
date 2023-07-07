#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "HEAD.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//�������˿ڶ���
#define OT_DIR1 PBout(6)	// ��ǰ����
#define OT_DIR2 PBout(7)	// �����	 
#define OT_DIR3 PBout(8)	// ��ǰ����
#define OT_DIR4 PBout(9)	// �Һ���	 

#define LLimit   2000     //����     2200        2000
#define RLimit   1000		  //�Ҽ���     1250        1000
#define Median 	 15556 		  //��ֵ       1700      1500


void Time_CH1234_Init(void);
u16 Speed_trans(u16 spped_in);
void TIM4_CH1_PWM_Init(u32 arr,u32 psc);
void TIM4_CH2_PWM_Init(u32 arr,u32 psc);
void TIM4_CH3_PWM_Init(u32 arr,u32 psc);
void TIM4_CH4_PWM_Init(u32 arr,u32 psc);
u32 Direction_Control(u8 num,float angle,float limit);



void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode);
void Circle_contr(float speed_in,float R,u8 mode);
void Vehicle_contr_Auto(float speed_in,float R,u8 mode);
void Vehicle_contr_Jog(float speed_in,u8 mode);
void Syma_vehicle_contr(u8 * syma_cmd,u8 * tmd_buf,float spd_Auto,float R,float spd_Jog);
void auto_vehicle_contr(float speed_in,u8 cmd);
u32 Steering_Engine_Caculate(float angle);
void Steering_contral(u32 pwmvalue);
//void Speed_set(u8 code_in);
#endif
