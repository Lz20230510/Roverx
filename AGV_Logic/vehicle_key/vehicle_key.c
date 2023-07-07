#include "delay.h" 
#include "vehicle_key.h" 
//#include "pstwo.h"
#include "AGV_contr.h" 
#include "RAD.h" 
#include "HEAD.h"


//按键初始化函数
void Vehicle_key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF,GPIOD,GPIOG 时钟
 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //启动/暂停
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOG
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //模式切换
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOF
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //急停
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOD
} 


//急停外部中断初始化设置 
void KEYEXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
  Vehicle_key_Init();            //按钮初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource14);//P14 连接到中断线14

  /* 配置EXTI_Line14 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;//LINE14
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;	//下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE14
  EXTI_Init(&EXTI_InitStructure);//配置
	 	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置	   
}


/************************************************************/
//
//
//
//
/***********************************************************/

void AGVsta_Scan(u8 mode2)
{
	
	u8 static  AJMode_Stor=0;
	u8 static  SONSetup_Stor=0;
	u8 static  AUTORun_Stor=0;
	u8 static  SON_Add=0;                 //伺服使能状态循环计数位
	u8 static  DacaiJOGSetupStor=0;                 //伺服使能状态循环计数位
	u8 static  DacaiAUTOmodeStor=0;						//大彩自动模式选择按钮状态存储
	u8 static  DacaiTeachModeStor=0;					//大彩示教模式启动按钮
	u8 static  DacaiMapDownStor=0;					//地图下载模式启动
	
	if(mode2==0)								//上电检查
	{
		if(IN_EmerStop==0)              	//当移动机器人进入急停状态
		{
			AGVStatus.SONSetup=0;             //移动机器人启动状态复位	
			AGVStatus.EmerStop=1;	      			 //急停标志位置位
			AGVStatus.AJMode=0;								//急停时，设备自动恢复手动状态
			OT_SON=0;												//断使能
				
		}
		AGVStatus.SONSetup=0;             //移动机器人启动状态复位
			               //移动机器人处于手动状态
	}	
	
	if(mode2==1)                         //移动机器人已经上电
	{
		if(IN_EmerStop==0)              	//当移动机器人进入急停状态
		{
			AGVStatus.EmerStop=1;	      			 //急停标志位置位
			AGVStatus.SONSetup=0;             //移动机器人启动状态复位	
			AGVStatus.Setup=0;										//非启动模式
			OT_SON=0;												//断使能
			AGVStatus.AJMode=0;								//急停时，设备自动恢复手动状态
		}
		else if(IN_EmerStop==1)  				//当移动机器人退出急停状态
		{
			AGVStatus.EmerStop=0;	      							 //急停标志复位
		}
		if(IN_EmerStop==1)		//当移动机器人退出急停状态
		{
			if(DacaiButton.Pg0_SONSetup==1 && DacaiButton.Pg0_SONSetup!=SONSetup_Stor)							//移动机器人启动/暂停									
			{		
				AGVStatus.SONSetup=!AGVStatus.SONSetup;             //移动机器人启动状态取反 
				AGVStatus.Setup=AGVStatus.SONSetup;	
				if(AGVStatus.SONSetup==1&&(AGVStatus.SevAlm[0]==1||AGVStatus.SevAlm[1]==1||AGVStatus.SevAlm[2]==1||AGVStatus.SevAlm[3]==1))         //当启动按钮按下且有报警输出
				{
					AGVStatus.SevAlm[0]=0;
					AGVStatus.SevAlm[1]=0;
					AGVStatus.SevAlm[2]=0;
					AGVStatus.SevAlm[3]=0;
				}
			}
			
			if(IN_JOGSetup == 1)
			{
				AGVStatus.AUTOmode = 1;AGVModeChange = 1;
			}
			else {AGVStatus.AUTOmode = 0;AGVModeChange = 1;}
//			if((IN_JOGSetup==1 && AJMode_Stor!=IN_JOGSetup)||(DacaiButton.Pg0_JOGSetup==1 && DacaiJOGSetupStor!=DacaiButton.Pg0_JOGSetup))			
//			{		
//				AGVStatus.AJMode=!AGVStatus.AJMode;             //移动机器人手自动状态取反	
//				
//				if(AGVStatus.AJMode==0)   AGVStatus.AUTOmode=0;   //手动模式启动时，自动状态为0
//				else AGVStatus.AUTOmode=1;   //手动模式关闭时，自动状态为1
//				
//			}			
//			if(AGVStatus.AJMode==1)					//当手动模式关闭时
//			{
//				if(AGVStatus.AUTOmode==1 && DacaiAUTOmodeStor!=DacaiButton.Pg0_AUTOmode && DacaiButton.Pg0_AUTOmode==1)  AGVStatus.AUTOmode=2;				//自动模式切换到室外模式
//				else if(AGVStatus.AUTOmode==2 && DacaiAUTOmodeStor!=DacaiButton.Pg0_AUTOmode && DacaiButton.Pg0_AUTOmode==1)  AGVStatus.AUTOmode=1;				//自动模式切换到室内模式
//				AGVStatus.TeachMode=0;															//手动模式关闭时，关闭示教模式
//				AGVStatus.MapDownMode=0;															//手动模式关闭时，关闭程序下载模式
//			}
		}			
	}	

		
		if((AGVStatus.AUTOmode==1||AGVStatus.AUTOmode==2)&&AGVStatus.SONSetup==1&&AGVStatus.EmerStop==0) 				//当自动状态下，上了使能，无急停报警则可以运行
		{
			if(IN_AUTOSetup==1&&IN_AUTOSetup!=AUTORun_Stor)	AGVStatus.AutoRun=!AGVStatus.AutoRun;			
		}
		else AGVStatus.AutoRun=0;				//否则禁止自动运行
	
//		if(AGVStatus.AJMode==0) AGVStatus.AUTOmode=0;					//手动状态下，自动状态自动关闭
		
		if(DacaiButton.Pg4_TeachMode==1&&DacaiTeachModeStor!=DacaiButton.Pg4_TeachMode&&AGVStatus.AJMode==0) AGVStatus.TeachMode=!AGVStatus.TeachMode;

		
		if(AGVStatus.AUTOmode==2||AGVStatus.TeachMode==1) OT_JD1=0;					//室外模式时
		else if(AGVStatus.AUTOmode!=2&&AGVStatus.TeachMode!=1) OT_JD1=1;
		
		if(OT_SON==1)             									  //当启动模式下驱动器突然断使能,说明驱动器报警
		{
			if(SON_Add==5&& !(IN_SEV1==1&&IN_SEV2==1&&IN_SEV3==1&&IN_SEV4==1))    //当循环计数到第5次，且伺服使能不能全就绪，说明驱动器报警
			{		
				AGVStatus.SONSetup=0;     //启动状态关闭
				OT_SON=0;     					//使能关闭	
					
				if(IN_SEV1==0)	AGVStatus.SevAlm[0]=1;																					//伺服1报警
				if(IN_SEV2==0)	AGVStatus.SevAlm[1]=1;																					//伺服2报警
				if(IN_SEV3==0)	AGVStatus.SevAlm[2]=1;																					//伺服3报警
				if(IN_SEV4==0)	AGVStatus.SevAlm[3]=1;																					//伺服4报警
			}	
			if(SON_Add<6) SON_Add++;																						//计数器累加
		}
		else SON_Add=0;              																					//计数器关闭		
		
		
		
		if(AGVStatus.AJMode==0&&DacaiButton.Pg4_MapDown==1&&DacaiMapDownStor!=DacaiButton.Pg4_MapDown)			//地图下载启动、关闭
		{
			if(AGVStatus.MapDownMode!=0)
			{
				AGVStatus.MapDownMode=0;							//地图下载关闭		
				uart_init(9600,3,3);        		  //USART1初始化  	修改串口1优先级
			}				
			else if(AGVStatus.MapDownMode==0)
			{
				AGVStatus.MapDownMode=1;							//地图下载启动
				uart_init(9600,0,1);        		  //USART1初始化  	修改串口1优先级	
			} 
		}

		
		AJMode_Stor=IN_JOGSetup;
		AUTORun_Stor=IN_AUTOSetup;
		SONSetup_Stor=DacaiButton.Pg0_SONSetup;
		DacaiJOGSetupStor=DacaiButton.Pg0_JOGSetup;
		DacaiAUTOmodeStor=DacaiButton.Pg0_AUTOmode;
		DacaiTeachModeStor=DacaiButton.Pg4_TeachMode;
		DacaiMapDownStor=DacaiButton.Pg4_MapDown;
}		


//////////////////////////////////////////////////////////////////////////
//按钮处理处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，IN_EmerStop按下
//2，mode按下
//3，start_up按下
//注意此函数有响应优先级,IN_EmerStop>mode>start_up
////////////////////////////////////////////////////////////////////////////
u8 Vehkey_scan(u8 mode1)
{	 
	static u8 vehiclekey_up=1;//按钮松开标志
	if(mode1)vehiclekey_up=1;  //支持连按		  
	if(vehiclekey_up&&(IN_JOGSetup==0||IN_AUTOSetup==0))
	{
		delay_ms(20);//去抖动 
		vehiclekey_up=0;
		if(IN_JOGSetup==0)return 2;
		if(IN_AUTOSetup==0)return 3;
	}
	else if(IN_JOGSetup==1&& IN_AUTOSetup==1)vehiclekey_up=1; 	    
 	return 0;// 无按键按下
}

/////////////////////外部中断15_10服务程序/////////////////////////
//
//
///////////////////////////////////////////////////////////////////
void EXTI15_10_IRQHandler(void)
{	
	delay_ms(10);	//消抖
	if(IN_EmerStop==0)												//当急停按钮按下	
	{
		AGVStatus.EmerStop=1;	      							//急停标志位置位
		OT_SON=0;																//断使能
		AGVStatus.Setup=0;
	}
	else if(IN_EmerStop==1)                   //急停按钮松开 
	{
		AGVStatus.EmerStop=0;	      							//急停标志复位
		
	}
	EXTI_ClearITPendingBit(EXTI_Line14);//清除LINE14上的中断标志位  	
}

