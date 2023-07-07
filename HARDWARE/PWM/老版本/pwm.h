#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#include "HEAD.h"
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


//电机方向端口定义
#define OT_DIR1 PBout(6)	// 左前方向
#define OT_DIR2 PBout(7)	// 左后方向	 
#define OT_DIR3 PBout(8)	// 右前方向
#define OT_DIR4 PBout(9)	// 右后方向	 







void Time_CH1234_Init(void);
u16 Speed_trans(u16 spped_in);
void TIM4_CH1_PWM_Init(u32 arr,u32 psc);
void TIM4_CH2_PWM_Init(u32 arr,u32 psc);
void TIM4_CH3_PWM_Init(u32 arr,u32 psc);
void TIM4_CH4_PWM_Init(u32 arr,u32 psc);
void DIR_Init(void);
void Speed_contr(float spd_lf2,float spd_lb2,float spd_rf2,float spd_rb2,u8 mode);
void Circle_contr(float speed_in,float R,u8 mode);
void Vehicle_contr_Auto(float speed_in,float R,u8 mode);
void Vehicle_contr_Jog(float speed_in,u8 mode);
void Syma_vehicle_contr(u8 * syma_cmd,u8 * tmd_buf,float spd_Auto,float R,float spd_Jog);
void auto_vehicle_contr(float speed_in,u8 cmd);
//void Speed_set(u8 code_in);
#endif
