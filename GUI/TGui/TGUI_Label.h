#ifndef _TGUI_LABEL_H_
#define _TGUI_LABEL_H_
#include <Include.h>
#include <Gui_Include.h>
#include <TGUI_Label.h>

typedef struct{
  TGUI_ControlBase Control;    
	uint16 ForeColor;  									//标签显示前景色
	uint16 BackColor;									//标签显示背景色
	uint16 BorderColor;									//标签边框色彩
	uint8  Alignment;									//标签文本对齐方式
	TGUI_FontType  Font;										//显示字体
	uint8  Text[TGUI_Label_Lenth];						//标签文本内容
}TGUI_Label;


void TGUI_Label_ReDraw(void *Object,uint8 State);
void TGUI_Label_Create(uint16 ID,uint16 SX,uint16 SY,uint16 W,uint16 H,void *InitText,uint8 Style,TGUI_Label *NewLabel);
void TGUI_Label_SetColor(TGUI_Label *CurLabel,uint16 Fcolor,uint16 Bcolor,uint16 Dcolor);
uint8 TGUI_Label_SetText(TGUI_Label *CurLabel,void *NewText);
#endif 
