#include "AGV_contr.h" 
#include "HEAD.h"
#include "gps.h" 
#include "East_nor.h" 
#include "map.h" 
#include "HEAD.h"
#include "RAD.h" 
#include "usart6.h"
#include "usart3.h"
#include "mpu9250.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��

u8 test4=0;

void SON1234_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��  GPIOEʱ��
	
	 //GPIOE8��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz	`
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��	
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);//GPIOE8���øߣ�����
}


void Sov_on_1234_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOD,GPIOEʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_15; //����0 1 15����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD0,1,15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //����7����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE7

}



/************************�ƶ������������Զ�ģʽ��ؿ���*********************/
//������ʽ��void Inside_Auto(void)			//
//���������������Զ�ģʽ��غ���
//���������
//���������
//���ز�����
//
//
/*************************************************************************/
void Inside_Auto(void)	
{
	u8 static add;
//	InitMagDate=magnetic_recei(USART3_RX_BUF,Mag_date,Mag_alarm);	//�ŵ������ݶ�ȡ
	 
	Station_CardNum(RFID_value);
	if(BranchPath.error==1)	SignalBranchPath_Station();																		//AGVͨ��RFID����ɲ��ֹ���	
	
			
	if(AGVStatus.AutoRun==1 && AGVStatus.AUTOmode==1 && add>=20 )
	{
//		EV_calcu(&front_pid,&rear_pid,Ea_Ed,EV,Mag_date,1,AGVSpd_IN);	//�����Զ�Ѱ��
		add=0;
	}		
	
	if(AGVStatus.AutoRun==0||Mag_date[1]==0) Speed_contr(0.0,0.0,0.0,0.0,0);   //���������йر�ʱ�������޴����ź�ʱ��С��ֹͣ
	delay_ms(10);
		add++;
}
/*************************�ƶ������������Զ�ģʽ��ؿ���*********************/
//������ʽ��void Outside_Auto(void)			//
//���������������Զ�ģʽ��غ���
//���������
//���������
//���ز�����
//
/*************************************************************************/

void Outside_Auto(void)	
{	
	u8 static firstMark=0;
			if(!firstMark) First_GPS_Msg();						//�ϵ���״ο���GPS���⵼������ȡ����
test4=1;
			SCM_345_contralCmd(5,Smc345.scm345_cmd);
			delay_ms(50);
test4=2;
			if(USART3_RX_STA&0x8000)																//GPS���ݶ�ȡ
			{							
				GPS_Analysis(&gpsx,(u8*)USART3_RX_BUF);    						//��������			
				Gps_Msg_Show(gpsx,lonti_lati);            						 //������ݣ������ǰ�ƶ������˵�ʵʱλ��ֵ�ȸ�ʽ	
				USART3_RX_STA=0; 
			}
test4=2;
			longti_lati_trans(0,lonti_lati[0],lonti_lati[1],x0_y0,lontlat_du); 					//������ȷ����x,yֵ			
			MapPot.longti_du=lontlat_du[0];			//��ȡ���ȶȸ�ʽ����
			MapPot.lati_du=lontlat_du[1];				//��ȡά�ȶȸ�ʽ����
test4=3;
			OAOB_caculate(lati_x0,lati_y0,x0_y0[0],x0_y0[1],longti_O,lontlat_du[0],OAOB);		//��������ϵ�¼���ĳ�������ԭ��O������ֵ	
						
			if(OAOB[0]<400&&OAOB[0]>-400&&OAOB[1]<400&&OAOB[1]>-400)    //��ֹ�����쳣
			{
//			Wakeup_position(OAOB[0],OAOB[1],OA,OB,OAOB_angle,cour_angle,near_pot);       //���С�����벻��·���ڣ�����ȡ��ͬ�Ŀ��Ʋ���			
				Scan_nearpot_re=Scan_nearpot(OAOB[0],OAOB[1],near_pot,45);         //������Ѱ��·��������ĵ㣬�����ʸ������
								
				MapPot=MapPot_Find(near_pot[0]);					//Ѱ�ҵ��ӵ�ͼ�Ķ�Ӧ��
				recent_near_pot[0]=MapPot.OA;
				recent_near_pot[1]=MapPot.OB;

				course_weighting_re=course_weighting(near_pot,Scan_nearpot_re);					//����ƶ������˵�ƫת�Ƕ�				
				cmd_vehicle=veichel_contr(Smc345.scm345Angle,course_weighting_re);     //С������													
			}	
			if(AGVStatus.AutoRun==1 /*&& MapPot.starNum>5*/)
			{
				auto_vehicle_contr(MapPot.AutoSpd,cmd_vehicle);						//�Զ�ģʽС������
			}
			else Speed_contr(0.0,0.0,0.0,0.0,0);   		//���������йر�ʱ��С��ֹͣ
			
			vehicle_upload(OAOB[0],OAOB[1],Smc345.scm345Angle,lonti_lati[0],lonti_lati[1],spd_1,MapPot.OA,MapPot.OB,near_pot[0]);		//�ϴ����⵼������lonti_lati��GPS�ȸ�ʽ
			JOG_mark=0;		

			
			firstMark=1;	//�״ζ�ȡ��־λ��1
}

/*************************�ƶ��������ֶ�ģʽ��ؿ���*********************/
//������ʽ��void JogMode_control(void)	//
//���������������Զ�ģʽ��غ���
//���������
//���������
//���ز�����
/*************************************************************************/
void JogMode_control(void)	
{
	
			if(JOG_mark==0)											//���ν����ֶ�ģʽͣ��
			{
				Vehicle_contr_Jog(0,0);           //�ֶ��ٶȸ���ֹͣ
				Vehicle_contr_Auto(0,0,0);				//�Զ��ٶȸ���ֹͣ	
				JOG_mark=1;
			}	
			if(USART3_RX_STA&0x8000)																//GPS���ݶ�ȡ
			{			
				GPS_Analysis(&gpsx,(u8*)USART3_RX_BUF);    						//��������			
				Gps_Msg_Show(gpsx,lonti_lati);            						 //������ݣ������ǰ�ƶ������˵�ʵʱλ��ֵ�ȸ�ʽ
				USART3_RX_STA=0;  
			}
			
			longti_lati_trans(0,lonti_lati[0],lonti_lati[1],x0_y0,lontlat_du); //������ȷ����x,yֵ
			OAOB_caculate(lati_x0,lati_y0,x0_y0[0],x0_y0[1],longti_O,lontlat_du[0],OAOB);		//��������ϵ�¼���ĳ�������ԭ��O������ֵ	
//			True_angle=MPU9250_data_receive(a1,w1,h1,Angle1,a2);    							//MPU9250���ݶ�ȡ		
//			vehicle_upload(OAOB[0],OAOB[1],Smc345.scm345Angle,lonti_lati[0],lonti_lati[1],spd_1,mapPot.OA,mapPot.OB,near_pot[0]);		//�ϴ����⵼������lonti_lati��GPS�ȸ�ʽ	


}


/*******************************************************************/
//������ʽ��void TeachMode_control(void)	
//���������
//���������
//���ز�����
//����������AGV�ֶ�ʾ��ģʽ
/*******************************************************************/
void TeachMode_control(void)											//ʾ��ģʽ
{
	u8 static timstor;
	static MapPotDef mapPot;
	if(AGVStatus.TeachMode==1&&AGVStatus.AJMode==0)				//AGVʾ��ģʽ�򿪣��Ҵ����ֶ�ģʽ
	{
		SCM_345_contralCmd(5,Smc345.scm345_cmd);
		delay_ms(100);
		if(USART3_RX_STA&0x8000)																//GPS���ݶ�ȡ
		{							
			GPS_Analysis(&gpsx,(u8*)USART3_RX_BUF);    						//��������			
			Gps_Msg_Show(gpsx,lonti_lati);            						 //������ݣ������ǰ�ƶ������˵�ʵʱλ��ֵ�ȸ�ʽ	
			USART3_RX_STA=0; 
		}
		longti_lati_trans(0,lonti_lati[0],lonti_lati[1],x0_y0,lontlat_du); 					//������ȷ����x,yֵ
		mapPot=MapPot_Find(near_pot[0]);														//Ѱ�Ҷ�Ӧ��
		
		if(AGVStatus.TeachMode==1&&timstor!=Tim1Add.reMark) vehicle_upload(OAOB[0],OAOB[1],Smc345.scm345Angle,lonti_lati[0],lonti_lati[1],spd_1,mapPot.OA,mapPot.OB,near_pot[0]);		//�ϴ����⵼������lonti_lati��GPS�ȸ�ʽ	,ÿ0.5���ϴ�һ��

		timstor=Tim1Add.reMark;
		
	}
}

/*******************************************************************/
//������ʽ��void MapDownLoad_control(void)	
//���������
//���������
//���ز�����
//�������������ӵ�ͼ���ش������
/*******************************************************************/
void MapDownLoad_control(void)	
{
	union MultiType multiType;
	switch(MapPot.mode)
	{
		case 0: break;					//
		case 1:									//���ӵ�ͼ����		
		{
					//Map_Save2FLASH(MapPot,FloatSize);		//��ͼ���ݴ洢	
			if(Map_StorageCheck(MapPot.Num,FloatSize))Map_Save2FLASH(MapPot,FloatSize);		//���洢������Ϊ 0xFFFFFFFF,���ܴ洢��ͼ����
			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
		}break;
		case 2:									//���ӵ�ͼ�����洢����ʽ��
		{
			SetScreen(25);														//��ʼ����ɣ��л������ʼ��
			STMFLASH_Clear(MAPOA_Addr,1);								//OA�洢������
			STMFLASH_Clear(MAPOB_Addr,1);								//OB�洢������
			STMFLASH_Clear(MAPAngle_Addr,1);								//angle�洢������
			SetScreen(4);														//�˻������ʼ����ɣ��л��ز�������3
			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
		}break;
		case 3:									//���ӵ�ͼ��ʼ���ݴ洢
		{	
			multiType.u32Array_[0]=Paramt_BUFF[Pg_OutsideParmt][1];				//��ȡ�����������������ݵ�8λ
			multiType.u32Array_[1]=Paramt_BUFF[Pg_OutsideParmt][2];				//��ȡ�����������������ݸ�8λ
			SetTextValueFloat(Pg_OutsideParmt,1,multiType.double_,_double);	//�������ݵ�HMI
				
			multiType.u32Array_[0]=Paramt_BUFF[Pg_OutsideParmt][3];				//��ȡ�����������������ݵ�8λ
			multiType.u32Array_[1]=Paramt_BUFF[Pg_OutsideParmt][4];				//��ȡ�����������������ݸ�8λ
			SetTextValueFloat(Pg_OutsideParmt,3,multiType.double_,_double);	//�������ݵ�HMI
				
			SetTextValueu16(Pg_OutsideParmt, 10,(u16)Paramt_BUFF[Pg_OutsideParmt][10]);    //���͵��ӵ�ͼ�ܵĵ�����
			SetTextValueu16(Pg_OutsideParmt, 11,(u16)Paramt_BUFF[Pg_OutsideParmt][11]);    //���͵��ӵ�ͼÿ���߶��ϵ������
				
			SetScreen(25);														//��ʼ����ɣ��л������ʼ��
			Buff2_Flash(Pg_OutsideParmt,120);															//�����洢ĳһҳ�Ĳ�����FLASH
			SetScreen(CurrtScreenID);														//�˻������ʼ����ɣ��л��ص�ǰҳ��

			MapPot.mode=0;						//ģʽ�رգ����ڴ洢
		}break;
		default:break;
		}

}


/*********************************�ƶ������˲�����ʼ��*********************/
//������ʽ��void AGVParamter_Init()
//���������
//���������
//���ز�����
//
//
/*************************************************************************/
void AGVParamter_Init(void)
{
	double temp[2],temp1[2];
	union MultiType multiType;
	
	//ң�ز�����ʼ��
	SymaStatus.Spd_cle[0]=1.0;											//�ٶ��趨��ʼֵΪ1��/�� 
	SymaStatus.Spd_cle[1]=2.5;											//ת��뾶��ʼֵ�趨Ϊ2.5��

	AGVStatus.Wheel_R=0.318;                //����ֱ��
	AGVStatus.pi1=3.1415926;								//�㶮��
	AGVStatus.vehicle_width=0.587/2;						//С�������ּ��һ��
	AGVStatus.vehicle_longth=0.412;						//С��ǰ���ּ��
	OT_SON=0;											//����ʱǿ�ƶ�ʹ��
	
	//�����ǲ�����ʼֵ
	Smc345.scm345_cmd[0]=0x68;
	Smc345.scm345_cmd[1]=0x04;
	Smc345.scm345_cmd[2]=0x00;
	Smc345.scm345_cmd[3]=0x03;
	Smc345.scm345_cmd[4]=0x07;
	
//	TIM_SetCompare1(TIM3,0);     //TIM2ͨ��1ռ�ձ�Ϊ100/500    	��ʼ״̬���ת��Ϊ0
//	TIM_SetCompare2(TIM3,0);     //TIM2ͨ��2ռ�ձ�Ϊ200/500			��ʼ״̬���ת��Ϊ0
//	TIM_SetCompare3(TIM3,0);     //TIM2ͨ��3ռ�ձ�Ϊ300/500			��ʼ״̬���ת��Ϊ0
//	TIM_SetCompare4(TIM3,0);     //TIM2ͨ��4ռ�ձ�Ϊ400/500			��ʼ״̬���ת��Ϊ0
		
	Battery_Send(7,Ask_Battery);							//���ط�������
	front_pid=AGVPID_Change_FLASH(0);		//PID��1  ǰ����
	AGVSpd_IN=front_pid.AGPspd;
	
	Ultrosonic.deviceEn=Paramt_BUFF[17][5];					//����״̬��ֵ
	
	MapPot.numFx= LontLati.totalPotNum/LontLati.linePotNum;					//����߶�������ֵ
	MapPot.numPot=LontLati.linePotNum;															//ÿ������߶��ϵ������
	multiType.u32_=Paramt_BUFF[Pg_OutsideParmt][1];				//��ȡ�����������������ݵ�8λ

	
	
	longti_lati_trans(0,longti_O,lati_O,temp1,temp);	
	LontLati= Origin_O_parmt_Init();				//GPS ԭ����Ϣ��ȡ
	MapPot.AutoSpd=LontLati.spdTemp;				//�Զ������ٶ�


	Ultrosonic.deviceEn=(u8)Paramt_BUFF[Pg_CommonParmt][5];						//���ϴ������Ƿ��


	VehicleParamt.fir_fram="FR";			//�����ϴ���֡
	VehicleParamt.last_fram="OR";			//�����ϴ�β��

}