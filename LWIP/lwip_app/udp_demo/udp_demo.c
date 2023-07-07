#include "udp_demo.h"
#include "lwip_comm.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "task.h"
#include "lwip/api.h"
#include "lwip/lwip_sys.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//NETCONN API��̷�ʽ��UDP���Դ���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/8/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   
 
//TCP�ͻ�������
#define UDP_PRIO		6
//�����ջ��С
#define UDP_STK_SIZE	300
//������
TaskHandle_t UDPthreadTask_Handler;
//������
void udp_thread_task(void *arg);



//udp������
static void udp_thread_task(void *arg)
{
	//OS_CPU_SR cpu_sr;
	err_t err;
	static struct netconn *udpconn;
	static struct netbuf  *recvbuf;
	static struct netbuf  *sentbuf;
	struct ip_addr destipaddr;
	u32 data_len = 0;
	struct pbuf *q;
	
	LWIP_UNUSED_ARG(arg);
	udpconn = netconn_new(NETCONN_UDP);  //����һ��UDP����
	udpconn->recv_timeout = 10;  		
	
	if(udpconn != NULL)  //����UDP���ӳɹ�
	{
		err = netconn_bind(udpconn,IP_ADDR_ANY,UDP_DEMO_PORT); 
		IP4_ADDR(&destipaddr,lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]); //����Ŀ��IP��ַ
		netconn_connect(udpconn,&destipaddr,UDP_DEMO_PORT); 	//���ӵ�Զ������
		if(err == ERR_OK)//�����
		{
			while(1)
			{
				if((udp_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
				{
					sentbuf = netbuf_new();
					netbuf_alloc(sentbuf,strlen((char *)udp_demo_sendbuf));
					memcpy(sentbuf->p->payload,(void*)udp_demo_sendbuf,strlen((char*)udp_demo_sendbuf));
					err = netconn_send(udpconn,sentbuf);  	//��netbuf�е����ݷ��ͳ�ȥ
					if(err != ERR_OK)
					{
						printf("����ʧ��\r\n");
						netbuf_delete(sentbuf);      //ɾ��buf
					}
					udp_flag &= ~LWIP_SEND_DATA;	//������ݷ��ͱ�־
					netbuf_delete(sentbuf);      	//ɾ��buf
				}	
				
				netconn_recv(udpconn,&recvbuf); //��������
				if(recvbuf != NULL)          //���յ�����
				{ 
					taskENTER_CRITICAL();           //�����ٽ���
					memset(udp_demo_recvbuf,0,UDP_DEMO_RX_BUFSIZE);  //���ݽ��ջ���������
					for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						//�ж�Ҫ������UDP_DEMO_RX_BUFSIZE�е������Ƿ����UDP_DEMO_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����UDP_DEMO_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
						if(q->len > (UDP_DEMO_RX_BUFSIZE-data_len)) memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_DEMO_RX_BUFSIZE-data_len));//��������
						else memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;  	
						if(data_len > UDP_DEMO_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
					}
					taskEXIT_CRITICAL();            //�˳��ٽ���
					data_len=0;  //������ɺ�data_lenҪ���㡣
					printf("%s\r\n",udp_demo_recvbuf);  //��ӡ���յ�������
					netbuf_delete(recvbuf);      //ɾ��buf
				}else vTaskDelay(5);  //��ʱ5ms������delay_ms(5).
			}
		}else printf("UDP��ʧ��\r\n");
	}else printf("UDP���Ӵ���ʧ��\r\n");
}


//����UDP�߳�
//����ֵ:0 UDP�����ɹ�
//		���� UDP����ʧ��
BaseType_t udp_demo_init(void)
{
	BaseType_t res;
	taskENTER_CRITICAL();	//���ж�
	res =	xTaskCreate((TaskFunction_t)udp_thread_task,
						(const char*  )"udp_thread_task",
						(uint16_t     )UDP_STK_SIZE,
						(void*        )NULL,
						(UBaseType_t  )UDP_PRIO,
						(TaskHandle_t*)&UDPthreadTask_Handler);//����UDP����	
	taskEXIT_CRITICAL();		//���ж�
	return res;
}

