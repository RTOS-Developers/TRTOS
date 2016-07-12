
#ifndef _TOS_TIMER_H_
#define _TOS_TIMER_H_

#include <Include.h>

typedef enum
{
	TTF_START=S0,
	TTF_RUN=S1,
	TTF_FOREVER=0xffff,
}TOS_TIMER_FLAG;

typedef struct 
{
	uint8 Year;
	uint8 Mon;
	uint8 Day;
	uint8 Week;
	uint8 Hour;
	uint8 Min;
	uint8 Sec;
}Date_Time;
typedef struct 
{
	uint8 Year;
	uint8 Mon;
	uint8 Day;
}Date;

typedef struct _Tos_Timer
{
	struct _Tos_Timer *NextTimer;			//指向下一个定时器
	uint16 Interval;							//定时器时间间隔
	uint16 Lifecycle;						//定时器生命周期 0xFFFF 无限生命
	uint16 Counter;							//计数器,用于周期计数
	uint8 Flags;							//定时器标志 B7/Run B6/sta B[5:0]/task_ID
	Func  Function;							//调用任务
}Tos_Timer;	

extern Tos_Timer *Tos_Timer_P;
void Tos_TimerReady(void);
void Tos_InitTimer(void);
extern Tos_Timer* Tos_CreateTimer(uint16 Interval,uint16 Lifecycle,Func Function);
void Tos_ScanTimer(void);
void Tos_ExecuteTimer(void);
BOOL Tos_DeleteTimer(Tos_Timer *Timer);

#endif																								
