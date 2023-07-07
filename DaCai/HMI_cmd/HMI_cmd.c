//#include "Usart.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "stdlib.h"
#include "usart3.h"
#include "HMI_cmd.h"
#include "hmi_driver.h"
#include "delay.h"
#include "vehicle_key.h"
#include "led.h"
#include "RAD.h"
#include "24l01.h"
#include "stmflash.h"



u16 CurrtScreenID=0;								//当前界面号
extern u8 cmd_buffer[CMD_MAX_SIZE];//指令缓存
u16 ScreenID = 0;//当前画面IDS
static long test_value = 0;//测试值

union MultiType multiType3;


/*! 
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, u16 size )
{
	u8 cmd_type = msg->cmd_type;//指令类型
	u8 ctrl_msg = msg->ctrl_msg;   //消息的类型
	u8 control_type = msg->control_type;//控件类型
	u16 screen_id = PTR2U16(&msg->screen_id);//画面ID
	u16 control_id = PTR2U16(&msg->control_id);//控件ID
	u32 value = PTR2U32(msg->param);//数值
	ScreenID=screen_id;
	
	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS: //触摸屏按下
	case NOTIFY_TOUCH_RELEASE://触摸屏松开
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://写FLASH成功
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://写FLASH失败
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://读取FLASH成功
		NotifyReadFlash(1,cmd_buffer+2,size-6);//去除帧头帧尾
		break;
	case NOTIFY_READ_FLASH_FAILD://读取FLASH失败
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://读取RTC时间
		NotifyReadRTC(cmd_buffer[1],cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7]);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//画面ID变化通知
			{
				NotifyScreen(screen_id);		
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //按钮控件
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://文本控件
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //进度条控件
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //滑动条控件
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //仪表控件
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://菜单控件
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://选择控件
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://倒计时控件
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;				
				
	//			void AnimationPlayFrame(u16 screen_id,u16 control_id,u8 frame_id);
				}
			}			
		}
		break;default:
		break;
	}
}


void SetTextValueInt32(u16 screen_id, u16 control_id,long value)
{
	u8 buffer[12] = {0};
	sprintf((char*)buffer,"%ld",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueInt16(u16 screen_id, u16 control_id,short int value)
{
	u8 buffer[12] = {0};
	sprintf((char*)buffer,"%d",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}


void SetTextValueFloat(u16 screen_id, u16 control_id,double value,u8 type)
{
	u8 buffer[20] = {0};
	if(type==_double) sprintf((char*)buffer,"%.10lf",value);//把浮点数转换为字符串(保留10位小数)
	else if(type==_float) sprintf((char*)buffer,"%.2f",value);//把浮点数转换为字符串(保留10位小数)
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueu16(u16 screen_id, u16 control_id,u16 value)
{
	u8 buffer[12] = {0};
	sprintf((char*)buffer,"%u",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}


/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(u16 screen_id)
{
	//TODO: 添加用户代码
	switch(screen_id)

	if(screen_id==1)
	{
			
	}
	else if(screen_id==3)//温度曲线
	{
		
	}
	else if(screen_id==9)//二维码
	{
//		SetTextValue(9,1,dat);
	}
}

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(u8 press,u16 x,u16 y)
{
	//TODO: 添加用户代码
}




void UpdateUI()
{


}


/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(u16 screen_id, u16 control_id, u8  state)
{
	u8 static buttonAdd=0;
	CurrtScreenID=ScreenID_Judge(screen_id,control_id);							//当前页面号获取
	Buff2_Flash(screen_id,control_id);															//批量存储某一页的参数到FLASH
	switch(screen_id)
	{
		case Pg_Start:										//主界面0 		
		{
			switch(control_id)						//按钮控件号
			{
				case 2:										//遥控启动按钮
				{
					DacaiButton.Pg0_JOGSetup=!DacaiButton.Pg0_JOGSetup;
					
				}break;
				case 3:										//室内/外自动按钮
				{
					DacaiButton.Pg0_AUTOmode=!DacaiButton.Pg0_AUTOmode;	
				}break;
				case 4:										//设备使能
				{
					DacaiButton.Pg0_SONSetup=!DacaiButton.Pg0_SONSetup;			//设备使能
					
				}break;
				case 29:									//报警复位
				{
						
				}break;			
				default:break;			
			}	
		}break;
		
		case 0x03:												//轨迹预览3		
		{
			switch(control_id)					//按钮控件号
			{
				case 2:										//轨迹预览按钮
				{
				
				}break;
				default:break;			
			}				
		}break;
		
		case 0x04:												//轨迹编辑4	
		{
			switch(control_id)					//按钮控件号
			{
				case 2:										//允许下载按钮
				{
					DacaiButton.Pg4_MapDown=!DacaiButton.Pg4_MapDown;					//		
					if(AGVStatus.AJMode!=0) DacaiButton.Pg4_MapDown=0;				//非手动模式按钮失效
				}break;	
				case 3:										//地图异常点修复
				{
					MapTable_repair((u16)Paramt_BUFF[10][10],2000.0);					//异常点修复
				}break;	
				case 4:										//启动示教按钮
				{
					DacaiButton.Pg4_TeachMode=!DacaiButton.Pg4_TeachMode;			//手动模式下，可以启动或者关闭示教模式
					if(AGVStatus.AJMode!=0) DacaiButton.Pg4_TeachMode=0;				//非手动模式按钮失效
					
				}break;
				case 5:										//电子地图存储数据总清
				{
					SetScreen(25);														//初始化完成，切换画面初始化
					STMFLASH_Clear(MAPOA_Addr,1);								//OA存储区总清
					STMFLASH_Clear(MAPOB_Addr,1);								//OB存储区总清
					STMFLASH_Clear(MAPAngle_Addr,1);								//angle存储区总清
					SetScreen(4);														//人机界面初始化完成，切换回参数运行3
				}break;
				default:break;	
				
			}				
		}break;
		case 10:			//室外参数设置界面
		{
			switch(control_id)			//按钮控件号
			{
				case 20:							//寻迹反向按钮
				{
					
					if((Paramt_BUFF[screen_id][13]==0xFF||Paramt_BUFF[screen_id][13]==0x00)&&buttonAdd==0)				
					{
						Paramt_BUFF[screen_id][13]=0x01;						//此时参数勾选
						AnimationPlayFrame(screen_id,13,1);      	 //控件显示					
					}						
					else if(Paramt_BUFF[screen_id][13]==0x01&&buttonAdd==0)																	
					{
						Paramt_BUFF[screen_id][13]=0x00;						//此时参数不勾选
						AnimationPlayFrame(screen_id,13,0);      	 //控件不显示*/
					}	
					buttonAdd++;
					if(buttonAdd>1) buttonAdd=0;
				}
				default:break;
			}				
			
		}break;
		case 0x11:						//常规参数设置界面
		{
			switch(control_id)					//按钮控件号
			{
				case 2:										//参数总清按钮
				{
					STMFLASH_Clear(AGVPa_Addr,0);								//参数总清
					SetScreen(25);														//初始化完成，切换画面初始化
					AllPg_Refresh(textFormat,textFormat,textFormat);//人机界面初始化
					SetScreen(17);														//人机界面初始化完成，切换回参数运行3
				}break;	
				
				case 4:						//关闭避障传感器
				{
					DacaiButton.Pg17_ultral=!DacaiButton.Pg17_ultral;						//避障传感器启动关闭
					if(DacaiButton.Pg17_ultral)		Paramt_BUFF[17][5]=!Paramt_BUFF[17][5];						//避障模式缓冲区取反//当按下按钮为1时	
					Ultrosonic.deviceEn=Paramt_BUFF[17][5];					//避障状态赋值
				}break;	
			}						
		}	
		default:break;
	}	
}

/*! 
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(u16 screen_id, u16 control_id, u8 *str)
{
//	union MultiType multiType1;
	//TODO: 添加用户代码
			if(control_id==61 && screen_id==0 )
			{
				Dacai_InOut(screen_id,control_id,str,multiType3);						//大彩屏的文本框功能参数实时输入输出
				BranchPath.selectPathNum=Paramt_BUFF[0][61];								//获取当前路径设置值
			}				
			if(CurrtScreenID!=0x00) Dacai_InOut(screen_id,control_id,str,multiType3);						//大彩屏的文本框功能参数实时输入输出	
}

/*! 
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(u16 screen_id, u16 control_id, u32 value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(u16 screen_id, u16 control_id, u32 value)
{
	//TODO: 添加用户代码
//	if(screen_id==5&&control_id==2)//滑块控制
//	{
//		test_value = value;

//		SetProgressValue(5,1,test_value); //更新进度条数值
//	}
}

/*! 
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(u16 screen_id, u16 control_id, u32 value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(u16 screen_id, u16 control_id, u8  item)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(u16 screen_id, u16 control_id)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(u8 status,u8 *_data,u16 length)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(u8 status)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取RTC时间，注意返回的是BCD码
 *  \param year 年（BCD）
 *  \param month 月（BCD）
 *  \param week 星期（BCD）
 *  \param day 日（BCD）
 *  \param hour 时（BCD）
 *  \param minute 分（BCD）
 *  \param second 秒（BCD）
 */
void NotifyReadRTC(u8 year,u8 month,u8 week,u8 day,u8 hour,u8 minute,u8 second)
{
}
