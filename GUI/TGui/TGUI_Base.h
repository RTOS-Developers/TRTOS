#ifndef _TGUI_BASE_H_
#define _TGUI_BASE_H_
#include <Include.h>

#define TGUI_STYLE_3D		0
#define TGUI_SoftKeyboard_En 1


/*-------------------------------------------------------------------------
TGUI键盘消息结构定义
-------------------------------------------------------------------------*/
typedef struct
{
	uint8 KeyType;				//键盘事件类型
	uint8 KeyValue;				//键盘键值
	uint8 KeyTime;				//键时间参数
}TGUI_KeyMsg;

/*-------------------------------------------------------------------------
TGUI触摸屏消息结构定义
-------------------------------------------------------------------------*/
typedef struct
{
	uint8  TouchType;				//触摸事件类型
	uint16 TouchValueX;				//触摸X值
	uint16 TouchValueY;				//触摸Y值
	uint8  TouchTime;				//触摸时间参数
}TGUI_TouchMsg;


typedef struct
{
	uint8  TP;														//控件元素类型
	uint16 ID;														//控件ID号
	uint16 SX;														//控件位置域坐标SX
	uint16 SY;														//控件位置域坐标SY
	uint16 EX;														//控件位置域坐标EX
	uint16 EY;														//控件位置域坐标EY
	uint8  State;													//控件当前状态
	uint8  Style;													//控件的绘制样式
	uint8  Action;													//控件的行为样式
	void * PrevControl;												//指向前一个控件
	void * NextControl;											   	//指向后一个控件
  struct _TGUI_Window *ParentWindow;								//指向控件的父窗口	
	void  (*ReDraw)(void *Object,uint8 Param);					  	//定义控件的重绘方法
	uint8 (*Event) (void *Object,uint8 EventType,uint8 EventValue);	//控件事件处理接口
	void *Control;
}TGUI_ControlBase;

#define TGUI_DefaultFont	TGUI_FontType_16X16

void TGUI_Draw_ObjectPopup(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 FaceColor);
void TGUI_Draw_ObjectDown(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 FaceColor);
void TGUI_Draw_ObjectIn(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 FaceColor);


#endif
