#ifndef _APP1_H_
#define _APP1_H_
#include <Include.h>


typedef struct{
	uint8 RecordNumber[12];//备案编号
	uint8 ProjectNumber[24];//项目编号
	uint8 LiftModel[16];//升降机型号
	uint8 LiftNumber[16];//设备编号
  uint8 BlockBoxNumber[16];
	uint8 HandVersion[5];
	uint8 SoftVersion[5];
	uint8 SIMNumber[12];
	float CorrectHeightV1;
	float CorrectHeightA1;
	float CorrectHeightV2;
	float CorrectHeightA2;
	float CorrectWeightV1;
	float CorrectWeightA1;
	float CorrectWeightV2;
	float CorrectWeightA2;
	uint8 Lamp;
	Date AllCheckDate;
	Date AntiFailCheckDate;
	uint16 Null;
}Run_ValueRegeStruct;
typedef enum
{
	MT_AllCheck=4,
	MT_AntiFailCheck=3,
	MT_YearCheck=2,
	MT_DayCheck=1,
	MT_AddCheck=0,
}MaintenanceType;
typedef enum
{
   VT_Null=0x00,//没有任何数据
   VT_Menu=0x01,//菜单型
   VT_Task=0x02,
	 VT_Value=0x03,
	 VT_Float=0x04,
	 VT_Uint16=0x05,
	 VT_Int8=0x06,
	 VT_TaskF32=0x07,
	 VT_ROMFloat=0x08,
	 VT_ROMUint32=0x09,
	 VT_ROMUint16=0x0a,
	 VT_Uint8=0x0b,
	 VT_ROMUint8=0x0c,
	 VT_ROMStr=0x0d,
	 VT_ROMInt8=0x0e,
	 VT_ROMInt16=0x0f,
	 VT_Str=0x10,
	 VT_TaskUint8=0x11,
	 VT_CheckBit=0x12,
}Value_Type;//低位为小数点位数

typedef struct 
{
	uint64_t Key;
	void *Value;
  uint8 Type;
	uint8 Note;//菜单类型Note=菜单深度
}LCD_MenuStruct;
typedef struct
{
	uint8 Cmd;
	uint8 Buf[100];
	uint8 Length;
}GPRS_RxMessage;
extern Run_ValueRegeStruct Run_ValueRege;
void LCD_Command(void *Tags);
#endif
