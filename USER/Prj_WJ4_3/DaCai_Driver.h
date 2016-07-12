#ifndef _DACAI_DRIVER_H_
#define _DACAI_DRIVER_H_
#include <Include.h>
#include <stdarg.h>
typedef enum 
{
	Register=0x01,
	
}SysTaskWell;
typedef struct 
{
	uint8 Year;
	uint8 Mon;
	uint8 Day;
	uint8 Hour;
	uint8 Min;
	uint8 Sec;
}_Date_Time;
typedef struct
{
	uint32 ServerIP;
	uint16 ServerPort;
	uint8 SaveOffSet;
	uint16 LeftHeight;
	uint16 LeftWeight;
	uint16 LeftSpeed;
	uint16 LeftActive;
	uint16 LeftAlarm;
	uint16 LeftError;
	uint16 LeftSafe;
	uint8  PeopleNum;
	uint8 OnlyRefresh;
	uint8 SysTaskWell;
	uint8 WorkMode;
	uint8 DriverID[18];
	Date_Time DT;
	Date_Time SDT;
	uint32 PicSize;
	uint8 DeverType;
	uint32 Sig; 
	uint32 Ctr;
	uint32 SysRunSec;
	uint8 TempBuf[20];
}RunValueStruct;

extern RunValueStruct Run_Value;
extern void vsprintf(char *,char *fmt,va_list ap);
void DaCai_SetTextBox(uint16 ScreenID,uint16 ControlID,uint8 *String,uint8 Length);
void DaCai_Printf(char *fmt,...);
void DaCai_TextBox(uint16 ScreenID,uint16 ControlID,char *fmt,...);
void DaCai_SetDateTime(Date_Time *DateTime);
void DaCai_SetDateLight(uint8 Light);
void DaCai_SetPage(uint8 Page);
void DaCai_SetSpk(void);
#endif
