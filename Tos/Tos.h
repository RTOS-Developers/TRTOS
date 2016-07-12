#ifndef _TOS_H_
#define _TOS_H_
#include <Include.h>

extern uint32 guiNextTaskSp;              /* 将要运行的任务堆栈指针 */
extern uint32* gpuiCurTaskSpAddr;         /* 存放当前运行任务堆栈指针的地址 */

#define STACKALIGNMASK      0xFFFFFFF8  /* 堆栈对齐掩码, cortex内核使用8字节对齐 */
#define MODE_USR            0x01000000/* THUMB指令集USR工作模式掩码 */

/***********************************************************
消息结构体变量类型申明
***********************************************************/
typedef struct{
	uint8  Qmsg_Type;						//消息标签
	uint8  Qmsg_Size;						//消息长度
	void  *Qmsg_Addr;						//消息地址
}Tos_ArrayMessage;
/***********************************************************
CPU信息结构体类型变量声明
***********************************************************/
typedef struct{
	uint8  Userate_CPU;//CPU的使用率
	uint8  RunStatus;
	uint32 Use_Ram;//内存的使用量
}_Tos_CPUInfor;
/***********************************************************
任务运行信息表结构体类型申明
***********************************************************/
typedef struct{
	uint32 TaskCurSp;              //任务栈指针
	Tos_Task  vfFunc;
  uint16  Task_ST;        	        //任务运行标志
	uint32  Task_MB;						      //任务邮箱
	uint32  Task_SP;									//任务堆栈地址
	uint32  Task_TD;									//任务延时时间
	uint32  Task_TQ;                  //任务超时时间
	uint32  Task_SA;              //任务被分配到堆栈的偏移地址 
  uint16  TaskStackSize;
	uint8 Task_WDID;           //等待设备的ID
	uint8	Task_QL;						//任务消息队列长度
	uint8	Task_QS;						//任务消息队列起始位置
	uint8	Task_QE;						//任务消息队列消息长度
	Tos_ArrayMessage * Task_QM;	
	char *Task_Name;
 }Tos_Tab;
typedef struct
{
	void(*vfFuncPointer)(void *Tags);
	void *Tags;
	char *TaskName;
	uint32 StackSize;
}TaskInitList;
/**********************************************************
任务的运行状态枚举
**********************************************************/
typedef enum Tos_TaskState
{
	_TTS_Deleted	 =S0,    //任务已经删除或未被创建不具备生命
	_TTS_Run		   =S1,		//任务正在执行或等待进入执行	
	_TTS_Stop		   =S2,		//任务已经停止活动但依然可以被执行，但需要被动切换			
	_TTS_Suspend	 =S3,		//挂起了就需要中断或它任务唤醒		
	_TTS_DelayWait =S4, 		//任务超时计数	
	_TTS_WaitDevice=S5,   //等待设备
	_TTS_Error     =S6,   //任务错误
}Tos_TaskState;

extern uint8  Tos_TaskCurrent;//当前任务ID
extern Tos_Tab *Tos_TaskTcb;//[Tos_TaskTotal];//任务列表
extern uint32 Tos_TaskMail;
extern _Tos_CPUInfor Tos_CPUInfor;
extern void TOS_ContextSwitch(void);
extern void TOS_SwitchToTask(void);

void Tos_Init(void);
void Tos_Start(void);
void Tos_TaskDelay(uint32 DelayTime);
void Tos_TaskDelayHMSM(uint8 Hour,uint8 Min,uint8 Sec,uint16 MSec);
void Tos_TaskSwitch(void);
void Tos_TaskRun(uint8 Index);
BOOL Tos_TaskDelete(uint8 Index);
uint16 Tos_GetTaskIDByName(uint8 *Name);
BOOL Tos_TaskCreateFindID(void(*vfFuncPointer)(void *Tags),uintbus Tags,char *TaskName,uint32 StackSize);
void Tos_TaskTimeout(uint8 Index,uint32 Timeout);
void Tos_SetTimeout(uint32 Timeout);
uint16 Tos_TaskGetState(uint8 Index);
void Tos_WaitDev(uint8 DevID);
void Tos_TaskRecover(uint8 Index);
BOOL Tos_TaskResume(uint8 Index);
void Tos_TaskReCreate(uint8 Id,Tos_Task vfFuncPointer,uintbus Tags);
void Tos_TaskCreate(uint8 Id,Tos_Task vfFuncPointer,uintbus Tags,char *TaskName,uint32 StackSize);
void Tos_ListCreate(const TaskInitList *TIL);
void Tos_TaskStop(uint8 Index);
BOOL Tos_TaskSuspend(uint8 Index);
void Tos_SysTick(void);
void Tos_TaskCreateQmsg(Tos_ArrayMessage *MsgAddr,uint8 MsgLenth);
void Tos_TaskPutQmsg(uint8 Index,uint8 MsgType,uint16 MsgSize,void * Qmsg);
Tos_ArrayMessage *Tos_TaskGetQMsgDelay(uint16 D);
void Tos_TaskSmail(uint8 Index,uint32 Mail);
void Tos_TaskDeleteQmsg(void);
Tos_ArrayMessage *Tos_TaskGetQMsg(void);
#endif
