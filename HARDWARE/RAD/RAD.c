#include "delay.h" 
#include "usart4.h" 
#include "RAD.h" 
#include "vehicle_key.h" 
#include "HEAD.h"


//u8 Comd_RAD[1]={0xAA};	//开始命令。请求数据
//u8 Data_RAD[13]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
//extern u8 RAD[13]={0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55};


//*************************实现函数*****************************/
//void ultrosonic_GPIO_Init(void)
//输入参数：		
//返回参数：  
//
//功能说明：超声波避障传感器使能口初始化
//**************************************************************/
void ultrosonic_GPIO_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF  GPIOA时钟	
	 //GPIOA8初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	
	GPIO_SetBits(GPIOE,GPIO_Pin_9);//GPIOE8设置高，灯灭
}


//*************************实现函数*****************************/
//u8 ultrosonic_check(u8 * buff)
//输入参数：		串口缓冲	
//返回参数：  =1：接收超声波数据正确，=0：接收超声波数据错误
//
//功能说明：对超声波数据进行检验检查
//**************************************************************/
u8 ultrosonic_check(u8 * buff)
{
	u8 i;
	u16 temp;
	if(*buff==0xAA&& *(buff+11)==0x55)
	{
		for(i=1;i<11;i++) temp+=(u16) buff[i];					//超声波数据累加	
		temp=temp&0x00FF;																//求低8位
	}
	if(temp==*(buff+11))	return 1;										//当校验正确返回1
	else return 1;																		//错误返回0
}


//*************************实现函数*****************************/
//UltrosonicDef ultrosonic_Read(u8 * buff)
//输入参数：		串口缓冲	
//返回参数：  =1：接收超声波数据正确，=0：接收超声波数据错误
//
//功能说明：读取每一路的超声波信号,读取每一路超声波安全距离设置
//**************************************************************/
UltrosonicDef ultrosonic_Read(u8 * buff)
{
	u8 i;
	static UltrosonicDef ultrosonic;
	for(i=1;i<9;i++) ultrosonic.channel[i-1]=buff[i];			//超声波各传感器数据赋值
	
	for(i=0;i<8;i++) 				//读取常规设置界面上的超声波避障数据
	{
		ultrosonic.safeDistance[i]=(u8)Paramt_BUFF[17][i+20];				//读取参数缓冲区数据
	}	
	return ultrosonic;
}


/******************************实现函数*************************/
//函数形式：void ultrosonic_process(void)
//输入参数：超声波相关参数结构体
//输出参数：
//
//返回参数：无
//功能说明：超声波避障
/**************************************************************/
void ultrosonic_process(void)
{
	u8 i;
	static u8 safeOk[4],addMark[4];						//safeOk=0 说明避障在安全范围内，safeOk=1 说明避障不在安全范围内  

	Ultrosonic.deviceEn=(u8)Paramt_BUFF[Pg_CommonParmt][5];						//避障传感器是否打开
	if(Ultrosonic.deviceEn==1 && AGVStatus.AJMode==1)						//当超声波使能
	{
		for(i=0;i<4;i++)
		{
			if(Ultrosonic.channel[i]<Ultrosonic.safeDistance[i])			//当超声波检测距离小于安全距离   通道1
			{
				if(addMark[i]<3)	addMark[i]++;
			}	
			if(Ultrosonic.channel[0]>=Ultrosonic.safeDistance[i])			//当超声波检测距离小于安全距离   通道1
			{
				if(addMark[i]>0)	addMark[i]--;
			}				
			if(addMark[i]==3)	safeOk[i]=1;											//当超过安全距离累加超过3次，则安全距离标记置1 说明有障碍物小于安全距离
			if(addMark[i]==0) safeOk[i]=0;
		}	
		UltalEn=0;								//避障传感器打开
	}	
	if(AGVStatus.AJMode==0||Ultrosonic.deviceEn==0)					//当超声波未使能
	{
		for(i=0;i<4;i++)					//状态标记清零
		{
			safeOk[i]=0;
			addMark[i]=0;						
		}	
		UltalEn=1;								//避障传感器关闭
	}
	if(safeOk[0]|safeOk[1]|safeOk[2]|safeOk[3]!=0x00)	Speed_contr(0.0,0.0,0.0,0.0,0);   		//当有安全标记不为0时，则AGV停止运行
}



