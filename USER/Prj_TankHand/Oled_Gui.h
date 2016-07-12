#ifndef _OLED_GUI_H_
#define _OLED_GUI_H_
#include <Include.h>
#ifdef BOARD_TANK_Hand
#include <Really_Driver.h>
#include <Really_GUI.h>
#include <text.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Really_Driver.h>
#include <Really_GUI.h>
#include <FuncTask_Include.h>
#include <HandKey_Driver.h>


#define FontType_Min TGUI_FontType_12X12
#define FontType_Max TGUI_FontType_24X24
typedef struct
{   
    uint8 DrawMode;
    uint8 DeepRecord[5];
    uint8 Index;
		uint8 OptionLength;
    uintbus SelectMenu;
}MenuStruct;

typedef struct
{   
   Task_Float Active;
	  void *Value;
}VT_MenuFuncValue;

typedef enum
{
	 WDM_Clear=0,
	 WDM_Normal=1,
	 WDM_Fill=2,
	 WDM_Mirror=3
}WordDrawMode;

typedef enum
{
   Draw_All=0xff,
   Draw_Title=0x01,
   Draw_Value=0x02,
   Draw_Clear=0x04,
   Draw_Base=0x08
}PageDrawMode;
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
	 VT_ROMBOOL=0x11
}Value_Type;//低位为小数点位数

//菜单项结构体数据
typedef struct 
{
	char *Option;
	void *Value;
  uint8 Type;
	uint32 Note;//菜单类型Note=菜单深度
}OLED_MenuStruct;

typedef struct 
{
	uint8 StartX;//菜单左上角坐标
	uint8 StartY;//菜单左上角坐标
	uint8 EndX;//菜单右下角坐标
	uint8 EndY;//菜单右下角坐标
	uint8 RowCount;//行数
	uint8 ColumnCount;//列数
	BOOL CursorEnable;
	MenuStruct *MS;//菜单动态变量
}OLED_MenuStyle;

extern MenuStruct PageStruct;

void SetDraw_Mode(uint8 Mode);
void Draw_Menu(void);
void Enter_Menu(void);
BOOL Printf_Infor(char *Msg,uint16 Delay);
BOOL Printf_Yes(char *Msg);
BOOL Printf_YesOrNo(char *Msg);
void Printf_Waiting(char *Msg,uint16 Delay);
float Page_GetData(float Value);
float Page_IfGetData(float Value, BOOL *IfOk);
void Menu_Init(const OLED_MenuStruct *MenuHome);
void Set_LocalFont(TGUI_FontType FontType);
BOOL PrintfVoid(char *fmt,...);
BOOL Printf_Message(char *Msg,uint16 Delay);
void Printf_InforFail(void);
#endif
#endif
