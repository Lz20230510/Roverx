#include "canfestival.h"
#include "sys.h"
#include "cancan.h"
#include "can.h"
/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
TIMEVAL last_time_set = TIMEVAL_MAX;

unsigned int TimeCNT=0;             //时间计数
unsigned int NextTime=0;            //下一次触发时间计数
unsigned int TIMER_MAX_COUNT=70000; //最大的时间计数


 
UNS8 canSend(CAN_PORT notused, Message *m)
{ 
    return can_send_msg((Message *) m);   //发送CAN消息
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
    return 0;
}

void setTimer(TIMEVAL value)
{ 
    NextTime = (TimeCNT+value)%TIMER_MAX_COUNT;
}

TIMEVAL getElapsedTime(void)
{
    int ret=0;
    
    ret = TimeCNT>= last_time_set ? TimeCNT - last_time_set : TimeCNT + TIMER_MAX_COUNT - last_time_set;

    return ret;
}


 


