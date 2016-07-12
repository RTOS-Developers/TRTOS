#include <Include.h>
#include <Gui_include.h>
#include <Tools_Include.h>
#include <TGUI_Button.h>                                                  
        







/*------------------------------------------------------------------------------
 Func: 显示按钮文本
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
static void TGUI_Button_ShowText(TGUI_Button *CurButton)
{		
	  TGUI_ControlBase *P=&(CurButton->Control);
	  TGUI_Set_Font(CurButton->Font);
	  TGUI_Set_Color(CurButton->ForeColor,Null,CurButton->BackColor,Null);
    TGUI_Draw_String(P->SX,P->SY,P->EX,P->EY,CurButton->Text,Align_CenterCenter);
}


/*------------------------------------------------------------------------------
 Func: 清按钮文本内容
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
static void TGUI_Button_ClearText(TGUI_Button *CurButton)
{
    TGUI_ControlBase *P=&(CurButton->Control);
	CurButton->Text[0]='\0';
	if(CurButton->Control.ParentWindow==CurFocusWindow){
		if(TGUI_STYLE_3D){
			TGUI_3D_Draw_ObjectBody(P->SX,P->SY,P->EX,P->EY,P->State);
		}else{
			TGUI_Fill_Rectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,CurButton->BackColor);
		}
	}
}

/*------------------------------------------------------------------------------
 Func: 设置按钮文本
 Time: 2009-12-26
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
void TGUI_Button_SetText(TGUI_Button *CurButton,void *NewText)
{
	TGUI_Button_ClearText(CurButton);
	BufferCoppy(NewText,CurButton->Text,TGUI_Button_Lenth-1);	
	if(CurButton->Control.ParentWindow==CurFocusWindow)
	{
		TGUI_Button_ShowText(CurButton);
	}	
}


/*------------------------------------------------------------------------------
 Func: 获取Button文本
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 *TGUI_Button_GetText(TGUI_Button *CurButton)
{
    return CurButton->Text;
}

void TGUI_Button_ReDraw(void *Object,uint8 State)
{
	TGUI_Button *CurButton=(TGUI_Button *)Object;
	TGUI_ControlBase *P=&(CurButton->Control);
	switch(State)
	{
		case TGUI_State_ReLoad:
				 TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
				 TGUI_Button_ShowText(CurButton);
			   P->State=TGUI_State_UnFocus;					//状态更新为已加载状态	
		break;
		case TGUI_State_OnFocus:							//按钮获得焦点
				 TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
         TGUI_Draw_DotRectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,Color_Black,CurButton->BackColor);
				 TGUI_Button_ShowText(CurButton);
			   P->State=State;	
		break;
		case TGUI_State_UnFocus:							//按钮失去焦点			
				 TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
			   TGUI_Button_ShowText(CurButton);
			   P->State=State;																		 
		break;	
		case TGUI_State_EntryEdit:							//按钮被按下
				 TGUI_Draw_ObjectDown(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
				 TGUI_Draw_DotRectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,Color_Black,CurButton->BackColor);
			   TGUI_Button_ShowText(CurButton);
			   P->State=State;									//状态更新为已加载状态			
    break;
		case TGUI_State_ExitEdit:							//按钮弹起
				 TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
				 TGUI_Draw_DotRectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,Color_Black,CurButton->BackColor);
			   TGUI_Button_ShowText(CurButton);
			   P->State=TGUI_State_OnFocus;					//状态更新为已加载状态			
    break;
		case TGUI_State_Hidden:
			   TGUI_Window_DrawBackGround(P->ParentWindow,P->SX,P->SY,P->EX,P->EY);
			   P->State=TGUI_State_Hidden;
		break;	
	}
	
}
/*------------------------------------------------------------------------------
 Func: 重绘当前Button控件
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void _TGUI_Button_ReDraw(void *Object,uint8 State)
{                                 
	TGUI_Button *CurButton=(TGUI_Button *)Object;
	TGUI_ControlBase *P=&(CurButton->Control);
    switch(State){
		case TGUI_State_ReLoad:								//按钮加载到窗口
			if(TGUI_STYLE_3D){
				TGUI_3D_Draw_ObjectBody(P->SX,P->SY,P->EX,P->EY,TGUI_State_UnFocus);
				TGUI_3D_Draw_ObjectBorder(P->SX,P->SY,P->EX,P->EY,TGUI_State_UnFocus);
			}else{
				TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
			}
			TGUI_Button_ShowText(CurButton);
			P->State=TGUI_State_UnFocus;					//状态更新为已加载状态																				 
			break;
		case TGUI_State_OnFocus:							//按钮获得焦点
			if(TGUI_STYLE_3D){
				TGUI_3D_Draw_ObjectBody(P->SX,P->SY,P->EX,P->EY,TGUI_State_OnFocus);
				TGUI_3D_Draw_ObjectBorder(P->SX,P->SY,P->EX,P->EY,TGUI_State_OnFocus);			
			}else{
				TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
               	TGUI_Draw_DotRectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,Color_Black,CurButton->BackColor);
			}
			TGUI_Button_ShowText(CurButton);
			P->State=State;	
			break;
        case TGUI_State_UnFocus:							//按钮失去焦点			
			if(TGUI_STYLE_3D){
				TGUI_3D_Draw_ObjectBody(P->SX,P->SY,P->EX,P->EY,TGUI_State_UnFocus);
				TGUI_3D_Draw_ObjectBorder(P->SX,P->SY,P->EX,P->EY,TGUI_State_UnFocus);			
			}else{
				TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
			}
			TGUI_Button_ShowText(CurButton);
			P->State=State;																		 
			break;
        case TGUI_State_EntryEdit:							//按钮被按下
			if(TGUI_STYLE_3D){
				TGUI_3D_Draw_ObjectBody(P->SX,P->SY,P->EX,P->EY,TGUI_State_EntryEdit);
				TGUI_3D_Draw_ObjectBorder(P->SX,P->SY,P->EX,P->EY,TGUI_State_EntryEdit);						
			}else{
				TGUI_Draw_ObjectDown(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
				TGUI_Draw_DotRectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,Color_Black,CurButton->BackColor);
			}
			TGUI_Button_ShowText(CurButton);
			P->State=State;									//状态更新为已加载状态			
            break;
		case TGUI_State_ExitEdit:							//按钮弹起
			if(TGUI_STYLE_3D){
				TGUI_3D_Draw_ObjectBody(P->SX,P->SY,P->EX,P->EY,TGUI_State_OnFocus);
				TGUI_3D_Draw_ObjectBorder(P->SX,P->SY,P->EX,P->EY,TGUI_State_OnFocus);						
			}else{	
				TGUI_Draw_ObjectPopup(P->SX,P->SY,P->EX,P->EY,CurButton->BackColor);
				TGUI_Draw_DotRectangle(P->SX+2,P->SY+2,P->EX-2,P->EY-2,Color_Black,CurButton->BackColor);
			}
			TGUI_Button_ShowText(CurButton);
			P->State=TGUI_State_OnFocus;					//状态更新为已加载状态			
            break;
		case TGUI_State_Hidden:
			TGUI_Window_DrawBackGround(P->ParentWindow,P->SX,P->SY,P->EX,P->EY);
			P->State=TGUI_State_Hidden;
			break;		        
	}
}


/*------------------------------------------------------------------------------
 Func: 设置按钮色彩
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Button_SetColor(TGUI_Button *CurButton,uint16 Fcolor,uint16 Bcolor)
{
	CurButton->ForeColor=Fcolor;
	CurButton->BackColor=Bcolor;
	if(CurButton->Control.ParentWindow==CurFocusWindow)
	{
		TGUI_Button_ReDraw(CurButton,CurButton->Control.State);	
	}
}	


/*------------------------------------------------------------------------------
 Func: 将按钮隐藏
 Time: 2009-12-26
 Ver.: V1.0
 Note:
-----------------------------------------------------------------------------*/
void TGUI_Button_Hidden(TGUI_Button *CurButton)
{	
	if(CurButton->Control.ParentWindow==CurFocusWindow)
	{
		TGUI_Button_ReDraw(CurButton,TGUI_State_Hidden);			//重绘控件
	}else{
		CurButton->Control.State=TGUI_State_Hidden;
	}
}

/*------------------------------------------------------------------------------
 Func: 将按钮显示
 Time: 2009-12-26
 Ver.: V1.0
 Note:
-----------------------------------------------------------------------------*/
void TGUI_Button_Show(TGUI_Button *CurButton)
{
	TGUI_ControlBase *P=&(CurButton->Control);
	if(P->State==TGUI_State_Hidden){
		if(P->ParentWindow->CurControl==CurButton)P->State=TGUI_State_OnFocus;
		else P->State=TGUI_State_ReLoad;
		if(P->ParentWindow==CurFocusWindow)TGUI_Button_ReDraw(CurButton,P->State);
	}
}


/*------------------------------------------------------------------------------
 Func: 处理Button事件
 Time: 2010-3-25
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
uint8 TGUI_Button_Event(void *Object,uint8 EventType,uint8 EventValue)
{
	uint8 Result=TGUI_Result_Null;
	TGUI_Button *CurButton=(TGUI_Button *)Object;
	TGUI_ControlBase *P=&(CurButton->Control);
	if(P->State==TGUI_State_Hidden)		return TGUI_Result_Ignore;				//控件隐藏,不响应交互事件
	if(!(P->Action&TGUI_Action_Enable))	return TGUI_Result_Ignore;				//控件禁用,不响应交互事件
	switch(EventType){
		case TGUI_Event_KeyClickDown:  											//有按键按下事件
			switch(EventValue){								  
				case TGUI_Key_Ok:												
					TGUI_Button_ReDraw(CurButton,TGUI_State_EntryEdit);			//按钮按下事件		
					break;				
				case TGUI_Key_Left:
					if(P->Action&TGUI_Action_TabDirH)Result=TGUI_Result_BackSpace;	//水平TAB Left
					break;
				case TGUI_Key_Right:
					if(P->Action&TGUI_Action_TabDirH)Result=TGUI_Result_NextSpace;	//水平TAB Right
					break;
				case TGUI_Key_Up:
					if(P->Action&TGUI_Action_TabDirV)Result=TGUI_Result_BackSpace;	//垂直TAB Up
					break;
				case TGUI_Key_Down:
					if(P->Action&TGUI_Action_TabDirV)Result=TGUI_Result_NextSpace;	//垂直TAB Down
					break;
				default: Result=TGUI_Result_Ignore;
					break;
			}
			break;
		case TGUI_Event_KeyClickUp:												
			switch(EventValue){
				case TGUI_Key_Ok:													//按钮弹起事件
					TGUI_Button_ReDraw(CurButton,TGUI_State_ExitEdit);				//重绘按钮弹起样式
					if(CurButton->OnClick!=0)CurButton->OnClick(P->ID);				//处理按钮事件回调					
					break;
				default: Result=TGUI_Result_Ignore;
					break;
			}
			break;
		case TGUI_Event_TouchDown:
			TGUI_Button_ReDraw(CurButton,TGUI_State_EntryEdit);						//触屏按下事件
			break;
		case TGUI_Event_TouchUp:
			if(P->State!=TGUI_State_OnFocus){
				TGUI_Button_ReDraw(CurButton,TGUI_State_ExitEdit);						//重绘按钮弹起样式			
			//	if( (TGUI_Mouse_X>P->SX) && (TGUI_Mouse_X<P->EX) &&	
			//		(TGUI_Mouse_Y>P->SY) && (TGUI_Mouse_Y<P->EY) )
			//	{
					if(CurButton->OnClick!=0)CurButton->OnClick(P->ID);						//处理触屏事件回调								
			//	}
			}
			break;
		case TGUI_Event_ControlOnFocus:		    									//控件获得焦点事件
			TGUI_Button_ReDraw(CurButton,TGUI_State_OnFocus);						//处理按钮获得焦点事件
			if(CurButton->OnFocus!=0)CurButton->OnFocus(P->ID);						//处理获得焦点回调
			break;
		case TGUI_Event_ControlUnFocus:												//控件失去焦点事件
			TGUI_Button_ReDraw(CurButton,TGUI_State_UnFocus);						//处理按钮失去焦点事件
			if(CurButton->UnFocus!=0)CurButton->UnFocus(P->ID);						//处理失去焦点回调
			break;
		default: Result=TGUI_Result_Ignore;
			break;				 
	}
	return Result;	
}


/*------------------------------------------------------------------------------
 Func: 创建Button控件
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Button_Create(uint16 ID,uint16 SX,uint16 SY,uint16 W,uint16 H,void *InitText,uint8 Style,TGUI_Button *NewButton)
{	
	TGUI_ControlBase *P=&(NewButton->Control);
	P->TP=TGUI_Type_Button;										//写按钮类型
	P->ID=ID;					 								//写按钮ID			
    P->SX=SX;													//计算相对位置
    P->SY=SY;
    P->EX=SX+W;
    P->EY=SY+H; 
	P->State=TGUI_State_UnLoad;									//初始化按钮为未加载状态
    P->Style=Style;												//控件默认样式
	P->Action=TGUI_Action_EnOnFucus|TGUI_Action_EnEdit|TGUI_Action_TabDirV|TGUI_Action_TabDirH|TGUI_Action_Enable;
	P->PrevControl=0;
	P->NextControl=0;
	P->ParentWindow=0;											//写按钮父窗口
	P->ReDraw=TGUI_Button_ReDraw;								//定义按钮重绘方法
	P->Event=TGUI_Button_Event;									//定义按钮事件方法
	StringCoppy(InitText,NewButton->Text);					//初始化按钮文本
	NewButton->ForeColor=TGUI_Button_ForeColor;
	NewButton->BackColor=TGUI_Button_BackColor;
	NewButton->Font=TGUI_DefaultFont;
	NewButton->OnFocus=0;
	NewButton->UnFocus=0;
	NewButton->OnClick=0;
	P->Control=NewButton;
}



