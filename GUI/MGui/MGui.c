#include <include.h>
#include <Gui_Include.h>
#include <FuncTask_Include.h>
#include <Tools_Include.h>
#include <LCD_Driver.h>
#include <Board_Driver.h>
#include <Gui_include.h>
#include <MGUI_Message.h>
#include <MGui.h>
#include <..\USER\Prj_Prafire\IO_Driver.h>
uint8 SelectIndex;
MGUI_MenuStruct *CurMenu,*CurOption;
MGUI_MenuStruct MGUI_MMS[40];
const char *YesOrNo[2]={"No","Yes"};
uint32 Font_Value=11;
uint32 TestU32=32;
f32   TestF32=32;
BOOL  TestBOOL=True;
uint8 TestASCII[10];
/******************************************************************************
 Func:创建选项
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_OptionCreate(MGUI_MenuStruct *MMS,char *Name,uintbus Value,MGUIOptionType MGOT)
{
	MMS->OptionName=Name;
	MMS->OptionValue=Value;
	MMS->NextMenu=0;
	MMS->ParentMenu=0;
	MMS->PrevMenu=0;
	MMS->SubMenu=0;
	MMS->SelectTrue=False;
	MMS->OptionType=MGOT;
}
/******************************************************************************
 Func:菜单子节点添加
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_SubMenuAdd(MGUI_MenuStruct *MMS,MGUI_MenuStruct *SubMMS)
{

	MMS->SubMenu=SubMMS;//子菜单强制覆盖
	SubMMS->ParentMenu=MMS;
}
/******************************************************************************
 Func:菜单节点添加
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_NextMenuAdd(MGUI_MenuStruct *MMS,MGUI_MenuStruct *NextMMS)
{
	MGUI_MenuStruct *CurMMS;
	CurMMS=MMS;
	while(CurMMS->NextMenu)CurMMS=CurMMS->NextMenu;
	CurMMS->NextMenu=NextMMS;//最后添加子菜单
	NextMMS->PrevMenu=CurMMS;//添加隶属菜单
	NextMMS->ParentMenu=MMS->ParentMenu;//同步属性
}
/******************************************************************************
 Func:节点删除
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_SubMenuDel(MGUI_MenuStruct *MMS)
{
	
}
/******************************************************************************
 Func:节点删除
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_NextMenuDel(MGUI_MenuStruct *MMS,MGUI_MenuStruct *NextMMS)
{

}
/******************************************************************************
 Func:获取节点的首节点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
MGUI_MenuStruct *MGUI_GetFrist(MGUI_MenuStruct *MMS)
{
	MGUI_MenuStruct *CurMMS;
	CurMMS=MMS;
	while(CurMMS->PrevMenu)
	{
		CurMMS=CurMMS->PrevMenu;
	}
	return CurMMS;
}
/******************************************************************************
 Func:菜单焦点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_SetMenu_OnFocus(MGUI_MenuStruct *MMS)
{
	CurMenu=MMS;
}
/******************************************************************************
 Func:选项节点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_SetOption_OnFocus(MGUI_MenuStruct *MMS)
{
	CurOption=MMS;
	CurOption->SelectTrue=True;
}
/******************************************************************************
 Func:跳到下一个节点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_Focus_ToNext()
{
	if(CurOption->NextMenu)//前提是要有
	{
		CurOption->NextMenu->SelectTrue=True;
		CurOption->SelectTrue=False;
		CurOption=CurOption->NextMenu;
	}
}
/******************************************************************************
 Func:跳到前一个节点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_Focus_ToPrev()
{
	if(CurOption->PrevMenu)//前提是要有
	{
		CurOption->PrevMenu->SelectTrue=True;
		CurOption->SelectTrue=False;
		CurOption=CurOption->PrevMenu;
	}
}
/******************************************************************************
 Func:跳到父节点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_Focus_ToParent()
{
	if(CurMenu->ParentMenu)//没爹就算了
	{
		MGUI_MenuReDraw(MGDM_ClrDrawOnly);
		CurOption->SelectTrue=False;
		CurMenu=CurMenu->ParentMenu;
		MGUI_SetOption_OnFocus(CurMenu);
	}
}
/******************************************************************************
 Func;跳到子节点
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
MGUIOptionType MGUI_Focus_ToSub()
{
	Func FuncLed;
	if(CurOption->SubMenu)
	{
		MGUI_MenuReDraw(MGDM_ClrDrawOnly);
		CurOption->SelectTrue=False;
		CurMenu=CurOption->SubMenu;
		MGUI_SetOption_OnFocus(CurMenu);
	}else if(CurOption->OptionType>MGOT_Menu)MGUI_GetPutIn();
	else if(CurOption->OptionType==MGOT_OptionFunc)
	{
		FuncLed=(Func)CurOption->OptionValue;
		FuncLed();
	}
	return CurOption->OptionType;
}
/******************************************************************************
 Func:重绘当前焦点菜单
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:
******************************************************************************/
void MGUI_MenuReDraw(MGUIDrawMode Mode)
{
	MGUI_MenuStruct *CurMMS;
	CurMMS=MGUI_GetFrist(CurMenu);
	TGUI_DefaultStyle();
	TGUI_Set_Font(TGUI_FontType_48X48);
	if(Mode==MGDM_ReDraw)TGUI_Set_Color(Color_Green,Magenta,Black,Null);
	if(Mode==MGDM_ClrDrawOnly)TGUI_Set_Color(Black,Black,Black,Null);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
	if(CurMMS->ParentMenu)Printf("%s",CurMMS->ParentMenu->OptionName);else Printf("Home");
	Tos_GetStyleHandle()->SY+=Tos_GetFontHandle()->SizeEH;
	LCD_Draw_LineX(Tos_GetStyleHandle()->SX,Tos_GetStyleHandle()->SY,LCD_XSIZE,Yellow);
	TGUI_Set_Font(TGUI_FontType_24X24);
	Tos_GetStyleHandle()->SY+=2;
	TGUI_Set_Region(0,Tos_GetStyleHandle()->SY,LCD_XSIZE,LCD_YSIZE,Align_LeftTop);
	while(CurMMS)
	{
		Tos_GetStyleHandle()->SelectTrue=CurMMS->SelectTrue;
		switch((uint8)CurMMS->OptionType)
		{
			case MGOT_OptionU32:Printf("%s [%d]",CurMMS->OptionName,VoidGetU32(CurMMS->OptionValue));break;
			case MGOT_OptionF32:Printf("%s [%.2f]",CurMMS->OptionName,VoidGetF32(CurMMS->OptionValue));break;
			case MGOT_OptionBool:Printf("%s [%s]",CurMMS->OptionName,YesOrNo[VoidGetBool(CurMMS->OptionValue)]);break;
			case MGOT_OptionASCII:Printf("%s [%s]",CurMMS->OptionName,CurMMS->OptionValue);break;
			default:Printf("%s",CurMMS->OptionName);break;
		}
		CurMMS=CurMMS->NextMenu;
	 Tos_GetStyleHandle()->SY+=Tos_GetFontHandle()->SizeEH;
	}
	Tos_GetStyleHandle()->SelectTrue=False;
}
/****************************************************************************
Date:2014-11-12
Func:测试函数
Note:
****************************************************************************/
void Func_Led2()
{
	static BOOL Led;
	Led=(BOOL)!Led;
#ifdef BOARD_PRAFIRE
	if(Led)LED2_ON;else LED2_OFF;
#endif
}
/****************************************************************************
Date:2014-11-12
Func:测试函数
Note:
****************************************************************************/
void Func_Led3()
{
	static BOOL Led;
	Led=(BOOL)!Led;
#ifdef BOARD_PRAFIRE
	if(Led)LED3_ON;else LED3_OFF;
#endif
}
/****************************************************************************
Date:2014-11-12
Func:测试函数
Note:
****************************************************************************/
void Func_Spk1()
{
	static BOOL Spk;
	Spk=(BOOL)!Spk;
#ifdef BOARD_LCD4_3
	if(Spk)SPK_ON;else SPK_OFF;
#endif
}
/****************************************************************************
Date:2014-11-12
Func:功能界面测试
Note:
****************************************************************************/
void Func_LookInfor()
{
		uint8 i;
//	uint16 Y;
//	uint8 Buf[20];
//	MGUIOptionType Type;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];	
//	Type=CurOption->OptionType;
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	while(1)
	{
//		Tos_DrawTable();
		//Tos_TaskTabInfor();
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
			TGUI_Set_Color(Black,Black,Black,Null);
			
			
			if(KeyMsg->KeyValue==Key_Esc)
			{
				Tos_TaskDeleteQmsg();
				return;
			}
		}
	}
}
/****************************************************************************
Date:2014-11-12
Func:菜单结构
Note:
****************************************************************************/
void MGUI_MenuCreate(MGUI_MenuStruct *MMS)
{
	MGUI_MenuStruct *CurMMS1,*CurMMS2,*CurMMS3;
	CurMMS1=MMS;
	MGUI_OptionCreate(CurMMS1,"Work Set",Null,MGOT_Menu);
	 
	  MGUI_OptionCreate(++MMS,"SD Card",Null,MGOT_Menu);CurMMS2=MMS; MGUI_SubMenuAdd(CurMMS1,CurMMS2);//    /
				MGUI_OptionCreate(++MMS,"Mount",Null,MGOT_Menu);CurMMS3=MMS; MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);  //    /
				MGUI_OptionCreate(++MMS,"UnMount",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"TypeTest",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS2,MMS);
				MGUI_OptionCreate(++MMS,"F32",(uintbus)&TestF32,MGOT_OptionF32);CurMMS3=MMS;MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);
				MGUI_OptionCreate(++MMS,"U32",(uintbus)&TestU32,MGOT_OptionU32); MGUI_NextMenuAdd(CurMMS3,MMS);
				MGUI_OptionCreate(++MMS,"BOOL",(uintbus)&TestBOOL,MGOT_OptionBool); MGUI_NextMenuAdd(CurMMS3,MMS);
	      MGUI_OptionCreate(++MMS,"ASCII",(uintbus)&TestASCII,MGOT_OptionASCII); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"Option13",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS2,MMS);
			MGUI_OptionCreate(++MMS,"Option14",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS2,MMS);
	
	  MGUI_OptionCreate(++MMS,"System Set",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS1,MMS);//   /
		
	  MGUI_OptionCreate(++MMS,"Screen Set",Null,MGOT_Menu);CurMMS2=MMS;MGUI_SubMenuAdd(CurMMS2-1,CurMMS2);
				MGUI_OptionCreate(++MMS,"Font",(uintbus)&Font_Value,MGOT_OptionU32);CurMMS3=MMS;MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);
				MGUI_OptionCreate(++MMS,"Color",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"Option22",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			MGUI_OptionCreate(++MMS,"Option23",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			MGUI_OptionCreate(++MMS,"Option24",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
	
	 MGUI_OptionCreate(++MMS,"DriverCFG",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS1,MMS);//    /
	 
	  MGUI_OptionCreate(++MMS,"Option31",Null,MGOT_Menu);CurMMS2=MMS;MGUI_SubMenuAdd(CurMMS2-1,CurMMS2);
			MGUI_OptionCreate(++MMS,"Option32",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			MGUI_OptionCreate(++MMS,"Option33",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			MGUI_OptionCreate(++MMS,"Option34",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
		
		 MGUI_OptionCreate(++MMS,"Langue Set",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS1,MMS);
	
	  MGUI_OptionCreate(++MMS,"KeyBoard",Null,MGOT_Menu);CurMMS2=MMS;MGUI_SubMenuAdd(CurMMS2-1,CurMMS2);
			MGUI_OptionCreate(++MMS,"Langue",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			MGUI_OptionCreate(++MMS,"Word Input",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			  MGUI_OptionCreate(++MMS,"pingyin",Null,MGOT_Menu);CurMMS3=MMS;MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);
				MGUI_OptionCreate(++MMS,"wubi",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"Option44",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
	  
		 MGUI_OptionCreate(++MMS,"IO Test",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS1,MMS);
	
	  MGUI_OptionCreate(++MMS,"CTRTOL",Null,MGOT_Menu);CurMMS2=MMS;MGUI_SubMenuAdd(CurMMS2-1,CurMMS2);
				MGUI_OptionCreate(++MMS,"Ctr1",Null,MGOT_Menu);CurMMS3=MMS;MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);
				MGUI_OptionCreate(++MMS,"Ctr2",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"LED",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
				MGUI_OptionCreate(++MMS,"Led1",(uintbus)&Func_Led2,MGOT_OptionFunc);CurMMS3=MMS;MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);
				MGUI_OptionCreate(++MMS,"Led2",(uintbus)&Func_Led3,MGOT_OptionFunc); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"SPK",Null,MGOT_Menu);MGUI_NextMenuAdd(CurMMS2,MMS);
			  MGUI_OptionCreate(++MMS,"Spk1",(uintbus)&Func_Spk1,MGOT_OptionFunc);CurMMS3=MMS;MGUI_SubMenuAdd(CurMMS3-1,CurMMS3);
				MGUI_OptionCreate(++MMS,"Spk2",Null,MGOT_Menu); MGUI_NextMenuAdd(CurMMS3,MMS);
			MGUI_OptionCreate(++MMS,"Func_LookInfor",(uintbus)Func_LookInfor,MGOT_OptionFunc);MGUI_NextMenuAdd(CurMMS2,MMS);
}
/****************************************************************************
Date:2014-11-12
Func:获取输入数据
Note:
****************************************************************************/
void MGUI_GetPutIn()
{
	uint8 i;
	uint16 Y;
	uint8 Buf[20];
	MGUIOptionType Type;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];	
	Type=CurOption->OptionType;
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	MGUI_MenuReDraw(MGDM_ClrDrawOnly);
	TGUI_Set_Color(Color_Green,Magenta,Black,Null);
	TGUI_Set_Font(TGUI_FontType_48X48);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
	Printf("%s",CurOption->OptionName);
	Tos_GetStyleHandle()->SY+=Tos_GetFontHandle()->SizeEH;
	LCD_Draw_LineX(Tos_GetStyleHandle()->SX,Tos_GetStyleHandle()->SY,LCD_XSIZE,Yellow);
	Tos_GetStyleHandle()->SY+=2;
	TGUI_Set_Font(TGUI_FontType_24X24);
	Y=Tos_GetStyleHandle()->SY;
	i=0;
	Tos_TaskCreateQmsg(&Msg[0],10);
	if(Type==MGOT_OptionU32)i=GetStringFromNumber(&Buf[0],*(uint32 *)CurOption->OptionValue,'D',0);
	if(Type==MGOT_OptionF32)i=GetStringFromNumber(&Buf[0],*(f32 *)CurOption->OptionValue,'D',2);
	if(Type==MGOT_OptionBool)i=StringCoppy((void *)YesOrNo[VoidGetBool(CurOption->OptionValue)],&Buf[0]);
	if(Type==MGOT_OptionASCII)i=StringCoppy((void *)CurOption->OptionValue,&Buf[0]);
	while(1)
	{
		TGUI_Set_Color(Color_White,Magenta,Black,Null);
		TGUI_Set_Region(0,Y,LCD_XSIZE,LCD_YSIZE,Align_LeftTop);
		Buf[i]=0;
		//Buf[i+1]=0;
		Printf("%s [%s]","Please PutIn!",&Buf[0]);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
			TGUI_Set_Color(Black,Black,Black,Null);
			Printf("%s [%s]","Please PutIn!",&Buf[0]);
			if(KeyMsg->KeyValue==Key_Right)
			{
				TGUI_Set_Color(Black,Black,Black,Null);
				Printf("%s [%s]","Please PutIn!",&Buf[0]);
				TGUI_Set_Font(TGUI_FontType_48X48);
				TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
				Printf("%s",CurOption->OptionName);
				Tos_TaskDeleteQmsg();
				switch((uint8)Type)
				{
					case MGOT_OptionU32:*(uint32 *)CurOption->OptionValue=GetStringToNumber(&Buf[0]);break;
					case MGOT_OptionF32:*(f32 *)CurOption->OptionValue=GetStringToNumber(&Buf[0]);break;
					case MGOT_OptionBool:*(BOOL *)CurOption->OptionValue=GetStringToBool(&Buf[0]);break;
					case MGOT_OptionASCII:StringCoppy((void *)&Buf[0],(void *)CurOption->OptionValue);break;
				}
				return;
			}
			if(KeyMsg->KeyValue==Key_Left)
			{
				TGUI_Set_Color(Black,Black,Black,Null);
				Printf("%s [%s]","Please PutIn!",&Buf[0]);
				TGUI_Set_Font(TGUI_FontType_48X48);
				TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterTop);
				Printf("%s",CurOption->OptionName);
				Tos_TaskDeleteQmsg();
				return;
			}
			switch(Type)
			{
				case MGOT_OptionU32:
					if(KeyMsg->KeyValue<='9'&&KeyMsg->KeyValue>='0')if(KeyMsg->KeyType==Key_UpIng)Buf[i++]=KeyMsg->KeyValue;
					break;
				case MGOT_OptionF32:
					if(KeyMsg->KeyValue<='9'&&KeyMsg->KeyValue>='0')if(KeyMsg->KeyType==Key_UpIng)Buf[i++]=KeyMsg->KeyValue;
				  if(KeyMsg->KeyValue==190)if(KeyMsg->KeyType==Key_UpIng)Buf[i++]='.';
				break;
				case MGOT_OptionBool:
					if(KeyMsg->KeyValue=='Y'||KeyMsg->KeyValue=='N')if(KeyMsg->KeyType==Key_UpIng)Buf[i++]=KeyMsg->KeyValue;
					if(KeyMsg->KeyValue==Key_Up)i=StringCoppy((void *)YesOrNo[1],&Buf[0]);
				  if(KeyMsg->KeyValue==Key_Down)i=StringCoppy((void *)YesOrNo[0],&Buf[0]);
					break;
				case MGOT_OptionASCII:
					if(KeyMsg->KeyValue>=' '&&KeyMsg->KeyValue<='~')if(KeyMsg->KeyType==Key_UpIng)Buf[i++]=KeyMsg->KeyValue;
				 if(KeyMsg->KeyValue==190)if(KeyMsg->KeyType==Key_UpIng)Buf[i++]='.';
					break;
				default:break;
			}
			if(KeyMsg->KeyValue==8)if(KeyMsg->KeyType==Key_UpIng)if(i)i--;
			
		}
	}
}
/****************************************************************************
Date:2014-11-12
Func:测试任务
Note:
****************************************************************************/
void MGUI_TestTask(void *Tags)
{
	uint8 i;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];
  DeBug_Get();	
	MessageBox("fsdaf");
	TGUI_DefaultStyle();
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	i=0;
  MGUI_MenuCreate(&MGUI_MMS[0]);
	MGUI_SetMenu_OnFocus(&MGUI_MMS[0]);
	MGUI_SetOption_OnFocus(&MGUI_MMS[0]);
R: Tos_TaskCreateQmsg(&Msg[0],10);
	
	Tos_TaskGetKeyBoard();
	while(1)
	{
	//	 Tos_TaskGetIO_InPut();
		 MGUI_MenuReDraw(MGDM_ReDraw);		
		 CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
			if(KeyMsg->KeyValue==Key_Down)MGUI_Focus_ToNext();//Key_Down
			if(KeyMsg->KeyValue==Key_Up)MGUI_Focus_ToPrev();//Key_Up
			if(KeyMsg->KeyValue==Key_Left)MGUI_Focus_ToParent();//Key_Left
			if(KeyMsg->KeyValue==Key_Right)
			{
				MGUI_Focus_ToSub();
				switch((uint8)CurOption->OptionType)
				{
					case MGOT_OptionF32:goto R;
					case MGOT_OptionU32:goto R;
					case MGOT_OptionBool:goto R;
					case MGOT_OptionASCII:goto R;
					case MGOT_OptionFunc:goto R;
					default:break;
				}
			}
		}
		Tos_TaskDelay(10);
	}
}
