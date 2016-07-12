#ifndef _TGUI_TEXTBOX_H_
#define _TGUI_TEXTBOX_H_
#include <Include.h>
typedef struct
{
    TGUI_ControlBase Control;
	uint16 ForeColor;
	uint16 BackColor;
	uint8  Text[TGUI_Textbox_Lenth];					//文本框内容
	uint8  MaxLength;									//最大输入字符长度
	uint8  EditType;									//文本框内容输入类型
	void  (*OnFocus)  (uint16 ID);						//文本框获得焦点事件
	void  (*UnFocus)  (uint16 ID);						//文本框失去焦点事件	
	void  (*EntryEdit)(uint16 ID);						//文本框进入编辑事件
	void  (*ExitEdit) (uint16 ID);						//文本框进入编辑事件	
	void  (*OnKeyEdit)(uint16 ID,uint8 KeyValue);		//文本框编辑回调处理	
}TGUI_Textbox; 

uint8	TGUI_Textbox_SetText(TGUI_Textbox *CurTextbox,void *NewText);			//设置文本框文本
uint8  *TGUI_Textbox_GetText(TGUI_Textbox *CurTextbox);							//获取文本框文本
uint8	TGUI_Textbox_ApendText(TGUI_Textbox *CurTextbox,void *NewText);			//拼接文本框文本
void	TGUI_Textbox_ClearText(TGUI_Textbox *CurTextbox);						//清文本框文本
void	TGUI_Textbox_SetNumber(TGUI_Textbox *CurTextbox,f32 Number,uint8 DP);	//设置文本框显示数字
f32		TGUI_Textbox_GetNumber(TGUI_Textbox *CurTextbox);						//试获取文本框中的数字值(如果输入是数字的话)
void	TGUI_Textbox_SetColor(TGUI_Textbox *CurTextbox,u16 Fcolor,u16 Bcolor);	//设置文本框色彩
void 	TGUI_Textbox_ReDraw(void *Object,uint8 State);							//刷新重绘文本框
void	TGUI_Textbox_Hidden(TGUI_Textbox *CurTextbox);							//隐藏文本框控件
void	TGUI_Textbox_Show(TGUI_Textbox *CurTextbox);							//显示文本框控件
uint8	TGUI_Textbox_ValueEdit(TGUI_Textbox *CurTextbox,uint8 Key);				//文本框字符输入编辑
uint8 	TGUI_Textbox_Event(void *Object,uint8 EventType,uint8 EventValue);		//文本框事件处理
void	TGUI_Textbox_Create(u16 ID,u16 SX,u16 SY,u16 W,u16 H,void *InitText,u8 Style,u8 Action,TGUI_Textbox *NewTextbox);	//创建文本框控件

#endif
