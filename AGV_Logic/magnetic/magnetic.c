#include "HEAD.h"
#include "math.h"

	int value=0;
	u16 num;
//u8 high,low,left,right;
float Mag_date[12]={0};					//����ŵ����������ֱ��ȡ������ֵ�洢
u8 Mag_alarm[4]={0};	

u8 mag1[8]={0x01,0x03,0x00,0x01,0x00,0x01,0xd5,0xca};		//��ȡ1�Ŵŵ�������������
u8 mag2[8]={0x02,0x03,0x00,0x01,0x00,0x01,0xd5,0xf9};		//��ȡ2�Ŵŵ�������������
u8 mag3[8]={0x03,0x03,0x00,0x01,0x00,0x01,0xd4,0x28};		//��ȡ3�Ŵŵ�������������
u8 mag4[8]={0x04,0x03,0x00,0x01,0x00,0x01,0xd5,0x9f};		//��ȡ4�Ŵŵ�������������


/*************�ŵ������ݻ�ȡ**********************/
//void magnetic_recei(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
//����ֵ��u8 * magnetic_in��ͨ�Ŷ������Ĵ���������  
//���ֵ��float * Sum   Sum[0]:�ŵ��������ܺͣ�Sum[1]:�ŵ������ݵ�������Sum[2]:�ŵ�����������
//����ֵ����
/*******************************************************/
InitMagDateDef magnetic_recei(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
{	
	InitMagDateDef initMagDate={0,0,0,0};

	//send_magntc_cmd(8,mag1);													//����1�Ŵŵ�������������  ǰ1	
	delay_us(1000);	
	initMagDate.front1=magbetic_tran(magnetic_in,Mag_date,mag_alarm);		//��ȡ����
	delay_us(1000);
	mag_date[2]=-mag_date[2];	
	return initMagDate;
}


/*************����ŵ����Ƕ�ƫתֵ**********************/
//int mag_calcul(u8 * magnetic_in)
//����ֵ��u8 * magnetic_in��ͨ�Ŷ������Ĵ���������  
//���ֵ��float * mag_date   mag_date[0]:�ŵ��������ܺͣ� mag_date[1]:�ŵ������ݵ������� mag_date[2]:�ŵ�����������
//����ֵ��u16:���شŵ���16λԭʼ����
/*******************************************************/

	//mag_date[0]=temp_stor;�������ݵ���ֵ
	//mag_date[1]=right-left+1;�������ݵ����
	//mag_date[2]=mag_date[0]/mag_date[1];	
	//	
	//0  1  2  3  4  5  6  7 |(�м��) 8  9  10 11 12 13 14 15     λ�ƴ������ɼ���
	//80 70 60 50 40 30 20 10|(�м��) 10 20 30 40 50 60 70 80	   λ�ƴ������ɼ����Ӧ����ֵ
u16 mag_calcul(u8 * magnetic_in,float * mag_date)
{
	int i;
	u16 all,alls;
	u8 high=0,low=0;
	u8 static left=0,right=0;
	float stor[16];
	u8 sum_pot[16];
	float temp_stor=0,temp_pot=0;

	
		//��ȡ���Ҳɼ���
		high=*(magnetic_in+3);				//��ȡ�߰�λ����
		low=*(magnetic_in+4);				//��ȡ�Ͱ�λ����
		all= (high << 8) | low;				//����8λ��8λ�ϲ���16λ
	
		if(RFID_LeftRight==1)//��ת
		{	
			for(i=0;i<=15;i++)
			{			
				if( ((all<<i)&0x8000)==0x8000 )//��ȡ��ߵ���һ���ɼ���	
				{	
					left=i;break;	
				}
			}
			if(left!=15)
			{
				for(i=left;i<=15;i++)
				{
					if( ((all<<i)&0x8000)==0)//��ȡ�ұߵ���һ���ɼ���	
					{
						right=i-1;break;
					}		
				}
			}
			else	right=i;
						
			all&=(0xffff<<(15-right));			//�µĲɼ���
			high=all>>8;					//��ȡ�߰�λ
			low =all;					//��ȡ�Ͱ�λ	
		}
		else if(RFID_LeftRight==2)//��ת
		{	
			for(i=0;i<=15;i++)
			{						
				if( ((all>>i)&0x0001)==1)//��ȡ��ߵ���һ���ɼ���
				{ 
					right=(15-i);break;
				}		
			}
			for(i=(15-right);i<=15;i++)
			{
				if( ((all>>i)&0x0001)==0)//��ȡ�ұߵ���һ���ɼ���	
				{
					left=(15-i);break;
				}		
			}	
			all&=(0xffff>>left);				//�µĲɼ���
			high=all>>8;								//��ȡ�߰�λ
			low =all;										//��ȡ�Ͱ�λ
		}														
		
		//�ɼ������//ֱ�� ����1��2������״̬����ֱ����ʻ
		for(i=7;i>=0;i--)						//��ȡ��ߵ�����
		{
			sum_pot[i]=(high>>(7-i))&0x01;
			stor[i]=(float)(sum_pot[i])*(8-i)*10;
			temp_pot+=sum_pot[i];
			temp_stor+=stor[i];
		}
		for(i=15;i>=8;i--)					//��ȡ�ұߵ�����
		{
			sum_pot[i]=(low>>(15-i))&0x01;
			stor[i]=-(float)(sum_pot[i])*(i-7)*10;
			temp_pot+=sum_pot[i];
			temp_stor+=stor[i];
		}	
			mag_date[0]=temp_stor;
			mag_date[1]=temp_pot;
			mag_date[2]=mag_date[0]/mag_date[1];				//����ŵ����ĵ���ֵ��		
		
		return all;														//���شŵ���ԭʼ����
}
	


/*******����ȡ�����1-4�Ĵŵ�������������ת��Ϊ��������*******/
//void magbetic_tran(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
//�������ݣ�magnetic_in  ����Ĵŵ���    
//������ݣ�u8 mag_out[4]:�ŵ����������               u8 mag_alarm[4]���ŵ����Ƿ��⵽���ݱ��� 0��û��⵽����   1����⵽����
//duqu
//mag_date[0]:1�Ŵŵ��������������ܺ�
//mag_date[1]:1�Ŵŵ������������ݵ�����
//mag_date[2]:1�Ŵŵ�����������������
//
//mag_date[3]:2�Ŵŵ��������������ܺ�
//mag_date[4]:2�Ŵŵ������������ݵ�����
//mag_date[5]:2�Ŵŵ�����������������
//
//mag_date[6]:3�Ŵŵ��������������ܺ�
//mag_date[7]:3�Ŵŵ������������ݵ�����
//mag_date[8]:3�Ŵŵ�����������������
//
//mag_date[9]:4�Ŵŵ��������������ܺ�
//mag_date[10]:4�Ŵŵ������������ݵ�����
//mag_date[11]:4�Ŵŵ�����������������
//����ֵ����
/**************************************************************/
u16 magbetic_tran(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
{
	u16 initMagDate=0,temp=1;
	u8 i;
	static u8 alarm_Add[4]={0,0,0,0};							//�����ۼ���
	u8 temp_Alarm[4];
	
	
	if(*(magnetic_in+1)==0x03 && *(magnetic_in+2)==0x02)						//���ڶ����ֽ�Ϊ0x03,�������ֽ�Ϊ0x02,˵��������Ч
	{
		switch(*magnetic_in)									//�ж��յ�����������Ӧ�ĵ�ַ
		{
			case 0x01:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[0]);								//��ȡ�ŵ�������
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);									//�ж��Ƿ��ж������ݷ���1										
				if(mag_date[1]==0||temp==1) temp_Alarm[0]=0;									//�ж��Ƿ��ж�������
				else temp_Alarm[0]=1;
				
				
			}break;		
			case 0x02:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[3]);							//��ȡ�ŵ�������
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);								//�ж��Ƿ��ж������ݷ���1			
				if(mag_date[4]==0||temp==1) temp_Alarm[1]=0;								//�ж��Ƿ��ж�������
				else temp_Alarm[1]=1;
			}break;
			case 0x03:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[6]);								//��ȡ�ŵ�������
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);								//�ж��Ƿ��ж������ݷ���1		
				if(mag_date[7]==0||temp==1) temp_Alarm[2]=0;										//�ж��Ƿ��ж�������
				else temp_Alarm[2]=1;
			}break;
			case 0x04:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[9]);								//��ȡ�ŵ�������
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);								//�ж��Ƿ��ж������ݷ���1
					if(mag_date[10]==0||temp==1) temp_Alarm[3]=0;								//�ж��Ƿ��ж�������
					else temp_Alarm[3]=1;
				}break;	
			}
		}
		
		if(temp_Alarm[0]!=1)						//��AGV����ǰ��״̬��û�ж�ȡ���ŵ����ź�
		{
			if(alarm_Add[0]<3)	alarm_Add[0]++;		//��AGV�ŵ��������ź�����������ŷ��ȡ����˵���ѹ�
		}
		else alarm_Add[0]=0;	
			
		if(alarm_Add[0]==3) mag_alarm[0]=0;
	else if(temp_Alarm[0]==1) mag_alarm[0]=1;
		
	return initMagDate;
}


/************************ʮ����תʮ����***********************/
//u8 deci_hex(u8 * magnetic_in)
//�������ݣ�magnetic_in  ����Ĵŵ���
//������ݣ���
//����ֵ��1����ȡ���ŵ������ݣ�0��û�ж�ȡ���ŵ�������
//���ֵ��65535
/**************************************************************/
float deci_hex(u8 * magnetic_in)
{
	u8 high,low; 
	float stor;
	high=*(magnetic_in+3);					//��ȡ�߰�λ
	low=*(magnetic_in+4);						//��ȡ�Ͱ�λ
	
	stor=	deci_hex_single((high&0xF0)>>4)*pow(16,3);					//��߰�λ�ĸ���λ
	stor+=deci_hex_single(high&0x0F)*pow(16,2);							//��߰�λ�ĵ���λ
	stor+=deci_hex_single((low&0xF0)>>4)*16;					//��Ͱ�λ�ĸ���λ
	stor+=deci_hex_single((low&0x0F));					//��Ͱ�λ�ĸ���λ
	return stor; 
}

/************************����16����ת��Ϊ10����***********************/
//u8 deci_hex_(u8 * magnetic_in)
//�������ݣ�u8 date_in  ���������
//������ݣ���
//����ֵ��1������ʮ��������
//
/**************************************************************/
float deci_hex_single(u8 date_in)
{
	float date;
	if(date_in>=0x0A) date=(float)(date_in-0x0A)+10;					//������>=0x0A,���ȥ0x0A
	else date=(float)date_in;																	//������<0x0A,���ü���0x0A																									
	return date;
}



/************************�ŵ���ƫ��ǣ�Ea�������ƫ����(Ed)����**********************/
//void Ea_Ed_calcu(float * mag_date,float Ea_Ed[4])
//�������ݣ���ȡ��������ŵ������������ݣ�
//������ݣ�
//Ea_Ed[0]:ǰ��������Ĵŵ���ƫ���    Ea_Ed[1]:ǰ��������ĺ���ƫ����
//Ea_Ed[2]:����������Ĵŵ���ƫ���    Ea_Ed[3]:����������ĺ���ƫ����
//
//
/**************************************************************/
void Ea_Ed_calcu(float * mag_date,float Ea_Ed[4])
{
	
	float temp;	
	
	Ea_Ed[1]=(mag_date[2]+mag_date[5])/2;								//��ǰ������������ƫ����		Ed=(E1+E2)/2;		
	temp=abs(mag_date[2])+abs(mag_date[5]);
	if(mag_date[5]>=mag_date[2])		Ea_Ed[0]=-(atan(temp/mag_d))*180/pi;		//��ǰ��������ƫת��				Ea=arctan((|E1|+|E2|)/d)  ����������ƫ  E2>E1
	else Ea_Ed[0]=atan(temp/mag_d)*180/pi;								//����������ƫ
	
	Ea_Ed[3]=(mag_date[8]+mag_date[11])/2;							//��ǰ������������ƫ����		Ed=(E1+E2)/2;		
	temp=abs(mag_date[8])+abs(mag_date[11]);
	if(mag_date[11]>mag_date[8])		Ea_Ed[2]=-(atan(temp/mag_d))*180/pi;		//�����������ƫת��				Ea=arctan((|E1|+|E2|)/d)  ����������ƫ  E2>E1
	else Ea_Ed[2]=atan(temp/mag_d)*180/pi;									//����������ƫ
	
	if(abs(Ea_Ed[0])<thresh_Ea)	Ea_Ed[0]=0;						//��ƫת��С��������ֵ����ǿ��Ϊ0
	if(abs(Ea_Ed[1])<thresh_Ed)	Ea_Ed[1]=0;						//������ƫ����С��������ֵ����ǿ��Ϊ0
	if(abs(Ea_Ed[2])<thresh_Ea)	Ea_Ed[2]=0;						//��ƫת��С��������ֵ����ǿ��Ϊ0
	if(abs(Ea_Ed[3])<thresh_Ed)	Ea_Ed[3]=0;						//������ƫ����С��������ֵ����ǿ��Ϊ0
}




/**********************IO���ʹŵ������������ݶ�ȡ****************************/
//��������ʽ�� int MNSV_X8IO_Read��void��
//���봫�Σ�
//������Σ�
//����ֵ����ȡ���Ĵŵ���������ֵ
//
//
/************************************************************************/
void MNSV_X8IO_Read(int * temp)
{
	u8 i;
	static int value;
	static int num;
	value=0;
	num=0;
	for(i=1;i<5;i++)	value+=10*(5-i)*IOStatus.IA[i];
	for(i=5;i<9;i++)	value+=-10*(i-4)*IOStatus.IA[i];
	
	for(i=0;i<9;i++)	num=IOStatus.IA[i]+num;
	
	value=value/num;
	if(num==0) value=0;
	temp[0]=value;
	temp[1]=num;
}




















