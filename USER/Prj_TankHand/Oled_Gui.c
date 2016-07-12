#include <Include.h>
#include <..\USER\Prj_TankHand\Oled_GUI.h>
#include <..\USER\Prj_TankHand\App_TankHand.h>
#include <..\USER\Prj_TankHand\Local_GBKLib.h>
#include <GUI_Include.h>
const uint8 GetDataCode[]={"0123456789.-+"};
MenuStruct PageMenu;	
OLED_MenuStyle *CurStyle,OMStyle;
const OLED_MenuStruct *CurMenu;

const char *BoolString[2]={"假","真 "};
/**************************************************************************************
 Func: 菜单初始化
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Menu_Init(const OLED_MenuStruct *MenuHome)
{
	TGUI_DefaultStyle();
	CurMenu=&MenuHome[0];
	PageMenu.DrawMode=Draw_Title|Draw_Value;
	PageMenu.Index=5;
	while(PageMenu.Index--)PageMenu.DeepRecord[PageMenu.Index]=0;
	PageMenu.Index=0;
	CurStyle=&OMStyle;
	CurStyle->StartX=0;
	CurStyle->StartY=0;
	CurStyle->EndX=127;
	CurStyle->EndY=50;
	CurStyle->CursorEnable=True;
	CurStyle->RowCount=3;
	CurStyle->ColumnCount=1;
	CurStyle->MS=&PageMenu;
	TGUI_DefaultStyle();
	Set_LocalFont(FontType_Min);

}
MGUI_KeyMsg * MUI_GetKey(uint16 D)
{
	Tos_ArrayMessage *CurMsg;
	MGUI_KeyMsg *CurKeyMsg;
	if(D)CurMsg=Tos_TaskGetQMsgDelay(D);
	else CurMsg=Tos_TaskGetQMsg();
	if(CurMsg)
	{
		CurKeyMsg=(MGUI_KeyMsg *)CurMsg->Qmsg_Addr;
		return CurKeyMsg;
	}
	return Null;
}
/**************************************************************************************
 Func: 获取当前菜单长度
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
uint8 GetMenu_Length()
{
	uint8 i=0;
	const OLED_MenuStruct *Menu;
	Menu=(const OLED_MenuStruct *)CurMenu;
	while(Menu->Option)
	{
		Menu++;
		i++;
	}
	return (i-1);
}
/**************************************************************************************
 Func: 设置显示模式
 Time: 2015-1-10
 Ver.: V1.0
 Note;正常显示1显示填充2翻转显示3清除显示4
**************************************************************************************/
void SetDraw_Mode(uint8 Mode)
{
	switch(Mode)
	{
		case WDM_Mirror://字符反打印
			Tos_GetStyleHandle()->SelectTrue=True;
		  TGUI_Set_Color(1,0,1,Null);
			break;
		case WDM_Normal://字符正常打印
		  TGUI_Set_Color(1,0,0,1);
		  Tos_GetStyleHandle()->SelectTrue=False;
			break;
		case WDM_Clear://清除字符显示
			TGUI_Set_Color(1,0,0,Null);
		  Tos_GetStyleHandle()->SelectTrue=True;
			break;
		case WDM_Fill://填充字符位置
			TGUI_Set_Color(1,1,1,Null);
			Tos_GetStyleHandle()->SelectTrue=False;
			break;
		default: break;
	}
}
/**************************************************************************************
 Func: 绘制菜单按照指定格式
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Draw_Menu()
{
	uint8 x,y,i=0,PageCount,Addr_X,Addr_Y;
	const OLED_MenuStruct *Menu;
	Addr_X=CurStyle->StartX;
	Addr_Y=CurStyle->StartY;
	Menu=CurMenu;
	Set_LocalFont(FontType_Min);
	CurStyle->MS->OptionLength=GetMenu_Length();
	PageCount=(CurStyle->MS->DeepRecord[CurStyle->MS->Index]/(CurStyle->ColumnCount*CurStyle->RowCount));
	Menu+=(PageCount*(CurStyle->ColumnCount*CurStyle->RowCount));
	for(x=0;x<CurStyle->ColumnCount;x++)
	{
		Addr_X=((CurStyle->EndX-CurStyle->StartX)/CurStyle->ColumnCount)*x;Addr_Y=0;
		for(y=0;y<CurStyle->RowCount;y++)
		{
		  if(Menu->Option==0)return;
		  TGUI_Set_Region(Addr_X,Addr_Y,Addr_X+(CurStyle->EndX-CurStyle->StartX)/CurStyle->ColumnCount,Addr_Y+(CurStyle->EndY-CurStyle->StartX)/CurStyle->RowCount,Align_LeftTop);
			if((CurStyle->MS->DeepRecord[CurStyle->MS->Index]==(PageCount*(CurStyle->ColumnCount*CurStyle->RowCount))+(i++))&(CurStyle->CursorEnable))
			{
				SetDraw_Mode(3);
				CurStyle->MS->SelectMenu=(uintbus)Menu;
			}else SetDraw_Mode(1);
			switch(Menu->Type)
			{
				case VT_Float: Printf((char *)Menu->Option,*(float *)Menu->Value);break;
			  case VT_Uint16:Printf((char *)Menu->Option,*(uint16 *)Menu->Value);break;
				case VT_Int8:Printf((char *)Menu->Option,*(int8 *)Menu->Value);break;
				case VT_ROMFloat:Printf((char *)Menu->Option,*(float *)Menu->Value);break;
				case VT_ROMUint32:Printf((char *)Menu->Option,*(uint32 *)Menu->Value);break;
				case VT_ROMUint16:Printf((char *)Menu->Option,*(uint16 *)Menu->Value);break;
				case VT_ROMInt16:Printf((char *)Menu->Option,*(int *)Menu->Value);break;
				case VT_Uint8:Printf((char *)Menu->Option,*(uint8 *)Menu->Value);break;
				case VT_ROMUint8:Printf((char *)Menu->Option,*(uint8 *)Menu->Value);break;
				case VT_ROMStr:Printf((char *)Menu->Option,(char *)*(uint32 *)Menu->Value);break;//指针指向的指针地址的数据指针
				case VT_ROMInt8:Printf((char *)Menu->Option,*(int8 *)Menu->Value);break;
				case VT_ROMBOOL:Printf((char *)Menu->Option,BoolString[((*(uint32 *)Menu->Value)&(Menu->Note))>0]);break;
				default:Printf((char *)Menu->Option);
			}
	    Menu++;
			Addr_Y+=(CurStyle->EndY-CurStyle->StartX)/CurStyle->RowCount;
		}
	}
}
void Page_Title(char *Title)
{
	Set_LocalFont(FontType_Min);
	SetDraw_Mode(1);
	TGui_Draw_Line(0,14,127,14,1);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
	Printf(Title);
}
/*******************************************************************************
Func:消息对话框
Date:2014-10-9
Note:消息
*******************************************************************************/
BOOL Printf_Infor(char *Msg,uint16 Delay)
{
	LCD_Clear_Screen(0x00);
	Set_LocalFont(FontType_Min);
	Page_Title("信息");
	Set_LocalFont(FontType_Max);
	SetDraw_Mode(1);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterCenter);
	Printf((char *)Msg);
	SSD1325_OutBuffer();
	Tos_TaskDelay(Delay);
	PageMenu.DrawMode=Draw_Title|Draw_Value;
	Set_LocalFont(FontType_Min);
	return True;
}
BOOL Printf_Message(char *Msg,uint16 Delay)
{
	LCD_Clear_Screen(0x00);
	Set_LocalFont(FontType_Min);
	Page_Title("信息");
	Set_LocalFont(FontType_Min);
	SetDraw_Mode(1);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterCenter);
	Printf((char *)Msg);
	SSD1325_OutBuffer();
	Tos_TaskDelay(Delay);
	PageMenu.DrawMode=Draw_Title|Draw_Value;
	Set_LocalFont(FontType_Min);
	return True;
}
BOOL PrintfVoid(char *fmt,...)
{
	va_list ap;
	char String[50];
	va_start(ap,fmt);
	vsprintf(String,fmt,ap);
	Printf_Infor(String,1000);
	va_end(ap);
	return True;
}

/*******************************************************************************
Func:确认对话框
Date:2014-10-9
Note:是
*******************************************************************************/
BOOL Printf_Yes(char *Msg)
{
  MGUI_KeyMsg *KeyMsg;
	LCD_Clear_Screen(0x00);
	Set_LocalFont(FontType_Min);
  SetDraw_Mode(1);
  TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
	Printf(Msg);
	Set_LocalFont(FontType_Max);
  SetDraw_Mode(3);
  TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterCenter);
	Printf("Ok");
	SSD1325_OutBuffer();
	while(1)
	{
			KeyMsg=MUI_GetKey(0);
			if(KeyMsg->KeyValue==Key_1)goto R;
	}
	R:Set_LocalFont(FontType_Min);
  return True; 
}
/*******************************************************************************
Func:询问对话框
Date:2014-10-9
Note:是否
*******************************************************************************/
BOOL Printf_YesOrNo(char *Msg)
{
  uint8 i;
  MGUI_KeyMsg *KeyMsg;
	LCD_Clear_Screen(0x00);
	i=0;
	while(1)
	{
		  Set_LocalFont(FontType_Min);
			SetDraw_Mode(1);
			TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
			Printf(Msg);
			Set_LocalFont(FontType_Max);
			TGUI_Set_Region(LCD_XSIZE*1/4,0,LCD_XSIZE*3/4,LCD_YSIZE,Align_LeftCenter);
			if(i==0)SetDraw_Mode(1);else SetDraw_Mode(3);
			Printf("Ok");
			TGUI_Set_Region(LCD_XSIZE*1/4,0,LCD_XSIZE*3/4,LCD_YSIZE,Align_RightCenter);
			if(i==1)SetDraw_Mode(1);else SetDraw_Mode(3);
			Printf("No");
			SSD1325_OutBuffer();
			KeyMsg=MUI_GetKey(0);
			if(KeyMsg->KeyValue==Key_A)i=!i;
			if(KeyMsg->KeyValue==Key_B)i=!i;
			if(KeyMsg->KeyValue==Key_1)goto R;
			if(KeyMsg->KeyValue==Key_3)goto R;
			if(KeyMsg->KeyValue==Key_Y)if(KeyMsg->KeyTime>0)i=!i;
			if(KeyMsg->KeyValue==Key_Y)if(KeyMsg->KeyTime<0)i=!i;
			if(KeyMsg->KeyValue==Key_X)goto R;
	}
	R:Set_LocalFont(FontType_Min);
  return (BOOL)i; 
}
/*******************************************************************************
Func:打印数据
Date:2014-10-9
Note:按位打印
*******************************************************************************/
void Printf_Data(uint8 *Buf,uint8 Length,uint8 Cursor)
{
    uint8 i;
    uint16 x;
    uint8 Buf1[2];
    Set_LocalFont(FontType_Max);
    x=(LCD_XSIZE-Tos_GetFontHandle()->SizeEW*Length)/2;
    if(PageMenu.DrawMode&Draw_Value)
    for(i=1;(i<Length-1);i++)
    {
        TGUI_Set_Region(x+i*Tos_GetFontHandle()->SizeEW,0,x+(i+1)*Tos_GetFontHandle()->SizeEW,LCD_YSIZE,Align_LeftCenter);
        Buf1[0]=Buf[i-1];
        Buf1[1]=0;
        if(i==Cursor)SetDraw_Mode(3);
        else SetDraw_Mode(1);
        Printf((char *)&Buf1[0]);
    }
		TGUI_Set_Region(0,0,LCD_XSIZE-1,LCD_YSIZE,Align_LeftCenter);
		if(Cursor==0)SetDraw_Mode(3);else SetDraw_Mode(1);
		 Printf("No");
		TGUI_Set_Region(0,0,LCD_XSIZE-1,LCD_YSIZE,Align_RightCenter);
		if(Cursor==5)SetDraw_Mode(3);else SetDraw_Mode(1);
		 Printf("Ok");
		
}
uint8 Index_Uchar(const uint8 *Msg,uint8 Key)
{
    uint8 Index=0;
    while(1)
    {         
        if(Msg[Index]==Key)return Index; 
        if(Msg[Index]==0)return 0;
        Index++;
    }
}
void Set_LocalFont(TGUI_FontType FontType)
{
	TGUI_Set_Font(FontType);
	Tos_GetFontHandle()->P_CLib=&LGBK1212Lib[0];
	Tos_GetFontHandle()->P_CIndex=&LGBK1212_Index[0];
	Tos_GetFontHandle()->LibCCount=GetCountSize(Tos_GetFontHandle()->P_CIndex);
}
void Key_GetASCII(MGUI_KeyMsg *KeyMsg,uint8 *Data)
{
   uint8 Index;
   if(KeyMsg->KeyValue==Key_X)if(KeyMsg->KeyTime<0)
   {
       Index=Index_Uchar(GetDataCode,*Data);
       Index++;
       if(Index>=(sizeof(GetDataCode)-1))Index=0;
       *Data=GetDataCode[Index];
   }
   if(KeyMsg->KeyValue==Key_X)if(KeyMsg->KeyTime>0)
   {
       Index=Index_Uchar(GetDataCode,*Data);
       if(Index==0)Index=sizeof(GetDataCode)-1;
       Index--;
       *Data=GetDataCode[Index];
   }
	 if(KeyMsg->KeyValue==Key_A)
	 {
		  Index=Index_Uchar(GetDataCode,*Data);
       Index++;
       if(Index>=(sizeof(GetDataCode)-1))Index=0;
       *Data=GetDataCode[Index];
	 }
	 if(KeyMsg->KeyValue==Key_B)
	 {
		 Index=Index_Uchar(GetDataCode,*Data);
       if(Index==0)Index=sizeof(GetDataCode)-1;
       Index--;
       *Data=GetDataCode[Index];
	 }
}
void Printf_InforFail()
{
		Printf_Message("错误！",1000);
}

float Page_IfGetData(float Value, BOOL *IfOk)
{
	uint8 Index=0;
	float Temp;
	uint8 Buf[10];  
	MGUI_KeyMsg *KeyMsg;	
	SetDraw_Mode(1);
	PageMenu.DrawMode=Draw_Title|Draw_Value;
	GetStringFromNumber(&Buf[0],Value,'D',3);
	while(1)
	{
		  Page_Title("请输入");
		  TGUI_Draw_Rectangle(LCD_XSIZE*5/20+1,LCD_YSIZE*3/10,LCD_XSIZE*15/20,LCD_YSIZE*7/10,1);
			Printf_Data(&Buf[0],6,Index);
			SSD1325_OutBuffer();
			KeyMsg=MUI_GetKey(0);
			if(KeyMsg->KeyValue==Key_Y)if(KeyMsg->KeyTime<0)if(Index)Index--;
			if(KeyMsg->KeyValue==Key_Y)if(KeyMsg->KeyTime>0)if(Index<5)Index++;
			if(KeyMsg->KeyValue==Key_1)if(Index)Index--;
			if(KeyMsg->KeyValue==Key_3)if(Index<5)Index++;
			if(Index>0&Index<5)Key_GetASCII(KeyMsg,&Buf[Index-1]);
			if(Index==0)if(KeyMsg->KeyValue==Key_X)if(KeyMsg->KeyTime>0)goto NOSAVE;
			if(Index==5)if(KeyMsg->KeyValue==Key_X)if(KeyMsg->KeyTime>0)goto YESSAVE;
			if(Index==0)if(KeyMsg->KeyValue==Key_X)if(KeyMsg->KeyTime<0)goto NOSAVE;
			if(Index==5)if(KeyMsg->KeyValue==Key_X)if(KeyMsg->KeyTime<0)goto YESSAVE;
			if(Index==0)if(KeyMsg->KeyValue==Key_A)goto NOSAVE;
			if(Index==5)if(KeyMsg->KeyValue==Key_A)goto YESSAVE;
			if(Index==0)if(KeyMsg->KeyValue==Key_B)goto NOSAVE;
			if(Index==5)if(KeyMsg->KeyValue==Key_B)goto YESSAVE;
	}
YESSAVE:
	Buf[4]=0;
	Temp=GetStringToNumber(&Buf[0]);*IfOk=True;return Temp;
NOSAVE:
	*IfOk=False;return Value;
}
/*******************************************************************************
Func:获取数据
Date:2014-10-9
Note:
*******************************************************************************/
float Page_GetData(float Value)
{
	BOOL Yes;
	return Page_IfGetData(Value,&Yes);
}

/*******************************************************************************
Func:等待窗口
Date:2014-10-9
Note:
*******************************************************************************/
void Printf_Waiting(char *Msg,uint16 Delay)
{
	uint16 Max;
	Max=Delay;
	LCD_Clear_Screen(0x00);
	Set_LocalFont(FontType_Min);
	SetDraw_Mode(1);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
	while(Delay--)
	{
		Tos_TaskDelay(10);
		TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
		Printf(Msg);
		TGUI_Draw_Rectangle(LCD_XSIZE*1/10,LCD_YSIZE*7/10,LCD_XSIZE*9/10,LCD_YSIZE*9/10,1);
		TGUI_Fill_Bar(LCD_XSIZE*1/10+1,LCD_YSIZE*7/10+1,LCD_XSIZE*9/10-10,LCD_YSIZE*9/10-1,Max,Max-Delay);
		SSD1325_OutBuffer();
	}
	Set_LocalFont(FontType_Min);
}


/**************************************************************************************
 Func: 菜单事件处理
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Enter_Menu()
{
	Func Task;
	VT_MenuFuncValue *FuncValue;
	const OLED_MenuStruct *Menu;
	Menu=(const OLED_MenuStruct *)CurStyle->MS->SelectMenu;
	switch(Menu->Type)
			{
				case VT_Menu:
					   CurMenu=(const OLED_MenuStruct *)Menu->Value;
				     if(Menu->Note>PageMenu.Index)PageMenu.DeepRecord[Menu->Note]=0;
						  PageMenu.Index=Menu->Note;break;                                                                                                                          
				case VT_Float:*(float *)(Menu->Value)=Page_GetData(*(float *)(Menu->Value));break;
				case VT_Int8:*(int8 *)(Menu->Value)=Page_GetData(*(int8 *)(Menu->Value));break;
				case VT_Uint8:*(uint8 *)(Menu->Value)=Page_GetData(*(uint8 *)(Menu->Value));break;
				case VT_ROMFloat:break;
				case VT_Task:Task=(Func)Menu->Value;if(NullFunc==Task)Printf_Infor("Not Build",1000);else Task();break;
				case VT_TaskF32:FuncValue=(VT_MenuFuncValue*)Menu->Value;
				FuncValue->Active(Page_GetData(*(float *)(FuncValue->Value)));
				break;
			}
}                                                                             

