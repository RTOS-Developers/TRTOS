
#ifndef _TGUI_BUTTON_H_
#define _TGUI_BUTTON_H_

#include <TGUI_base.h>
#include <Gui_Include.h>

/*------------------------------------------------------------------------------
 Func: TGUI Button结构
------------------------------------------------------------------------------*/
typedef struct
{		
	TGUI_ControlBase Control;										//按钮基本结构
	uint16	ForeColor;												//按钮文本色彩
	uint16  BackColor;												//按钮文本背景色彩
	TGUI_FontType  Font;
  uint8 	Text[TGUI_Button_Lenth];  								//按钮文本	
	void	(*OnFocus)(uint16 ID);									//按钮获得焦点事件回调方法
	void	(*UnFocus)(uint16 ID);									//按钮失去焦点事件回调方法
	void	(*OnClick)(uint16 ID);									//按钮被按下事件回调方法
}TGUI_Button;

void	TGUI_Button_SetText(TGUI_Button *CurButton,void *NewText);			//设置按钮文本
uint8  *TGUI_Button_GetText(TGUI_Button *CurButton);						//获取按钮文本
void	TGUI_Button_SetColor(TGUI_Button *CurButton,u16 Fcolor,u16 Bcolor);	//设置按钮色彩
void 	TGUI_Button_ReDraw(void *Object,uint8 State);						//刷新重绘按钮
void	TGUI_Button_Hidden(TGUI_Button *CurButton);							//隐藏按钮控件
void	TGUI_Button_Show(TGUI_Button *CurButton);							//显示按钮控件
uint8 	TGUI_Button_Event(void *Object,uint8 EventType,uint8 EventValue);	//按钮事件处理
void	TGUI_Button_Create(u16 ID,u16 SX,u16 SY,u16 W,u16 H,void *InitText,u8 Flag,TGUI_Button *NewButton);	//创建按钮控件

#endif

