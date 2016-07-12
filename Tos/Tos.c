#include <Include.h>
#include <Board_Driver.h>
#include <Text.h>
#include <Tos.h>
#include <DeBug.h>
#include <Tos_Mema.h>
uint32 guiNextTaskSp;              /* 将要运行的任务堆栈指针 */
uint32* gpuiCurTaskSpAddr;         /* 存放当前运行任务堆栈指针的地址 */
_Tos_CPUInfor Tos_CPUInfor;				 /* 存放OS运行信息 */
uint8  Tos_TaskCurrent;
uint32 Tos_TaskMail;
Tos_Tab *Tos_TaskTcb;
uint32 Tos_TaskStackIndex;
/********************************************************************************
Func:初始化任务表
Date:2014-6-10
Note:堆栈是倒增长
********************************************************************************/
uint32 Tos_TaskTcbInit(uint8* pucTaskStack, uint32 uiStackSize)
{
    uint8* pucStackBy4;
    pucStackBy4 = (uint8*)(((uint32)pucTaskStack + uiStackSize) & 0xFFFFFFFC);
    return(((uint32)pucStackBy4 - sizeof(Tos_Tab)) & STACKALIGNMASK);
}
/********************************************************************************
Func:初始化任务栈
Date:2014-6-10
Note:
********************************************************************************/
uint32 Tos_TaskStackInit(uint32 pstrTcb, Tos_Task vfFuncPointer,uintbus Tags)
{
    uint32* puiSp;
    puiSp = (uint32*)pstrTcb; /* 获取存放变量的堆栈指针 */
    *(--puiSp) = (uint32)vfFuncPointer;    /* R14 */
    *(--puiSp) = pstrTcb;  /* R13 */
    *(--puiSp) = 0;                     /* R12 */
    *(--puiSp) = 0;                     /* R11 */
    *(--puiSp) = 0;                     /* R10 */
    *(--puiSp) = 0;                     /* R9 */
    *(--puiSp) = 0;                     /* R8 */
    *(--puiSp) = 0;                     /* R7 */
    *(--puiSp) = 0;                     /* R6 */
    *(--puiSp) = 0;                     /* R5 */
    *(--puiSp) = 0;                     /* R4 */
    *(--puiSp) = 0;                     /* R3 */
    *(--puiSp) = 0;                     /* R2 */
    *(--puiSp) = 0;                     /* R1 */
    *(--puiSp) =Tags;                   /* R0 */
    *(--puiSp) = MODE_USR;              /* XPSR */
    return (uint32)puiSp;
}

/********************************************************************************
Func:创建任务
Date:2014-6-10
Note:使用默认堆栈
********************************************************************************/
void Tos_TaskCreate(uint8 Id,void(*vfFuncPointer)(void *Tags),uintbus Tags,char *TaskName,uint32 StackSize)
{
	uint32 pstrTcb;
	uint8 *P;
	if(NullFunc == vfFuncPointer)return;
	if(Tos_TaskTcb[Id].Task_ST!=_TTS_Deleted)return;
	if(Tos_TaskTcb[Id].TaskStackSize==0)
	{
		P=Mema_Malloc(StackSize);
		Tos_TaskTcb[Id].TaskStackSize=StackSize;
		Tos_TaskTcb[Id].Task_SA=(uint32)P;
	}	
	pstrTcb = Tos_TaskTcbInit((uint8 *)Tos_TaskTcb[Id].Task_SA, Tos_TaskTcb[Id].TaskStackSize);//内存对齐等
	Tos_TaskTcb[Id].Task_ST=_TTS_Run;
	Tos_TaskTcb[Id].vfFunc=vfFuncPointer;
	Tos_TaskTcb[Id].Task_Name=TaskName;
	Tos_TaskTcb[Id].TaskCurSp=Tos_TaskStackInit(pstrTcb, vfFuncPointer,Tags);//堆栈初始化寄存器
}
/********************************************************************************
Func:搜索空位新建任务
Date:2014-11-3
Note:
********************************************************************************/
BOOL Tos_TaskCreateFindID(void(*vfFuncPointer)(void *Tags),uintbus Tags,char *TaskName,uint32 StackSize)
{
	uint16 i=0;
	while(1)
	{
		if(Tos_TaskTcb[i].Task_ST==Null)break;
		if(Tos_TaskTcb[i].Task_ST==_TTS_Deleted)break;
		if(++i>=Tos_TaskTotal)return False;
	}
	Tos_TaskCreate(i,vfFuncPointer,Tags,TaskName,StackSize);
	return True;
}
/********************************************************************************
Func:按名索号
Date:2014-11-3
Note:
********************************************************************************/
uint16 Tos_GetTaskIDByName(uint8 *Name)
{
	uint16 i=0;
	while(1)
	{
		if(CompareWithString(Name,(uint8 *)Tos_TaskTcb[i].Task_Name))return i;
		if(++i>=Tos_TaskTotal)return Tos_TaskNull;
	}
}
/********************************************************************************
Func:任务列表初始化
Date:2015-9-10
Note:
********************************************************************************/
void Tos_ListCreate(const TaskInitList *TIL)
{
	Tos_Init();
	while(TIL->vfFuncPointer)
	{
		Tos_TaskCreateFindID(TIL->vfFuncPointer,(uintbus)TIL->Tags,TIL->TaskName,TIL->StackSize);
		TIL++;
	}
	Tos_Start();
}

/********************************************************************************
Func:运行任务
Date:2014-6-10
Note:使用默认堆栈
********************************************************************************/
void Tos_TaskRun(uint8 Id)
{
	  if(Tos_TaskCurrent==Id)return;
    gpuiCurTaskSpAddr = &Tos_TaskTcb[Tos_TaskCurrent].TaskCurSp;
	  Tos_TaskCurrent=Id;
    guiNextTaskSp =Tos_TaskTcb[Tos_TaskCurrent].TaskCurSp;
    TOS_ContextSwitch();
}

/********************************************************************************
 Func: 任务切换
 Time: 2014-1-31
 Ver.: V1.0
 Note: 本方法用于完成任务切换
********************************************************************************/
void Tos_TaskSwitch(void)
{
	 uint8 i;
	Tos_TaskTcb[Tos_TaskTotal-1].Task_ST=_TTS_Run;//系统核心任务视为空闲任务
	for(i=0;i<Tos_TaskTotal;i++)
	{
		switch(Tos_TaskTcb[i].Task_ST)
		{
			case _TTS_Run:
				   if(Tos_TaskCurrent!=i)Tos_TaskRun(i);	 
					 return; 
			case _TTS_WaitDevice:
					if(Tos_Device_Tab[Tos_TaskTcb[i].Task_WDID].DeviceState==NotBusy)
						{
							Tos_TaskTcb[i].Task_ST=_TTS_DelayWait;
							Tos_Device_Tab[Tos_TaskTcb[i].Task_WDID].DeviceState=Ready;
						}
					 break;
			default:break;
		}
	}
}

/********************************************************************************
 Func: 初
始化Tos
 Time: 2014-1-31
 Ver.: V1.0
 Note; 用于初始化系统变量
********************************************************************************/
void Tos_Init(void)
{
	uint8 i;
	Tos_TaskTcb=Mema_Malloc(sizeof(Tos_Tab)*Tos_TaskTotal);
	Tos_CPUInfor.RunStatus=_TTS_Run;
	Tos_CPUInfor.Use_Ram=0;
	for(i=0;i<Tos_TaskTotal;i++)
	{
		Tos_TaskTcb[i].Task_ST=_TTS_Deleted;
		Tos_TaskTcb[i].Task_TQ=0;			//任务超过该时间直接挂起
		Tos_TaskTcb[i].TaskCurSp=Null;
		Tos_TaskTcb[i].Task_SA=Null;
		Tos_TaskTcb[i].TaskStackSize=0;
	}
	Tos_TaskCurrent=Tos_TaskTotal-1; 
	Tos_TaskStackIndex=0;//任务堆栈控件使用指针
	Tos_Timer_P=0;
}
/********************************************************************************
 Func: 核心任务也可认为是空闲任务
 Time: 2014-6-13
 Ver.: V4.0
 Note;
********************************************************************************/
void Tos_RootTask(void *Tags)
{
	
	Device_Init();//设备初始化放在系统未启动时会导致部分驱动异常，如fmsc
#ifdef IWDG_ENABLE
	IWDG_Configuration();
#endif
#ifdef APPLIC_ENABLE
	if(!AppLicense())return;//注册设备
#endif
	while(1)
	{
#ifdef IWDG_ENABLE
		IWDG_ReloadCounter();//系统狗/使能休眠功能后需要关闭系统狗
#endif
		Tos_TaskDelay(100);
		Tos_ScanTimer();
	}
}
/********************************************************************************
 Func: 任务启动
 Time: 2014-6-13
 Ver.: V4.0
 Note;
********************************************************************************/
void Tos_TaskStart(uint8 Id)
{
    guiNextTaskSp = Tos_TaskTcb[Id].TaskCurSp;
    TOS_SwitchToTask();
}

/********************************************************************************
 Func: 启动tos操作系统
 Time: 2013-12-13
 Ver.: V4.0
 Note;创建空闲任务设置节拍周期启动第一个任务
********************************************************************************/
void Tos_Start(void)
{	
	SysTick_Config(Tos_Cpu_Freq/1000);
	Tos_TaskCreate(Tos_TaskTotal-1,Tos_RootTask,Null,"TaskRoot",Tos_RootTaskStackSize);
  Tos_TaskStart(Tos_TaskTotal-1);

}



/********************************************************************************
 Func: 获取任务当前状态
 Time: 2014-1-31
 Ver.: V1.0
 Note; 本方法返回任务的当前状态
********************************************************************************/
uint16 Tos_TaskGetState(uint8 Index)
{
	if(Index>=Tos_TaskTotal)return 0;
	return Tos_TaskTcb[Index].Task_ST;	
}

/********************************************************************************
 Func: 系统延时调用
 Time: 2014-1-31
 Ver.: V1.0
 Note; 本方法处理任务延迟执行			RT: 高字节/压栈长度 低字节/R4在栈中的位置
********************************************************************************/
void Tos_TaskDelay(uint32 DelayTime)
{
	if(Tos_CPUInfor.RunStatus!=_TTS_Deleted)//系统运行时调用的是系统延时
	{
	Tos_TaskTcb[Tos_TaskCurrent].Task_TQ=0;//TOS系统看门狗复位
	Tos_TaskTcb[Tos_TaskCurrent].Task_TD=DelayTime;																 
	Tos_TaskTcb[Tos_TaskCurrent].Task_ST=_TTS_DelayWait;
	Tos_TaskSwitch();		
	}else BareDelay(DelayTime);//在系统没有运行之前，调用软件延时
}
/********************************************************************************
 Func: 系统延时调用
 Time: 2014-1-31
 Ver.: V1.0
 Note; 延时时间紧缺
********************************************************************************/
void Tos_TaskDelayHMSM(uint8 Hour,uint8 Min,uint8 Sec,uint16 MSec)
{
	uint32 DelayTime;
	DelayTime=Hour;
	DelayTime*=60;
	DelayTime+=Min;
	DelayTime*=60;
	DelayTime+=Sec;
	DelayTime*=1000;
	DelayTime+=MSec;
	Tos_TaskDelay(DelayTime);
}
/********************************************************************************
 Func: 任务超时唤醒功能
 Time: 2009-12-28
 Ver.: V1.0
 Note: 本调用为目标任务设置一个超时时间,目标任务在指定时间后将强制进入运行态
********************************************************************************/
void Tos_TaskTimeout(uint8 Index,uint32 Timeout)
{
	if(Index>=Tos_TaskTotal)return;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Stop)return;//停止的任务无法挂起
	Tos_TaskTcb[Index].Task_TQ=0;//TOS系统看门狗复位
	Tos_TaskTcb[Index].Task_ST=_TTS_DelayWait;
	Tos_TaskTcb[Index].Task_TD=Timeout;
}

/********************************************************************************
 Func: 任务超时设置
 Time: 2009-12-31
 Ver.: V1.0
 Note: 本调用将启动任务超时处理,任何任务在指定的超时时间之后将由系统强制唤醒
 	   超时时间按系统节拍计,当为0时表示无超时
	   从超时时间设定起立即生效,超时时间最高可精确到一个系统节拍
	   一次超时设定只能管理当前时间段,本次到运行态后超时将取消,中途若到运行态，则超时结束
********************************************************************************/
void Tos_SetTimeout(uint32 Timeout)
{
	Tos_TaskTimeout(Tos_TaskCurrent,Timeout);
}
/**************************************************************************************
 Func: 等待设备
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Tos_WaitDev(uint8 DevID)
{
	Tos_TaskTcb[Tos_TaskCurrent].Task_WDID=DevID;
	Tos_TaskTcb[Tos_TaskCurrent].Task_ST=_TTS_WaitDevice;
	Tos_TaskSwitch();
}
/********************************************************************************
 Func: 任务由有生命的不可执行态转入运行状态
 Time: 2014-1-31
 Ver.: V1.0
 Note;
********************************************************************************/
void Tos_TaskRecover(uint8 Index)
{
	if(Index>=Tos_TaskTotal)return;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Deleted)return;//删除的任务无法挂起
	Tos_TaskTcb[Index].Task_ST=_TTS_DelayWait;
}
/********************************************************************************
 Func: 任务挂起等待中断或其他任务唤醒
 Time: 2014-11-09
 Ver.: V1.0
 Note;禁用在中断内
********************************************************************************/
BOOL Tos_TaskSuspend(uint8 Index)
{
	if(Index>=Tos_TaskTotal)return False;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Deleted)return False;//删除的任务无法挂起
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Stop)return False;//停止的任务无法挂起
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Suspend)return False;//无法重复挂起
	Tos_TaskTcb[Index].Task_ST=_TTS_Suspend; 
	Tos_TaskTcb[Index].Task_TD=0; 
	Tos_TaskSwitch();
	return True;
}
/********************************************************************************
 Func: 任务从停止状态恢复
 Time: 2014-1-31
 Ver.: V4.0  
 Note: 本方法产生主动任务切换//特别要申明的是在中断里不能调用
********************************************************************************/
BOOL Tos_TaskResume(uint8 Index)
{	
	if(Index>=Tos_TaskTotal)return False;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Deleted)return False;//删除的任务无法恢复
	if(Tos_TaskTcb[Index].Task_ST==_TTS_DelayWait)return False;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Run)return False;
	Tos_TaskTcb[Index].Task_ST=_TTS_Run; 
	Tos_TaskSwitch();
	return True;
}
/********************************************************************************
 Func: 任务进入停止状态
 Time: 2014-1-31
 Ver.: V1.0
 Note: 本方法产生主动任务切换
********************************************************************************/
void Tos_TaskStop(uint8 Index)
{	
	if(Index>=Tos_TaskTotal)return;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Deleted)return;//删除的任务无法停止
	Tos_TaskTcb[Index].Task_ST=_TTS_Stop; 
	Tos_TaskTcb[Index].Task_TD=0; 
	Tos_TaskSwitch();
}
/********************************************************************************
 Func: 系统任务删除
 Time: 2009-12-12
 Ver.: V4.0
 Note; 本方法使优先级高的任务能将当前优先级低于它的任务删除
********************************************************************************/
BOOL Tos_TaskDelete(uint8 Index) 
{
	if(Index>=Tos_TaskTotal)return False;
	if(Tos_TaskTcb[Index].Task_ST==_TTS_Deleted)return False;
	Tos_TaskTcb[Index].Task_Name=Null;			
	Tos_TaskTcb[Index].Task_TQ=0;								 
	Tos_TaskTcb[Index].Task_TD=0;								 		
	Tos_TaskTcb[Index].Task_ST=_TTS_Deleted; 										//更新任务状态	
	Mema_Free((uint8 *)Tos_TaskTcb[Index].Task_SA);
	Tos_TaskTcb[Index].TaskStackSize=0;
	Tos_TaskSwitch();													//切换任务
	return True;
}
/********************************************************************************
 Func: 系统节拍定时器处理
 Time: 2009-12-12
 Ver.: V4.0
 Note:就绪任务排队
********************************************************************************/
void Tos_TaskReady()
{
	uint8 i;
	for(i=0;i<Tos_TaskTotal;i++)
	{
		switch(Tos_TaskTcb[i].Task_ST)
		{
			case _TTS_Run:
				   Tos_TaskTcb[i].Task_TQ++;//当任务处于运行态却又没有正常运行或运行超时即进入系统复位
						if(Tos_TaskTcb[i].Task_TQ>Tos_TaskWatchDog)Tos_CPUInfor.RunStatus=_TTS_Error;
					 break; //之前用Return，换成break后CPU使用率下降4%
			case _TTS_DelayWait:
					 Tos_TaskTcb[i].Task_TQ=0;
					 if(Tos_TaskTcb[i].Task_TD==0)Tos_TaskTcb[i].Task_ST=_TTS_Run;
			     else Tos_TaskTcb[i].Task_TD--;
					 break;
			default:break;
		}
	}
}
/********************************************************************************
 Func: 计算CPU的使用率
 Time: 20014-5-25
 Ver.: V1.0
 Note; 可以进行修改获取单个任务的CPU使用率
********************************************************************************/
void Tos_ComputeCPURate()
{
	static uint8 Count,i;
	static uint32 Pool;
	static uint8 Use;
	if(Tos_TaskTotal-1!=Tos_TaskCurrent)Use++;//计算单个任务CPU使用率直接填任务id
	Count++;
	if(Count>100){Count=0;Tos_CPUInfor.Userate_CPU=FlowPoolFilter(&Pool,Use,&i,20);Use=0;}
}
/********************************************************************************
 Func: 系统节拍处理
 Time: 20014-2-2
 Ver.: V4.0
 Note; 本方法处理系统节拍中断
********************************************************************************/
void Tos_SysTick()
 {
	  Tos_ComputeCPURate();//定时器扫描
		Tos_TaskReady();//任务就绪
	 	if(Tos_CPUInfor.RunStatus==_TTS_Error){DeBug( "Task TimeOut Will TOS Reset" ,Infor_Infor);MCU_Reset();}//系统有任务运行超时自动复位
		if(Tos_Timer_P)Tos_TimerReady();//节约一点是一点
 }
 

/********************************************************************************
 Func: 获取消息
 Time: 2014-1-31
 Ver.: V1.0
 Note: 接收其他任务发送过来的消息
********************************************************************************/
Tos_ArrayMessage *Tos_TaskGetQMsg(void) 
{
	Tos_ArrayMessage *_CurMsg;	
	Tos_Tab *Tos_Addr;
	Tos_Addr=&Tos_TaskTcb[Tos_TaskCurrent];
	if(Tos_Addr->Task_QM==0)return 0;	
	if(Tos_Addr->Task_QE==0)Tos_TaskStop(Tos_TaskCurrent);		
	if(Tos_Addr->Task_QE==0)return 0;	
	_CurMsg = Tos_Addr->Task_QM + Tos_Addr->Task_QS;
	Tos_Addr->Task_QS=(Tos_Addr->Task_QS+1)%Tos_Addr->Task_QL;
	Tos_Addr->Task_QE--;	
	return _CurMsg;
}
/********************************************************************************
 Func: 获取消息
 Time: 2014-1-31
 Ver.: V1.0
 Note: 非阻塞式接收消息
********************************************************************************/
Tos_ArrayMessage *Tos_TaskGetQMsgDelay(uint16 D) 
{
	Tos_ArrayMessage *_CurMsg;	
	Tos_Tab *Tos_Addr;
	Tos_Addr=&Tos_TaskTcb[Tos_TaskCurrent];
	if(Tos_Addr->Task_QM==0)return 0;	
	if(Tos_Addr->Task_QE==0)Tos_TaskDelay(D);		
	if(Tos_Addr->Task_QE==0)return 0;	
	_CurMsg = Tos_Addr->Task_QM + Tos_Addr->Task_QS;
	Tos_Addr->Task_QS=(Tos_Addr->Task_QS+1)%Tos_Addr->Task_QL;
	Tos_Addr->Task_QE--;	
	return _CurMsg;
}
 /********************************************************************************
 Func: 创建任务消息队列
 Time: 2014-2-4
 Ver.: V1.0
 Note: 
********************************************************************************/
void Tos_TaskCreateQmsg(Tos_ArrayMessage *MsgAddr,uint8 MsgLenth)
{
	Tos_TaskTcb[Tos_TaskCurrent].Task_QM=MsgAddr;
	Tos_TaskTcb[Tos_TaskCurrent].Task_QL=MsgLenth;
	Tos_TaskTcb[Tos_TaskCurrent].Task_QS=0;
	Tos_TaskTcb[Tos_TaskCurrent].Task_QE=0;
}
/********************************************************************************
 Func: 删除任务消息队列
 Time: 2009-12-28
 Ver.: V1.0
 Note: 
********************************************************************************/
void Tos_TaskDeleteQmsg(void)
{
 	Tos_TaskTcb[Tos_TaskCurrent].Task_QM=0;
	Tos_TaskTcb[Tos_TaskCurrent].Task_QL=0;
	Tos_TaskTcb[Tos_TaskCurrent].Task_QS=0;
	Tos_TaskTcb[Tos_TaskCurrent].Task_QE=0;
}
/********************************************************************************
 Func: 系统队列,数据共享
 Time: 2009-12-28
 Ver.: V1.0
 Note: 本调用向系统队列尾部写入新消息
       Index/消息标记	MsgType/消息类型	MsgMark/消息标记	MsgSize/消息长度   *Qmsg/消息地址
	   若任务消息队列已满,则消息将拒绝写入,消息将丢失
********************************************************************************/

void Tos_TaskPutQmsg(uint8 Index,uint8 MsgType,uint16_t MsgSize,void * Qmsg)
{
	uint8 NextPt;
	Tos_Tab *Tos_Addr;
	Tos_ArrayMessage *_CurMsg;
	if(Index>=Tos_TaskTotal)return;
	Tos_Addr=&Tos_TaskTcb[Index];							//取目标任务控制块
	if(Tos_Addr->Task_QM==0)return ;
	if(Tos_Addr->Task_QE<Tos_Addr->Task_QL){										//任务消息队列是否已满
		NextPt=(Tos_Addr->Task_QS+Tos_Addr->Task_QE)%(Tos_Addr->Task_QL);	//计算下一个消息地址偏移量
		if(NextPt>=Tos_Addr->Task_QL)return;
		_CurMsg=Tos_Addr->Task_QM+NextPt;											//取下一个消息地址
		_CurMsg->Qmsg_Type =MsgType;
		BufferCoppy(Qmsg,(void *)_CurMsg->Qmsg_Addr,MsgSize);
		_CurMsg->Qmsg_Size =MsgSize;
		Tos_Addr->Task_QE++;
		}
	Tos_TaskRecover(Index);
}
/********************************************************************************
 Func: 任务发送邮件
 Time: 2013-11-13
 Note; 本方法将邮件发送到对方邮箱中
********************************************************************************/
void Tos_TaskSmail(uint8 Index,uint32 Mail)
{
	uint8 i;
	if(Index==0xff)for(i=0;i<Tos_TaskTotal;i++)Tos_TaskTcb[Index].Task_MB=Mail;
	else Tos_TaskTcb[Index].Task_MB=Mail;//取目标任务控制块
}
/********************************************************************************
 Func: 任务打开邮件
 Time: 2013-11-13
 Note; 本方法打开邮箱
********************************************************************************/
uint32 Tos_TaskQmail()
{
	return Tos_TaskTcb[Tos_TaskCurrent].Task_MB;//取目标任务控制块
}
