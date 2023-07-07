//#include "Usart.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "stdlib.h"
#include "usart3.h"
#include "HMI_cmd.h"
#include "hmi_driver.h"
#include "delay.h"
#include "vehicle_key.h"
#include "led.h"
#include "RAD.h"
#include "24l01.h"
#include "stmflash.h"



u16 CurrtScreenID=0;								//��ǰ�����
extern u8 cmd_buffer[CMD_MAX_SIZE];//ָ���
u16 ScreenID = 0;//��ǰ����IDS
static long test_value = 0;//����ֵ

union MultiType multiType3;


/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, u16 size )
{
	u8 cmd_type = msg->cmd_type;//ָ������
	u8 ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	u8 control_type = msg->control_type;//�ؼ�����
	u16 screen_id = PTR2U16(&msg->screen_id);//����ID
	u16 control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	u32 value = PTR2U32(msg->param);//��ֵ
	ScreenID=screen_id;
	
	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS: //����������
	case NOTIFY_TOUCH_RELEASE://�������ɿ�
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://дFLASH�ɹ�
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://дFLASHʧ��
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://��ȡFLASH�ɹ�
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD://��ȡFLASHʧ��
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[1],cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7]);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id);		
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://�ı��ؼ�
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //�������ؼ�
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://�˵��ؼ�
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://ѡ��ؼ�
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://����ʱ�ؼ�
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;				
				
	//			void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id);
				}
			}			
		}
		break;default:
		break;
	}
}


void SetTextValueInt32(u16 screen_id, u16 control_id,long value)
{
	u8 buffer[12] = {0};
	sprintf((char*)buffer,"%ld",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueInt16(u16 screen_id, u16 control_id,short int value)
{
	u8 buffer[12] = {0};
	sprintf((char*)buffer,"%d",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}


void SetTextValueFloat(u16 screen_id, u16 control_id,double value,u8 type)
{
	u8 buffer[20] = {0};
	if(type==_double) sprintf((char*)buffer,"%.10lf",value);//�Ѹ�����ת��Ϊ�ַ���(����10λС��)
	else if(type==_float) sprintf((char*)buffer,"%.2f",value);//�Ѹ�����ת��Ϊ�ַ���(����10λС��)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueu16(u16 screen_id, u16 control_id,u16 value)
{
	u8 buffer[12] = {0};
	sprintf((char*)buffer,"%u",value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id,control_id,buffer);
}


/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(u16 screen_id)
{
	//TODO: ����û�����
	switch(screen_id)

	if(screen_id==1)
	{
			
	}
	else if(screen_id==3)//�¶�����
	{
		
	}
	else if(screen_id==9)//��ά��
	{
//		SetTextValue(9,1,dat);
	}
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(u8 press,u16 x,u16 y)
{
	//TODO: ����û�����
}




void UpdateUI()
{


}


/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(u16 screen_id, u16 control_id, u8  state)
{
	u8 static buttonAdd=0;
	CurrtScreenID=ScreenID_Judge(screen_id,control_id);							//��ǰҳ��Ż�ȡ
	Buff2_Flash(screen_id,control_id);															//�����洢ĳһҳ�Ĳ�����FLASH
	switch(screen_id)
	{
		case Pg_Start:										//������0 		
		{
			switch(control_id)						//��ť�ؼ���
			{
				case 2:										//ң��������ť
				{
					DacaiButton.Pg0_JOGSetup=!DacaiButton.Pg0_JOGSetup;
					
				}break;
				case 3:										//����/���Զ���ť
				{
					DacaiButton.Pg0_AUTOmode=!DacaiButton.Pg0_AUTOmode;	
				}break;
				case 4:										//�豸ʹ��
				{
					DacaiButton.Pg0_SONSetup=!DacaiButton.Pg0_SONSetup;			//�豸ʹ��
					
				}break;
				case 29:									//������λ
				{
						
				}break;			
				default:break;			
			}	
		}break;
		
		case 0x03:												//�켣Ԥ��3		
		{
			switch(control_id)					//��ť�ؼ���
			{
				case 2:										//�켣Ԥ����ť
				{
				
				}break;
				default:break;			
			}				
		}break;
		
		case 0x04:												//�켣�༭4	
		{
			switch(control_id)					//��ť�ؼ���
			{
				case 2:										//�������ذ�ť
				{
					DacaiButton.Pg4_MapDown=!DacaiButton.Pg4_MapDown;					//		
					if(AGVStatus.AJMode!=0) DacaiButton.Pg4_MapDown=0;				//���ֶ�ģʽ��ťʧЧ
				}break;	
				case 3:										//��ͼ�쳣���޸�
				{
					MapTable_repair((u16)Paramt_BUFF[10][10],2000.0);					//�쳣���޸�
				}break;	
				case 4:										//����ʾ�̰�ť
				{
					DacaiButton.Pg4_TeachMode=!DacaiButton.Pg4_TeachMode;			//�ֶ�ģʽ�£������������߹ر�ʾ��ģʽ
					if(AGVStatus.AJMode!=0) DacaiButton.Pg4_TeachMode=0;				//���ֶ�ģʽ��ťʧЧ
					
				}break;
				case 5:										//���ӵ�ͼ�洢��������
				{
					SetScreen(25);														//��ʼ����ɣ��л������ʼ��
					STMFLASH_Clear(MAPOA_Addr,1);								//OA�洢������
					STMFLASH_Clear(MAPOB_Addr,1);								//OB�洢������
					STMFLASH_Clear(MAPAngle_Addr,1);								//angle�洢������
					SetScreen(4);														//�˻������ʼ����ɣ��л��ز�������3
				}break;
				default:break;	
				
			}				
		}break;
		case 10:			//����������ý���
		{
			switch(control_id)			//��ť�ؼ���
			{
				case 20:							//Ѱ������ť
				{
					
					if((Paramt_BUFF[screen_id][13]==0xFF||Paramt_BUFF[screen_id][13]==0x00)&&buttonAdd==0)				
					{
						Paramt_BUFF[screen_id][13]=0x01;						//��ʱ������ѡ
						AnimationPlayFrame(screen_id,13,1);      	 //�ؼ���ʾ					
					}						
					else if(Paramt_BUFF[screen_id][13]==0x01&&buttonAdd==0)																	
					{
						Paramt_BUFF[screen_id][13]=0x00;						//��ʱ��������ѡ
						AnimationPlayFrame(screen_id,13,0);      	 //�ؼ�����ʾ*/
					}	
					buttonAdd++;
					if(buttonAdd>1) buttonAdd=0;
				}
				default:break;
			}				
			
		}break;
		case 0x11:						//����������ý���
		{
			switch(control_id)					//��ť�ؼ���
			{
				case 2:										//�������尴ť
				{
					STMFLASH_Clear(AGVPa_Addr,0);								//��������
					SetScreen(25);														//��ʼ����ɣ��л������ʼ��
					AllPg_Refresh(textFormat,textFormat,textFormat);//�˻������ʼ��
					SetScreen(17);														//�˻������ʼ����ɣ��л��ز�������3
				}break;	
				
				case 4:						//�رձ��ϴ�����
				{
					DacaiButton.Pg17_ultral=!DacaiButton.Pg17_ultral;						//���ϴ����������ر�
					if(DacaiButton.Pg17_ultral)		Paramt_BUFF[17][5]=!Paramt_BUFF[17][5];						//����ģʽ������ȡ��//�����°�ťΪ1ʱ	
					Ultrosonic.deviceEn=Paramt_BUFF[17][5];					//����״̬��ֵ
				}break;	
			}						
		}	
		default:break;
	}	
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(u16 screen_id, u16 control_id, u8 *str)
{
//	union MultiType multiType1;
	//TODO: ����û�����
			if(control_id==61 && screen_id==0 )
			{
				Dacai_InOut(screen_id,control_id,str,multiType3);						//��������ı����ܲ���ʵʱ�������
				BranchPath.selectPathNum=Paramt_BUFF[0][61];								//��ȡ��ǰ·������ֵ
			}				
			if(CurrtScreenID!=0x00) Dacai_InOut(screen_id,control_id,str,multiType3);						//��������ı����ܲ���ʵʱ�������	
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(u16 screen_id, u16 control_id, u32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(u16 screen_id, u16 control_id, u32 value)
{
	//TODO: ����û�����
//	if(screen_id==5&&control_id==2)//�������
//	{
//		test_value = value;

//		SetProgressValue(5,1,test_value); //���½�������ֵ
//	}
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(u16 screen_id, u16 control_id, u32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(u16 screen_id, u16 control_id, u8  item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(u16 screen_id, u16 control_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(u8 status,u8 *_data,u16 length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(u8 status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(u8 year,u8 month,u8 week,u8 day,u8 hour,u8 minute,u8 second)
{
}
