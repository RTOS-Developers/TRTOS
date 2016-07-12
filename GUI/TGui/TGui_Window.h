#ifndef _TGUI_WINDOW_H_
#define _TGUI_WINDOW_H_

#include <Include.h>
#include <Tos_Include.h>
#include <Gui_Include.h>


/*-------------------------------------------------------------------------
 结构声明
-------------------------------------------------------------------------*/
typedef struct _TGUI_Window
{		
	TGUI_ControlBase Control;									//GUI基础结构
	uint8  Mark;												//窗口的私有标志
	void * Title;												//窗口标题
	uint16 BackColor;											//窗口背景色
	uint16 ForeColor;											//窗口前景色		
	TGUI_FontType Font;
	void * BackImage;											//窗口背景图片				
	uint8  ControlsCount;										//当前窗口中元素个数统计
	void * CurControl;											//当前窗口中的焦点元素
	uint8  Buffer[TGUI_Window_Buffer_Lenth];					//本窗口内共享缓冲区
	uint8 (*OnEvent)(uint16 ID,Tos_ArrayMessage *Msg);		//窗口消息事件回调
	void  (*OnPaint)(uint16 ID);
	Tos_Timer Timer;											//窗口定时器
	uint16 CursorSX;											//窗口内光标位置X
	uint16 CursorSY;											//窗口内光标位置Y
}TGUI_Window;		


extern TGUI_Window *PreFocusWindow;								//前一个焦点窗口
extern TGUI_Window *CurFocusWindow;								//当前击活窗口

							
void * TGUI_GetType_OnFocus(TGUI_Window *CurWindow);
void TGUI_Window_DrawBackGround(TGUI_Window *CurWindow,uint16 SX,uint16 SY,uint16 EX,uint16 EY);
void TGUI_Window_DrawBody(TGUI_Window *CurWindow);
void TGUI_Window_ReDraw(void *Object,uint8 State);
uint8 TGUI_Window_Event(void *Object,uint8 EventType,uint8 EventValue);
void TGUI_Window_CursorShow(TGUI_Window *CurWindow);
void TGUI_Window_SetCursor(TGUI_Window *CurWindow,uint16 SX,uint16 SY,uint8 Enable);
void TGUI_Window_AddControl(TGUI_Window *CurWindow,void *Control);
//void TGUI_Window_OverCut(Rectangle *Rect1,Rectangle *Rect2);
void TGUI_Window_Move(TGUI_Window *CurWindow,uint16 SX,uint16 SY);
																				;
uint8 TGUI_Event_Filter(TGUI_Window *CurWindow,Tos_ArrayMessage *CurMsg);
uint8 TGUI_Window_BackSpace(void);
uint8 TGUI_Window_NextSpace(void);
uint16 TGUI_Window_GetEventControl(TGUI_Window *CurWindow,uint16 X,uint16 Y);
uint8 TGUI_Window_ControlsSetFocus(TGUI_Window *CurWindow,uint16 ID);
void * TGUI_Window_GetControl(TGUI_Window *CurWindow,uint16 ID);
void TGUI_SetWindow_OnFocus(TGUI_Window *CurWindow);
void TGUI_Window_Create(uint16 ID,uint16 SX,uint16 SY,uint16 W,uint16 H,void *Title,uint8 Style,TGUI_Window *NewWindow);
void TGUI_Window_Config(TGUI_Window *CurWindow,uint16 Fcolor,uint16 Bcolor,void *ImageAddr);

#endif
