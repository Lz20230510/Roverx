#include "HEAD.h"

//ͨ�ö�ʱ��1�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��1!
void TIM1_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  //ʹ��TIM1ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;;/*�ظ������Ĵ���*/ //�߼���ʱ��1���ö�ʱ��������������ſ����������ļ���Ƶ��һ��ʼ��72mhz ֵ��ע��ĵط�	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);/*����±�־λ*/
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //����ʱ��1�����ж�
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��1
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; //��ʱ��1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


//��ʱ��1�жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{	
	u8 static tim1_mark;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
	{
		
		if(Tim1Add.tiAdd>250)	Tim1Add.tiAdd=0;					//time1������									
		Tim1Add.tiAdd++;
		Time1_Task();
		
		
		if(tim1_mark>=5)
		{
			tim1_mark=0;
			Tim1Add.reMark=!Tim1Add.reMark;					//ÿ��0.5ms  ȡ��һ��״̬			
		}		
		tim1_mark++;	
		
		
	}
	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //����жϱ�־λ
}



////////////////////////��ʱ��ɨ�蹤��///////////////////////////
//�������ƣ�void Time1_Task(void)
//�������ܣ���ʱ��1ʱ�亯��
//��������u
//���������
//����ֵ��
//////////////////////////////////////////////////////////////
void Time1_Task(void)
{

	SymaStatus.Spd_Jog=Syma_yaogan_receive(SymaStatus.Yaogan,SymaStatus.Tmp_buf,SymaStatus.Syma_cmd,SymaStatus.Spd_cle[0]);			//ң����ҡ�����ݶ�ȡ
	Syma_vehicle_contr(SymaStatus.Syma_cmd,SymaStatus.Tmp_buf,SymaStatus.Spd_Jog,SymaStatus.Spd_cle[1],0.5);					//ң�����Զ�С���Ŀ���	
	Syma_button_receive(SymaStatus.Tmp_buf,SymaStatus.Spd_cle);																	//ң������ť���ݶ�ȡ	
//	AGVsta_Scan(1);	           //��ť״̬���	
//	InitMagDate=magnetic_recei(USART3_RX_BUF,Mag_date,Mag_alarm);	//�ŵ������ݶ�ȡ
	



}








