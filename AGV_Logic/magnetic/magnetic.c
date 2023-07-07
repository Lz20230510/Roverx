#include "HEAD.h"
#include "math.h"

	int value=0;
	u16 num;
//u8 high,low,left,right;
float Mag_date[12]={0};					//四组磁导航传感器分别读取到的数值存储
u8 Mag_alarm[4]={0};	

u8 mag1[8]={0x01,0x03,0x00,0x01,0x00,0x01,0xd5,0xca};		//读取1号磁导航传感器数据
u8 mag2[8]={0x02,0x03,0x00,0x01,0x00,0x01,0xd5,0xf9};		//读取2号磁导航传感器数据
u8 mag3[8]={0x03,0x03,0x00,0x01,0x00,0x01,0xd4,0x28};		//读取3号磁导航传感器数据
u8 mag4[8]={0x04,0x03,0x00,0x01,0x00,0x01,0xd5,0x9f};		//读取4号磁导航传感器数据


/*************磁导航数据获取**********************/
//void magnetic_recei(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
//输入值：u8 * magnetic_in：通信读到到的传感器数据  
//输出值：float * Sum   Sum[0]:磁导航数据总和，Sum[1]:磁导航数据点数量，Sum[2]:磁导航导航数据
//返回值：无
/*******************************************************/
InitMagDateDef magnetic_recei(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
{	
	InitMagDateDef initMagDate={0,0,0,0};

	//send_magntc_cmd(8,mag1);													//发送1号磁导航传感器数据  前1	
	delay_us(1000);	
	initMagDate.front1=magbetic_tran(magnetic_in,Mag_date,mag_alarm);		//读取数据
	delay_us(1000);
	mag_date[2]=-mag_date[2];	
	return initMagDate;
}


/*************计算磁导航角度偏转值**********************/
//int mag_calcul(u8 * magnetic_in)
//输入值：u8 * magnetic_in：通信读到到的传感器数据  
//输出值：float * mag_date   mag_date[0]:磁导航数据总和， mag_date[1]:磁导航数据点数量， mag_date[2]:磁导航导航数据
//返回值：u16:返回磁导航16位原始数据
/*******************************************************/

	//mag_date[0]=temp_stor;保存数据点数值
	//mag_date[1]=right-left+1;保存数据点个数
	//mag_date[2]=mag_date[0]/mag_date[1];	
	//	
	//0  1  2  3  4  5  6  7 |(中间点) 8  9  10 11 12 13 14 15     位移传感器采集点
	//80 70 60 50 40 30 20 10|(中间点) 10 20 30 40 50 60 70 80	   位移传感器采集点对应的数值
u16 mag_calcul(u8 * magnetic_in,float * mag_date)
{
	int i;
	u16 all,alls;
	u8 high=0,low=0;
	u8 static left=0,right=0;
	float stor[16];
	u8 sum_pot[16];
	float temp_stor=0,temp_pot=0;

	
		//读取左右采集点
		high=*(magnetic_in+3);				//读取高八位数据
		low=*(magnetic_in+4);				//读取低八位数据
		all= (high << 8) | low;				//将高8位低8位合并成16位
	
		if(RFID_LeftRight==1)//左转
		{	
			for(i=0;i<=15;i++)
			{			
				if( ((all<<i)&0x8000)==0x8000 )//获取左边的那一个采集点	
				{	
					left=i;break;	
				}
			}
			if(left!=15)
			{
				for(i=left;i<=15;i++)
				{
					if( ((all<<i)&0x8000)==0)//获取右边的那一个采集点	
					{
						right=i-1;break;
					}		
				}
			}
			else	right=i;
						
			all&=(0xffff<<(15-right));			//新的采集点
			high=all>>8;					//提取高八位
			low =all;					//提取低八位	
		}
		else if(RFID_LeftRight==2)//右转
		{	
			for(i=0;i<=15;i++)
			{						
				if( ((all>>i)&0x0001)==1)//获取左边的那一个采集点
				{ 
					right=(15-i);break;
				}		
			}
			for(i=(15-right);i<=15;i++)
			{
				if( ((all>>i)&0x0001)==0)//获取右边的那一个采集点	
				{
					left=(15-i);break;
				}		
			}	
			all&=(0xffff>>left);				//新的采集点
			high=all>>8;								//提取高八位
			low =all;										//提取低八位
		}														
		
		//采集点计算//直行 除了1，2，其它状态都是直线行驶
		for(i=7;i>=0;i--)						//读取左边的数据
		{
			sum_pot[i]=(high>>(7-i))&0x01;
			stor[i]=(float)(sum_pot[i])*(8-i)*10;
			temp_pot+=sum_pot[i];
			temp_stor+=stor[i];
		}
		for(i=15;i>=8;i--)					//读取右边的数据
		{
			sum_pot[i]=(low>>(15-i))&0x01;
			stor[i]=-(float)(sum_pot[i])*(i-7)*10;
			temp_pot+=sum_pot[i];
			temp_stor+=stor[i];
		}	
			mag_date[0]=temp_stor;
			mag_date[1]=temp_pot;
			mag_date[2]=mag_date[0]/mag_date[1];				//求出磁导航的导航值域		
		
		return all;														//返回磁导航原始数据
}
	


/*******将读取到编号1-4的磁导航传感器数据转换为可用数据*******/
//void magbetic_tran(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
//输入数据：magnetic_in  输入的磁导航    
//输出数据：u8 mag_out[4]:磁导航数据输出               u8 mag_alarm[4]：磁导航是否检测到数据报警 0：没检测到磁条   1：检测到磁条
//duqu
//mag_date[0]:1号磁导航传感器数据总和
//mag_date[1]:1号磁导航传感器数据点数量
//mag_date[2]:1号磁导航传感器导航数据
//
//mag_date[3]:2号磁导航传感器数据总和
//mag_date[4]:2号磁导航传感器数据点数量
//mag_date[5]:2号磁导航传感器导航数据
//
//mag_date[6]:3号磁导航传感器数据总和
//mag_date[7]:3号磁导航传感器数据点数量
//mag_date[8]:3号磁导航传感器导航数据
//
//mag_date[9]:4号磁导航传感器数据总和
//mag_date[10]:4号磁导航传感器数据点数量
//mag_date[11]:4号磁导航传感器导航数据
//返回值：无
/**************************************************************/
u16 magbetic_tran(u8 * magnetic_in,float mag_date[12],u8 mag_alarm[4])
{
	u16 initMagDate=0,temp=1;
	u8 i;
	static u8 alarm_Add[4]={0,0,0,0};							//报警累加器
	u8 temp_Alarm[4];
	
	
	if(*(magnetic_in+1)==0x03 && *(magnetic_in+2)==0x02)						//当第二个字节为0x03,第三个字节为0x02,说明数据有效
	{
		switch(*magnetic_in)									//判断收到的数据所对应的地址
		{
			case 0x01:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[0]);								//读取磁导航数据
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);									//判断是否有读到数据方法1										
				if(mag_date[1]==0||temp==1) temp_Alarm[0]=0;									//判断是否有读到数据
				else temp_Alarm[0]=1;
				
				
			}break;		
			case 0x02:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[3]);							//读取磁导航数据
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);								//判断是否有读到数据方法1			
				if(mag_date[4]==0||temp==1) temp_Alarm[1]=0;								//判断是否有读到数据
				else temp_Alarm[1]=1;
			}break;
			case 0x03:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[6]);								//读取磁导航数据
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);								//判断是否有读到数据方法1		
				if(mag_date[7]==0||temp==1) temp_Alarm[2]=0;										//判断是否有读到数据
				else temp_Alarm[2]=1;
			}break;
			case 0x04:
			{
				initMagDate=mag_calcul(magnetic_in,&mag_date[9]);								//读取磁导航数据
				for(i=0;i<15;i++) temp&=!((initMagDate>>i)&0x0001);								//判断是否有读到数据方法1
					if(mag_date[10]==0||temp==1) temp_Alarm[3]=0;								//判断是否有读到数据
					else temp_Alarm[3]=1;
				}break;	
			}
		}
		
		if(temp_Alarm[0]!=1)						//当AGV处于前进状态且没有读取到磁导航信号
		{
			if(alarm_Add[0]<3)	alarm_Add[0]++;		//当AGV磁导航报警信号连续三次美欧读取到，说明脱轨
		}
		else alarm_Add[0]=0;	
			
		if(alarm_Add[0]==3) mag_alarm[0]=0;
	else if(temp_Alarm[0]==1) mag_alarm[0]=1;
		
	return initMagDate;
}


/************************十六进转十进制***********************/
//u8 deci_hex(u8 * magnetic_in)
//输入数据：magnetic_in  输入的磁导航
//输出数据：无
//返回值：1：读取到磁导航数据，0：没有读取到磁导航数据
//最大值是65535
/**************************************************************/
float deci_hex(u8 * magnetic_in)
{
	u8 high,low; 
	float stor;
	high=*(magnetic_in+3);					//提取高八位
	low=*(magnetic_in+4);						//提取低八位
	
	stor=	deci_hex_single((high&0xF0)>>4)*pow(16,3);					//求高八位的高四位
	stor+=deci_hex_single(high&0x0F)*pow(16,2);							//求高八位的低四位
	stor+=deci_hex_single((low&0xF0)>>4)*16;					//求低八位的高四位
	stor+=deci_hex_single((low&0x0F));					//求低八位的高四位
	return stor; 
}

/************************单个16进制转换为10进制***********************/
//u8 deci_hex_(u8 * magnetic_in)
//输入数据：u8 date_in  输入的数据
//输出数据：无
//返回值：1：返回十进制数据
//
/**************************************************************/
float deci_hex_single(u8 date_in)
{
	float date;
	if(date_in>=0x0A) date=(float)(date_in-0x0A)+10;					//如果结果>=0x0A,则减去0x0A
	else date=(float)date_in;																	//如果结果<0x0A,则不用减速0x0A																									
	return date;
}



/************************磁导航偏差角（Ea）与横向偏移量(Ed)计算**********************/
//void Ea_Ed_calcu(float * mag_date,float Ea_Ed[4])
//输入数据：读取到的四组磁导航传感器数据，
//输出数据：
//Ea_Ed[0]:前驱动输出的磁导航偏差角    Ea_Ed[1]:前驱动输出的横向偏移量
//Ea_Ed[2]:后驱动输出的磁导航偏差角    Ea_Ed[3]:后驱动输出的横向偏移量
//
//
/**************************************************************/
void Ea_Ed_calcu(float * mag_date,float Ea_Ed[4])
{
	
	float temp;	
	
	Ea_Ed[1]=(mag_date[2]+mag_date[5])/2;								//求前驱动机构横向偏移量		Ed=(E1+E2)/2;		
	temp=abs(mag_date[2])+abs(mag_date[5]);
	if(mag_date[5]>=mag_date[2])		Ea_Ed[0]=-(atan(temp/mag_d))*180/pi;		//求前驱动机构偏转角				Ea=arctan((|E1|+|E2|)/d)  驱动机构右偏  E2>E1
	else Ea_Ed[0]=atan(temp/mag_d)*180/pi;								//驱动机构左偏
	
	Ea_Ed[3]=(mag_date[8]+mag_date[11])/2;							//求前驱动机构横向偏移量		Ed=(E1+E2)/2;		
	temp=abs(mag_date[8])+abs(mag_date[11]);
	if(mag_date[11]>mag_date[8])		Ea_Ed[2]=-(atan(temp/mag_d))*180/pi;		//求后驱动机构偏转角				Ea=arctan((|E1|+|E2|)/d)  驱动机构右偏  E2>E1
	else Ea_Ed[2]=atan(temp/mag_d)*180/pi;									//驱动机构左偏
	
	if(abs(Ea_Ed[0])<thresh_Ea)	Ea_Ed[0]=0;						//当偏转角小于死区阈值，则强制为0
	if(abs(Ea_Ed[1])<thresh_Ed)	Ea_Ed[1]=0;						//当横向偏移量小于死区阈值，则强制为0
	if(abs(Ea_Ed[2])<thresh_Ea)	Ea_Ed[2]=0;						//当偏转角小于死区阈值，则强制为0
	if(abs(Ea_Ed[3])<thresh_Ed)	Ea_Ed[3]=0;						//当横向偏移量小于死区阈值，则强制为0
}




/**********************IO类型磁导航传感器数据读取****************************/
//函数名形式： int MNSV_X8IO_Read（void）
//输入传参：
//输出传参：
//返回值：获取到的磁导航传感器值
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




















