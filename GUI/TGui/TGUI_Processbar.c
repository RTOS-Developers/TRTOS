#include <Include.h>
#include <Tools_Include.h>
#include <TGUI_Include.h>
#include <TGUI_Processbar.h>
/*------------------------------------------------------------------------------
 Func: 重绘当前控件字体
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_ProcessBar_ShowText(void *Object)
{
	uint8 Text[8];
	TGUI_ControlBase *P;
	TGUI_ProcessBar *CurProcessBar=(TGUI_ProcessBar *)Object;
	P=(TGUI_ControlBase *)(&(CurProcessBar->Control));
	GetStringFromNumber(Text,CurProcessBar->CurProcess,'D',0);
	StringEndingAppend("%",&Text[0]);																																																														
	TGUI_Set_Color(CurProcessBar->FontColor,Null,CurProcessBar->BackColor,Null);
    TGUI_Draw_String(P->SX,P->SY,P->EX,P->EY,Text,Align_CenterCenter);
}

/*------------------------------------------------------------------------------
 Func: 重绘当前Label控件
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_ProcessBar_ReDraw(void *Object,uint8 State)
{									
	uint16 X;
	f32 F;
	TGUI_ProcessBar *CurProcessBar=(TGUI_ProcessBar *)Object;
	TGUI_ControlBase *P=&(CurProcessBar->Control);
	F=(f32)(CurProcessBar->CurValue)/(f32)(CurProcessBar->MaxValue);
	P->State=State;
    if(State!=TGUI_State_Hidden){										//处理标签非隐藏事件        				
		if(CurProcessBar->Changed>0)CurProcessBar->Changed(P->ID);
		else{		 			
			switch(P->Style&(TGUI_Style_NoDirH|TGUI_Style_NoDirInc)){
				case 0x00:												//向右增长型
					X=(uint16)(F*(f32)(P->EX - P->SX));
					TGUI_Fill_Rectangle(P->SX,P->SY,P->SX+X,P->EY,CurProcessBar->ForeColor);
					TGUI_Fill_Rectangle(P->SX+X,P->SY,P->EX,P->EY,CurProcessBar->BackColor);
					break;	
				case TGUI_Style_NoDirH:									//向上增长型				
					X=(uint16)(F*(P->EY - P->SY));
					TGUI_Fill_Rectangle(P->SX,P->EY-X,P->EX,P->EY,CurProcessBar->ForeColor);
					TGUI_Fill_Rectangle(P->SX,P->SY,P->EX,P->EY-X,CurProcessBar->BackColor);
					break;
				case TGUI_Style_NoDirH|TGUI_Style_NoDirInc:				//向下增长型
					X=(uint16)(F*(P->EY - P->SY));
					TGUI_Fill_Rectangle(P->SX,P->SY,P->EX,P->SY+X,CurProcessBar->ForeColor);
					TGUI_Fill_Rectangle(P->SX,P->SY+X,P->EX,P->EY,CurProcessBar->BackColor);				
					break;
				case TGUI_Style_NoDirInc:								//向左增长型
					X=(uint16)(F*(P->EX - P->SX));
					TGUI_Fill_Rectangle(P->EX-X,P->SY,P->EX,P->EY,CurProcessBar->ForeColor);
					TGUI_Fill_Rectangle(P->SX,P->SY,P->EX-X,P->EY,CurProcessBar->BackColor);				
					break;	
			}
			TGUI_Draw_Rectangle(P->SX,P->SY,P->EX,P->EY,CurProcessBar->BorderColor);
			TGUI_ProcessBar_ShowText(CurProcessBar);		
		}
    }else{       
     	TGUI_Fill_Rectangle(P->SX,P->SY,P->EX,P->EY,P->ParentWindow->BackColor);
    }  
}


/*------------------------------------------------------------------------------
 Func: 更新进度条
 Time: 2009-12-26
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void TGUI_ProcessBar_SetProcess(TGUI_ProcessBar *CurProcessBar,uint16 Value)
{   
	uint16 T;
	if(Value>=CurProcessBar->MaxValue){T=100;Value=CurProcessBar->MaxValue;}
	else {T=(uint8)((uint32)Value*100/CurProcessBar->MaxValue);}
	CurProcessBar->CurValue=Value;
	if(T!=CurProcessBar->CurProcess){
		CurProcessBar->CurProcess=T;
		if(CurFocusWindow==CurProcessBar->Control.ParentWindow)								//是否应该重绘
		{
			if(CurProcessBar->Changed>0)CurProcessBar->Changed(CurProcessBar->Control.ID);
			else	TGUI_ProcessBar_ReDraw(CurProcessBar,CurProcessBar->Control.State);
		}
	}                             
}				


/*------------------------------------------------------------------------------
 Func: 从进度条获取当前的进度值
 Time: 2010-6-2
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
uint8 TGUI_ProcessBar_GetProcess(TGUI_ProcessBar *CurProcessBar)
{
	return CurProcessBar->CurValue;
}

/*------------------------------------------------------------------------------
 Func: 设置进度条色彩
 Time: 2010-6-2
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
void TGUI_ProcessBar_SetColor(TGUI_ProcessBar *CurProcessBar,uint16 Fcolor,uint16 Bcolor,uint16 Dcolor)
{
	TGUI_ControlBase *P=&(CurProcessBar->Control);
	if((CurProcessBar->ForeColor!=Fcolor)||(CurProcessBar->BackColor!=Bcolor)){
		CurProcessBar->ForeColor=Fcolor;
		CurProcessBar->BackColor=Bcolor;
		CurProcessBar->BorderColor=Dcolor;
		if(CurFocusWindow==CurProcessBar->Control.ParentWindow){
			TGUI_ProcessBar_ReDraw(CurProcessBar,CurProcessBar->Control.State);
		}
	}else{
		if(CurProcessBar->BorderColor!=Dcolor){
			if(CurFocusWindow==CurProcessBar->Control.ParentWindow){
				TGUI_Draw_Rectangle(P->SX,P->SY,P->EX,P->EY,CurProcessBar->BorderColor);	
			}
		}
	}
}


/*------------------------------------------------------------------------------
 Func: 处理进度条事件
 Time: 2010-3-25
 Ver.: V1.0
 Note: 
------------------------------------------------------------------------------*/
uint8 TGUI_ProcessBar_Event(void *Object,uint8 EventType,uint8 EventValue)
{
	return TGUI_Result_Ignore;	
}


/*------------------------------------------------------------------------------
 Func: 创建TextBox控件
 Time: 2009-3-25
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
void  TGUI_ProcessBar_Create(u16 ID,u16 SX,u16 SY,u16 W,u16 H,u16 MaxValue,u8 Style,u8 Action,TGUI_ProcessBar *NewProcessBar)
{
	TGUI_ControlBase *P=&(NewProcessBar->Control);

	P->TP=TGUI_Type_ProcessBar;
	P->ID=ID;
    P->SX=SX;
    P->SY=SY;
    P->EX=SX+W;
    P->EY=SY+H;
	P->State=TGUI_State_UnLoad;
	P->Style=Style;
	P->Action=Action;
	P->ParentWindow=0;
	P->ReDraw=TGUI_ProcessBar_ReDraw;							//定义当前控件的重绘方法
	P->Event=TGUI_ProcessBar_Event;
	NewProcessBar->ForeColor=TGUI_ProcessBar_Fcolor;
	NewProcessBar->BackColor=TGUI_ProcessBar_Bcolor;
	NewProcessBar->BorderColor=TGUI_ProcessBar_BDcolor;
	NewProcessBar->MaxValue=MaxValue;
	NewProcessBar->CurValue=0;
	NewProcessBar->CurProcess=0;
	NewProcessBar->Changed=0;
}




