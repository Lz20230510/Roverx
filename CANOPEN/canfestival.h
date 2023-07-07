

#ifndef __CANFESTIVAL_H__
#define __CANFESTIVAL_H__
 
#include <applicfg.h>
#include "timer.h"
 
extern void initTimer(void);
extern UNS8 canSend(CAN_PORT notused, Message *m);
extern UNS8 canChangeBaudRate(CAN_PORT port, char* baud);
extern void setTimer(TIMEVAL value);
extern TIMEVAL getElapsedTime(void);

/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
extern TIMEVAL last_time_set;

extern unsigned int TimeCNT;             //ʱ�����
extern unsigned int NextTime;            //��һ�δ���ʱ�����
extern unsigned int TIMER_MAX_COUNT; //����ʱ�����
extern TIMEVAL last_time_set;
#endif

