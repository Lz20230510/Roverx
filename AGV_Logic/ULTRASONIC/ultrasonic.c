#include "ultrasonic.h"
#include "delay.h"

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 ������Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultra_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =ULTRA_TRIG;       //���͵�ƽ����GPIO_Pin_5
 	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode=0x00;//�������
 	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =ULTRA_ECHO;	   //���ص�ƽ����GPIO_Pin_6
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = 0x00;//��������
 	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOB,ULTRA_TRIG);
}

/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 float *p:��໺�����
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i;
	u32 j;
	float Ultr_Temp;	
	for(i=0;i<5;i++)
	{
		TRIG_Send=1;
		delay_ms(10);
		TRIG_Send=0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			delay_us(10);
			j++;
		}
		Ultr_Temp+=340/2*j*10;//ģ�����ɲ��3m 
		j=0;
		delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	*p=Ultr_Temp/5/1000000; 	
}

/********************************************************************************************************************************************
@f_name: short Get_Distance(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@return: short:����ֵ(������100��,��λ:mm.)
*********************************************************************************************************************************************/
short Get_Distance(void)
{
	u8 i;
	u32 j;
	float Distance;
	short result;
	for(i=0;i<5;i++)
	{
		TRIG_Send = 1;
		delay_ms(15);
		TRIG_Send = 0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			delay_us(10);
			j++;
		}
		Distance+=340/2*j*10;//  ģ�����ɲ��3m 
		j=0;
		delay_ms(100);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	result=Distance/5/100; 
	return result;
}
	 

