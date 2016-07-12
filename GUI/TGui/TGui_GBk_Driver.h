#ifndef _TGUI_GBK_DRIVER_H_
#define _TGUI_GBK_DRIVER_H_
#include <Include.h>
#include <stdarg.h>
#include <GBK_Lib.h>

typedef enum TGUI_FontType
{
	TGUI_FontType_12X12=0x01,
	TGUI_FontType_16X16=0x02,
	TGUI_FontType_16X24=0x03,
	TGUI_FontType_24X24=0x04,		 
	TGUI_FontType_32X32=0x05,	
	TGUI_FontType_48X48=0x06,
}TGUI_FontType;

#define Font_Max TGUI_FontType_24X24
#define Font_Def TGUI_FontType_16X16
#define Font_Min TGUI_FontType_12X12

extern uint16 Cursor_X,Cursor_Y;
typedef struct{
	uint8 FontType;
	BOOL  SelectTrue;
	uint8 Line;
	uint16 SX;
	uint16 SY;
	uint16 EX;
	uint16 EY;
	uint16 B_Color;//背景色
	uint16 F_Color;//前景色
	uint16 S_Color;//高亮色
	uint16 A_Color;//透明色
	uint16 AlignMode;
}_TGui_Style;

typedef struct
{
	uint8 SizeEH;
	uint8 SizeEW;
	uint8 SizeCH;
	uint8 SizeCW;
	uint8 LibSizeE;
	uint16 LibSizeC;
	const uint8 *P_ELib;
	const uint8 *P_CLib;
	const uint8 *P_CIndex;
	uint16 LibCCount;
	uint16 LibECount;
	const FontTypeStruct *FontLib;
}_TGUI_Font;

void TGUI_Set_Font(TGUI_FontType Font);
void TGUI_Set_Color(uint16 FColor,uint16 SColor,uint16 BColor,uint16 AColor);
void TGUI_Draw_String(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint8 *String,uint8 Align);
void TGUI_Draw_FormatString(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint8 *Pstr,uint8 AlignMode,uint8 Line);
void TGUI_Draw_Word(uint16 X,uint16 Y,uint16 Word);
void TGUI_Set_LineN(uint8 Line);
void Tos_SetStyleHandle(_TGui_Style *PTG);
_TGui_Style *Tos_GetStyleHandle(void);
void Tos_SetFontHandle(_TGUI_Font *TF);
_TGUI_Font *Tos_GetFontHandle(void);
void TGUI_Draw_ASCII(uint16 X,uint16 Y,uint16 Char);
void TGUI_Set_Region(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint8 AlignMode);
void TGUI_Clear_Show(void);
void TGUI_DefaultStyle(void);
extern void vsprintf(char *,char *fmt,va_list ap);
void TGUI_Draw_Number(uint16 SX, uint16 SY,uint16 EX,uint16 EY, uint16 Number,uint8 AlignMode);
void Printf(char *fmt,...);
void MGUI_DrawBitMap(uint16 X,uint16 Y,uintbus BMP,uint8 Mode);
#endif
