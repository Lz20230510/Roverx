/******************************************************************************
* �ļ�����FreeRTOSDef.c
* ��  ����
* ��  �ߣ�
* ��  �ڣ�
* ��汾��   
* �ļ���������������ͷ�ļ�
*******************************************************************************/
#include "FreeRTOSDef.h"
#include "HEAD.h"
#include "Motor.h"
#include "stmflash.h"
#include "vehicle.h" 
#include "pid.h" 
//#include "mpu6050.h"
#include "usart2.h"
#include "usart6.h"
#include "usart3.h"
#include "vehicle_key.h"
#include "gps.h"
#include "mpu9250.h"
#include "Motor.h"
u8 test3=0;//���ڡ��������
u8 test5=0;//�ֶ��Զ���������
u8 testled=0;
u8 mpu6050test=0;
u8 keyTemp;	
float angle_value,angleTemp;
u8 temp11[9];
u8 SetUp=0;
u16 Add1=0;

u8 AGVModeChange;

u8 DacaiMark=0;

//��ʼ����������
void start_task(void *pvParameters)
{	
//		while(lwip_comm_init()) delay_ms(500);	    //lwip��ʼ��
//		while(udp_demo_init()==pdFAIL)delay_ms(500);		//�������DHCP����֮���ٿ���UDP���񡣳�ʼ��udp_demo(����udp_demo�߳�)
//		taskENTER_CRITICAL();           //�����ٽ���	
//		#if LWIP_DHCP
//			lwip_comm_dhcp_creat(); //����DHCP�����������а�����UDP����Ŀ�����
//		#endif
//		VehiclePar.Ratio=DriveMotor_Ratio(0.24,30);		//���ٱ�
//		VehiclePar.Ratio=DriveMotor_Ratio(0.24,30);		//���ٱ�
		VehiclePar.Ratio=2387.326;
		MotorMsg[0].Ratio=VehiclePar.Ratio;
		MotorMsg[1].Ratio=VehiclePar.Ratio;
		VehicleSpd=0.5;
	

    //��������ң������	
    xTaskCreate((TaskFunction_t )JogAutoMode_task,     	
                (const char*    )"JogAutoMode_task",   	
                (uint16_t       )JOGAUTOMODE_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )JOGAUTOMODE_TASK_PRIO,	
                (TaskHandle_t*  )&JOGAUTOMODETask_Handler); 
	
//	  //IMU data read task //IMU���ݶ�ȡ����
//		xTaskCreate((TaskFunction_t )MPU6050_task, 
//								(const char*    )"MPU6050_task", 
//								(uint16_t       )MPU6050_STK_SIZE, 
//								(void*          )NULL, 
//								(UBaseType_t    )MPU6050_TASK_PRIO, 
//								(TaskHandle_t*  )NULL);	
		
		//Usart6������������					
		xTaskCreate((TaskFunction_t )data_task,
								(const char*    )"data_task",
								(uint16_t       )DATA_STK_SIZE,
								(void*          )NULL, 
								(UBaseType_t    )DATA_TASK_PRIO,
								(TaskHandle_t*  )NULL);				
		//��ɢ��GPIO״̬��ȡ						
		xTaskCreate((TaskFunction_t )IO_Scan_task,     
                (const char*    )"IO_Scan_task",   
                (uint16_t       )IOSCAN_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )IOSCAN_TASK_PRIO,
                (TaskHandle_t*  )&IOSCANTask_Handler);	

							
		//CANOPEN����					
		xTaskCreate((TaskFunction_t )CANOPEN_task,     
                (const char*    )"CANOPEN_task",   
                (uint16_t       )CANOPEN_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )CANOPEN_TASK_PRIO,
                (TaskHandle_t*  )&CANOPENTask_Handler);	
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler); 
#if 0								 
		//IMU�����Լ����������ݶ�ȡ����   2023.3.30
		xTaskCreate((TaskFunction_t )MPU9250_task, 
								(const char*    )"MPU9250_task", 
								(uint16_t       )MPU9250_STK_SIZE, 
								(void*          )NULL, 
								(UBaseType_t    )MPU9250_TASK_PRIO, 
								(TaskHandle_t*  )NULL);							
								
#endif								
								
		//UDP���ݷ�������					
//		xTaskCreate((TaskFunction_t )UDP_SEND_task,     
//                (const char*    )"UDP_SEND_task",   
//                (uint16_t       )UDP_SEND_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )UDP_SEND_TASK_PRIO,
//                (TaskHandle_t*  )&UDP_SENDTask_Handler);		


		//HMI_FLASH����					
//		xTaskCreate((TaskFunction_t )HMI_FLASH_task,     
//                (const char*    )"HMI_FLASH_task",   
//                (uint16_t       )HMI_FLASH_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )HMI_FLASH_TASK_PRIO,
//                (TaskHandle_t*  )&HMI_FLASHTask_Handler);
								
							
		vTaskDelay(1500);
		CANOPEN_Node_Start();									//CANOPEN�����ڵ�																				
    vTaskDelete(StartTask_Handler); 			//ɾ����ʼ����				
    taskEXIT_CRITICAL();            			//�˳��ٽ���								
		TIM_SetCompare4(TIM4,5000-Median); 		//�������
}

//LED0������   //LED1 ���İ�ָʾ��
void led0_task(void *pvParameters)
{
    while(1)
    {
			testled=1;
//			if(Add1<=10)	Add1++;							
//			else SetUp=1;
			  
//      if(SetUp==0)	
				LED1=!LED1;
//			if(SetUp==1)
//			{
//				LED1=!LED1;
//				LED0=1;
//			}
			vTaskDelay(500);
			//delay_ms(500);
   testled=2;
		}
}   

	
//�ֶ�/�Զ����������� 
void JogAutoMode_task(void *pvParameters)
{
  
	
	
	while(1)
    {
			if(AGVStatus.AUTOmode==0 && (AGVStatus.EmerStop == 0))			//�ֶ�����
			{
        if(AGVModeChange == 1)
				{
						AGVModeChange = 0;
		
						MotorMsg[0].WtStatus = 5;
		        MotorMsg[0].WtSpd = 0;
		
		        WtStatus_1=5;									//д���ֿ�����
		        WtSpd_1=0;
		
		        MotorMsg[1].WtStatus = 5;
		        MotorMsg[1].WtSpd = 0;
		
						WtStatus_2=5;									//д���ֿ�����
						WtSpd_2=0;
				}
				
				if(AGVStatus.SLAMAutoMark==1&&barry==1)	SLAM_AutoContral();//2023.3.20gai
        
					
			}
			else if(AGVStatus.AUTOmode==1 && (AGVStatus.EmerStop ==0))				//�Զ�����
			{
//				front_pid.Proportion=2.5;
//				front_pid.Integral=0.001;
//				front_pid.SetPoint=0;
//				

//				angle_value=PID_RUN(&front_pid,(float)Magnetic.value[0],1);		//PID 
//				angleTemp=angle_value/10+90;				//���ݵ���
//				angle_value=angle_value/10+90;
//				if(angle_value>90+15)  angle_value=105;			//�޷�
//				if(angle_value<90-15)  angle_value=75;
//				VehiclePar=Vehicle_count(VehicleSpd,angle_value);			
//				TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//				Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);	
//				if(Magnetic.value[1]==0) Wheel_control(0,0,MotorMsg);	
				if(AGVModeChange == 1)
				{
						AGVModeChange = 0;
		
						MotorMsg[0].WtStatus = 5;
		        MotorMsg[0].WtSpd = 0;
		
		        WtStatus_1=5;									//д���ֿ�����
		        WtSpd_1=0;
		
		        MotorMsg[1].WtStatus = 5;
		        MotorMsg[1].WtSpd = 0;
		
						WtStatus_2=5;									//д���ֿ�����
						WtSpd_2=0;;
				}	
				
				HC_05_Trans(Bluetooth.buff);	
									
					
			}			
        vTaskDelay(20);				
    }
}  

						

//��ɢ��GPIO�ź�״̬��ȡ�����
void IO_Scan_task(void *pvParameters)
{
	while(1)
	{		
	test3=1;
		GPIO_INOUT_Scan();
		AGVsta_Scan(1);	           //��ť״̬���	
		if(IOStatus.IC[1]==1 && keyTemp!=1)	AGVStatus.temp=!AGVStatus.temp;
		keyTemp=IOStatus.IC[1];
		if(AGVStatus.temp==1) LED2=0;
		else LED2=1;
		MNSV_X8IO_Read(Magnetic.value);
		
		
	
////��ʳ������
//		if(!AGVStatus.EmerStop&&AGVStatus.AUTOmode==2&&AGVStatus.MapDownMode==0)          				 //�Ǽ�ͣ�����Զ�ģʽ
//		{		
//			Outside_Auto();	//�����Զ�ģʽ
//			test3=2;
//		}
//		else if(!AGVStatus.EmerStop&&AGVStatus.AUTOmode==1&&AGVStatus.MapDownMode==0)							//�Ǽ�ͣ�����Զ�ģʽ
//		{
//			Inside_Auto();													//�����Զ�ģʽ		
//		test3=3;	
//		}
//		else if(!AGVStatus.EmerStop&&!AGVStatus.AJMode&&AGVStatus.MapDownMode==0)									//�Ǽ�ͣ�ֶ�ģʽ
//		{
//			JogMode_control();											//�ֶ�ģʽ����		
//			TeachMode_control();									  //ʾ��ģʽ	
//		test3=4;	
//		}	
//		else if(AGVStatus.MapDownMode==1)					//��ͼ����ģʽ
//		{
//			MapDownLoad_control();									//���ӵ�ͼ���ش���	
//		test3=5;
//			if((u16)Paramt_BUFF[10][10]==MapPot.Num) MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);			//��ͼ������			
//		}
//	  test3=6;

////////////////////////////////////////////////////////		
//		if(DacaiMark==2)															//���ۼ�5�Σ���ر�ˢ�´��������
//		{
//			Dacai_Display((PCTRL_MSG)cmd_buffer);				//AGV״̬ʵʱ��ʾ;
//			DacaiMark=0;
//		}	
//		test3=7;
//		//SymaStatus.linkStatus=Syma_check(SymaStatus.Yaogan,SymaStatus.Tmp_buf);				//ң�������ݼ��
//		//BatteryMsg=Battery_Capacity(USART1_RX_BUF);																		//���״̬��Ϣ����				
//		DacaiMark++;																																	//�����ѭ���������ۼ�		
////	MapPot1=Map_ReadFromFLASH(1,4);		
////	MapPot2=Map_ReadFromFLASH(5200,4);	
////	MapPot3=Map_ReadFromFLASH(3,4);	
////	MapPot.errNum=MapTable_check(200+2,2000.0);				//��ͼ������		
////	MapPotDsiplay= AGV_OutSideValue(gpsx,Smc345.scm345Angle,LontLati);
////	Ultrosonic=ultrosonic_Read(UART_RX_BUF4);	
//		
//	//	ultrosonic_process();					//���������ϴ���		
//		
//		//UltalEn=1;	
//		vTaskDelay(10);
	}
}

//CANOPENͨ��
void CANOPEN_task(void *pvParameters)
{
	static u8 i=0;

	while(1)
	{	
	
		MotorValue_Read(MotorMsg,&IOStatus.IB[1],&IOStatus.JD[1]);
		vTaskDelay(20);		
		i++;				
	}
}
///**************************************************************************
//Function: MPU6050 task
//Input   : none
//Output  : none
//�������ܣ�MPU6050����
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void MPU6050_task(void *pvParameters)
//{ 
//    u32 lastWakeTime = getSysTickCnt();
//    while(1)
//    {	
//			//��������100Hz��Ƶ������	
//			vTaskDelayUntil(&lastWakeTime, F2T(100));	  		
//		  //Acceleration information is read through IIC
//		  //ͨ��IIC��ȡ���ٶ���Ϣ   
//			MPU6050_Get_Accelerometer(accel);	
//		  //Read angular velocity information via IIC
//		  //ͨ��IIC��ȡ���ٶ���Ϣ		 
//			MPU6050_Get_Gyroscope(gyro);	
//    }
//} 

/**************************************************************************
Function: MPU9250 task
Input   : none
Output  : none
�������ܣ�MPU6050����
��ڲ�������
����  ֵ����
**************************************************************************/
void MPU9250_task(void *pvParameters)
{ 
   u32 lastWakeTime = getSysTickCnt();
    while(1)
    {	
			//��������100Hz��Ƶ������	
			vTaskDelayUntil(&lastWakeTime, F2T(100));	  		
		  //Acceleration information is read through IIC
		  //ͨ��IIC��ȡ���ٶ���Ϣ   
			MPU_Get_Accelerometer(accel);	
		  //Read angular velocity information via IIC
		  //ͨ��IIC��ȡ���ٶ���Ϣ		 
			MPU_Get_Gyroscope(gyro);  //Ҫ��
			
			//�õ�����������
		  MPU_Get_Magnetometer(magnetometer);
			
    }
}






void data_task(void *pvParameters)
{
	 u32 lastWakeTime = getSysTickCnt();
	
   while(1)
    {	
			//��������20Hz��Ƶ������
			vTaskDelayUntil(&lastWakeTime, F2T(20));
			//��Ҫ���з��͵����ݽ��и�ֵ
			
			#if barry      //2023.3.20
			data_transition(); 
			//��������1����������Ҫ�رպ�ģ��ʼ��TIM1_Cap_Init(0XFFFF,72-1);
			USART6_SEND();   
//			USART1_SEND();     //Serial port 3 (ROS) sends data  //����3(ROS)��������	
		  #endif
			
		}
}
//UDP��������
void UDP_SEND_task(void *p_arg)
{
	while(1)
	{
		if(0)
		{
			udp_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
		}
		vTaskDelay(1000);  //��ʱ10ms
	}
}


//�˻����漰�����洢���ܵ� 
void HMI_FLASH_task(void *pvParameters)
{
    while(1)
    {       		
			Dacai_Display((PCTRL_MSG)cmd_buffer);													//AGV״̬ʵʱ��ʾ		
			AGV_Init(CurrtScreenID);																			//AGV�����޸�
			SignalBranchPath_Station();																		//AGVͨ��RFID����ɲ��ֹ���		
			//Msg2AGV_Process(CallOutPos,AGVWorkStatus,(u8)Paramt_BUFF[37][8]);							//���AGV����Ϣ����	������
//			Trumpet_jude((u8)Paramt_BUFF[38][12]);													//����������������
//			ALMMSG2_HMI();																			//������Ϣ���͸�HMI	
	 		vTaskDelay(200);  //��ʱ100ms	
    }
}   





