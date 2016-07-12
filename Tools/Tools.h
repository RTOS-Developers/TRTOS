#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <Include.h>
#include <Tos_Include.h>
#include <Tools_Include.h>

uint16 GetLockCode(void);
uint16 Get_CPUID(uint8 *Buf,uint8 Index);
void Printf_LockCode(void);
uint8 GetTosTask_Count(void);
void Printf_SysInfor(void);
void Tos_TaskTabInfor(void);
void Printf_DateTime(char *Str,Date_Time *DT);
void Printf_Bytes(uint8 *Bytes,uint8 Length);
uint16 Get_FlashSize(void);

#endif
