#ifndef __RAD_H
#define __RAD_H
#include "sys.h" 
//#include "led.h" 
//#include "key.h" 
#include "HEAD.h" 


#define UltalEn PEout(9)				//超声波设备使能输出


typedef struct
{
	u8 channel[8];						//分别存储8个通道的数据 		//单位CM
	u8 deviceEn;							//超声波壁障使能   0：关闭超声波   1：打开超声波
	u8 safeDistance[8];				//8个通道允许的安全距离			//单位CM
	u8 error;          				//超声波数据接收错误        
}UltrosonicDef;
extern UltrosonicDef Ultrosonic;

void ultrosonic_GPIO_Init(void);					
UltrosonicDef ultrosonic_Read(u8 * buff);			//
u8 ultrosonic_check(u8 * buff);				//校验
void ultrosonic_process(void);
UltrosonicDef ultrosonic_Read(u8 * buff);
u8 ultrosonic_check(u8 * buff);
#endif

