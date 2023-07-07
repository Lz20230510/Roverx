#include "sys.h"
#include "usart6.h"
#include "HEAD.h"
//#include "mpu6050.h"
#include "mpu9250.h"
#include "vehicle.h" 




////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos ʹ��	  
//#endif
u8 REBUFF;
SEND_DATA Send_Data;
RECEIVE_DATA Receive_Data;
float Vz,Move_X;

uint8 cmd_buffer[CMD_MAX_SIZE];//�����ָ���

u8 USART6_RX_BUF[USART6_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART6_SD_BUF[USART6_REC_LEN]; //���ͻ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART6_RX_STA=0;       //����״̬���	


#if barry
#if EN_USART6_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART6_RX_BUF[USART6_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART6_RX_STA=0;       //����״̬���	
u8 mark=0;

//��ʼ��IO ����6 
//bound:������
void usart6_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��
 
	//����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOG9����ΪUSART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOG14����ΪUSART6
	
	//USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOG9��GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
	
#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����6�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
	
}


 void USART6_IRQHandler(void)                	//����6�жϷ������
{
	static u8 Count=0;
	u8 Usart_Receive;
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Usart_Receive =USART_ReceiveData(USART6);//(USART6->DR);	//��ȡ���յ�������			
		USART_SendData(USART6,Usart_Receive);
//		USART_SendData(USART6,accel[2]);	
//    if(Time_count<CONTROL_DELAY)
//		  //����10��ǰ����������
//		  return 0;	
		//����������������
    Receive_Data.buffer[Count]=Usart_Receive;
		//ȷ�������һ������ΪFRAME_HEADER
		if(Usart_Receive == FRAME_HEADER||Count>0) 
			Count++; 
		else 
			Count=0;
		if (Count == 11)                            //��֤���ݰ��ĳ���
		{   
				Count=0;                                  //Ϊ����������������������׼��
				if(Receive_Data.buffer[10] == FRAME_TAIL) //��֤���ݰ���֡β
				{					
					AGVStatus.SLAMAutoMark=1;
					
			  }
		}
  }		  		 
} 
 

  
#endif	
#endif

///////////////////////////bcd��ת������//////////////////////////////
//�������ƣ�float bcd_to_hex(u8 * BCD)
//���룺BCDֵ
//�����
//���أ�������
//////////////////////////////////////////////////////////////////
float bcd_to_hex(u8 * BCD)
{
	float temp;
	if (*BCD==0x68)
	{
		temp=(float) ((*(BCD+4)>>4)*10 + (*(BCD+4)&0x0f))*100;
		temp+=(float) ((*(BCD+5)>>4)*10 + (*(BCD+5)&0x0f));
		temp+=(float) ((*(BCD+5)>>4)*10 + (*(BCD+5)&0x0f))/100;
	}

  return temp;
}

//////SCM_345_contralCmd ����λ��




/**************************************************************************
Function: Calculates the check bits of data to be sent/received
Input   : Count_Number: The first few digits of a check; Mode: 0-Verify the received data, 1-Validate the sent data
Output  : Check result
�������ܣ�����Ҫ����/���յ�����У����
��ڲ�����Count_Number��У���ǰ��λ����Mode��0-�Խ������ݽ���У�飬1-�Է������ݽ���У��
����  ֵ��У����
**************************************************************************/
u8 Check_Sum(unsigned char Count_Number,unsigned char Mode)
{
	unsigned char check_sum=0,k;
	//��Ҫ���͵����ݽ���У��
	if(Mode==1)
	for(k=0;k<Count_Number;k++)
	{
	check_sum=check_sum^Send_Data.buffer[k];
	}
	//Verify the data received
	//�Խ��յ������ݽ���У��
	if(Mode==0)
	for(k=0;k<Count_Number;k++)
	{
	check_sum=check_sum^Receive_Data.buffer[k];
	}
	return check_sum;	
}


/**************************************************************************
Function: The data sent by the serial port is assigned
Input   : none
Output  : none
�������ܣ����ڷ��͵����ݽ��и�ֵ
��ڲ�������
����  ֵ����
**************************************************************************/
void data_transition(void)
{
	Send_Data.Sensor_Str.Frame_Header = FRAME_HEADER; //Frame_header //֡ͷ
	Send_Data.Sensor_Str.Frame_Tail = FRAME_TAIL;     //Frame_tail //֡β
	
	//Forward kinematics solution, from the current speed of each wheel to calculate the current speed of the three axis
	//�˶�ѧ���⣬�Ӹ����ֵ�ǰ�ٶ�������ᵱǰ�ٶ�
	Send_Data.Sensor_Str.X_speed = (MotorMsg[0].RdSpd+MotorMsg[1].RdSpd)/MotorMsg[0].Ratio/2*1000; //С��x���ٶ�
	Send_Data.Sensor_Str.Y_speed = 0;
	Send_Data.Sensor_Str.Z_speed = (MotorMsg[1].RdSpd-MotorMsg[0].RdSpd)/MotorMsg[0].Ratio/Vehicle_W * 1000;//С��z���ٶ�
//	Send_Data.Sensor_Str.Z_speed = (MotorMsg[0].RdSpd+MotorMsg[1].RdSpd)/MotorMsg[0].Ratio/2    Vehicle_L * 1000;//С��z���ٶ�
	
	//The acceleration of the triaxial acceleration //���ٶȼ�������ٶ�
	Send_Data.Sensor_Str.Accelerometer.X_data= accel[1]; //The accelerometer Y-axis is converted to the ros coordinate X axis //���ٶȼ�Y��ת����ROS����X��
	Send_Data.Sensor_Str.Accelerometer.Y_data= accel[0]; //The accelerometer X-axis is converted to the ros coordinate y axis //���ٶȼ�X��ת����ROS����Y��
	Send_Data.Sensor_Str.Accelerometer.Z_data= accel[2]; //The accelerometer Z-axis is converted to the ros coordinate Z axis //���ٶȼ�Z��ת����ROS����Z��
	
	//The Angle velocity of the triaxial velocity //���ٶȼ�������ٶ�
	Send_Data.Sensor_Str.Gyroscope.X_data=  gyro[1]; //The Y-axis is converted to the ros coordinate X axis //���ٶȼ�Y��ת����ROS����X��
	Send_Data.Sensor_Str.Gyroscope.Y_data= -gyro[0]; //The X-axis is converted to the ros coordinate y axis //���ٶȼ�X��ת����ROS����Y��
	if(Send_Data.buffer[1]==0) 
		//If the motor control bit makes energy state, the z-axis velocity is sent normall
	  //����������λʹ��״̬����ô��������Z����ٶ�
		Send_Data.Sensor_Str.Gyroscope.Z_data=gyro[2];  
	else  
		//If the robot is static (motor control dislocation), the z-axis is 0
    //����������Ǿ�ֹ�ģ��������λʧ�ܣ�����ô���͵�Z����ٶ�Ϊ0		
		Send_Data.Sensor_Str.Gyroscope.Z_data=0;  
	
	//Battery voltage (this is a thousand times larger floating point number, which will be reduced by a thousand times as well as receiving the data).
	//��ص�ѹ(���ｫ�������Ŵ�һǧ�����䣬��Ӧ���ڽ��ն��ڽ��յ����ݺ�Ҳ����Сһǧ��)
	Send_Data.Sensor_Str.Power_Voltage = 22.222*1000; 
	
	Send_Data.buffer[0]=Send_Data.Sensor_Str.Frame_Header; //Frame_heade //֡ͷ
	
	if((MotorMsg[0].RdStatus==0x01||MotorMsg[0].RdStatus==0x02||MotorMsg[0].RdStatus==0x03)&&
		(MotorMsg[1].RdStatus==0x01||MotorMsg[1].RdStatus==0x02||MotorMsg[1].RdStatus==0x03))
		Send_Data.buffer[1]=1;                        //Car software loss marker //С�����ʧ�ܱ�־λ
	else Send_Data.buffer[1]=0;
  

	
	//The three-axis speed of / / car is split into two eight digit Numbers
	//С�������ٶ�,���ᶼ���Ϊ����8λ�����ٷ���
	Send_Data.buffer[2]=Send_Data.Sensor_Str.X_speed >>8; 
	Send_Data.buffer[3]=Send_Data.Sensor_Str.X_speed ;    
	Send_Data.buffer[4]=Send_Data.Sensor_Str.Y_speed>>8;  
	Send_Data.buffer[5]=Send_Data.Sensor_Str.Y_speed;     
	Send_Data.buffer[6]=Send_Data.Sensor_Str.Z_speed >>8; 
	Send_Data.buffer[7]=Send_Data.Sensor_Str.Z_speed ;    
	
	//The acceleration of the triaxial axis of / / imu accelerometer is divided into two eight digit reams
	//IMU���ٶȼ�������ٶ�,���ᶼ���Ϊ����8λ�����ٷ���
	Send_Data.buffer[8]=Send_Data.Sensor_Str.Accelerometer.X_data>>8; 
	Send_Data.buffer[9]=Send_Data.Sensor_Str.Accelerometer.X_data;   
	Send_Data.buffer[10]=Send_Data.Sensor_Str.Accelerometer.Y_data>>8;
	Send_Data.buffer[11]=Send_Data.Sensor_Str.Accelerometer.Y_data;
	Send_Data.buffer[12]=Send_Data.Sensor_Str.Accelerometer.Z_data>>8;
	Send_Data.buffer[13]=Send_Data.Sensor_Str.Accelerometer.Z_data;
	
	//The axis of the triaxial velocity of the / /imu is divided into two eight digits
	//IMU���ٶȼ�������ٶ�,���ᶼ���Ϊ����8λ�����ٷ���
	Send_Data.buffer[14]=Send_Data.Sensor_Str.Gyroscope.X_data>>8;
	Send_Data.buffer[15]=Send_Data.Sensor_Str.Gyroscope.X_data;
	Send_Data.buffer[16]=Send_Data.Sensor_Str.Gyroscope.Y_data>>8;
	Send_Data.buffer[17]=Send_Data.Sensor_Str.Gyroscope.Y_data;
	Send_Data.buffer[18]=Send_Data.Sensor_Str.Gyroscope.Z_data>>8;
	Send_Data.buffer[19]=Send_Data.Sensor_Str.Gyroscope.Z_data;
	
	//Battery voltage, split into two 8 digit Numbers
	//��ص�ѹ,���Ϊ����8λ���ݷ���
	Send_Data.buffer[20]=Send_Data.Sensor_Str.Power_Voltage >>8; 
	Send_Data.buffer[21]=Send_Data.Sensor_Str.Power_Voltage; 

  //Data check digit calculation, Pattern 1 is a data check
  //����У��λ���㣬ģʽ1�Ƿ�������У��
	Send_Data.buffer[22]=Check_Sum(22,1); 
	Send_Data.buffer[23]=Send_Data.Sensor_Str.Frame_Tail; //Frame_tail //֡β
}


/**************************************************************************
�������ܣ�����6��������
��ڲ�������
����  ֵ����
**************************************************************************/
void USART6_SEND(void)
{
  unsigned char i = 0;
//  USART_SendData(USART6,Send_Data.buffer[14]);	
	for(i=0; i<24; i++)
	{
		usart6_send(Send_Data.buffer[i]);
	}	 
}


/**************************************************************************
�������ܣ�����3��������
��ڲ�����Ҫ���͵�����
����  ֵ����
**************************************************************************/
void usart6_send(u8 data)
{
	USART6->DR = data;
	while((USART6->SR&0x40)==0);	
}


void SLAM_AutoContral(void)
{
		if(Receive_Data.buffer[9] ==Check_Sum(9,0))	 
		{							
//		float Vz,Move_X;
			//�Ӵ�������������Ŀ���ٶȣ� ��λm/s
				Move_X=XYZ_Target_Speed_transition(Receive_Data.buffer[3],Receive_Data.buffer[4]);
				Vz    =XYZ_Target_Speed_transition(Receive_Data.buffer[7],Receive_Data.buffer[8]);						
							
				VehiclePar=Vehicle_count_Usart(Move_X,Vz);						//�ٶȿ������

				Steering_contral(VehiclePar.PwmWheel);			
				//TIM_SetCompare4(TIM4,5000-VehiclePar.PwmWheel);	//�޸ıȽ�ֵ���޸�ռ�ձ�
			
				Wheel_control(-VehiclePar.SpdLBrmp,VehiclePar.SpdRBrmp,MotorMsg);				//�����ٶȿ���
			}	
		AGVStatus.SLAMAutoMark=0;
}
//#endif



///////////////////////���������ݷ���//////////////////////////////
//�������ƣ�SCM_345_contralCmd(u8 lenth,u8 * cmd)
//���룺u8 lenth:���ݳ���   u8 * cmd��ָ��
//�����
//���أ�������
//////////////////////////////////////////////////////////////////
void SCM_345_contralCmd(u8 lenth,u8 * cmd)
{
	u8 t;
	for(t=0;t<lenth;t++)
	{
		USART_SendData(USART6, cmd[t]);         //�򴮿�1��������
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}

}


#if dacai

void usart6_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��
 
	//����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOG9����ΪUSART6
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOG14����ΪUSART6
	
	//USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOG9��GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1  
	USART_ClearFlag(USART6, USART_FLAG_TC);	
	
	
#if EN_USART6_RX
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
#endif
	
}



void USART6_IRQHandler(void)
{
	u8 Res;
   if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET)//�����ж���Ч,���������ݼĴ�����
   {													
			uint8_t data = USART_ReceiveData(USART6);
			queue_push(data);
		 
			if((USART6_RX_STA&0x8000)==0)//����δ���
			{
				if(USART6_RX_STA&0x4000)//���յ���0x0d
				{
					if(Res!=0x0a)USART6_RX_STA=0;//���մ���,���¿�ʼ
					else USART6_RX_STA|=0x8000;	//��������� 
				}
				else //��û�յ�0X0D
				{	
					if(Res==0x0d)USART6_RX_STA|=0x4000;
					else
					{
						USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res ;
						USART6_RX_STA++;
						if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			} 		 	 
	  }
}


#endif
void SendChar(uchar t)
{
    USART_SendData(USART6,t);
    while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
}