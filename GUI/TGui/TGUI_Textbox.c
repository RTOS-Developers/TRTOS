#include <Include.h>
#include <Tgui_Include.h>
#include <Tools_Include.h>
#include <TGui_Textbox.h>


/*------------------------------------------------------------------------------
 Func: 重绘文本框文字
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_ShowText(TGUI_Textbox *CurTextbox,uint16 Fcolor,uint16 Bcolor)
{
	TGUI_ControlBase *P=&(CurTextbox->Control);
	TGUI_Set_Color(Fcolor,Null,Bcolor,Null);
	TGUI_Fill_Rectangle(P->SX+1,P->SY+1,P->EX-1,P->EY-1,Bcolor);
	TGUI_Draw_String(P->SX,P->SY,P->EX,P->EY,CurTextbox->Text,TGUI_Textbox_AlignMent);
}	

/*------------------------------------------------------------------------------
 Func: 清文本框文本内容
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_ClearText(TGUI_Textbox *CurTextbox)
{ 
    CurTextbox->Text[0]='\0';											//清文本缓冲区
	TGUI_Textbox_ShowText(CurTextbox,CurTextbox->ForeColor,CurTextbox->BackColor);
}


/*------------------------------------------------------------------------------
 Func: 向文本区域写入文本
 Time: 2010-3-25
 Ver.: V2.0
 Note: return TGUI_Result_Error/TGUI_Result_Success
------------------------------------------------------------------------------*/
uint8 TGUI_Textbox_SetText(TGUI_Textbox *CurTextbox,void *NewText)
{
	uint16 L;
	TGUI_ControlBase *P=&(CurTextbox->Control);
	L=GetTextLength(NewText);
	if(L>=TGUI_Textbox_Lenth)return TGUI_Result_Error;	
	StringCoppy(NewText,CurTextbox->Text);
	if(P->ParentWindow==CurFocusWindow){
		if(P->State==TGUI_State_EntryEdit){		
			TGUI_Window_SetCursor(P->ParentWindow,0,0,0);							//关闭光标
		}
		TGUI_Textbox_ReDraw(CurTextbox,P->State);									//重绘文本框
		if(P->State==TGUI_State_EntryEdit){
			if(L==0){
				Cursor_X=(P->SX+P->EX)/2;
				Cursor_Y=(P->SY+P->EY)/2-Tos_GetFontHandle()->SizeEH/2;
			}
			TGUI_Window_SetCursor(P->ParentWindow,Cursor_X,Cursor_Y,1);		//重设光标位置
		}															   
	}
	return TGUI_Result_Success;	
}

/*------------------------------------------------------------------------------
 Func: 向文本标签写入数字
 Time: 2010-3-25
 Ver.: V2.0
 Note: 
------------------------------------------------------------------------------*/
void TGUI_Textbox_SetNumber(TGUI_Textbox *CurTextbox,f32 Number,uint8 DP)
{
	uint8 Text[10];	
	GetStringFromNumber(Text,Number,'D',DP);
	Text[TGUI_Textbox_Lenth]='\0';
	TGUI_Textbox_SetText(CurTextbox,Text);
}


/*------------------------------------------------------------------------------
 Func: 向文本区域增加文本
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 TGUI_Textbox_ApendText(TGUI_Textbox *CurTextbox,void *NewText)
{	
	uint8 L;	
	L =GetTextLength(NewText);
	L+=GetTextLength(CurTextbox->Text);
	if(L>=TGUI_Textbox_Lenth)return TGUI_Result_Error;
	StringEndingAppend(CurTextbox->Text,NewText);					//拼接新字符串到文本区
	TGUI_Textbox_SetText(CurTextbox,CurTextbox->Text);			//重绘文本框
	return TGUI_Result_Success;
}


/*------------------------------------------------------------------------------
 Func: 获取文本框内容
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 * TGUI_Textbox_GetText(TGUI_Textbox *CurTextbox)
{
    return CurTextbox->Text;
}


/*------------------------------------------------------------------------------
 Func: 获取文本框内容，格式化为数字
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
f32 TGUI_Textbox_GetNumber(TGUI_Textbox *CurTextbox)
{
	return GetStringToNumber(CurTextbox->Text);
}

/*------------------------------------------------------------------------------
 Func: 重绘当前Textbox控件
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_ReDraw(void *Object,uint8 State)
{   
	TGUI_Textbox *CurTextbox=(TGUI_Textbox *)Object;
	TGUI_ControlBase *P=&(CurTextbox->Control);
	P->State=State;
	switch(State){			
		case TGUI_State_ReLoad:
			if(!(P->Style&TGUI_Style_NoFrame)){
				//TGUI_Draw_Rectangle(P->SX,P->SY,P->EX,P->EY,TGUI_Textbox_BDcolor);	
				//TGUI_Draw_ObjectPopDown(P->SX,P->SY,P->EX,P->EY,Color_Red,Color_Green,Color_White);
				TGUI_Draw_ObjectIn(P->SX,P->SY,P->EX,P->EY,Color_White);
			}
			TGUI_Textbox_ShowText(CurTextbox,CurTextbox->ForeColor,CurTextbox->BackColor);
			break;
		case TGUI_State_OnFocus:							//按钮获得焦点时的弹起状态			
			if(CurTextbox->EditType&TGUI_EditType_OkEdit){
				TGUI_Textbox_ShowText(CurTextbox,TGUI_OnFocus_ForeColor,TGUI_OnFocus_BackColor);
			}else{
				TGUI_Textbox_ShowText(CurTextbox,TGUI_OnEdit_ForeColor,TGUI_OnEdit_BackColor);
			}
			break;
        case TGUI_State_UnFocus:							//按钮失去焦点
			TGUI_Textbox_ShowText(CurTextbox,CurTextbox->ForeColor,CurTextbox->BackColor);
            break;
		case TGUI_State_EntryEdit:							//进入编辑模式
			TGUI_Textbox_ShowText(CurTextbox,TGUI_OnEdit_ForeColor,TGUI_OnEdit_BackColor);
			break;
		case TGUI_State_Hidden:
			break;
       }
}

/*------------------------------------------------------------------------------
 Func: 设置文本框显示色彩
 Time: 2010-1-2
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_SetColor(TGUI_Textbox *CurTextbox,u16 Fcolor,u16 Bcolor)
{
	CurTextbox->ForeColor=Fcolor;
	CurTextbox->BackColor=Bcolor;
	if(CurTextbox->Control.ParentWindow==CurFocusWindow){
		TGUI_Textbox_ReDraw(CurTextbox,CurTextbox->Control.State);
	}
}

/*------------------------------------------------------------------------------
 Func: 隐藏文本框
 Time: 2010-1-2
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_Hidden(TGUI_Textbox *CurTextbox)
{
	if(CurTextbox->Control.ParentWindow==CurFocusWindow){
		TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_Hidden);
	}else{
		CurTextbox->Control.State=TGUI_State_Hidden;	
	}
}


/*------------------------------------------------------------------------------
 Func: 重新显示文本框
 Time: 2010-1-2
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_Show(TGUI_Textbox *CurTextbox)
{
	TGUI_ControlBase *P=&(CurTextbox->Control);
	if(P->State==TGUI_State_Hidden){
		if(P->ParentWindow->CurControl==CurTextbox)P->State=TGUI_State_OnFocus;
		else P->State=TGUI_State_ReLoad;
		if(P->ParentWindow==CurFocusWindow)TGUI_Textbox_ReDraw(CurTextbox,P->State);
	}
}


/*------------------------------------------------------------------------------
 Func: 使文本框进入编辑模式
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_InEditMode(TGUI_Textbox *CurTextbox)
{
	TGUI_ControlBase *P=&(CurTextbox->Control);
	StringCoppy(CurTextbox->Text,P->ParentWindow->Buffer);
	P->State=TGUI_State_EntryEdit;										//进入编辑模式
	TGUI_Textbox_SetText(CurTextbox,P->ParentWindow->Buffer);
}

/*------------------------------------------------------------------------------
 Func: 使文本框退出编辑模式
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_ExEditMode(TGUI_Textbox *CurTextbox)
{
	TGUI_ControlBase *P=&(CurTextbox->Control);
	if(CurTextbox->Text[0]==0){
		StringCoppy(P->ParentWindow->Buffer,CurTextbox->Text);
	}
	TGUI_Window_SetCursor(P->ParentWindow,0,0,0);							//光闭光标		
}


/*------------------------------------------------------------------------------
 Func: 获取文本框内容，格式化为数字
 Time: 2010-5-20
 Ver.: V1.0
 Note: return 0/未产生有效更改  1/产生有效更改
------------------------------------------------------------------------------*/
uint8 TGUI_Textbox_ValueEdit(TGUI_Textbox *CurTextbox,uint8 KeyValue)
{  	
	uint8 i,R=TGUI_Result_Ignore;
	uint8 Text[TGUI_Textbox_Lenth+2];
	if((KeyValue>=TGUI_Key_Num0)&&(KeyValue<=TGUI_Key_Num9)){
		if(CurTextbox->EditType & TGUI_EditType_Num)
		{		
			i=StringCoppy(CurTextbox->Text,Text);
			if(i<CurTextbox->MaxLength){
				Text[i++]='0'+KeyValue-TGUI_Key_Num0;Text[i]='\0';
				R=TGUI_Textbox_SetText(CurTextbox,Text);
			}
		}
	}else if(KeyValue==TGUI_Key_Dot){
		if(CurTextbox->EditType & TGUI_EditType_Dot){
			i=StringCoppy(CurTextbox->Text,Text);
			if(i<CurTextbox->MaxLength){
				Text[i++]='.';Text[i]='\0';
				R=TGUI_Textbox_SetText(CurTextbox,Text);
			}
			//if(i<TGUI_Textbox_Lenth){				
			//	j=TTS_StringSearch(Text,0,'.');
			//	if(j==0xFFFF){									//检查是否重复输入'.'符号							
			//		Text[i++]='.';Text[i]='\0';
			//		R=TGUI_Textbox_SetText(CurTextbox,Text);
			//	}
			//}			
		}
	}else if(KeyValue==TGUI_Key_Sub){
		if(CurTextbox->EditType & TGUI_EditType_Sig){			
			if(CurTextbox->Text[0]!='-'){
				Text[0]='-';
				i=StringCoppy(CurTextbox->Text,&Text[1]);
				if(i<=TGUI_Textbox_Lenth)R=TGUI_Textbox_SetText(CurTextbox,Text);
			}			
		}
	}
	else if(KeyValue==TGUI_Key_Esc){
		i=StringCoppy(Text,CurTextbox->Text);
		if(i>0)Text[i-1]='\0';							//去掉最后字符
		R=TGUI_Textbox_SetText(CurTextbox,Text);		//刷新缓冲
	}
	return R;	
}

/*------------------------------------------------------------------------------
 Func: 处理Button事件
 Time: 2010-3-25
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
//uint8 TGUI_Textbox_Event(void *Object,uint8 EventType,uint8 EventValue)
//{
//	uint8 Result=TGUI_Result_Null;
//	TGUI_Textbox *CurTextbox=(TGUI_Textbox *)Object;
//	TGUI_ControlBase *P=&(CurTextbox->Control);
//	switch(EventType){
//		case TGUI_Event_KeyClickDown:  											//有按键按下事件			
//			switch(EventValue){
//				case TGUI_Key_Ok:												//按钮按下
//					if(CurTextbox->EditType&TGUI_EditType_OkEdit){
//						if(CurTextbox->EditType&TGUI_EditType_EnEdit){
//							CurTextbox->EditType&=~TGUI_EditType_EnEdit;
//							TGUI_Textbox_ExEditMode(CurTextbox);													
//							TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_OnFocus);	//进入焦点模式								
//							if(CurTextbox->ExitEdit!=0)CurTextbox->ExitEdit(P->ID);
//						}else {
//							CurTextbox->EditType|=TGUI_EditType_EnEdit;
//							TGUI_Textbox_InEditMode(CurTextbox);
//							TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_EntryEdit);	//进入编辑模式
//							if(CurTextbox->EntryEdit!=0)CurTextbox->EntryEdit(P->ID);
//
//						}
//					}else if(CurTextbox->ExitEdit!=0)CurTextbox->ExitEdit(P->ID);
//					break;				
//				case TGUI_Key_Left:
//					if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
//						if(P->Action&TGUI_Action_TabDirH)Result=TGUI_Result_BackSpace;						
//					}					
//					break;
//				case TGUI_Key_Right:
//					if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
//						if(P->Action&TGUI_Action_TabDirH)Result=TGUI_Result_NextSpace;
//					}
//					break;
//				case TGUI_Key_Esc:
//					if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
//						Result=TGUI_Result_Ignore;					
//					}
//				default: //Result=TGUI_Result_Ignore;					
//					if(EventValue==TGUI_Key_Up){
//						if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
//							if(P->Action&TGUI_Action_TabDirV)return TGUI_Result_BackSpace;
//						}						
//					}else if(EventValue==TGUI_Key_Down){
//						if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
//							if(P->Action&TGUI_Action_TabDirV)return TGUI_Result_NextSpace;
//						}											
//					}
//					if(CurTextbox->EditType&TGUI_EditType_EnEdit){
//						if(CurTextbox->OnKeyEdit != 0)CurTextbox->OnKeyEdit(P->ID,EventValue);
//						else TGUI_Textbox_ValueEdit(CurTextbox,EventValue);
//					}
//			}
//			break;
//		case TGUI_Event_TouchDown:
//			if(!(CurTextbox->EditType&TGUI_EditType_EnEdit)){
//				CurTextbox->EditType|=TGUI_EditType_EnEdit;
//				TGUI_Textbox_InEditMode(CurTextbox);								
//				if(TGUI_SoftKeyboard_En)TGUI_SoftKeyboard_Show();
//				if(CurTextbox->EntryEdit!=0)CurTextbox->EntryEdit(P->ID);
//			}
//			break;
//		case TGUI_Event_ControlOnFocus:		    									//控件获得焦点事件
//			if(CurTextbox->Control.State!=TGUI_State_Hidden){
//				TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_OnFocus);					//处理按钮获得焦点事件				
//				if(CurTextbox->OnFocus!=0)CurTextbox->OnFocus(P->ID);				//处理获得焦点回调
//			}else{
//				Result=TGUI_Result_Ignore;
//			}			
//			break;
//		case TGUI_Event_ControlUnFocus:											//控件失去焦点事件
//			TGUI_Textbox_ExEditMode(CurTextbox);
//			TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_UnFocus);					//处理按钮失去焦点事件
//			if(CurTextbox->EditType&TGUI_EditType_EnEdit){
//				CurTextbox->EditType&=~TGUI_EditType_EnEdit;
//				if(CurTextbox->ExitEdit!=0)CurTextbox->ExitEdit(P->ID);				
//			}
//			if(CurTextbox->UnFocus!=0)CurTextbox->UnFocus(P->ID);				//处理失去焦点回调
//			break;
//		default: Result=TGUI_Result_Ignore;
//			break;		 
//	}
//	return Result;	
//}

uint8 TGUI_Textbox_Event(void *Object,uint8 EventType,uint8 EventValue)
{
	uint8 Result=TGUI_Result_Null;
	TGUI_Textbox *CurTextbox=(TGUI_Textbox *)Object;
	TGUI_ControlBase *P=&(CurTextbox->Control);
	switch(EventType){
		case TGUI_Event_KeyClickDown:  											//有按键按下事件			
			switch(EventValue){
				case TGUI_Key_Ok:												//按钮按下
					if(CurTextbox->EditType&TGUI_EditType_OkEdit){
						if(CurTextbox->EditType&TGUI_EditType_EnEdit){
							CurTextbox->EditType&=~TGUI_EditType_EnEdit;
							TGUI_Textbox_ExEditMode(CurTextbox);													
							TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_OnFocus);	//进入焦点模式								
							if(CurTextbox->ExitEdit!=0)CurTextbox->ExitEdit(P->ID);
						}else {
							CurTextbox->EditType|=TGUI_EditType_EnEdit;
							TGUI_Textbox_InEditMode(CurTextbox);
							TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_EntryEdit);	//进入编辑模式
							if(CurTextbox->EntryEdit!=0)CurTextbox->EntryEdit(P->ID);

						}
					}else if(CurTextbox->ExitEdit!=0)CurTextbox->ExitEdit(P->ID);
					break;				
				case TGUI_Key_Left:
					if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
						if(P->Action&TGUI_Action_TabDirH)Result=TGUI_Result_BackSpace;						
					}					
					break;
				case TGUI_Key_Right:
					if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
						if(P->Action&TGUI_Action_TabDirH)Result=TGUI_Result_NextSpace;
					}
					break;
				case TGUI_Key_Esc:
					if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
						Result=TGUI_Result_Ignore;					
					}
				default: //Result=TGUI_Result_Ignore;					
					if(EventValue==TGUI_Key_Up){
						if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
							if(P->Action&TGUI_Action_TabDirV)return TGUI_Result_BackSpace;
						}						
					}else if(EventValue==TGUI_Key_Down){
						if((CurTextbox->EditType&TGUI_EditType_EnEdit)==0){
							if(P->Action&TGUI_Action_TabDirV)return TGUI_Result_NextSpace;
						}											
					}
					if(CurTextbox->EditType&TGUI_EditType_EnEdit){
						if(CurTextbox->OnKeyEdit != 0)CurTextbox->OnKeyEdit(P->ID,EventValue);
						else TGUI_Textbox_ValueEdit(CurTextbox,EventValue);
					}
			}
			break;    
		case TGUI_Event_TouchDown:
			if(!(CurTextbox->EditType&TGUI_EditType_EnEdit)){			
				CurTextbox->EditType|=TGUI_EditType_EnEdit;
				if(CurTextbox->Control.State!=TGUI_State_EntryEdit){
					TGUI_Textbox_InEditMode(CurTextbox);								
					if(TGUI_SoftKeyboard_En)TGUI_SoftKeyboard_Show();
					if(CurTextbox->EntryEdit!=0)CurTextbox->EntryEdit(P->ID);
				}
			}
			break;
		case TGUI_Event_ControlOnFocus:		    									//控件获得焦点事件
			if(CurTextbox->Control.State!=TGUI_State_Hidden){
				if((CurTextbox->Control.State!=TGUI_State_OnFocus)&&(CurTextbox->Control.State!=TGUI_State_EntryEdit)){
					TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_OnFocus);					//处理按钮获得焦点事件				
					if(CurTextbox->OnFocus!=0)CurTextbox->OnFocus(P->ID);				//处理获得焦点回调
				}
			}else{
				Result=TGUI_Result_Ignore;
			}			
			break;
		case TGUI_Event_ControlUnFocus:											//控件失去焦点事件
			TGUI_Textbox_ExEditMode(CurTextbox);
			TGUI_Textbox_ReDraw(CurTextbox,TGUI_State_UnFocus);					//处理按钮失去焦点事件
			if(CurTextbox->EditType&TGUI_EditType_EnEdit){
				CurTextbox->EditType&=~TGUI_EditType_EnEdit;
				if(CurTextbox->ExitEdit!=0)CurTextbox->ExitEdit(P->ID);				
			}
			if(CurTextbox->UnFocus!=0)CurTextbox->UnFocus(P->ID);				//处理失去焦点回调
			if(TGUI_SoftKeyboard_En)TGUI_SoftKeyboard_Hidden();
			break;
		default: Result=TGUI_Result_Ignore;
			break;		 
	}
	return Result;	
}


/*------------------------------------------------------------------------------
 Func: 创建Textbox控件
 Time: 2009-3-25
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Textbox_Create(u16 ID,u16 SX,u16 SY,u16 W,u16 H,void *InitText,u8 Style,u8 Action,TGUI_Textbox *NewTextbox)
{
	TGUI_ControlBase *P=&(NewTextbox->Control);
	P->TP=TGUI_Type_Textbox;
	P->ID=ID;
    P->SX=SX;
    P->SY=SY;
    P->EX=SX+W;
    P->EY=SY+H;
	P->State=TGUI_State_UnLoad;
	P->Style=Style;
	P->Action=Action;
	P->PrevControl=0;
	P->NextControl=0;
	P->ParentWindow=0;
  StringCoppy(InitText,NewTextbox->Text);
	P->ReDraw=TGUI_Textbox_ReDraw;							//定义当前控件的重绘方法
	P->Event=TGUI_Textbox_Event;
	NewTextbox->ForeColor=TGUI_Textbox_FaceColor;
	NewTextbox->BackColor=TGUI_Textbox_BackColor;
	NewTextbox->EditType=TGUI_EditType_Default;
	NewTextbox->OnFocus=0;
	NewTextbox->UnFocus=0;
	NewTextbox->EntryEdit=0;
	NewTextbox->ExitEdit=0;
	NewTextbox->OnKeyEdit=0;		  
}																										  
