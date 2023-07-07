/******************************************************************************
* �ļ�����HEAD.c
* ��  ����
* ��  �ߣ�
* ��  �ڣ�
* ��汾��   
* �ļ���������������ͷ�ļ�
*******************************************************************************/
#include "HEAD.h"
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
#include "Motor.h"


#include "vehicle_key.h" 
#include "AGV_contr.h" 
#include "HMI_cmd.h" 
#include "24l01.h" 
#include "usart.h" 
#include "usart6.h" 
#include "gps.h"
#include "East_nor.h" 
#include "myiic.h" 
#include "pid.h" 
#include "map.h" 
#include "RAD.h" 
#include "usart3.h" 
#include "BranchPath.h"
#include "StationCardNum.h"
#include "magnetic.h"
#include "RFID.h"
#include "timer1.h"
#include "vehicle.h" 
#include "mpu9250.h"


IOStatusDef IOStatus;     //��ɢ��GPIO״̬



u8 udp_demo_recvbuf[UDP_DEMO_RX_BUFSIZE];	//UDP�������ݻ�����
const u8 *udp_demo_sendbuf="1-------1------1\r\n";//UDP������������
u8 udp_flag;							//UDP���ݷ��ͱ�־λ



//////////����ϵͳ��������������///////////
TaskHandle_t StartTask_Handler;
TaskHandle_t LED0Task_Handler;
TaskHandle_t IOSCANTask_Handler;
TaskHandle_t CANOPENTask_Handler;
TaskHandle_t UDP_SENDTask_Handler;
TaskHandle_t HMI_FLASHTask_Handler;		//HMI FLASH
TaskHandle_t JOGAUTOMODETask_Handler;


AGVStatusDef AGVStatus;						//�ƶ�������״̬�ź�		
DacaiButtonDef DacaiButton;				//�������ť�ź�
SymaStatusDef SymaStatus;					//ң����״̬�ź�
//BatteryMsgDef BatteryMsg;					//��ز�����Ϣ
TimeAddDef Tim1Add,Tim7Add,Tim6Add;												//time1��ʱ���ṹ��洢
CRC16Def CRC16RFID,CRC16Wireless,CRC16Mag;				//CRC16У��
InitMagDateDef InitMagDate;								//�ŵ���ԭʼ���ݽṹ��
Smc345Def Smc345;													//����������
UltrosonicDef Ultrosonic;									//����������
MagneticDef Magnetic;

u8 cmd_vehicle=0;                //��λ�����͸������˵Ŀ��ƺ�
float spd_1=0;
double O_OA=0,O_OB=0;               			 //��ǰϵͳĬ�ϵĶ�������ϵ��ԭ������ֵ
double lati_x0=0,lati_y0=0;                //���ȷ�����ԭ���x,yֵ
double longti_O=0,lati_O=0;                		//ԭ��㾭�ȣ�ά��ֵ�ȸ�ʽ

double longti_OA,lati_OB,x0_y0[2];
float cour_angle[2];							//��õ�·���������ĺ����
double OAOB[2];           //ĳһ���OA��OB����ֵ
double lontlat_du[2];       //�洢��γ�ȶȸ�ʽ
double  recent_near_pot[2];          //����С������ĵ�
u16 near_pot[2];              //����������ĵ�[0] λ�洢
u8 Wakeup_position_re=0;		//��ʼλ�ü��㷴��ֵ
float Scan_nearpot_re=0;					//С����·�����껹���ұߵķ���ֵ
float course_weighting_re;

nmea_msg gpsx;   
double lonti_lati[2];     //�洢��ǰʵʱGPSλ��

float angle_deviat=293;             //�����ǽ���ֵ

float a1[3]={0,0,0};
float w1[3]={0,0,0};
float h1[3]={0,0,0};
float Angle1[3]={0,0,0};

float Ea_Ed[4],EV[2];				//PID���ݴ���ƫת�ǵ�


u8 RFID_LeftRight=1;											//RFID����ת�ź�,=1��ת��=2��ת
u32 RFID_value=0;													//RFID����ֵ
u16 AGVSpd_IN;														//PID���ڻ����ٶ�
float AGVSpd_OUT;													//�����Զ�AGV�ٶ�
PIDtypedef front_pid;											//front_pid�趨:Kp,Ki,Kd,SetPoint,
PIDtypedef rear_pid;											//behind_pid�趨:Kp,Ki,Kd,��SetPoint
BranchPathDef BranchPath;									//��֧·��ģʽ�ṹ��
StationCardNumDef StationCardNum;					//վ�㿨�Žṹ��
VehicleParamtDef VehicleParamt;						//AGV����λ�����͵�����
MapPotDef MapPot,MapPotDsiplay;													//����������OAOBֵ
AGVStatusDef AGVStatus;											//AGV״̬
LontLatiDef LontLati;											//GPS��ز���
MotorMsgDef MotorMsg;		//������ݽṹ��

u8 JOG_mark=0;					//�״ν����ֶ�����
u8 LCD_mark=0;
u8 Ask_Battery[7]={0xDD,0xA5,0x03,0x00,0xFF,0xFD,0x77};				//ѯ�ʵ��ʣ�����

//u8 Ask_Battery[14]={0x3A,0x30,0x30,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x65,0x65,0x36,0x7E};
//Ask_Battery[14]={0x3A,0x30,0x30,0x31,0x33,0x30,0x30,0x30,0x30,0x30,0x65,0x65,0x36,0x7E};				//ѯ�ʵ��ʣ�����
/****************************AGV���в�����ʼ��*****************************/
//void AGV_Init(u16 currntScreenID)
//���������u16 currntScreenID�� ��currntScreenID=0x00--0x28     ��currntScreenID=0xFF,��˵�������в�����ʼ��װ��
//���������
//���ز�����
/************************************************************************/
void AGV_Init(u16 currntScreenID)
{
	switch(currntScreenID)
	{
		case 0x24:						//���в���1
		case 0x27:						//AGV��������
		{
			if(currntScreenID==0x24)					
			{

			}
			else if(currntScreenID==0x27)
			{

			}		
		}break;
		case 0x25:						//���в���2����
		{

		}break;
		case 0xFF:						//AGV���в�����ʼ��װ��
		{

					
		}break;		
		default:break;
	}
}

//���ߴ���ͨ��,GPS,      USART1
//


