#include "cancan.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "canfestival.h"	
#include "def.h" 
#include "string.h"
//#include "master_objdict.h"
#include "TestMaster.h"
#include "can.h"
#include "dcf.h"

//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 


//u8 can_init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
//{

//  	GPIO_InitTypeDef GPIO_InitStructure; 
//	  CAN_InitTypeDef        CAN_InitStructure;
//  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
//#if CAN1_RX0_INT_ENABLE 
//   	NVIC_InitTypeDef  NVIC_InitStructure;
//#endif
//    //ʹ�����ʱ��
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
//	
//    //��ʼ��GPIO
//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
//	
//	  //���Ÿ���ӳ������
//	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
//	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
//	  
//  	//CAN��Ԫ����
//   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
//  	CAN_InitStructure.CAN_ABOM=DISABLE;	//�����Զ����߹���	  
//  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ����������(���CAN->MCR��SLEEPλ)
//  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
//  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
//  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
//  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
//  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ����(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
//  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
//  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
//  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
//  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
//    
//		//���ù�����
// 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
//  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
//  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
//  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
//  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
//  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
//  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
//   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
//  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
//  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
//		
//#if CAN1_RX0_INT_ENABLE
//	
//	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
//  
//  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  	NVIC_Init(&NVIC_InitStructure);
//#endif
//	return 0;
//}   
// 
//#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
//void CAN1_RX0_IRQHandler(void)
//{
//  	CanRxMsg RxMessage;
//	Message canopenMessage;
//    CAN_Receive(CAN1, 0, &RxMessage);

//	canopenMessage.cob_id=RxMessage.StdId;
//	if(RxMessage.RTR == CAN_RTR_REMOTE)
//		canopenMessage.rtr=REQUEST;
//	else
//		canopenMessage.rtr=NOT_A_REQUEST;
//	
//	canopenMessage.len=RxMessage.DLC;
//	memcpy(canopenMessage.data,RxMessage.Data,sizeof(RxMessage.Data));
//	
//	canDispatch(&master_objdict_Data, &canopenMessage);    //������Ϣ
//	
//}
//#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 can_send_msg(Message *msg)
{	
	u8 mbox;
	u16 i=0;
    
	CanTxMsg TxMessage;
	TxMessage.StdId = msg->cob_id;			// ��׼��ʶ�� 
	TxMessage.ExtId = 0;			        // ������չ��ʾ�� 
	TxMessage.IDE = CAN_Id_Standard; 	    // ��׼֡
	if(msg->rtr)
		TxMessage.RTR = CAN_RTR_REMOTE;		        // ����֡
	else
		TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = msg->len;				// Ҫ���͵����ݳ���
	for(i=0; i<msg->len; i++)
    {
        TxMessage.Data[i] = msg->data[i];			          
    }
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ��� 
	if(i>=0XFFF)return 1;
	return 0;		

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
//u8 CAN1_Receive_Msg(u8 *buf)
//{		   		   
// 	u32 i;
//	CanRxMsg RxMessage;
//    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
//    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
//    for(i=0;i<RxMessage.DLC;i++)
//    buf[i]=RxMessage.Data[i];  
//	return RxMessage.DLC;	
//}






u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//�����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ����������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ����(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
#if CAN1_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
//void CAN1_RX0_IRQHandler(void)
//{
//  	CanRxMsg RxMessage;
//	int i=0;
//    CAN_Receive(CAN1, 0, &RxMessage);
//	for(i=0;i<8;i++)
//	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
//}

void CAN1_RX0_IRQHandler(void)
{
   u32 i;
   Message m;
   CanRxMsg RxMessage;
   CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

   m.cob_id=RxMessage.StdId;
   if(RxMessage.RTR == CAN_RTR_REMOTE)
   m.rtr=1;
   else if(RxMessage.RTR == CAN_RTR_DATA)
   m.rtr=0;
   m.len=RxMessage.DLC;
   for(i = 0; i < RxMessage.DLC; i++)
   m.data[i] = RxMessage.Data[i];
   canDispatch(&TestMaster_Data, &m);
}

//CANOPEN�ڵ�����
void CANOPEN_Node_Start(void)
{
	start_node(&TestMaster_Data,0);												//�������нڵ�
	setNodeId(&TestMaster_Data, 0x00);
	setState(&TestMaster_Data, Initialisation);
	setState(&TestMaster_Data, Operational);	
	
//	check_and_start_node(&TestMaster_Data,2);				//���dcf״̬���ڵ�����
//	start_node(&TestMaster_Data,0);												//�������нڵ�
	
}


//CANOPEN��λ�ڵ�
void CANOPEN_Reset_comunication(void)
{

	Reset_Comunication(&TestMaster_Data,0);												//��λ���нڵ�
	
}



#endif















