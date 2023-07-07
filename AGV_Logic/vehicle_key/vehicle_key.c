#include "delay.h" 
#include "vehicle_key.h" 
//#include "pstwo.h"
#include "AGV_contr.h" 
#include "RAD.h" 
#include "HEAD.h"


//������ʼ������
void Vehicle_key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOF,GPIOD,GPIOG ʱ��
 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //����/��ͣ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //ģʽ�л�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOF
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //��ͣ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOD
} 


//��ͣ�ⲿ�жϳ�ʼ������ 
void KEYEXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
  Vehicle_key_Init();            //��ť��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource14);//P14 ���ӵ��ж���14

  /* ����EXTI_Line14 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;//LINE14
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;	//�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE14
  EXTI_Init(&EXTI_InitStructure);//����
	 	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����	   
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
	u8 static  SON_Add=0;                 //�ŷ�ʹ��״̬ѭ������λ
	u8 static  DacaiJOGSetupStor=0;                 //�ŷ�ʹ��״̬ѭ������λ
	u8 static  DacaiAUTOmodeStor=0;						//����Զ�ģʽѡ��ť״̬�洢
	u8 static  DacaiTeachModeStor=0;					//���ʾ��ģʽ������ť
	u8 static  DacaiMapDownStor=0;					//��ͼ����ģʽ����
	
	if(mode2==0)								//�ϵ���
	{
		if(IN_EmerStop==0)              	//���ƶ������˽��뼱ͣ״̬
		{
			AGVStatus.SONSetup=0;             //�ƶ�����������״̬��λ	
			AGVStatus.EmerStop=1;	      			 //��ͣ��־λ��λ
			AGVStatus.AJMode=0;								//��ͣʱ���豸�Զ��ָ��ֶ�״̬
			OT_SON=0;												//��ʹ��
				
		}
		AGVStatus.SONSetup=0;             //�ƶ�����������״̬��λ
			               //�ƶ������˴����ֶ�״̬
	}	
	
	if(mode2==1)                         //�ƶ��������Ѿ��ϵ�
	{
		if(IN_EmerStop==0)              	//���ƶ������˽��뼱ͣ״̬
		{
			AGVStatus.EmerStop=1;	      			 //��ͣ��־λ��λ
			AGVStatus.SONSetup=0;             //�ƶ�����������״̬��λ	
			AGVStatus.Setup=0;										//������ģʽ
			OT_SON=0;												//��ʹ��
			AGVStatus.AJMode=0;								//��ͣʱ���豸�Զ��ָ��ֶ�״̬
		}
		else if(IN_EmerStop==1)  				//���ƶ��������˳���ͣ״̬
		{
			AGVStatus.EmerStop=0;	      							 //��ͣ��־��λ
		}
		if(IN_EmerStop==1)		//���ƶ��������˳���ͣ״̬
		{
			if(DacaiButton.Pg0_SONSetup==1 && DacaiButton.Pg0_SONSetup!=SONSetup_Stor)							//�ƶ�����������/��ͣ									
			{		
				AGVStatus.SONSetup=!AGVStatus.SONSetup;             //�ƶ�����������״̬ȡ�� 
				AGVStatus.Setup=AGVStatus.SONSetup;	
				if(AGVStatus.SONSetup==1&&(AGVStatus.SevAlm[0]==1||AGVStatus.SevAlm[1]==1||AGVStatus.SevAlm[2]==1||AGVStatus.SevAlm[3]==1))         //��������ť�������б������
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
//				AGVStatus.AJMode=!AGVStatus.AJMode;             //�ƶ����������Զ�״̬ȡ��	
//				
//				if(AGVStatus.AJMode==0)   AGVStatus.AUTOmode=0;   //�ֶ�ģʽ����ʱ���Զ�״̬Ϊ0
//				else AGVStatus.AUTOmode=1;   //�ֶ�ģʽ�ر�ʱ���Զ�״̬Ϊ1
//				
//			}			
//			if(AGVStatus.AJMode==1)					//���ֶ�ģʽ�ر�ʱ
//			{
//				if(AGVStatus.AUTOmode==1 && DacaiAUTOmodeStor!=DacaiButton.Pg0_AUTOmode && DacaiButton.Pg0_AUTOmode==1)  AGVStatus.AUTOmode=2;				//�Զ�ģʽ�л�������ģʽ
//				else if(AGVStatus.AUTOmode==2 && DacaiAUTOmodeStor!=DacaiButton.Pg0_AUTOmode && DacaiButton.Pg0_AUTOmode==1)  AGVStatus.AUTOmode=1;				//�Զ�ģʽ�л�������ģʽ
//				AGVStatus.TeachMode=0;															//�ֶ�ģʽ�ر�ʱ���ر�ʾ��ģʽ
//				AGVStatus.MapDownMode=0;															//�ֶ�ģʽ�ر�ʱ���رճ�������ģʽ
//			}
		}			
	}	

		
		if((AGVStatus.AUTOmode==1||AGVStatus.AUTOmode==2)&&AGVStatus.SONSetup==1&&AGVStatus.EmerStop==0) 				//���Զ�״̬�£�����ʹ�ܣ��޼�ͣ�������������
		{
			if(IN_AUTOSetup==1&&IN_AUTOSetup!=AUTORun_Stor)	AGVStatus.AutoRun=!AGVStatus.AutoRun;			
		}
		else AGVStatus.AutoRun=0;				//�����ֹ�Զ�����
	
//		if(AGVStatus.AJMode==0) AGVStatus.AUTOmode=0;					//�ֶ�״̬�£��Զ�״̬�Զ��ر�
		
		if(DacaiButton.Pg4_TeachMode==1&&DacaiTeachModeStor!=DacaiButton.Pg4_TeachMode&&AGVStatus.AJMode==0) AGVStatus.TeachMode=!AGVStatus.TeachMode;

		
		if(AGVStatus.AUTOmode==2||AGVStatus.TeachMode==1) OT_JD1=0;					//����ģʽʱ
		else if(AGVStatus.AUTOmode!=2&&AGVStatus.TeachMode!=1) OT_JD1=1;
		
		if(OT_SON==1)             									  //������ģʽ��������ͻȻ��ʹ��,˵������������
		{
			if(SON_Add==5&& !(IN_SEV1==1&&IN_SEV2==1&&IN_SEV3==1&&IN_SEV4==1))    //��ѭ����������5�Σ����ŷ�ʹ�ܲ���ȫ������˵������������
			{		
				AGVStatus.SONSetup=0;     //����״̬�ر�
				OT_SON=0;     					//ʹ�ܹر�	
					
				if(IN_SEV1==0)	AGVStatus.SevAlm[0]=1;																					//�ŷ�1����
				if(IN_SEV2==0)	AGVStatus.SevAlm[1]=1;																					//�ŷ�2����
				if(IN_SEV3==0)	AGVStatus.SevAlm[2]=1;																					//�ŷ�3����
				if(IN_SEV4==0)	AGVStatus.SevAlm[3]=1;																					//�ŷ�4����
			}	
			if(SON_Add<6) SON_Add++;																						//�������ۼ�
		}
		else SON_Add=0;              																					//�������ر�		
		
		
		
		if(AGVStatus.AJMode==0&&DacaiButton.Pg4_MapDown==1&&DacaiMapDownStor!=DacaiButton.Pg4_MapDown)			//��ͼ�����������ر�
		{
			if(AGVStatus.MapDownMode!=0)
			{
				AGVStatus.MapDownMode=0;							//��ͼ���عر�		
				uart_init(9600,3,3);        		  //USART1��ʼ��  	�޸Ĵ���1���ȼ�
			}				
			else if(AGVStatus.MapDownMode==0)
			{
				AGVStatus.MapDownMode=1;							//��ͼ��������
				uart_init(9600,0,1);        		  //USART1��ʼ��  	�޸Ĵ���1���ȼ�	
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
//��ť��������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��IN_EmerStop����
//2��mode����
//3��start_up����
//ע��˺�������Ӧ���ȼ�,IN_EmerStop>mode>start_up
////////////////////////////////////////////////////////////////////////////
u8 Vehkey_scan(u8 mode1)
{	 
	static u8 vehiclekey_up=1;//��ť�ɿ���־
	if(mode1)vehiclekey_up=1;  //֧������		  
	if(vehiclekey_up&&(IN_JOGSetup==0||IN_AUTOSetup==0))
	{
		delay_ms(20);//ȥ���� 
		vehiclekey_up=0;
		if(IN_JOGSetup==0)return 2;
		if(IN_AUTOSetup==0)return 3;
	}
	else if(IN_JOGSetup==1&& IN_AUTOSetup==1)vehiclekey_up=1; 	    
 	return 0;// �ް�������
}

/////////////////////�ⲿ�ж�15_10�������/////////////////////////
//
//
///////////////////////////////////////////////////////////////////
void EXTI15_10_IRQHandler(void)
{	
	delay_ms(10);	//����
	if(IN_EmerStop==0)												//����ͣ��ť����	
	{
		AGVStatus.EmerStop=1;	      							//��ͣ��־λ��λ
		OT_SON=0;																//��ʹ��
		AGVStatus.Setup=0;
	}
	else if(IN_EmerStop==1)                   //��ͣ��ť�ɿ� 
	{
		AGVStatus.EmerStop=0;	      							//��ͣ��־��λ
		
	}
	EXTI_ClearITPendingBit(EXTI_Line14);//���LINE14�ϵ��жϱ�־λ  	
}

