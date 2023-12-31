//神舟III号开发板
#include "24l01.h"
#include "delay.h"
#include "spi.h"
#include "led.h"
#include "math.h"
#include "AGV_contr.h"
#include "vehicle_key.h" 
#include "HEAD.h"
#include "Time.h" 
//NRF24L01 驱动函数 
			
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x01}; //发送地址							    
//初始化24L01的IO口
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOB,G时钟
	
	 //GPIOB14初始化设置:推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PB14
			
//	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);           //使能GPIO的时钟
    GPIO_InitStructure.GPIO_Pin = NRF24L01_CE;              //NRF24L01 模块片选信号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CE, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);   //使能GPIO的时钟
//	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);          
  GPIO_InitStructure.GPIO_Pin = NRF24L01_CSN;      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_CSN, &GPIO_InitStructure);
	
	Set_NRF24L01_CE;                                           //初始化时先拉高
  Set_NRF24L01_CSN;                                   //初始化时先拉高

    //配置NRF2401的IRQ
	GPIO_InitStructure.GPIO_Pin = NRF24L01_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_NRF24L01_IRQ, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_NRF24L01_IRQ,NRF24L01_IRQ);

	SPI1_Init();                                       //初始化SPI
	Clr_NRF24L01_CE; 	                               //使能24L01
	Set_NRF24L01_CSN;                                  //SPI片选取消
}
//上电检测NRF24L01是否在位
//写5个数据然后再读回来进行比较，相同时返回值:0，表示在位;否则返回1，表示不在位	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i;   	 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5);              //读出写入的地址  	
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;					   
	if(i!=5)return 1;                               //NRF24L01不在位	
	return 0;		                                //NRF24L01在位
}	 	 
//通过SPI写寄存器
u8 NRF24L01_Write_Reg(u8 regaddr,u8 data)
{
	u8 status;	
    Clr_NRF24L01_CSN;                    //使能SPI传输
  	status =SPI1_ReadWriteByte(regaddr); //发送寄存器号 
  	SPI1_ReadWriteByte(data);            //写入寄存器的值
  	Set_NRF24L01_CSN;                    //禁止SPI传输	   
  	return(status);       		         //返回状态值
}
//读取SPI寄存器值 ，regaddr:要读的寄存器
u8 NRF24L01_Read_Reg(u8 regaddr)
{
	u8 reg_val;	    
 	Clr_NRF24L01_CSN;                //使能SPI传输		
  	SPI1_ReadWriteByte(regaddr);     //发送寄存器号
  	reg_val=SPI1_ReadWriteByte(0XFF);//读取寄存器内容
  	Set_NRF24L01_CSN;                //禁止SPI传输		    
  	return(reg_val);                 //返回状态值
}	
//在指定位置读出指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 regaddr,u8 *pBuf,u8 datalen)
{
	u8 status,u8_ctr;	       
  	Clr_NRF24L01_CSN;                     //使能SPI传输
  	status=SPI1_ReadWriteByte(regaddr);   //发送寄存器值(位置),并读取状态值   	   
 	for(u8_ctr=0;u8_ctr<datalen;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//读出数据
  	Set_NRF24L01_CSN;                     //关闭SPI传输
  	return status;                        //返回读到的状态值
}
//在指定位置写指定长度的数据
//*pBuf:数据指针
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 regaddr, u8 *pBuf, u8 datalen)
{
	u8 status,u8_ctr;	    
 	Clr_NRF24L01_CSN;                                    //使能SPI传输
  	status = SPI1_ReadWriteByte(regaddr);                //发送寄存器值(位置),并读取状态值
  	for(u8_ctr=0; u8_ctr<datalen; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //写入数据	 
  	Set_NRF24L01_CSN;                                    //关闭SPI传输
  	return status;                                       //返回读到的状态值
}				   
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 state;   
	Clr_NRF24L01_CE;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	Set_NRF24L01_CE;                                     //启动发送	   
	while(READ_NRF24L01_IRQ!=0);                              //等待发送完成
	state=NRF24L01_Read_Reg(STATUS);                     //读取状态寄存器的值	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //清除TX_DS或MAX_RT中断标志
	if(state&MAX_TX)                                     //达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //发送完成
	{
		return TX_OK;
	}
	return 0xff;                                         //其他原因发送失败
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //清除TX_DS或MAX_RT中断标志
	if(state&RX_OK)                                 //接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;                                      //没收到任何数据
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void RX_Mode(void)
{
	Clr_NRF24L01_CE;	  
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x3f);    //使能通道0的自动应答    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x3f);//使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);	     //设置RF通信频率		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x3f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x71);  
  	Set_NRF24L01_CE;                                //CE为高,进入接收模式 
	
		Clr_NRF24L01_CE;
		NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,NRF24L01_Read_Reg(STATUS)); //清除TX_DS或MAX_RT中断标�
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //清除RX FIFO寄存器 �
		Set_NRF24L01_CE;                                //CE为高,进入接收模式 
	
	
	
}						 
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void TX_Mode(void)
{														 
	Clr_NRF24L01_CE;	    
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);       //设置RF通道为40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	Set_NRF24L01_CE;                                  //CE为高,10us后启动发送
}		  



//////////////////////////////////////////////遥控器失联检查////////////////////////////////////
//函数名称：u8 Syma_check(float *yaogan,u8 *tmp_buf)
//输入：tmp_buf：接收到的遥控器缓存数据    
//输出：yaogan: 摇杆模拟量数据
//返回：1：遥控器失联     0：遥控器正常
//
////////////////////////////////////////////////////////////////////////////////////////////////
u8 Syma_check(float *yaogan,u8 *tmp_buf)
{
	u8 static Err_num=20;																				//遥控器失联循环扫描次数
	u8 static Err_mark=0;                                      //报警标志位
	u8 static Err[2]={0,0};																				//遥控器失效检测
	float static Err_stor[2]={0};                           //遥控器失效检测寄存器
	
	u8 i;
	if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
	{			
		for(i=0;i<4;i++)
		{
			yaogan[i]=-(*(float *)(&tmp_buf[i*4]));  //两个摇杆的数据											
		}								
	}
		
	if(Err_stor[0]!=yaogan[0]||Err_stor[1]!=yaogan[1])  Err[0]=0;																										//当数据不相同时重新计数
	if((Err_stor[0]==yaogan[0])&&(Err_stor[1]==yaogan[1])&&Err[0]<Err_num) 	Err[0]++;                                //当数据相同且小于报警循环次数是开始计数
	
	if(Err[0]==0&&Err_mark==0)                                      //当无报警且计数为0时，数据存储
	{
		Err_stor[0]=yaogan[0];
		Err_stor[1]=yaogan[1];
	}		

	if(Err[0]>Err_num-2)	Err_mark=1;													//当数据超过20次都相等，说明遥控器失联													
	else Err_mark=0;																		//遥控器没有失联
	

	if(Err_mark)															//当遥控器失联，所有数值初始化
	{
		AGVStatus.Setup=0;									//启动停止
		OT_SON=0;											//断使能
		for(i=0;i<4;i++)
		{
			SymaStatus.Yaogan[i]=0;								//摇杆输出的模拟量值置0
			SymaStatus.Syma_cmd[i]=0;							//遥控器摇杆方方向指令置0	
		}
	}
	
	return Err_mark;
}


//////////////////////////////////////////////遥控器摇杆数据接收及处理/////////////////////////////////
//说明：遥控器摇杆指令的读取
////函数名称：Syma_yaogan_receive(float *yaogan,u8 *tmp_buf,u8 * Syma_cmd,float spd)
////输入： float *yaogan：摇杆浮点数  u8 *tmp_buf：遥控器接受数据  
////输出：u8 * Syma_cmd：遥控器指令    float spd：输入速度（速度最大值）
////返回：调速值
//Syma_cmd:
//[0]:左摇杆（左右）  自转指令 =1：左自转   =2：右自转
//[1]:左摇杆（上下）  8个档7档最高速，0档停止
//[2]:右摇杆（左右）  =1：左转      =2：右转
//[3]:右摇杆（上下）  =1：前进        =2：后退
//////////////////////////////////////////////////////////////////////////////////////////////////

float Syma_yaogan_receive(float *yaogan,u8 *tmp_buf,u8 * syma_cmd,float spd)
{
	//////////////////////////左摇杆左右///////////////////////			
	
	
	if(AGVStatus.Setup==1&&AGVStatus.EmerStop!=1)													//启动且非急停状态下，遥控方向及速度功能有效						
	{
		//////////////////////////右摇杆上下//////////////////////	
		if(yaogan[3]<-0.3f) syma_cmd[3]=1;	                           //右摇杆（上下）前进
		else if(yaogan[3]>0.75f)  syma_cmd[3]=2; 									 		//右摇杆（上下）后退
		else syma_cmd[3]=0; 
			
		if(syma_cmd[3]==0)                    											  //当前后没有数据输出
		{
			if(yaogan[2]<-0.3f) syma_cmd[2]=1;	                           //右摇杆（左右）左转
			else if(yaogan[2]>0.85f) 
			{
				if(syma_cmd[3]!=2) syma_cmd[2]=2; 									 //右摇杆（左右）右转
				else syma_cmd[2]=0;                                  //前进的优先级高于转弯
			}	
		}	
		if(yaogan[2]>=-0.3f&&yaogan[2]<0.85f) syma_cmd[2]=0; 			//当摇杆左右在初始位置，则数据位0		

		//////////////////////////左摇杆左右自转//////////////////////			
		
		if(syma_cmd[3]==0&&syma_cmd[2]==0)																			//右摇杆优先级高于左摇杆左右自转
		{
			if(yaogan[0]<-0.3f)  syma_cmd[0]=1;                              			 //左摇杆数据处理（左右）  左自转
			else if(yaogan[0]>0.75f) 	syma_cmd[0]=2;															//右自转
			else	syma_cmd[0]=0;																										//停止		
		}
		else 	syma_cmd[0]=0;	
		
		//////////////////////////左摇杆上下//////////////////////			
		if(yaogan[1]<-0.3f)   syma_cmd[1]=5 ;                            			  //左摇杆数据处理（上下）6档  
		else if(yaogan[1]<0&& yaogan[1]>=-0.3f)  syma_cmd[1]=4;							  //左摇杆数据处理（上下）4档 
		else if(yaogan[1]<0.365f&& yaogan[1]>=0)  syma_cmd[1]=3;								//左摇杆数据处理（上下）3档 
		else if(yaogan[1]<0.6f&& yaogan[1]>=0.365f)  syma_cmd[1]=2;							//左摇杆数据处理（上下）2档 
		else if(yaogan[1]<0.9f&& yaogan[1]>=0.6f)  syma_cmd[1]=1;              //左摇杆数据处理（上下）1档 
		else if(yaogan[1]>=0.9f)  syma_cmd[1]=0;																  //左摇杆数据处理（上下）0档 	
		
//		if(yaogan[1]<-0.7)   syma_cmd[1]=9 ;                            			  //左摇杆数据处理（上下）9档  
//		else if(yaogan[1]<-0.5&& yaogan[1]>=-0.7)  syma_cmd[1]=8;							  //左摇杆数据处理（上下）8档 
//		else if(yaogan[1]<-0.3&& yaogan[1]>=-0.5)  syma_cmd[1]=7;								//左摇杆数据处理（上下）7档 
//		else if(yaogan[1]<-0.15&& yaogan[1]>=-0.3)  syma_cmd[1]=6;							//左摇杆数据处理（上下）6档 
//		else if(yaogan[1]<0.1&& yaogan[1]>=-0.15)  syma_cmd[1]=5;              //左摇杆数据处理（上下）8档 
//		else if(yaogan[1]<0.3&& yaogan[1]>=0.1)  syma_cmd[1]=4;								//左摇杆数据处理（上下）4档 
//		else if(yaogan[1]<0.5&& yaogan[1]>=0.3)  syma_cmd[1]=3;							//左摇杆数据处理（上下）3档 
//		else if(yaogan[1]<0.7&& yaogan[1]>=0.5)  syma_cmd[1]=2;              //左摇杆数据处理（上下）2档 
//		else if(yaogan[1]<0.85&& yaogan[1]>=0.7)  syma_cmd[1]=1;              //左摇杆数据处理（上下）1档 
//		else if(yaogan[1]>=0.85)  syma_cmd[1]=0;																  //左摇杆数据处理（上下）0档 	
				
	}
	else
	{
		syma_cmd[0]=0;								
		syma_cmd[1]=0;
		syma_cmd[2]=0;
		syma_cmd[3]=0;
	}		
	
	//遥控动作模式判断     //优先级   前进后退>转弯>自转
		if(syma_cmd[0]==0&&syma_cmd[2]==0&&syma_cmd[3]==0)				//当摇杆数据都为0，则说明小车手动无动作
		{
			SymaStatus.RunMode=0;
		}
		else
		{
			if(syma_cmd[3]==1) SymaStatus.RunMode=1;				// 小车前进
			else if(syma_cmd[3]==2) SymaStatus.RunMode=2;  // 小车后退
			else if(syma_cmd[2]==1)	SymaStatus.RunMode=3;  // 小车左转							
			else if(syma_cmd[2]==2) SymaStatus.RunMode=4;  // 小车右转	
			else if(syma_cmd[0]==1||syma_cmd[0]==2) SymaStatus.RunMode=5;  // 小车自转
		
		}
	
	
	return spd/8*syma_cmd[1]; 																							//返回速度值
}



//////////////////////////////////////////////摇杆按钮指令读取并处理�/////////////////////////////////
//说明：摇杆按钮指令读取并处理
////函数名称：void Syma_button_receive(u8 *tmp_buf)
////输入： u8 *tmp_buf：遥控器接受数据  
////输出：u8 * syma_cmd：遥控器指令   
////返回：无
//tmp_buf[]:对应遥控器寄存器详见手册
//////////////////////////////////////////////////////////////////////////////////////////////////
void Syma_button_receive(u8 *tmp_buf,float spd_cle[2])
{
	u8 static Temp_spd_on=0;								//速度值修改标志位
	u8 static Temp_spd_down=0;							//速度值修改标志位
	u8 static Temp_clc_on=0;								//转弯半径值修改标志位
	u8 static Temp_clc_down=0;							//转弯半径值修改标志位
	
	if(tmp_buf[19]==1) AGVStatus.Setup=1;	              //接收到启动信号小车上使能
	if(tmp_buf[17]==1) AGVStatus.Setup=0;								//接收到停止信号，小车断使能	
	
	if(AGVStatus.Setup==1)  OT_SON=1;											//当小车启动，上使能
	else  OT_SON=0;																//非启动，断使能

	if(AGVStatus.Setup==1) AGVStatus.SONSetup=1;      //启动状态置位
	else AGVStatus.SONSetup=0;;								//启动状态复位
	
	if(!AGVStatus.AJMode)									//手动模式下生效
	{
		if(tmp_buf[22]==1&&tmp_buf[22]!=Temp_spd_on)						// 当左摇杆右按钮上为上升沿信号
		{
			if(spd_cle[0]<2.0f)																//当数据小于3.0米/秒，可以累加
			{
				spd_cle[0]=spd_cle[0]+1.0f;		
			}	
			else spd_cle[0]=2.0f;
		}
		if(tmp_buf[23]==1&&tmp_buf[23]!=Temp_spd_down)		// 当左摇杆右按钮下为上升沿信号
		{
			if(spd_cle[0]>1.0)																//当数据大于1.0米/秒  可以累减
			{
				spd_cle[0]=	spd_cle[0]-1.0f;		
			}	
			else spd_cle[0]=1.0f;
		}

		if(tmp_buf[20]==1&&tmp_buf[20]!=Temp_clc_on)						// 当右摇杆左按钮上为上升沿信号
		{
			if(spd_cle[1]<5.0f)																//当数据小5.0米，可以累加
			{
				spd_cle[1]=spd_cle[1]+0.1f;		
			}	
			else spd_cle[1]=5.0f;
		}
		if(tmp_buf[21]==1&&tmp_buf[21]!=Temp_clc_down)		// 当右摇杆左按钮下为上升沿信号
		{
			if(spd_cle[1]>0.5f)																//当数据大于0.5米 可以累减
			{
				spd_cle[1]=	spd_cle[1]-0.1f;		
			}	
			else spd_cle[1]=0.5f;
		}
	}
	
	
	/////////////////////照明灯控制//////////////////////////
	if(tmp_buf[18]==1)	OT_JD2=1;		//照明灯启动
	else OT_JD2=0;									//照明灯关闭
	
	Temp_spd_on=tmp_buf[20];						//数据存储
	Temp_spd_down=tmp_buf[21];					//数据存储
	Temp_clc_on=tmp_buf[26];						//数据存储
	Temp_clc_down=tmp_buf[27];					//数据存储
}
