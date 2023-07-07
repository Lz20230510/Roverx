#include "sys.h"
#include "usart6.h"
#include "HEAD.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
//#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//ucos ʹ��	  
//#endif
 
#if EN_USART6_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART6_RX_BUF[USART6_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART6_RX_STA=0;       //����״̬���	
u8 mark=0;

//��ʼ��IO ����6 
//bound:������
void usart6_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
 
	//����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource6,GPIO_AF_USART6); //GPIOG9����ΪUSART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource7,GPIO_AF_USART6); //GPIOG14����ΪUSART6
	
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
	u8 Res;
	u32 static RFID_valueStor;
	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART6);//(USART6->DR);	//��ȡ���յ�������			
		
//		if(AGVStatus.AUTOmode==1)				//����ģʽ
//		{
//			mark=1;
//			USART6_RX_BUF[USART6_RX_STA]=Res;
//			USART6_RX_STA++;
//			if(USART6_RX_STA>(RFID_REC_LEN-1))USART6_RX_STA=0;	
//			
//			StationCardNum.StationCardNumstor=0;							//վ�㿨����ʱ�洢������
//			RFID_value=RFID_receive(USART6_RX_BUF);
//			if(RFID_valueStor!=RFID_value)													//����ȡ����ͬ����
//			{
//				Station_CardNum(RFID_value);																				//վ�㿨�Ż�ȡ	
//				StatnNum_Search(StationCardNum.cardNum,BranchPath.selectPathNum);				//������ǰ�Ŀ��Ŷ�Ӧ�ķ�֧·������	
//				BranchPath.selectPathNum=AGVPathJump_Cmd(BranchPath.jumpCmd,BranchPath.selectPathNum);		//·����תָ��	
//			
//				BranchPath.uart4Mark=1;																										//��ȡ��RFID�źű�־
//				mark=2;
//			}		
//			RFID_valueStor=RFID_value;
//				
//		}
//		else if(AGVStatus.AUTOmode==2||AGVStatus.TeachMode==1)			//����ģʽ����ʾ��ģʽ
//		{
//			if(Res==0x68)	USART6_RX_STA=0;								//�����յ�0x68
//			USART6_RX_BUF[USART6_RX_STA]=Res;
//			USART6_RX_STA++;
//			if(USART6_RX_STA>(Smc345_REC_LEN-1))USART6_RX_STA=0;				//���ݽ�����ɣ�����
//			Smc345.scm345Angle =bcd_to_hex(USART6_RX_BUF);  
//		}
	} 
			 
} 





  
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




