#ifndef _APP_REALLY_H_
#define _APP_REALLY_H_
#include <include.h>
#ifdef BOARD_REALLY
typedef enum
{
	
	PMFF_SetAddr=0,
	PMFF_GetAddr=1,
	PMFF_GetUint16=2,
	PMFF_GetUint8=3,
	PMFF_GetBuf=4,
	PMFF_SetBuf=5,
	PMFF_TestCmd=6,
}PDA_Modbus_FrameFlag;
extern uint16 SleepCount;
extern uint8 PDA_Buf[PDA_Dev_Size][PDA_Buf_Size];
void Task_GetPDA(void *Tags);
void Tos_TaskGetPDA(void);
#endif
#endif
