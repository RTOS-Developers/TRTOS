#ifndef _TOS_TIMED_TIMED_H_
#define _TOS_TIMED_TIMED_H_
#include <Include.h>

#define TIMEDTASKS_FLAG_START				0x40
#define TIMEDTASKS_TIMER_FLAG_RUN				0x80
#define TIMEDTASKS_TIMER_LIFE_FOREVER			0xFFFF


/*---------------------定时器结构----------------------------*/
//任务定时器固定于100ms定时周期运行
//最长定时时间为25.5S
typedef struct Tos_Timed
{
	uint8 ID;								//定时器ID号
	struct Tos_Timed *NextTimer;			//指向下一个定时器
	uint8 Interval;							//定时器时间间隔
	uint8 Lifecycle;						//定时器生命周期 0xFFFF 无限生命
	uint8 Counter;							//计数器,用于周期计数
	uint8 Flags;							//定时器标志 B7/Run B6/sta B[5:0]/task_ID
	Func  Function;							//调用任务
}Tos_Timed;	


uint8 Timed_Create(Tos_Timed *NewTimer,uint16 Interval,uint16 Lifecycle,Func Function);
void Tos_TimedScan(void);
Tos_Timed *Tos_TimedFind(uint8 TimerID);
uint8 Tos_TimedCount(void);
uint8 Tos_TimedAlter(uint8 TimerID,uint16 Interval,uint16 Lifecycle,Func Function);
uint8 Tos_TimedDelete(uint8 TimerID);
void Tos_TimedExecute(void);

#endif																								
