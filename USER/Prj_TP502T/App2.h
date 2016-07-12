#ifndef _APP2_H_
#define _APP2_H_
#include <Include.h>
#include <..\USER\Prj_TP500\Ht1621_Face.h>
#define RECORDADDR 0x0
#define HISTORYADDR 0x4096*50
typedef struct
{
	Date_Time DT;
	float CH_Value[CH_LENGTH];
}HistoryRecord;
typedef struct
{
	uint32 NextAddr;
	HistoryRecord HR;
}NandHistoryRecord;
void Task_SaveConfigs(void);
void Task_LoadConfigs(void);
void Task_BackConfigs(void);
void Task_ReBackConfigs(void);
void Task_BeepDouble(void);
BOOL BOOL_LoadConfigs(void);
void System_Reset(void);
void Task_PrintfHistory(void);
void Task_CleanHistory(void);
void System_SaveConfig(void);
#endif
