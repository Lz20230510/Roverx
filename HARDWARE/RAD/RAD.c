#include "delay.h" 
#include "usart4.h" 
#include "RAD.h" 
#include "vehicle_key.h" 
#include "HEAD.h"


//u8 Comd_RAD[1]={0xAA};	//��ʼ�����������
//u8 Data_RAD[13]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
//extern u8 RAD[13]={0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55};


//*************************ʵ�ֺ���*****************************/
//void ultrosonic_GPIO_Init(void)
//���������		
//���ز�����  
//
//����˵�������������ϴ�����ʹ�ܿڳ�ʼ��
//**************************************************************/
void ultrosonic_GPIO_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOF  GPIOAʱ��	
	 //GPIOA8��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);//GPIOE8���øߣ�����
}


//*************************ʵ�ֺ���*****************************/
//u8 ultrosonic_check(u8 * buff)
//���������		���ڻ���	
//���ز�����  =1�����ճ�����������ȷ��=0�����ճ��������ݴ���
//
//����˵�����Գ��������ݽ��м�����
//**************************************************************/
u8 ultrosonic_check(u8 * buff)
{
	u8 i;
	u16 temp;
	if(*buff==0xAA&& *(buff+11)==0x55)
	{
		for(i=1;i<11;i++) temp+=(u16) buff[i];					//�����������ۼ�	
		temp=temp&0x00FF;																//���8λ
	}
	if(temp==*(buff+11))	return 1;										//��У����ȷ����1
	else return 1;																		//���󷵻�0
}


//*************************ʵ�ֺ���*****************************/
//UltrosonicDef ultrosonic_Read(u8 * buff)
//���������		���ڻ���	
//���ز�����  =1�����ճ�����������ȷ��=0�����ճ��������ݴ���
//
//����˵������ȡÿһ·�ĳ������ź�,��ȡÿһ·��������ȫ��������
//**************************************************************/
UltrosonicDef ultrosonic_Read(u8 * buff)
{
	u8 i;
	static UltrosonicDef ultrosonic;
	for(i=1;i<9;i++) ultrosonic.channel[i-1]=buff[i];			//�����������������ݸ�ֵ
	
	for(i=0;i<8;i++) 				//��ȡ�������ý����ϵĳ�������������
	{
		ultrosonic.safeDistance[i]=(u8)Paramt_BUFF[17][i+20];				//��ȡ��������������
	}	
	return ultrosonic;
}


/******************************ʵ�ֺ���*************************/
//������ʽ��void ultrosonic_process(void)
//�����������������ز����ṹ��
//���������
//
//���ز�������
//����˵��������������
/**************************************************************/
void ultrosonic_process(void)
{
	u8 i;
	static u8 safeOk[4],addMark[4];						//safeOk=0 ˵�������ڰ�ȫ��Χ�ڣ�safeOk=1 ˵�����ϲ��ڰ�ȫ��Χ��  

	Ultrosonic.deviceEn=(u8)Paramt_BUFF[Pg_CommonParmt][5];						//���ϴ������Ƿ��
	if(Ultrosonic.deviceEn==1 && AGVStatus.AJMode==1)						//��������ʹ��
	{
		for(i=0;i<4;i++)
		{
			if(Ultrosonic.channel[i]<Ultrosonic.safeDistance[i])			//��������������С�ڰ�ȫ����   ͨ��1
			{
				if(addMark[i]<3)	addMark[i]++;
			}	
			if(Ultrosonic.channel[0]>=Ultrosonic.safeDistance[i])			//��������������С�ڰ�ȫ����   ͨ��1
			{
				if(addMark[i]>0)	addMark[i]--;
			}				
			if(addMark[i]==3)	safeOk[i]=1;											//��������ȫ�����ۼӳ���3�Σ���ȫ��������1 ˵�����ϰ���С�ڰ�ȫ����
			if(addMark[i]==0) safeOk[i]=0;
		}	
		UltalEn=0;								//���ϴ�������
	}	
	if(AGVStatus.AJMode==0||Ultrosonic.deviceEn==0)					//��������δʹ��
	{
		for(i=0;i<4;i++)					//״̬�������
		{
			safeOk[i]=0;
			addMark[i]=0;						
		}	
		UltalEn=1;								//���ϴ������ر�
	}
	if(safeOk[0]|safeOk[1]|safeOk[2]|safeOk[3]!=0x00)	Speed_contr(0.0,0.0,0.0,0.0,0);   		//���а�ȫ��ǲ�Ϊ0ʱ����AGVֹͣ����
}



