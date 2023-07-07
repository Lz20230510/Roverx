#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart4.h"
#include "usart3.h"
#include "usart6.h"
#include "FreeRTOS.h"
#include "task.h"
#include "HEAD.h"
#include "FreeRTOSDef.h"
#include "I_O.h"
#include "cantimer.h"
#include "cancan.h"
#include "dcf.h"
#include "TestMaster.h"
#include "Motor.h"

#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lan8720.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "udp_demo.h" 
#include "HEAD.h" 
#include "myiic.h"
//#include "mpu6050.h"
#include "mpu9250.h"
#include "AGV_contr.h"
#include "vehicle_key.h"
#include "cmd_queue.h"
#include "HMI_cmd.h"
u8 mark20=0;
u16 RdStatus;
float RdSpd;
float RdCurrent;
u8 i;
//u8 DacaiMark=0;

int main(void)
{ u8 res;
	 mark20=1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//����ϵͳ�ж����ȼ�����4
	delay_init(168);																//��ʼ����ʱ����
	IIC_Init();//��ʼ��IIC����
	uart_init(9600,0,1);     												//��ʼ������
  usart2_init(9600,1,1);                          //����
	usart3_init(9600);	    												//��ʼ������3������λ38400  GPS
	usart6_init(115200); 
	mark20=2;
 // MPU6050_Init();
	MPU9250_Init();
	//USART_SendData(USART3,res);
	mark20=3;
	
//	my_mem_init(SRAMIN); 
//	my_mem_init(SRAMCCM);
	
	GPIO_OUT_Init();																//GPIO�����ʼ��i
	GPIO_IN_Init();																	//GPIO�����ʼ��
  
	TIM4_CH4_PWM_Init(5000-1,84-1);														//��ʱ��4ͨ��4PWM��ʼ��
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);				//CANͨ�ų�ʼ��
 cantimer_init(10-1,8400-1);	 																									//canopen��ʱ����ʼ��
	
//	my_mem_init(SRAMIN); 
//	my_mem_init(SRAMCCM);
	
	Vehicle_key_Init();						//С����ť��ʼ��
	AGVsta_Scan(0);	           		//��ť״̬��ʼ�����
	
	mark20=4;
	//Cycle_SetScreen(25,2);				//����ʱ���˻������л�����ʼ����
	mark20=5;	
	//delay_ms(300);								/*��ʱ�ȴ���������ʼ�����,����ȴ�300ms*/	
  mark20=6;
	//for(i=0;i<1;i++) AllPg_Refresh(textFormat,textFormat,textFormat);		//ѭ����ʼ���˻�����
	mark20=7;
//	AGVParamter_Init();					//AGV������ʼ��	
////	while(NRF24L01_Check()) delay_ms(150);             //24L01�Ƿ���ϼ��
	mark20=8;
	delay_ms(500); 
	mark20=9;

Cycle_SetScreen(0,2);					//�л���������
	//delay_ms(100);
	//queue_reset();
	
  Vehicle_key_Init();						//С����ť��ʼ��
  mark20=9;

//��ʼ���ڲ��ڴ��	
//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������       
			
    vTaskStartScheduler();          										//�����������
mark20=10;
}

