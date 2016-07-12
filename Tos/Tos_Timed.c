#include <Include.h>
#include <Tos_Timed.h>
Tos_Timed Timed_SystemTimer;


uint8 Timed_Create(Tos_Timed *NewTimer,uint16 Interval,uint16 Lifecycle,Func Function)
{
	uint8 ID=1;
	Tos_Timed *CurTimer=&Timed_SystemTimer;
	if(CurTimer->Flags>0){										//若系统定时器还未初始化
		while(CurTimer->NextTimer){				  				//查找Timer键表尾部的空缺位置
			CurTimer=CurTimer->NextTimer;
			if(ID>0){											//还未找到
				if(CurTimer->ID!=ID){							//查找最小的未用到的ID号
					NewTimer->ID=ID;
					ID=0;
				}else ID++;
			}
		}
		if(ID>0)NewTimer->ID=ID;
		NewTimer->Flags=Tos_TaskCurrent;
		CurTimer->NextTimer=NewTimer;		
	}else{
		NewTimer=CurTimer;
		NewTimer->ID=0;
		NewTimer->Flags=Tos_TaskCurrent;
	}
	NewTimer->Interval=Interval;
	NewTimer->Lifecycle=Lifecycle;
	NewTimer->Counter=0;
	NewTimer->Function=Function;	
	NewTimer->NextTimer=0;	
	return NewTimer->ID;	
}
/*--------------------------------------------------------------------------------------------------------------
 Func: 处理定时器
 Time: 2011-5-26
 Ver.: V1.0
 Note: 扫描定时器链表
--------------------------------------------------------------------------------------------------------------*/
void Tos_TimedScan(void)
{
	Tos_Timed *CurTimer=&Timed_SystemTimer;
	while(CurTimer->NextTimer)
	{
		CurTimer=CurTimer->NextTimer;
		if(CurTimer->Flags&TOS_TIMER_FLAG_START)
		{
			 if(CurTimer->Lifecycle>0){											//扫描生命结束的定时器
				if(CurTimer->Counter<CurTimer->Interval)CurTimer->Counter++;	//是否定时时间到
				else{
					if(CurTimer->Lifecycle<0xFF)CurTimer->Lifecycle--;			//更新生命周期				
					if(CurTimer->Lifecycle>0x00)CurTimer->Counter=0;			//计数器清零,对于0生命清零无意义
					CurTimer->Flags|=TOS_TIMER_FLAG_RUN;						//时间到达,需要执行定时任务
				}
				}
		}
	}
}
/*--------------------------------------------------------------------------------
 Func: 查找定时器
 Time: 2014-5-26
 Ver.: V1.0
 Note: 
--------------------------------------------------------------------------------*/
Tos_Timed *Tos_TimedFind(uint8 TimerID)
{
	Tos_Timed *CurTimer=&Timed_SystemTimer;
	while(CurTimer->NextTimer){				  					//查找Timer键表尾部的空缺位置
		CurTimer=CurTimer->NextTimer;
		if(CurTimer->ID==TimerID){								//已找到目标定时器			
			return CurTimer;
		}
	}
	return 0;
}
/********************************************************************************
 Func: 定时器个数
 Time: 2014-8-3
 Ver.: V1.0
 Note; 
********************************************************************************/
uint8 Tos_TimedCount()
{
	uint8 i=1;//系统定时器一个
	Tos_Timed *CurTimer=&Timed_SystemTimer;
	while(CurTimer->NextTimer)
	{
		CurTimer=CurTimer->NextTimer;
		i++;
	}
	return i;
}
/*--------------------------------------------------------------------------------
 Func: 修改Tos定时器
 Time: 2014-5-26
 Ver.: V1.0
 Note: 从Tos_Timer链表中修改指定的定时器
--------------------------------------------------------------------------------*/
uint8 Tos_TimedAlter(uint8 TimerID,uint16 Interval,uint16 Lifecycle,Func Function)
{
	Tos_Timed *CurTimer=Tos_TimedFind(TimerID);
	if(CurTimer>0){
		CurTimer->Interval=Interval;
		CurTimer->Lifecycle=Lifecycle;
		CurTimer->Counter=0;									//定时器重装
		CurTimer->Function=Function;
		return TimerID;
	}
	return 0;			
}
/*--------------------------------------------------------------------------------
 Func: 删除Tos定时器
 Time: 2014-5-26
 Ver.: V1.0
 Note: 从Tos_Timer链表中删除定时器
--------------------------------------------------------------------------------*/
uint8 Tos_TimedDelete(uint8 TimerID)
{
	Tos_Timed *CurTimer=&Timed_SystemTimer;
	while(CurTimer->NextTimer){				  					//查找Timer键表尾部的空缺位置
		if(CurTimer->NextTimer->ID==TimerID){					//已找到目标定时器
			CurTimer->NextTimer->Lifecycle=0;					//定时器生命结束
			CurTimer->NextTimer=CurTimer->NextTimer->NextTimer;	//将本定时器从链表删除
			return TimerID;
		}
		CurTimer=CurTimer->NextTimer;
	}
	return 0;			
}


/*--------------------------------------------------------------------------------
 Func: 系统定时器任务
 Time: 2014-5-26
 Ver.: V1.0
 Note: 扫描定时器链表中就绪的项
--------------------------------------------------------------------------------*/
void Tos_TimedExecute(void)
{
	Tos_Timed *CurTimer=&Timed_SystemTimer;
	while(CurTimer->NextTimer){
		if(CurTimer->NextTimer->Flags&TOS_TIMER_FLAG_START){
			if(CurTimer->NextTimer->Flags&TOS_TIMER_FLAG_RUN){
				CurTimer->NextTimer->Function();									//执行定时任务
				CurTimer->NextTimer->Flags&=~TOS_TIMER_FLAG_RUN;
				if(CurTimer->NextTimer->Lifecycle==0){
					Tos_TimerDelete(CurTimer->NextTimer->ID);
				}else{
					CurTimer=CurTimer->NextTimer;				
				}
			}else{
				CurTimer=CurTimer->NextTimer;
			}
		}else{
			CurTimer=CurTimer->NextTimer;
		}
	}
}








