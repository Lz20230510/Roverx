#include "delay.h" 
#include "usart.h" 
extern u8 HF_BUFF_long[16]={0xAA,0x20,0x20,0x00,0x08,0x00,0x00,0x00,0x00,0x88,0x13,0x00,0x00,0x00,0x00,0x0D};
extern u8 HF_BUFF[14]={0xAA,0x20,0x20,0x00,0x06,0x00,0x00,0x88,0x13,0x00,0x00,0x00,0x00,0x0D};
extern u8 HF_BUFF_stop[8]={0xaa,0x20,0x20,0x31,0x00,0xe6,0xeb,0x0d};

//*************************实现函数*****************************/
//函数原型：HF_vehicle(u8 * HF_BUFF,u8 command)
//功能：对小车进行速度控制
//输入/输出：u8 * HF_com：航发小车控制数据存储缓冲区,8 command:速度指令
//
//前进：0x01
//后退：0x02
//左圆弧：0x03
//右圆弧：0x04
//原地左转：0x05
//原地右转：0x06
//前进点动：0x07
//后退点动：0x08
//
//
//左圆弧小：0x09 
//左圆弧中：0x0A
//左圆弧较大：0x0B
//左圆弧大：0x0C
//
//右圆弧小：0x0D
//右圆弧中：0x0E
//右圆弧较大：0x10
//右圆弧大：0x11
//
//小车暂停：0x30
//返回：void
//**************************************************************/
void HF_vehicle(u8 * HF_BUFF,u8 command)
{	
	u8 i;
	switch(command)
	{
		case 0x01:				//  连续前进
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1 ,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		
		}break;
		case 0x02:		// 连续后退
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
			
		}break;
		case 0x03:		//左圆弧  角速度为正  左转
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
			
		}break;
		case 0x04:		//右圆弧   角速度为负 右转
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
			
		}break;
		case 0x05:		//前进点动
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;
		case 0x06:		//后退点动
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
			                                                                                                                                                                       
		}break;
		case 0x07:		//左转点动
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
				
		}break;	
		case 0x08:		//右转点动
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
				USART_SendData(USART1, HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
			
		}break;	
		case 0x09:		//					左转105°,角速度10
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
				USART_SendData(USART1,HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;	
		case 0x0A:		//          左转110°，角速度15
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
				USART_SendData(USART1,HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;	
		case 0x0B:		//					逆时针左转70°,半径2m
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
				USART_SendData(USART1,HF_BUFF_long[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;
		case 0x0C:		//         左转115°，角速度25
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
				USART_SendData(USART1,HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;
		case 0x0D:		//					右转75°,角速度-10
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
				USART_SendData(USART1,HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;	
		case 0x0E:		//         右转70°,角速度-15
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
				USART_SendData(USART1,HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;	
		case 0x10:		//					顺时针右转70°,半径2m
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
				USART_SendData(USART1,HF_BUFF_long[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;
		case 0x11:		//         右转65°,角速度-25
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
				USART_SendData(USART1,HF_BUFF[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;
		case 0x30:		//					小车暂停
		{
			HF_BUFF_stop[3]=0x20;
			HF_BUFF_stop[4]=0x31;
			HF_BUFF_stop[5]=0x00;
			HF_BUFF_stop[6]=0xe6;
			HF_BUFF_stop[7]=0xeb;
			HF_BUFF_stop[8]=0x0d;
			
			for(i=0;i<8;i++)
			{										
				USART_SendData(USART1,HF_BUFF_stop[i]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束						
			}
			USART_RX_STA=0;   //RIFD读卡器接收状态初始化
		}break;
		
	}

}



