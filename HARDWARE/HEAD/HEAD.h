#ifndef __HEAD_H_
#define __HEAD_H_
#include  "sys.h"
#include "TestMaster.h"
#include "I_O.h"
#include "FreeRTOSDef.h"
#include "task.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "FreeRTOSDef.h"
#include "cantimer.h"
#include "cancan.h"
#include "dcf.h"
#include "TestMaster.h"

//#include "usart2.h"
#include "usart6.h"

#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lan8720.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "udp_demo.h" 
#include "stdio.h"

#include "stm32f4xx.h"
#include  <stdio.h>
#include  <math.h>
#include "string.h"	


#include "HMI_Cmd.h"
#include "cmd_process.h"
#include "vehicle_key.h"
#include "AGV_contr.h" 
#include "gps.h"
#include "map.h"
//#include "myiic.h" 
//#include "Motor.h"
#include "pwm.h"
#include "24l01.h" 
//#include "exti.h"
#include "magnetic.h"
#include "East_nor.h" 
#include "pid.h"
//#include "spi.h"
#include "RFID.h"
#include "timer1.h"
//#include "timer3.h"
#include "usart.h"	
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"

#include "hmi_driver.h"

#include "stmflash.h"

#include "AGV_contr.h" 

#include "BranchPath.h"
#include "StationCardNum.h"
//#include "vehicle.h" 









#define MotorCount 2             	//������������
#define MotorMode 0              	//�������Ĺ���ģʽ    0��ȫ���ٶ�   1��ȫ��λ��   3:ȫ������    4:8�֣�4��λ�ã������ٶ�ģʽ
#define Pi 3.14159								//Բ����


#define OT_JD1  PDout(6)    		 	//�̵���1
#define OT_JD2  PDout(7)     			//�̵���2
#define OT_JD3  PDout(11)    			//�̵���3
#define OT_JD4  PDout(12)     		//�̵���4
	








extern u8 cmd_vehicle;                //��λ�����͸������˵Ŀ��ƺ�
extern float spd_1;
extern double O_OA,O_OB;               			 //��ǰϵͳĬ�ϵĶ�������ϵ��ԭ������ֵ
extern double lati_x0,lati_y0;                //���ȷ�����ԭ���x,yֵ
extern double longti_O,lati_O;                //ԭ��㾭�ȣ�ά��ֵ�ȸ�ʽ
//��ʱ����ֵ

extern double longti_OA,lati_OB,x0_y0[2];
extern float cour_angle[2];							//��õ�·���������ĺ����
extern double OAOB[2];           //ĳһ���OA��OB����ֵ
extern double lontlat_du[2];       //�洢��γ�ȶȸ�ʽ
extern double  recent_near_pot[2];          //����С������ĵ�
extern u16 near_pot[2];              //����������ĵ�[0] λ�洢
extern u8 Wakeup_position_re;		//��ʼλ�ü��㷴��ֵ
extern float Scan_nearpot_re;					//С����·�����껹���ұߵķ���ֵ
extern float course_weighting_re;
 
extern double lonti_lati[2];     //�洢��ǰʵʱGPSλ��

extern float angle_deviat;

extern float a1[3],w1[3],h1[3],Angle1[3];








//������ݣ�float EV[2]��ǰ�������ٶ�ƫ��ֵ�洢  EV[0]��ǰ  EV[1]����   
//Ea_Ed[0]:ǰ��������Ĵŵ���ƫ���    Ea_Ed[1]:ǰ��������ĺ���ƫ����
//Ea_Ed[2]:����������Ĵŵ���ƫ���    Ea_Ed[3]:����������ĺ���ƫ����
extern float Ea_Ed[4],EV[2]; 		//PID���ݴ���ƫת�ǵ�



extern u16 AGVSpd_IN;
extern u8 RFID_LeftRight;				//RFID����ת�ź�,=1��ת��=2��ת
extern u8 RFID_stor;					//RFID_stor��ʱ�洢ֵ
extern u8 button;						//������尴��
extern u8 JOG_mark;					//�״ν����ֶ�����
//extern PIDtypedef front_pid,rear_pid;//�ĸ�����ʽPID

/*****************************************************************/
extern u16 CurrtScreenID;					//��ǰ��ȡ�Ľ���ID ��
/****************************************************************/
extern u8 PathTemp;
extern u8 LCD_mark;

void AGV_Init(u16 currntScreenID);							//AGV�˻����������ʼ��ˢ��









#endif		    

