#ifndef __I_O_H
#define __I_O_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


//输入端口的宏定义
#define IA1		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1) //PG1         PE0-PG1
#define IA2		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7) //PE7         PE1-PE7
#define IA3		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8) //PE8         PE2-PE8
#define IA4		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10) //PE10	      PE3-PE10
#define IA5		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12) //PE12       PE4-PE12
#define IA6		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15) //PE15       PE5-PE15
#define IA7		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) //PB10       PE6-PB10   
#define IA8		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) //PB11       PE7-PB11

//#define IB1		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)  //PD11       PE8-PD11
//#define IB2		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) //PD13        PE10-PD13
//#define IB3		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) //PD14        PE12-PD14
//#define IB4 	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15) //PD15	       PE15-PD15
//#define IB5		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_2)  //PG2         PF0-PG2
//#define IB6		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3)  //PG3         PF1-PG3
//#define IB7		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_4)  //PG4         PF2-PG4
//#define IB8		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5)  //PG5         PF3-PG5

#define IC1		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)  //PC3         PF4-PC3
#define IC2		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)  //PC2         PF5-PC2
#define IC3		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)  //PC0         PG1-PC0
#define IC4		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)  //PB0	       PG2-PB0
#define IC5		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)  //PB1         PG3-PB1
#define IC6		GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0)  //PG0         PG4-PG0
#define IC7 	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_15) //PF15          PF10-PF15           
#define IC8		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12) //PD12          PF11-PD12           
 
//#define ID1		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)  //PE2          PB11-PE2
//#define ID2		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)  //PE3         PC0-PE3
//#define ID3		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)  //PE4         PC2-PE4
//#define ID4 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)  //PE5	       PC3-PE5
//#define ID5		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)  //PE1         PD4-PE1 
//#define ID6		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)  //PE0          PD10-PE0 
//#define ID7		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)  //PE6         PD15-PE6    
//#define ID8		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)  //PF0         PG5-PF0


//输出端口的宏定义
#define QE1  	PFout(1)		// PF1              PF12-PF1              
#define QE2  	PFout(2)		// PF2	            PG9-PF2
#define QE3  	PFout(3)		// PF3              PG10-PF3
#define QE4  	PFout(4)		// PF4              PG12-PF4
#define QE5 	PFout(5)		// PF5              PG15-PF5
#define QE6 	PFout(6)	 	// PF6	            PD0-PF6
#define QE7  	PFout(8)		// PF8	            PD1-PF8
#define QE8  	PFout(7)	 	// PF7              PD3-PF7

//#define QE9  	  PFout(10)	 	// PF10          PD13-PF10 
//#define QE10  	PFout(9)	 	// PF9           PD14-PF9
//#define QE11  	PAout(15)	 	// PA15          PD15-PA15
//#define QE12  	PAout(8)	 	// PA8           PF6-PA8

 
#define QF1  	PGout(15)		// PG15            PF15-PG15
#define QF2  	PGout(12)		// PG12	           PG0-PG12
#define QF3 	PGout(10)		// PG10            PA6-PG10          
#define QF4  	PGout(9)		// PG9             PA8-PG9
#define QF5  	PDout(4)		// PD4             PA15-PD4
#define QF6  	PDout(3)	 	// PD3	           PB0-PD3
#define QF7  	PDout(1)		// PD1	           PB1-PD1
#define QF8  	PDout(0)		// PD0             PB10-PD0


//#define QF9  	PGout()	         
//#define QF10  PGout()		           
//#define QF11 	PGout()		                    
//#define QF12  PGout()		      








//继电器输出定义
//#define JD1  	PDout(12)	 	// PD12	
//#define JD2  	PDout(13)		// PD13	
//#define JD3  	PDout(14)		// PD14

#define LED1 PFout(11)	// DS1 核心板
#define LED2 PFout(12)	// DS3	 

//离散量GPIO结构体
typedef struct
{
	u8 IA[9];
	u8 IB[9];
	u8 IC[9];
	u8 ID[9];	
	u8 QE[9];
	u8 QF[9];
	u8 JD[4];
}IOStatusDef;

extern IOStatusDef IOStatus;

void GPIO_OUT_Init(void);   //输出初始化
void GPIO_IN_Init(void);			//输入初始化
void GPIO_INOUT_Scan(void);				//离散量GPIO输入输出
#endif
