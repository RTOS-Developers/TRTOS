#include <Include.h>
#include <Tgui_Include.h>
#include <TGui_Window.h>

TGUI_Window *PreFocusWindow;								//前一个焦点窗口
TGUI_Window *CurFocusWindow;								//当前击活窗口


/*------------------------------------------------------------------------------
 Func: 获取指定窗口焦点项控件的类型
 Time: 2014-6-4
 Ver.: V2.0
 Note: 若焦点项
------------------------------------------------------------------------------*/
void * TGUI_GetType_OnFocus(TGUI_Window *CurWindow)
{	
	return CurWindow->CurControl;
}
/*------------------------------------------------------------------------------
 Func: 窗口背景重绘
 Time: 2010-3-24
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Window_DrawBackGround(TGUI_Window *CurWindow,uint16 SX,uint16 SY,uint16 EX,uint16 EY)
{
	uint32 M=(uint32)CurWindow->BackImage;
	if(M != TGUI_Image_Null){
		TGUI_Draw_Image(SX,SY,EX,EY,SX,SY,CurWindow->BackImage);
	}else{
		if(!(CurWindow->Control.Style&TGUI_Style_NoBcolor)){
			TGUI_Fill_Rectangle(SX,SY,EX,EY,CurWindow->BackColor);
		}
	}	
}
/*------------------------------------------------------------------------------
 Func: 窗口重绘
 Time: 2014-3-24
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Window_DrawBody(TGUI_Window *CurWindow)
{	
	uint16 T;
	TGUI_ControlBase *P=&(CurWindow->Control);
	T=P->SY;
	if(!(P->Style & TGUI_Style_NoTitle)){
		T+=TGUI_Window_TitleHeight;
		TGUI_Set_Font(CurWindow->Font);
		TGUI_Fill_Rectangle(P->SX,P->SY,P->EX,T,TGUI_Window_TitleBackColor);
		TGUI_Set_Color(TGUI_Window_TitleForeColor,Null,TGUI_Window_TitleBackColor,Null);
		TGUI_Draw_String(P->SX,P->SY,P->EX,T,CurWindow->Title,Align_CenterCenter);
	}
	TGUI_Window_DrawBackGround(CurWindow,P->SX,T,P->EX,P->EY);	
	if(!(P->Style & TGUI_Style_NoFrame)){
		TGUI_Draw_Rectangle(P->SX,P->SY,P->EX,P->EY,CurWindow->ForeColor);
	}	
}
/*------------------------------------------------------------------------------
 Func: 重绘指定的窗口
 Time: 2010-6-4
 Ver.: V2.0
 Note: 
------------------------------------------------------------------------------*/
void TGUI_Window_ReDraw(void *Object,uint8 State)
{																									 
	void *PT;	
	TGUI_Window *CurWindow=(TGUI_Window *)Object;
	TGUI_ControlBase *P=&(CurWindow->Control);
	TGUI_Window_DrawBody(CurWindow);
	PT=P->NextControl;
	while(PT!=CurWindow){
		P=(TGUI_ControlBase *)PT;
		if(P->State!=TGUI_State_Hidden){
			P->ReDraw(P,TGUI_State_ReLoad);
		}		
		PT=P->NextControl;
	}
	if(CurWindow->OnPaint)CurWindow->OnPaint(CurWindow->Control.ID);
}


/*------------------------------------------------------------------------------
 Func: 处理窗口事件
 Time: 2010-3-25
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
uint8 TGUI_Window_Event(void *Object,uint8 EventType,uint8 EventValue)
{
	return TGUI_Result_Ignore;	
}


/*------------------------------------------------------------------------------
 Func: 窗口光标显示
 Time: 2010-6-6											  
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Window_CursorShow(TGUI_Window *CurWindow)
{
	if(CurWindow->Mark & TGUI_Window_CursorEnable)
	{
		if(CurWindow->Mark & TGUI_Window_CursorState){						
			TGUI_Fill_Rectangle(CurWindow->CursorSX,CurWindow->CursorSY,
							CurWindow->CursorSX+TGUI_Cursor_Width,CurWindow->CursorSY+TGUI_Cursor_Height,
							TGUI_OnEdit_ForeColor);							//先清除原位置光标	
		}else{
			TGUI_Fill_Rectangle(CurWindow->CursorSX,CurWindow->CursorSY,
							CurWindow->CursorSX+TGUI_Cursor_Width,CurWindow->CursorSY+TGUI_Cursor_Height,
							TGUI_OnEdit_BackColor);					//先清除原位置光标
		}
		CurWindow->Mark^=TGUI_Window_CursorState;					//取反光标状态
	}					
}


/*------------------------------------------------------------------------------
 Func: 窗口光标设置
 Time: 2010-6-6
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Window_SetCursor(TGUI_Window *CurWindow,uint16 SX,uint16 SY,uint8 Enable)
{
	if(CurWindow->Mark&TGUI_Window_CursorEnable){
		TGUI_Fill_Rectangle(CurWindow->CursorSX,CurWindow->CursorSY,
							CurWindow->CursorSX+TGUI_Cursor_Width,CurWindow->CursorSY+TGUI_Cursor_Height,
							TGUI_OnEdit_BackColor);	//先清除原位置光标
		CurWindow->Mark&=~TGUI_Window_CursorState;	
	}
	if(Enable){		
		CurWindow->CursorSX=SX;
		CurWindow->CursorSY=SY;
		CurWindow->Mark|=TGUI_Window_CursorEnable;
		TGUI_Fill_Rectangle(CurWindow->CursorSX,CurWindow->CursorSY,
							CurWindow->CursorSX+TGUI_Cursor_Width,CurWindow->CursorSY+TGUI_Cursor_Height,
							TGUI_OnEdit_ForeColor);	//显示光标
	}else{								  									
		CurWindow->Mark&=~TGUI_Window_CursorEnable;
	}
}


/*------------------------------------------------------------------------------
 Func: 向窗口链表插入新的控件
 Time: 2010-6-4
 Ver.: V2.0
 Note: *CurWindow/操作窗口   *Control/要插入的图元指针  
 	   返回/前一个图元指针
------------------------------------------------------------------------------*/
void TGUI_Window_AddControl(TGUI_Window *CurWindow,void *Control)
{
	uint16 T;
	TGUI_ControlBase *P,*Pw;
	P=(TGUI_ControlBase *)Control;										//取新控件结构	
	Pw=((TGUI_ControlBase *)(CurWindow->CurControl));					//取出焦点项(指向队尾)	
	P->PrevControl=Pw;
	Pw->NextControl=P;
	Pw=&(CurWindow->Control);		
	P->NextControl=Pw;
	Pw->PrevControl=P;	
	T=Pw->SY;															//求窗口起始Y坐标
	if(!(Pw->Style&TGUI_Style_NoTitle))T+=TGUI_Window_TitleHeight;		//根据标题栏调整控件位置		
	P->SX+=Pw->SX; 														//调整控件显示位置
	P->SY+=T;											 
	P->EX+=Pw->SX;
	P->EY+=T;
	P->ParentWindow=CurWindow; 											//更新当前控件的父窗口
	CurWindow->CurControl=Control;										//更新窗口焦点图元
	CurWindow->ControlsCount++;											//更新图元总数
}


/*-------------------------------------------------------------------------------
 Func: 窗口区域剪切
 Time: 2010-3-26
 Ver.: V1.0
 Note: Rect2窗口对Rect1窗口进行剪切操作
-------------------------------------------------------------------------------*/
void TGUI_Window_OverCut(Rectangle *Rect1,Rectangle *Rect2)
{
	uint16 T,K;
	T=Rect1->SY;K=Rect1->EY;
	if(Rect2->SY > Rect1->SY){											//剪切上平面
		TGUI_Draw_Rectangle(Rect1->SX,Rect1->SY,Rect1->EX,Rect2->SY,Color_White);
		TGUI_Fill_Rectangle(Rect1->SX,Rect1->SY,Rect1->EX,Rect2->SY,Color_White);	
		T=Rect2->SY;
	}
	
	if(Rect2->EY < Rect1->EY){											//剪切下平面
		TGUI_Draw_Rectangle(Rect1->SX,Rect2->EY,Rect1->EX,Rect1->EY,Color_White);
		TGUI_Fill_Rectangle(Rect1->SX,Rect2->EY,Rect1->EX,Rect1->EY,Color_White);		
		K=Rect2->EY;
	}
	if(Rect2->SX > Rect1->SX){											//剪切左平面
		TGUI_Draw_Rectangle(Rect1->SX,T,Rect2->SX,K,Color_White);
		TGUI_Fill_Rectangle(Rect1->SX,T,Rect2->SX,K,Color_White);
	}
	if(Rect2->EX < Rect1->EX){											//剪切右平面
		TGUI_Draw_Rectangle(Rect2->EX,T,Rect1->EX,K,Color_White);
		TGUI_Fill_Rectangle(Rect2->EX,T,Rect1->EX,K,Color_White);	
	}
}



/*------------------------------------------------------------------------------
 Func: 窗口移动到新的位置
 Time: 2010-3-24
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Window_Move(TGUI_Window *CurWindow,uint16 SX,uint16 SY)
{
	int16 TX,TY;
	void *PT;
	TGUI_ControlBase *P=&(CurWindow->Control);	
	Rectangle Rect1,Rect2;
	TGUI_Set_Rectangle(&Rect1,P->SX,P->SY,P->EX,P->EY);	
	TX=(int16)SX-(int16)P->SX;
	TY=(int16)SY-(int16)P->SY;	
	P->SX=SX;
	P->SY=SY;
	P->EX=(uint16)(((int16)P->EX)+TX);
	P->EY=(uint16)(((int16)P->EY)+TY);
	TGUI_Set_Rectangle(&Rect2,P->SX,P->SY,P->EX,P->EY);
	PT=P->NextControl;
	while(PT!=CurWindow){
		P=(TGUI_ControlBase *)PT;
		P->SX=(uint16)(((int16)P->SX)+TX);
		P->SY=(uint16)(((int16)P->SY)+TY);
		P->EX=(uint16)(((int16)P->EX)+TX);
		P->EY=(uint16)(((int16)P->EY)+TY);
		PT=P->NextControl;
	}
	TGUI_Window_OverCut(&Rect1,&Rect2);	
	TGUI_Window_ReDraw(CurWindow,0);
}


																				
/*------------------------------------------------------------------------------
 Func: TGUI消息过滤器
 Time: 2010-6-4
 Ver.: V2.0
 Note:
------------------------------------------------------------------------------*/
uint8 TGUI_Event_Filter(TGUI_Window *CurWindow,Tos_ArrayMessage *CurMsg)
{
	uint8  R=TGUI_Result_Ignore;
	uint16 ID;
	TGUI_KeyMsg *KeyMsg;
	TGUI_TouchMsg *TouchMsg;
	TGUI_ControlBase *P;
	if(CurMsg->Qmsg_Type==TGUI_MsgType_KeyBoard){				//取键盘事件
		KeyMsg=(TGUI_KeyMsg *)(CurMsg->Qmsg_Addr);				//取事件内容									
		P=(TGUI_ControlBase *)(CurWindow->CurControl);
		R=P->Event(P,KeyMsg->KeyType,KeyMsg->KeyValue);			//调用控件事件
	}else if(CurMsg->Qmsg_Type==TGUI_MsgType_TouchScreen){		//取触屏事件									  
		TouchMsg=(TGUI_TouchMsg *)(CurMsg->Qmsg_Addr);			//取事件内容
		if(TouchMsg->TouchType==TGUI_Event_TouchDown){			
			ID=TGUI_Window_GetEventControl(CurWindow,TouchMsg->TouchValueX,TouchMsg->TouchValueY);
			if(ID==0xFFFF)return R;
			R=TGUI_Window_ControlsSetFocus(CurWindow,ID);
			if(R!=TGUI_Result_Success)return R;
		}									
		P=(TGUI_ControlBase *)(CurWindow->CurControl);
		R=P->Event(P,TouchMsg->TouchType,0);					//调用控件事件			
	}	
	return R;																											
}


/*------------------------------------------------------------------------------
 Func: 窗口控件退格焦点处理
 Time: 2010-3-24
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 TGUI_Window_BackSpace()
{
	uint8 R=TGUI_Result_Null;
	void *PT,*PK;
	TGUI_ControlBase *P0,*P;		
	P0=(TGUI_ControlBase *)(CurFocusWindow->CurControl);					//取当前焦点项
	PT=P0->PrevControl;	
	if(PT){
		PK=PT;
		do{
			P=(TGUI_ControlBase *)PT;
			if((P->State!=TGUI_State_Hidden)&&(P->Action&TGUI_Action_Enable)){			
				R=P->Event(P,TGUI_Event_ControlOnFocus,0);					//测试控件是否可能接受焦点
				if(R!=TGUI_Result_Ignore){			
					P0->Event(P0,TGUI_Event_ControlUnFocus,0);
					CurFocusWindow->CurControl=P;
					return TGUI_Result_Success; 
				}
			}
			PT=P->PrevControl;												//取上层控件
		}while(PT!=PK);
	}
	return R;
}


/*------------------------------------------------------------------------------
 Func: 窗口控件进格焦点处理
 Time: 2010-3-24
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 TGUI_Window_NextSpace()
{
	uint8 R=TGUI_Result_Null;
	void *PT,*PK;
	TGUI_ControlBase *P0,*P;
	P0=(TGUI_ControlBase *)(CurFocusWindow->CurControl);				//取当前焦点项
	PT=P0->NextControl;
	if(PT){
		PK=PT;
		do{
			P=(TGUI_ControlBase *)PT;
			if((P->State!=TGUI_State_Hidden)&&(P->Action&TGUI_Action_Enable)){
				R=P->Event(P,TGUI_Event_ControlOnFocus,0);						//测试控件是否可以接受焦点		
				if(R!=TGUI_Result_Ignore){
					P0->Event(P0,TGUI_Event_ControlUnFocus,0);					//当前焦点丢失
					CurFocusWindow->CurControl=P;								//更新当前焦点
					return TGUI_Result_Success;
				}
			}
			PT=P->NextControl;
		}while(PT!=PK);
	}
	return R;	
}


/*------------------------------------------------------------------------------
 Func: 获取当前窗口(X,Y)处的控件ID
 Time: 2011-6-9
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint16 TGUI_Window_GetEventControl(TGUI_Window *CurWindow,uint16 X,uint16 Y)
{
	void *PT;	
	TGUI_ControlBase *P=&(CurWindow->Control);
	PT=P->NextControl;
	while(PT!=CurWindow){
		P=(TGUI_ControlBase *)PT;
		if( (P->SX<X) && (P->SY<Y) && (P->EX>X) && (P->EY>Y))return P->ID;
		PT=P->NextControl;
	}
	return 0xFFFF;
}



/*------------------------------------------------------------------------------
 Func: 窗口控件指定焦点处理
 Time: 2010-3-24
 Ver.: V1.0
 Note: 返回 TGUI_Result_Null/未找到目标控件   TGUI_Result_Error/为隐藏控件设置焦点  TGUI_Result_Sucess/正确设置焦点
------------------------------------------------------------------------------*/
uint8 TGUI_Window_ControlsSetFocus(TGUI_Window *CurWindow,uint16 ID)
{
	uint8 R=TGUI_Result_Null,M=0;
	void *PT;
	TGUI_ControlBase *P0,*P;		
	if(CurWindow==CurFocusWindow)M=1;
	P0=(TGUI_ControlBase *)(CurWindow->CurControl);									//取当前焦点项	
	if(P0->ID==ID){
		if((P0->State!=TGUI_State_Hidden)&&(P0->Action&TGUI_Action_Enable)){						
			if(M) P0->Event(P0,TGUI_Event_ControlOnFocus,0);						//对于前台窗口,需要立即重绘				
			else  P0->State=TGUI_State_OnFocus;										//对于后台窗口，更新状态				
		}
		return TGUI_Result_Success;													//控件已经获得焦点
	}	
	PT=CurWindow->Control.NextControl;												//取窗口首控件
	while(PT!=CurWindow){		
		P=(TGUI_ControlBase *)PT;
		if(P->ID == ID){														//向后依次搜索
			if((P->State!=TGUI_State_Hidden)&&(P->Action&TGUI_Action_Enable)){
				if(M)	P->Event(P,TGUI_Event_ControlOnFocus,0);				//对于前台窗口,需要立即重绘
				else	P->State=TGUI_State_OnFocus;							//对于后台窗口，更新状态
				CurWindow->CurControl=P;										//重置窗口焦点控件				
			}else{														
				return TGUI_Result_Error;										//为隐藏控件设置焦点时将产生错误
			}	
			if((P0->State!=TGUI_State_Hidden)&&(P0->Action&TGUI_Action_Enable)){
				if(M)	P0->Event(P0,TGUI_Event_ControlUnFocus,0);				//对于前台窗口,需要立即重绘		
				else	P0->State=TGUI_State_UnFocus;							//对于后台窗口,更新状态
			}
			return TGUI_Result_Success;											//焦点设置成功					
		}else{
			PT=P->NextControl;													//搜索下一控件
		}
	}
	return R;																	//若查找失败，则返回对像为空
}


/*------------------------------------------------------------------------------
 Func: 获取指定窗口中的控件
 Time: 2010-6-4
 Ver.: V2.0
 Note: 返回 TGUI_Object_Null/控件未在窗口中注册 !TGUI_Object_Null/控件基础结构
------------------------------------------------------------------------------*/
void * TGUI_Window_GetControl(TGUI_Window *CurWindow,uint16 ID)
{
 	void * PT;
	TGUI_ControlBase *P=&(CurWindow->Control);
	PT=P->NextControl;											//首窗口首控件
	while(PT!=CurWindow)
	{
		P=(TGUI_ControlBase *)PT;								//格式化控件
		if(P->ID==ID)	return P;								//查找到指定控件
		else	PT=P->NextControl;								//取下一控件
	}
	return TGUI_Object_Null;
}


/*------------------------------------------------------------------------------
 Func: 设置当前活动窗口
 Time: 2010-6-4
 Ver.: V2.0
 Note:
---------------------------------------- ---------------------------------------*/
void TGUI_SetWindow_OnFocus(TGUI_Window *CurWindow)
{
	TGUI_ControlBase *P=&(CurWindow->Control);
	if(CurFocusWindow->Timer.Lifecycle)Tos_TimerPause(CurFocusWindow->Timer.ID);
	PreFocusWindow=CurFocusWindow;
	CurFocusWindow=CurWindow;	
	TGUI_Window_ReDraw(CurWindow,0);
	if(CurWindow->ControlsCount >0){
		CurWindow->CurControl=P->PrevControl;
		if(TGUI_Window_NextSpace()!=TGUI_Result_Success){
			CurWindow->CurControl=P->NextControl;
		}
	}
	if(CurFocusWindow->Timer.Lifecycle)Tos_TimerStart(CurFocusWindow->Timer.ID);
}


/*------------------------------------------------------------------------------
 Func: 窗口初始化
 Time: 2010-3-24
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_Window_Create(uint16 ID,uint16 SX,uint16 SY,uint16 W,uint16 H,void *Title,uint8 Style,TGUI_Window *NewWindow)
{
	TGUI_ControlBase *P=&(NewWindow->Control);
	P->TP=TGUI_Type_Window;
	P->ID=ID;
    P->SX=SX;
    P->SY=SY;
    P->EX=SX+W;
    P->EY=SY+H;
	P->State=TGUI_State_UnLoad;
	P->Style=Style;
	P->Action=0;
	P->PrevControl=NewWindow;
	P->NextControl=NewWindow;	
	P->ParentWindow=0;
	P->ReDraw=TGUI_Window_ReDraw;
	P->Event=TGUI_Window_Event;
	NewWindow->Mark=0;
	NewWindow->BackColor=TGUI_Window_BackColor;							   
	NewWindow->ForeColor=TGUI_Window_ForeColor;
	NewWindow->Font=TGUI_DefaultFont;
	NewWindow->BackImage=(void *)TGUI_Image_Null;
	NewWindow->ControlsCount=0;
	NewWindow->CurControl=NewWindow;
	NewWindow->Buffer[0]='\0';
	NewWindow->Title=Title;
	NewWindow->OnEvent=0;
	NewWindow->OnPaint=0;
	NewWindow->CursorSX=0;
	NewWindow->CursorSY=0;
	NewWindow->Timer.Lifecycle=0;
}

/*------------------------------------------------------------------------
 Func: TGUI窗口属性配置
 Time: 2010-6-4
 Ver.: V1.0
 Note:
------------------------------------------------------------------------*/
void TGUI_Window_Config(TGUI_Window *CurWindow,uint16 Fcolor,uint16 Bcolor,void *ImageAddr)
{
	CurWindow->BackColor=Bcolor;
	CurWindow->ForeColor=Fcolor;
	CurWindow->BackImage=ImageAddr;
}



