#include "delay.h" 
#include "usart.h" 
extern u8 HF_BUFF_long[16]={0xAA,0x20,0x20,0x00,0x08,0x00,0x00,0x00,0x00,0x88,0x13,0x00,0x00,0x00,0x00,0x0D};
extern u8 HF_BUFF[14]={0xAA,0x20,0x20,0x00,0x06,0x00,0x00,0x88,0x13,0x00,0x00,0x00,0x00,0x0D};
extern u8 HF_BUFF_stop[8]={0xaa,0x20,0x20,0x31,0x00,0xe6,0xeb,0x0d};

//*************************ʵ�ֺ���*****************************/
//����ԭ�ͣ�HF_vehicle(u8 * HF_BUFF,u8 command)
//���ܣ���С�������ٶȿ���
//����/�����u8 * HF_com������С���������ݴ洢������,8 command:�ٶ�ָ��
//
//ǰ����0x01
//���ˣ�0x02
//��Բ����0x03
//��Բ����0x04
//ԭ����ת��0x05
//ԭ����ת��0x06
//ǰ���㶯��0x07
//���˵㶯��0x08
//
//
//��Բ��С��0x09 
//��Բ���У�0x0A
//��Բ���ϴ�0x0B
//��Բ����0x0C
//
//��Բ��С��0x0D
//��Բ���У�0x0E
//��Բ���ϴ�0x10
//��Բ����0x11
//
//С����ͣ��0x30
//���أ�void
//**************************************************************/
void HF_vehicle(u8 * HF_BUFF,u8 command)
{	
	u8 i;
	switch(command)
	{
		case 0x01:				//  ����ǰ��
		{
			HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x28;
			HF_BUFF[6]=0x23;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0x00;
			HF_BUFF[10]=0x00;
			HF_BUFF[11]=0x19;
			HF_BUFF[12]=0x03;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1 ,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		
		}break;
		case 0x02:		// ��������
		{
		
			HF_BUFF[3]=0x42;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x00;
			HF_BUFF[6]=0x00;
			HF_BUFF[7]=0x9c;
			HF_BUFF[8]=0xff;
			HF_BUFF[9]=0x00;
			HF_BUFF[10]=0x00;
			HF_BUFF[11]=0x81;
			HF_BUFF[12]=0x21;
		
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
			
		}break;
		case 0x03:		//��Բ��  ���ٶ�Ϊ��  ��ת
		{
			HF_BUFF_long[3]=0x38;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x4c;
			HF_BUFF[6]=0x1d;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0xdc;
			HF_BUFF[10]=0x05;
			HF_BUFF[11]=0x09;
			HF_BUFF[12]=0x99;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
			
		}break;
		case 0x04:		//��Բ��   ���ٶ�Ϊ�� ��ת
		{
			HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x4c;
			HF_BUFF[6]=0x1d;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0x24;
			HF_BUFF[10]=0xfa;
			HF_BUFF[11]=0x91;
			HF_BUFF[12]=0x1d;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
			
		}break;
		case 0x05:		//ǰ���㶯
		{
			HF_BUFF[3]=0x42;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x00;
			HF_BUFF[6]=0x00;
			HF_BUFF[7]=0x64;
			HF_BUFF[8]=0x00;
			HF_BUFF[9]=0x00;
			HF_BUFF[10]=0x00;
			HF_BUFF[11]=0x6c;
			HF_BUFF[12]=0xf4;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;
		case 0x06:		//���˵㶯
		{
			HF_BUFF[3]=0x42;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x00;
			HF_BUFF[6]=0x00;
			HF_BUFF[7]=0x9c;
			HF_BUFF[8]=0xff;
			HF_BUFF[9]=0x00;
			HF_BUFF[10]=0x00;
			HF_BUFF[11]=0x81;
			HF_BUFF[12]=0x21;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
			                                                                                                                                                                       
		}break;
		case 0x07:		//��ת�㶯
		{
			HF_BUFF[3]=0x42;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x00;
			HF_BUFF[6]=0x00;
			HF_BUFF[7]=0x00;
			HF_BUFF[8]=0x00;
			HF_BUFF[9]=0x64;
			HF_BUFF[10]=0x00;
			HF_BUFF[11]=0xa1;
			HF_BUFF[12]=0xa0;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
				
		}break;	
		case 0x08:		//��ת�㶯
		{
			HF_BUFF[3]=0x42;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x00;
			HF_BUFF[6]=0x00;
			HF_BUFF[7]=0x00;
			HF_BUFF[8]=0x00;
			HF_BUFF[9]=0x9c;
			HF_BUFF[10]=0xff;
			HF_BUFF[11]=0x39;
			HF_BUFF[12]=0x24;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1, HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
			
		}break;	
		case 0x09:		//					��ת105��,���ٶ�10
		{
			HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x04;
			HF_BUFF[6]=0x29;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0xe8;
			HF_BUFF[10]=0x03;
			HF_BUFF[11]=0x24;
			HF_BUFF[12]=0xd2;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1,HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;	
		case 0x0A:		//          ��ת110�㣬���ٶ�15
		{
		 HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0xf8;
			HF_BUFF[6]=0x2a;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0xdc;
			HF_BUFF[10]=0x05;
			HF_BUFF[11]=0x3e;
			HF_BUFF[12]=0x0f;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1,HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;	
		case 0x0B:		//					��ʱ����ת70��,�뾶2m
		{
			HF_BUFF_long[3]=0x39;
			//HF_BUFF_long[4]=0x08;
			HF_BUFF_long[5]=0x58;
			HF_BUFF_long[6]=0x1b;
			HF_BUFF_long[7]=0xc8;
			HF_BUFF_long[8]=0x00;
			HF_BUFF_long[9]=0x88;
			HF_BUFF_long[10]=0x13;
			HF_BUFF_long[11]=0x00;
			HF_BUFF_long[12]=0x00;
			HF_BUFF_long[13]=0xb4;
			HF_BUFF_long[14]=0x24;
			
			for(i=0;i<16;i++)
			{										
				USART_SendData(USART1,HF_BUFF_long[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;
		case 0x0C:		//         ��ת115�㣬���ٶ�25
		{
			HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0xec;
			HF_BUFF[6]=0x2c;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0xc4;
			HF_BUFF[10]=0x09;
			HF_BUFF[11]=0xc8;
			HF_BUFF[12]=0x95;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1,HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;
		case 0x0D:		//					��ת75��,���ٶ�-10
		{
			HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x4c;
			HF_BUFF[6]=0x1d;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0x18;
			HF_BUFF[10]=0xfc;
			HF_BUFF[11]=0xaf;
			HF_BUFF[12]=0x3d;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1,HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;	
		case 0x0E:		//         ��ת70��,���ٶ�-15
		{
		  HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x58;
			HF_BUFF[6]=0x1b;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0x24;
			HF_BUFF[10]=0xfa;
			HF_BUFF[11]=0x31;
			HF_BUFF[12]=0xcc;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1,HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;	
		case 0x10:		//					˳ʱ����ת70��,�뾶2m
		{
			HF_BUFF_long[3]=0x38;
			//HF_BUFF_long[4]=0x08;
			HF_BUFF_long[5]=0x58;
			HF_BUFF_long[6]=0x1b;
			HF_BUFF_long[7]=0xc8;
			HF_BUFF_long[8]=0x00;
			HF_BUFF_long[9]=0x88;
			HF_BUFF_long[10]=0x13;
			HF_BUFF_long[11]=0x00;
			HF_BUFF_long[12]=0x00;
			HF_BUFF_long[13]=0xf1;
			HF_BUFF_long[14]=0x4b;
			
			for(i=0;i<16;i++)
			{										
				USART_SendData(USART1,HF_BUFF_long[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;
		case 0x11:		//         ��ת65��,���ٶ�-25
		{
			HF_BUFF[3]=0x37;
			//HF_BUFF[4]=0x06;
			HF_BUFF[5]=0x64;
			HF_BUFF[6]=0x19;
			HF_BUFF[7]=0x88;
			HF_BUFF[8]=0x13;
			HF_BUFF[9]=0x3c;
			HF_BUFF[10]=0xf6;
			HF_BUFF[11]=0x8b;
			HF_BUFF[12]=0xe7;
			
			for(i=0;i<14;i++)
			{										
				USART_SendData(USART1,HF_BUFF[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;
		case 0x30:		//					С����ͣ
		{
			HF_BUFF_stop[3]=0x20;
			HF_BUFF_stop[4]=0x31;
			HF_BUFF_stop[5]=0x00;
			HF_BUFF_stop[6]=0xe6;
			HF_BUFF_stop[7]=0xeb;
			HF_BUFF_stop[8]=0x0d;
			
			for(i=0;i<8;i++)
			{										
				USART_SendData(USART1,HF_BUFF_stop[i]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���						
			}
			USART_RX_STA=0;   //RIFD����������״̬��ʼ��
		}break;
		
	}

}



