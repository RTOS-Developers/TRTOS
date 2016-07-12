#ifndef _MENU_BASE_H_
#define _MENU_BASE_H_
#include <Include.h>
#include <TGui_GBk_Driver.h>


//页绘画模式
typedef enum
{
   Draw_All=0xff,
   Draw_Title=0x01,
   Draw_Menu=0x02,
   Draw_Clear=0x04,
   Draw_Value=0x08
}PageDrawMode;
//数据类型
typedef enum
{
    VT_End=0x00,//没有任何数据
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
    VT_Uint16Bool=0x10,
    VT_EEPFloat=0x011,
    VT_EEPUint8=0x12,  
    VT_ROMEEPUint8=0x13,
    VT_EEPUint8Task=0x14,
		VT_PageAct=0x15,
		VT_TaskUint8=0x16,
		VT_Uint8Task=0x17,
		VT_BOOL=0x18,
		VT_Null=0x19,
		VT_Str=0x1a,
		VT_IP=0x1b,
		VT_Color=0x1c
}Value_Type;//低位为小数点位数
typedef enum{
	IT_Dot=S0,
	IT_Minus=S1,
	IT_Num=S2,
	IT_ASCIIS=S3,
	IT_ASCIIB=S4,
	IT_CurCLiB=S5,
	IT_End=S6,
}InputType;
//界面绘制风格
typedef struct 
{
	uint16 F_Color;
	uint16 B_Color;
	uint16 S_Color;
	uint16 A_Color;
	uint16 K_Color;
	TGUI_FontType  MenuFont;
	TGUI_FontType  TitleFont;
	TGUI_FontType  MsgFont;
}MUI_MenuConfigs;
//单元菜单结构体
typedef struct 
{
  const char *Option;
	void *Value;
  uint8 Type;
	uint8 Note;//菜单类型Note=菜单深度
}MUI_MenuStruct;

typedef struct
{
	ConfigHeadStruct CH;
	char Name[10];
	uint8 Age;
	uint8 Hight;
	f32 Weight;
	MUI_MenuConfigs MenuCFG;
	BOOL DeBugEn;
	BOOL LED[3];
	uint8 LifePlayBuf[10];
	uint32 IP;
	uint32 LocalHost;
	uint32 SubnetMask;
	uint32 GateWay;
}App_Value;
typedef struct
{   
	uint8 DrawMode;
	uint8 DeepRecord[5];
	uint8 Index;
	uint8 OptionLength;
	uintbus SelectMenu;
	const MUI_MenuStruct *CurMenu;
	const MUI_MenuStruct *OldMenu;
	char *Title;
	char *OldTitle;
	uint16 StartX;//菜单左上角坐标
	uint16 StartY;//菜单左上角坐标
	uint16 EndX;//菜单右下角坐标
	uint16 EndY;//菜单右下角坐标
	uint8 RowCount;//行数
	uint8 ColumnCount;//列数
	BOOL CursorEnable;
}MUI_MenuValue;


extern MUI_MenuValue *MenuValue;
void MUI_Title(char *Title);
void MUI_ReSetTitle(char *Title);
void MUI_SetDrawClear(uint8 En);
void MUI_Waiting(char *Title,uint8 D);
void MUI_Draw(const MUI_MenuStruct *CurMenu);
uint8 MUI_GetKey(uint16 D);
void MUI_Enter(const MUI_MenuStruct *CurMenu);
uint8 MUI_OptionLength(const MUI_MenuStruct *CurMenu);
const MUI_MenuStruct *MUI_GetCurMenuInfor(void);
BOOL Printf_Infor(char *Msg,uint16 Delay);
BOOL Printf_Yes(char *Msg);
BOOL PrintfVoid(char *fmt,...);
BOOL Printf_YesOrNo(char *Msg);
#endif
