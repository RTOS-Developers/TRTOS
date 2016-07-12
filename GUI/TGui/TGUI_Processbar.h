#ifndef _TGUI_PROCESSBAR_H_
#define _TGUI_PROCESSBAR_H_

#include <Include.h>

typedef struct
{
    TGUI_ControlBase Control;
	uint16 FontColor;				  //字体颜色
	uint16 BackColor;				  //进度条背景色
	uint16 ForeColor;				  //进度条前景色
	uint16 BorderColor;				  //进度条边框色彩
	uint16 MaxValue;				  //进度条最大值域
	uint16 CurValue;  				  //进度条当前值域
	uint8  CurProcess;				  //进度条当前显示进度值
	void  (*Changed)(uint8 ID);       //当进度值发生改变的时候触发事件 
}TGUI_ProcessBar;


void  TGUI_ProcessBar_SetProcess(TGUI_ProcessBar *CurProcessBar,uint16 Value);
uint8 TGUI_ProcessBar_GetProcess(TGUI_ProcessBar *CurProcessBar);
void  TGUI_ProcessBar_SetColor(TGUI_ProcessBar *CurProcessBar,uint16 Fcolor,uint16 Bcolor,uint16 Dcolor);
void  TGUI_ProcessBar_ReDraw(void *Object,uint8 State);
uint8 TGUI_ProcessBar_Event(void *Object,uint8 EventType,uint8 EventValue);
void  TGUI_ProcessBar_Create(u16 ID,u16 SX,u16 SY,u16 W,u16 H,u16 MaxValue,u8 Style,u8 Action,TGUI_ProcessBar *NewProcessBar);


#endif
