/******************************************************************************
* 文件名：FreeRTOSDef.c
* 版  本：
* 作  者：
* 日  期：
* 库版本：   
* 文件描述：调用所有头文件
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
u8 test3=0;//室内、室外测试
u8 test5=0;//手动自动函数测试
u8 testled=0;
u8 mpu6050test=0;
u8 keyTemp;	
float angle_value,angleTemp;
u8 temp11[9];
u8 SetUp=0;
u16 Add1=0;

u8 AGVModeChange;

u8 DacaiMark=0;

//开始任务任务函数
void start_task(void *pvParameters)
{	
//		while(lwip_comm_init()) delay_ms(500);	    //lwip初始化
//		while(udp_demo_init()==pdFAIL)delay_ms(500);		//当完成了DHCP任务之后，再开启UDP任务。初始化udp_demo(创建udp_demo线程)
//		taskENTER_CRITICAL();           //进入临界区	
//		#if LWIP_DHCP
//			lwip_comm_dhcp_creat(); //创建DHCP任务，这里面有包含了UDP任务的开启。
//		#endif
//		VehiclePar.Ratio=DriveMotor_Ratio(0.24,30);		//减速比
//		VehiclePar.Ratio=DriveMotor_Ratio(0.24,30);		//减速比
		VehiclePar.Ratio=2387.326;
		MotorMsg[0].Ratio=VehiclePar.Ratio;
		MotorMsg[1].Ratio=VehiclePar.Ratio;
		VehicleSpd=0.5;
	

    //创建蓝牙遥控任务	
    xTaskCreate((TaskFunction_t )JogAutoMode_task,     	
                (const char*    )"JogAutoMode_task",   	
                (uint16_t       )JOGAUTOMODE_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )JOGAUTOMODE_TASK_PRIO,	
                (TaskHandle_t*  )&JOGAUTOMODETask_Handler); 
	
//	  //IMU data read task //IMU数据读取任务
//		xTaskCreate((TaskFunction_t )MPU6050_task, 
//								(const char*    )"MPU6050_task", 
//								(uint16_t       )MPU6050_STK_SIZE, 
//								(void*          )NULL, 
//								(UBaseType_t    )MPU6050_TASK_PRIO, 
//								(TaskHandle_t*  )NULL);	
		
		//Usart6发送数据任务					
		xTaskCreate((TaskFunction_t )data_task,
								(const char*    )"data_task",
								(uint16_t       )DATA_STK_SIZE,
								(void*          )NULL, 
								(UBaseType_t    )DATA_TASK_PRIO,
								(TaskHandle_t*  )NULL);				
		//离散量GPIO状态读取						
		xTaskCreate((TaskFunction_t )IO_Scan_task,     
                (const char*    )"IO_Scan_task",   
                (uint16_t       )IOSCAN_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )IOSCAN_TASK_PRIO,
                (TaskHandle_t*  )&IOSCANTask_Handler);	

							
		//CANOPEN任务					
		xTaskCreate((TaskFunction_t )CANOPEN_task,     
                (const char*    )"CANOPEN_task",   
                (uint16_t       )CANOPEN_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )CANOPEN_TASK_PRIO,
                (TaskHandle_t*  )&CANOPENTask_Handler);	
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler); 
#if 0								 
		//IMU数据以及磁力计数据读取任务   2023.3.30
		xTaskCreate((TaskFunction_t )MPU9250_task, 
								(const char*    )"MPU9250_task", 
								(uint16_t       )MPU9250_STK_SIZE, 
								(void*          )NULL, 
								(UBaseType_t    )MPU9250_TASK_PRIO, 
								(TaskHandle_t*  )NULL);							
								
#endif								
								
		//UDP数据发送任务					
//		xTaskCreate((TaskFunction_t )UDP_SEND_task,     
//                (const char*    )"UDP_SEND_task",   
//                (uint16_t       )UDP_SEND_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )UDP_SEND_TASK_PRIO,
//                (TaskHandle_t*  )&UDP_SENDTask_Handler);		


		//HMI_FLASH任务					
//		xTaskCreate((TaskFunction_t )HMI_FLASH_task,     
//                (const char*    )"HMI_FLASH_task",   
//                (uint16_t       )HMI_FLASH_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )HMI_FLASH_TASK_PRIO,
//                (TaskHandle_t*  )&HMI_FLASHTask_Handler);
								
							
		vTaskDelay(1500);
		CANOPEN_Node_Start();									//CANOPEN启动节点																				
    vTaskDelete(StartTask_Handler); 			//删除开始任务				
    taskEXIT_CRITICAL();            			//退出临界区								
		TIM_SetCompare4(TIM4,5000-Median); 		//舵机回中
}

//LED0任务函数   //LED1 核心板指示灯
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

	
//手动/自动控制任务函数 
void JogAutoMode_task(void *pvParameters)
{
  
	
	
	while(1)
    {
			if(AGVStatus.AUTOmode==0 && (AGVStatus.EmerStop == 0))			//手动控制
			{
        if(AGVModeChange == 1)
				{
						AGVModeChange = 0;
		
						MotorMsg[0].WtStatus = 5;
		        MotorMsg[0].WtSpd = 0;
		
		        WtStatus_1=5;									//写左轮控制字
		        WtSpd_1=0;
		
		        MotorMsg[1].WtStatus = 5;
		        MotorMsg[1].WtSpd = 0;
		
						WtStatus_2=5;									//写左轮控制字
						WtSpd_2=0;
				}
				
				if(AGVStatus.SLAMAutoMark==1&&barry==1)	SLAM_AutoContral();//2023.3.20gai
        
					
			}
			else if(AGVStatus.AUTOmode==1 && (AGVStatus.EmerStop ==0))				//自动控制
			{
//				front_pid.Proportion=2.5;
//				front_pid.Integral=0.001;
//				front_pid.SetPoint=0;
//				

//				angle_value=PID_RUN(&front_pid,(float)Magnetic.value[0],1);		//PID 
//				angleTemp=angle_value/10+90;				//数据调整
//				angle_value=angle_value/10+90;
//				if(angle_value>90+15)  angle_value=105;			//限幅
//				if(angle_value<90-15)  angle_value=75;
//				VehiclePar=Vehicle_count(VehicleSpd,angle_value);			
//				TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//修改比较值，修改占空比
//				Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);	
//				if(Magnetic.value[1]==0) Wheel_control(0,0,MotorMsg);	
				if(AGVModeChange == 1)
				{
						AGVModeChange = 0;
		
						MotorMsg[0].WtStatus = 5;
		        MotorMsg[0].WtSpd = 0;
		
		        WtStatus_1=5;									//写左轮控制字
		        WtSpd_1=0;
		
		        MotorMsg[1].WtStatus = 5;
		        MotorMsg[1].WtSpd = 0;
		
						WtStatus_2=5;									//写左轮控制字
						WtSpd_2=0;;
				}	
				
				HC_05_Trans(Bluetooth.buff);	
									
					
			}			
        vTaskDelay(20);				
    }
}  

						

//离散型GPIO信号状态读取与输出
void IO_Scan_task(void *pvParameters)
{
	while(1)
	{		
	test3=1;
		GPIO_INOUT_Scan();
		AGVsta_Scan(1);	           //按钮状态检测	
		if(IOStatus.IC[1]==1 && keyTemp!=1)	AGVStatus.temp=!AGVStatus.temp;
		keyTemp=IOStatus.IC[1];
		if(AGVStatus.temp==1) LED2=0;
		else LED2=1;
		MNSV_X8IO_Read(Magnetic.value);
		
		
	
////大彩程序添加
//		if(!AGVStatus.EmerStop&&AGVStatus.AUTOmode==2&&AGVStatus.MapDownMode==0)          				 //非急停室外自动模式
//		{		
//			Outside_Auto();	//室外自动模式
//			test3=2;
//		}
//		else if(!AGVStatus.EmerStop&&AGVStatus.AUTOmode==1&&AGVStatus.MapDownMode==0)							//非急停室内自动模式
//		{
//			Inside_Auto();													//室内自动模式		
//		test3=3;	
//		}
//		else if(!AGVStatus.EmerStop&&!AGVStatus.AJMode&&AGVStatus.MapDownMode==0)									//非急停手动模式
//		{
//			JogMode_control();											//手动模式控制		
//			TeachMode_control();									  //示教模式	
//		test3=4;	
//		}	
//		else if(AGVStatus.MapDownMode==1)					//地图下载模式
//		{
//			MapDownLoad_control();									//电子地图下载处理	
//		test3=5;
//			if((u16)Paramt_BUFF[10][10]==MapPot.Num) MapPot.errNum=MapTable_check((u16)Paramt_BUFF[10][10],2000.0);			//地图错误检查			
//		}
//	  test3=6;

////////////////////////////////////////////////////////		
//		if(DacaiMark==2)															//当累加5次，则关闭刷新大彩屏数据
//		{
//			Dacai_Display((PCTRL_MSG)cmd_buffer);				//AGV状态实时显示;
//			DacaiMark=0;
//		}	
//		test3=7;
//		//SymaStatus.linkStatus=Syma_check(SymaStatus.Yaogan,SymaStatus.Tmp_buf);				//遥控器数据检查
//		//BatteryMsg=Battery_Capacity(USART1_RX_BUF);																		//电池状态信息处理				
//		DacaiMark++;																																	//大彩屏循环计数器累加		
////	MapPot1=Map_ReadFromFLASH(1,4);		
////	MapPot2=Map_ReadFromFLASH(5200,4);	
////	MapPot3=Map_ReadFromFLASH(3,4);	
////	MapPot.errNum=MapTable_check(200+2,2000.0);				//地图错误检查		
////	MapPotDsiplay= AGV_OutSideValue(gpsx,Smc345.scm345Angle,LontLati);
////	Ultrosonic=ultrosonic_Read(UART_RX_BUF4);	
//		
//	//	ultrosonic_process();					//超声波避障处理		
//		
//		//UltalEn=1;	
//		vTaskDelay(10);
	}
}

//CANOPEN通信
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
//函数功能：MPU6050任务
//入口参数：无
//返回  值：无
//**************************************************************************/
//void MPU6050_task(void *pvParameters)
//{ 
//    u32 lastWakeTime = getSysTickCnt();
//    while(1)
//    {	
//			//此任务以100Hz的频率运行	
//			vTaskDelayUntil(&lastWakeTime, F2T(100));	  		
//		  //Acceleration information is read through IIC
//		  //通过IIC读取加速度信息   
//			MPU6050_Get_Accelerometer(accel);	
//		  //Read angular velocity information via IIC
//		  //通过IIC读取角速度信息		 
//			MPU6050_Get_Gyroscope(gyro);	
//    }
//} 

/**************************************************************************
Function: MPU9250 task
Input   : none
Output  : none
函数功能：MPU6050任务
入口参数：无
返回  值：无
**************************************************************************/
void MPU9250_task(void *pvParameters)
{ 
   u32 lastWakeTime = getSysTickCnt();
    while(1)
    {	
			//此任务以100Hz的频率运行	
			vTaskDelayUntil(&lastWakeTime, F2T(100));	  		
		  //Acceleration information is read through IIC
		  //通过IIC读取加速度信息   
			MPU_Get_Accelerometer(accel);	
		  //Read angular velocity information via IIC
		  //通过IIC读取角速度信息		 
			MPU_Get_Gyroscope(gyro);  //要改
			
			//得到磁力计数据
		  MPU_Get_Magnetometer(magnetometer);
			
    }
}






void data_task(void *pvParameters)
{
	 u32 lastWakeTime = getSysTickCnt();
	
   while(1)
    {	
			//此任务以20Hz的频率运行
			vTaskDelayUntil(&lastWakeTime, F2T(20));
			//对要进行发送的数据进行赋值
			
			#if barry      //2023.3.20
			data_transition(); 
			//开启串口1发送数据需要关闭航模初始化TIM1_Cap_Init(0XFFFF,72-1);
			USART6_SEND();   
//			USART1_SEND();     //Serial port 3 (ROS) sends data  //串口3(ROS)发送数据	
		  #endif
			
		}
}
//UDP发送任务
void UDP_SEND_task(void *p_arg)
{
	while(1)
	{
		if(0)
		{
			udp_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
		}
		vTaskDelay(1000);  //延时10ms
	}
}


//人机界面及参数存储功能等 
void HMI_FLASH_task(void *pvParameters)
{
    while(1)
    {       		
			Dacai_Display((PCTRL_MSG)cmd_buffer);													//AGV状态实时显示		
			AGV_Init(CurrtScreenID);																			//AGV参数修改
			SignalBranchPath_Station();																		//AGV通过RFID卡完成部分功能		
			//Msg2AGV_Process(CallOutPos,AGVWorkStatus,(u8)Paramt_BUFF[37][8]);							//针对AGV的信息处理	呼叫器
//			Trumpet_jude((u8)Paramt_BUFF[38][12]);													//报警灯扬声器工作
//			ALMMSG2_HMI();																			//报警信息发送给HMI	
	 		vTaskDelay(200);  //延时100ms	
    }
}   





