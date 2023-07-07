#ifndef __HMI_CMD_H
#define __HMI_CMD_H
#include "HEAD.h"
#include "hmi_driver.h"

							//当前界面号
//extern u8 cmd_buffer[CMD_MAX_SIZE];//指令缓存
extern u16 ScreenID;//当前画面IDS
extern u16 CurrtScreenID;	

static long test_value;//测试值
static u8 update_en;//更新标记
extern uint16_t  motor_speed;

//参数监控1区  数据存储  
extern double lf_spd;                       //左前轮转速				参数监控1
extern double	lb_spd;												//左后轮转速				参数监控1
extern double	rf_spd;												//右前轮转速				参数监控1
extern double	rb_spd;												//右后轮转速				参数监控1

extern u8	BD_num;												//北斗星数							参数监控1
extern u8	GPS_num;											//GPS星数								参数监控1	
extern double	longti;												//经度							参数监控1
extern double	lati;													//维度							参数监控1
extern double	OA_x;														//当前东北天坐标系纵坐标值	参数监控1
extern double	OB_y;														//当前东北天坐标系横坐标值	参数监控1
extern double	Angle;												//小车当前导航角				参数监控1


//参数设置1区  数据存储          
extern double Auto_spd;										//自动默认速度  		参数输入1
extern double Jog_spd;										//手动默认速度			参数输入1
extern double Jog_spdlmt;									//手动速度限制			参数输入1
extern double Auto_spdlmt;									//自动速度限制		参数输入1
extern double Jog_cirR;           //手动圆弧半径							参数输入1
extern double ultro_dis;          //超声波避障设置距离				参数输入1



typedef	struct
{
	u8 Pg0_JOGSetup;							//主界面  遥控启动按钮
	u8 Pg0_AUTOmode;								//主界面  手自动模式按钮
	u8 Pg0_SONSetup;							//主界面  设备使能按钮
	u8 Pg0_Rest;									//主界面 报警复位按钮
	u8 Pg4_TeachMode;							//轨迹编辑界面  示教模式切换按钮
	u8 Pg4_MapDown;							//轨迹编辑界面  地图下载按钮
	u8 Pg4_MapClean;							//轨迹编辑界面  地图总清按钮
	u8 Pg17_ultral;								//避障传感器启动关闭
}DacaiButtonDef;
extern DacaiButtonDef DacaiButton;





void SetTextValueFloat(u16 screen_id, u16 control_id,double value,u8 type);
void SetTextValueu16(u16 screen_id, u16 control_id,u16 value);



#endif
