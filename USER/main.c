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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//设置系统中断优先级分组4
	delay_init(168);																//初始化延时函数
	IIC_Init();//初始化IIC总线
	uart_init(9600,0,1);     												//初始化串口
  usart2_init(9600,1,1);                          //蓝牙
	usart3_init(9600);	    												//初始化串口3波特率位38400  GPS
	usart6_init(115200); 
	mark20=2;
 // MPU6050_Init();
	MPU9250_Init();
	//USART_SendData(USART3,res);
	mark20=3;
	
//	my_mem_init(SRAMIN); 
//	my_mem_init(SRAMCCM);
	
	GPIO_OUT_Init();																//GPIO输出初始化i
	GPIO_IN_Init();																	//GPIO输入初始化
  
	TIM4_CH4_PWM_Init(5000-1,84-1);														//定时器4通道4PWM初始化
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);				//CAN通信初始化
 cantimer_init(10-1,8400-1);	 																									//canopen定时器初始化
	
//	my_mem_init(SRAMIN); 
//	my_mem_init(SRAMCCM);
	
	Vehicle_key_Init();						//小车按钮初始化
	AGVsta_Scan(0);	           		//按钮状态初始化检测
	
	mark20=4;
	//Cycle_SetScreen(25,2);				//启动时，人机界面切换到初始界面
	mark20=5;	
	//delay_ms(300);								/*延时等待串口屏初始化完毕,必须等待300ms*/	
  mark20=6;
	//for(i=0;i<1;i++) AllPg_Refresh(textFormat,textFormat,textFormat);		//循环初始化人机界面
	mark20=7;
//	AGVParamter_Init();					//AGV参数初始化	
////	while(NRF24L01_Check()) delay_ms(150);             //24L01是否故障检查
	mark20=8;
	delay_ms(500); 
	mark20=9;

Cycle_SetScreen(0,2);					//切换回主界面
	//delay_ms(100);
	//queue_reset();
	
  Vehicle_key_Init();						//小车按钮初始化
  mark20=9;

//初始化内部内存池	
//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄       
			
    vTaskStartScheduler();          										//开启任务调度
mark20=10;
}

