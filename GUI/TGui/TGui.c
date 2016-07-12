#include <Include.h>
#include <GUI_Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <TGui_Window.h>
#include <TGUI.h> 
uint16 TGUI_Mouse_X;
uint16 TGUI_Mouse_Y;
uint8  TGUI_TaskID;
TGUI_ControlBase *StrP;
/********************************************************************************
 Func: TGUI初始化,液晶初始化
 Time: 2010-1-5
 Ver.: V1.0
 Note:
********************************************************************************/
void TGUI_Init()
{
	PreFocusWindow=0;
	CurFocusWindow=(TGUI_Window *)0xFFFFFFFF;
	TGUI_TaskID=Tos_TaskNull;
	TGUI_SoftKeyboardLoad=0;
  LCD_Clear_Screen(Color_Black);
}
void UpLoad()
{
	TGUI_Button_ReDraw(StrP,TGUI_State_ExitEdit);
}
/********************************************************************************
 Func: 窗口服务任务
 Time: 2010-1-5
 Ver.: V1.0
 Note:
********************************************************************************/
void TGUI_Window_MsgTask(void *Tags)
{
	uint16 i;
	Tos_ArrayMessage *CurMsg,Msg[10];
//	TGUI_ControlBase *P;
  TGUI_TouchMsg *KeyMsg,KeyMsgBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetTouch();//获取触摸屏
	while(1)
	{
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
		  //DeBug("TouchValueX=%d,TouchValueY=%d",KeyMsg->TouchValueX,KeyMsg->TouchValueY,Infor_Warning);
		// TGUI_DrawRound(KeyMsg->TouchValueX,KeyMsg->TouchValueY,10,Color_Blue);
			i=TGUI_Window_GetEventControl(CurFocusWindow,KeyMsg->TouchValueX,KeyMsg->TouchValueY);
			if(i!=0xFFFF)
			//if(KeyMsg->TouchType==TOUCH_DOWNING)
			{
				TGUI_Window_ControlsSetFocus(CurFocusWindow,i);
				StrP=TGUI_Window_GetControl(CurFocusWindow,i);
				TGUI_Button_ReDraw(StrP,TGUI_State_EntryEdit);
				Touch_Event.UpIng_Event=UpLoad;
			}//else TGUI_Window_ControlsSetFocus(CurFocusWindow,i);
			if(i==0xffff)
			{
				if(StrP)TGUI_Button_ReDraw(StrP,TGUI_State_ExitEdit);
			}
		//	TGUI_Event_Filter(CurFocusWindow,CurMsg);
		//	TGUI_Window_GetControl(CurFocusWindow,i);
		}
	}

}
	TGUI_Button Button1;
	TGUI_Button Button2;
	TGUI_Label Label1;
	TGUI_Window Window1;
	TGUI_ProcessBar ProcessBar1;
/*------------------------------------------------------------------------------
 Func: 处理窗口消息队列GUI任务
 Time: 2010-3-24													   
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void WGUI_Window_MsgTask(void *Tags)
{
	uint8  Result,i=0;
	uint16 MsgWindowID;
	TGUI_TouchMsg *TouchMsg;
	TGUI_Window *TempFocusWindow;
	Tos_ArrayMessage *CurMsg,Msg[10];
  TGUI_TouchMsg KeyMsgBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetTouch();//获取触摸屏
	//CurFocusWindow=0;														 
	
	
	
	Tos_TaskGetTouch();
	//while(CurFocusWindow==0)Tos_TaskDelay(10);
	while(1){
WaitMsg:
		CurMsg=Tos_TaskGetQMsg();	  			    //  等待消息		
		if(CurMsg>0){			
			switch(CurMsg->Qmsg_Type)
			{
				case TGUI_MsgType_KeyBoard:		break;
				case TGUI_MsgType_TouchScreen:
					TouchMsg=(TGUI_TouchMsg *)(CurMsg->Qmsg_Addr);
					TGUI_Mouse_X=TouchMsg->TouchValueX;
					TGUI_Mouse_Y=TouchMsg->TouchValueY;		
							
					if(TouchMsg->TouchType==TOUCH_DOWNING){
						DeBug("TouchValueX=%d,TouchValueY=%d",TGUI_Mouse_X,TGUI_Mouse_Y,Infor_Warning);		
						if(TGUI_SoftKeyboardLoad){								//对于触屏软键盘,看是否触点位于其窗口内
							if( (TGUI_Mouse_X > Window_KeyBoard.Control.SX)&&(TGUI_Mouse_X < Window_KeyBoard.Control.EX) &&
							   	(TGUI_Mouse_Y > Window_KeyBoard.Control.SY)&&(TGUI_Mouse_X < Window_KeyBoard.Control.EY) ){
								TempFocusWindow=CurFocusWindow;
								CurFocusWindow=&Window_KeyBoard;
								TGUI_Event_Filter(CurFocusWindow,CurMsg);		//当前消息送往屏幕软键盘窗口
								CurFocusWindow=TempFocusWindow;
								MsgWindowID=Window_KeyBoard.Control.ID;	
								goto WaitMsg;
							}	
						}
						MsgWindowID=CurFocusWindow->Control.ID;
					}else if(TouchMsg->TouchType==TOUCH_UPING){
						if(MsgWindowID==Window_KeyBoard.Control.ID){
							TempFocusWindow=CurFocusWindow;
							CurFocusWindow=&Window_KeyBoard;							
							TGUI_Event_Filter(CurFocusWindow,CurMsg);		//当前消息送往屏幕软键盘窗口
							CurFocusWindow=TempFocusWindow;	
							goto WaitMsg;
						}
					}
					break;
			}
			if(CurFocusWindow->OnEvent!=0){
				Result=CurFocusWindow->OnEvent(CurFocusWindow->Control.ID,CurMsg);		//处理消息事件回调
			}else	Result=TGUI_Event_Filter(CurFocusWindow,CurMsg);					//当前消息送往消息过滤器
			switch(Result){
				case TGUI_Result_BackSpace:								//退格处理请求
					TGUI_Window_BackSpace();					
					break;
				case TGUI_Result_NextSpace:								//进格处理请求					
					TGUI_Window_NextSpace();
					break;
				case TGUI_Result_Ignore:								//当前事件被控件忽略
					break;			
			}
		}	
		Tos_TaskDelay(100);     
	}
}
void Gui_Button_OnClick(uint16 ID)
{
	DeBug("Gui_Button_OnClick",Infor_Warning);
}
void TGUI_TaskTest(void *Tags)
{
//	uint16 i;
	TGUI_DefaultStyle();
	TGUI_Set_Font(TGUI_FontType_16X16);
	TGUI_Set_Color(Color_Green,Color_Black,Color_Blue,Null);
	TGUI_Window_Create(0x0001,0,0,LCD_XMAX,LCD_YMAX,"Window1",Null,&Window1);
	TGUI_Button_Create(0x2001,10,10,80,30,"测试",Null,&Button1);
	TGUI_Button_Create(0x2002,100,10,80,30,"Button1",Null,&Button2);
	Button2.OnClick=Gui_Button_OnClick;
	TGUI_Label_Create(0x3001,100,100,80,20,"Label1",Null,&Label1);
	TGUI_ProcessBar_Create(0x1001,0,200,480,20,100,Null,Null,&ProcessBar1);
	ProcessBar1.CurValue=10;
	TGUI_Window_AddControl(&Window1,&Button1);
	TGUI_Window_AddControl(&Window1,&Button2);
	TGUI_Window_AddControl(&Window1,&Label1);
	TGUI_Window_AddControl(&Window1,&ProcessBar1);
	TGUI_Window_ReDraw(&Window1,Null);
	//TGUI_SetWindow_OnFocus(&Window1);
	Tos_TaskGetTouch();//获取触摸屏
	while(1)
	{		
		Tos_TaskDelay(10); 
	}
}

