
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

void TIM7_Int_Init(u16 arr,u16 psc);

extern u8 a2[44]; 
extern float a1[3],w1[3],h1[3],Angle1[3];


void TIM6_Int_Init(u16 arr,u16 psc);
u8 Time7_Timekeep(u8 time,u8 timekeep,u8 start,u8 Num,u8 reset);																//计算当前经过的时间
