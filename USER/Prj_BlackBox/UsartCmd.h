#ifndef _USARTCMD_H_
#define _USARTCMD_H_
#include <Include.h>

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
}Value_Type;//低位为小数点位数

typedef struct 
{
	uint32 Key;
	void *Value;
  uint8 Type;
	uint8 Note;//菜单类型Note=菜单深度
}PELCO_DStruct;


typedef struct
{
	uint8 StartFlag;
	uint8 Address;
	uint16 Cmd;
	uint16 Data;
	uint8 Mod;
}PELCO_D;
void Usart_Cmd(void *Tags);


#endif
