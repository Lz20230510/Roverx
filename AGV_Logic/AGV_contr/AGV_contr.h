#ifndef __AGV_CONTR_H__
#define __AGV_CONTR_H__
#include "sys.h"
#include "HEAD.h"
#include "usart3.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define NO PEout(8)			// 	
#define NO1 PEout(9)		// 	
#define OT_SON PEout(10)		// 	  伺服使能输出

#define IN_SEV1 PDin(15)  //伺服1就绪
#define IN_SEV2 PDin(0)  //伺服2就绪
#define IN_SEV3 PDin(1)  //伺服3就绪
#define IN_SEV4 PEin(7)  //伺服4就绪

void SON1234_Init(void);
void Sov_on_1234_Init(void);

void Inside_Auto(void);													//室内自动模式
void Outside_Auto(void);												//室外自动模式
void JogMode_control(void);											//手动控制模式
void TeachMode_control(void);											//手动控制模式
void AGVParamter_Init(void);												//AGV 参数初始化		
void MapDownLoad_control(void);										//电子地图下载处理
#endif
