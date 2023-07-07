#ifndef __FREERTOSDEF_H_
#define __FREERTOSDEF_H_
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "HEAD.h"


//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
extern TaskHandle_t StartTask_Handler;

//任务优先级
#define LED0_TASK_PRIO		5
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
extern TaskHandle_t LED0Task_Handler;

//任务优先级
#define IOSCAN_TASK_PRIO		4   
//任务堆栈大小	
#define IOSCAN_STK_SIZE 		512//2外加大彩屏切换程序
//任务句柄
extern TaskHandle_t IOSCANTask_Handler;

//任务优先级
#define CANOPEN_TASK_PRIO		2   
//任务堆栈大小	
#define CANOPEN_STK_SIZE 		128
//任务句柄
extern TaskHandle_t CANOPENTask_Handler;

//任务优先级
#define UDP_SEND_TASK_PRIO		2   
//任务堆栈大小	
#define UDP_SEND_STK_SIZE 		128
//任务句柄
extern TaskHandle_t UDP_SENDTask_Handler;

#define MPU6050_TASK_PRIO		  3 
#define MPU6050_STK_SIZE 		  256 

#define MPU9250_TASK_PRIO		  3       //2023.3.30
#define MPU9250_STK_SIZE 		  256     //2023.3.30

#define DATA_STK_SIZE   512 
#define DATA_TASK_PRIO  4

//任务优先级
#define HMI_FLASH_TASK_PRIO		3   
//任务堆栈大小	
#define HMI_FLASH_STK_SIZE 		128
//任务句柄
extern TaskHandle_t HMI_FLASHTask_Handler;


//任务优先级
#define JOGAUTOMODE_TASK_PRIO		3   
//任务堆栈大小	
#define JOGAUTOMODE_STK_SIZE 		128
//任务句柄
extern TaskHandle_t JOGAUTOMODETask_Handler;

extern u8 SetUp;





////////////////工作任务定义////////////////
void start_task(void *pvParameters);
void led0_task(void *pvParameters);
void IO_Scan_task(void *pvParameters);
void CANOPEN_task(void *pvParameters);
void UDP_SEND_task(void *pvParameters);
void HMI_FLASH_task(void *pvParameters);
void JogAutoMode_task(void *pvParameters);
void MPU6050_task(void *pvParameters);
void data_task(void *pvParameters);
void MPU9250_task(void *pvParameters);
#endif				    
