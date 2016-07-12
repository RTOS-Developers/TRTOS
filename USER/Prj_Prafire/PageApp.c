#include <Include.h>
#include <..\USER\Prj_Prafire\PageApp.h>
#include <MGUI_Lib.c>
App_Value Run_Value;

void Task_RePaint()
{
	MenuValue->DrawMode=Draw_All;
	MenuValue->RowCount=(MenuValue->EndY-MenuValue->StartY)/Tos_GetFontHandle()->SizeEH;
}
void Task_SaveConfigs(uint8 D)
{
	Run_Value.CH.Size=sizeof(Run_Value);
	Save_AppConfig(&Run_Value,0);
	if(D)Task_Waiting(D);
}
void Task_LoadConfigs(uint8 D)
{
	if(!Load_AppConfig(&Run_Value,0)){Task_ReSave();Printf_Infor("Load Error!",1000);}
	if(D)Task_Waiting(D);
	Task_RePaint();
}
void Task_OutAction()
{
	if(Run_Value.LED[0])CTR1_ON;else CTR1_OFF;
	if(Run_Value.LED[1])CTR2_ON;else CTR2_OFF; 
	if(Run_Value.LED[2])CTR3_ON;else CTR3_OFF;
}
void Task_ReSave()
{
	Run_Value.MenuCFG.B_Color=Color_Black;
	Run_Value.MenuCFG.S_Color=Color_Red;
	Run_Value.MenuCFG.F_Color=Color_Green;
	Run_Value.MenuCFG.A_Color=Color_Blue;
	Run_Value.MenuCFG.K_Color=Color_White;
	Run_Value.MenuCFG.MenuFont=TGUI_FontType_24X24;
	Run_Value.MenuCFG.TitleFont=TGUI_FontType_48X48;
	Task_SaveConfigs(0);
}
void Task_Waiting(uint8 D)
{
	MUI_Waiting("Waiting",D);
}
void Task_ShowInfor(uint8 D)
{
	Printf_Infor("Hello!",100*D);
}
void Task_ShowAsk()
{
	Printf_Yes("Being Reset");
}
extern void Task2(void *Tags);
void Task_TaskManage(uint8 D)
{
	uint8 ID;
	ID=Tos_GetTaskIDByName((uint8 *)&Run_Value.Name[0]);
	switch(D)
	{
		case 0:
			if(ID==Tos_TaskNull){Printf_Infor("No Find Task",1000);return;}
			if(Tos_TaskDelete(ID))Printf_Infor("Delete Succeed!",1000);else Printf_Infor("Delete Fail!",1000);
			break;
		case 1:
			if(Tos_TaskCreateFindID(Task2,(uintbus)&C3D_Default,&Run_Value.Name[0],3000))Printf_Infor("Create Succeed!",1000);else Printf_Infor("Create Fail!",1000);
			break;
		case 2:
			if(ID==Tos_TaskNull){Printf_Infor("No Find Task",1000);return;}
			if(Tos_TaskSuspend(ID))Printf_Infor("Suspend Succeed!",1000);else Printf_Infor("Suspend Fail!",1000);
			break;
		case 3:
			if(ID==Tos_TaskNull){Printf_Infor("No Find Task",1000);return;}
			if(Tos_TaskResume(ID))Printf_Infor("Resume Succeed!",1000);else Printf_Infor("Resume Fail!",1000);
			break;
	}
}
void Task_ShowYesOrNo()
{
	if(Printf_YesOrNo("Sure?"))Printf_Infor("Choose Yes",1000);
	else Printf_Infor("Choose No",1000);
}
void Task_SetTitleFont(TGUI_FontType TFT)
{
	Run_Value.MenuCFG.TitleFont=TFT;
	Task_RePaint();
}

void Task_SetMenuFont(TGUI_FontType TFT)
{
	Run_Value.MenuCFG.MenuFont=TFT;
	TGUI_Set_Font(Run_Value.MenuCFG.MenuFont);
	Task_RePaint();
}
void Task_SetMsgFont(TGUI_FontType TFT)
{
	Run_Value.MenuCFG.MsgFont=TFT;
	TGUI_Set_Font(Run_Value.MenuCFG.MsgFont);
	Printf_Infor("Sample",1000);
}
void Task_SaveConfigsToNand(uint8 Page)
{
	Run_Value.CH.Size=sizeof(Run_Value);
	W25Q16_SaveConfig(&Run_Value,Page);
	Task_Waiting(3);
}
void Task_LoadConfigsFromNand(uint8 Page)
{
	Run_Value.CH.CrcValue=0;
	if(!W25Q16_LoadConfig(&Run_Value,Page))
	{
		Printf_Infor("Check Error! ReLoad Default.",1000);//加载失败，恢复默认
		Task_LoadConfigs(0);
	}
	Task_Waiting(3);
	Task_RePaint();
}

void ROM_ReadMap()
{
	uint16 X=0,Y=0,Page=0;
	uint8 *P,KeyData;
	P=Mema_Malloc(240);
	TGUI_Clear_Show();
	while(1)
	{
		Y=0;
		PrintfVoid("PageIndex[%d] UpDownExit",Page);
		for(X=0;X<320;X+=8)
		{
		Flash_ReadData(FLASH_ADDR_BASE+Y++*240+40*Page*240,&P[0],240);
		Draw_Full_List(&P[0],240,X,0,Color_Red,Color_Blue,Color_Green);
		}		
		KeyData=MUI_GetKey(0);
		if(KeyData==Key_Left)if(Page>0)Page--;
		if(KeyData==Key_Right)Page++;
		if(KeyData==Key_Down)break;
		if(KeyData==Key_Up)break;
	}
	Mema_Free(P);
	Task_RePaint();
}
void RAM_ReadMap()
{
	uint16 X=0,Y=0,Page=0;
	uint8 *P,KeyData;
	P=Mema_Malloc(240);
	TGUI_Clear_Show();
	while(1)
	{
		Y=0;
		PrintfVoid("PageIndex[%d] UpDownExit",Page);
		for(X=0;X<2;X+=8)
		{
		Flash_ReadData(0x02000000+Y++*240+40*Page*240,&P[0],240);
		Draw_Full_List(&P[0],240,X,0,Color_Red,Color_Blue,Color_Green);
		}		
		KeyData=MUI_GetKey(0);
		if(KeyData==Key_Left)if(Page>0)Page--;
		if(KeyData==Key_Right)Page++;
		if(KeyData==Key_Down)break;
		if(KeyData==Key_Up)break;
	}
	Mema_Free(P);
	Task_RePaint();
}
void Nand_ReadMap()
{
	uint16 X=0,Y=0,Page=0;
	uint8 *P,KeyData;
	P=Mema_Malloc(240);
	TGUI_Clear_Show();
	while(1)
	{
		Y=0;
		PrintfVoid("PageIndex[%d] UpDownExit",Page);
		for(X=0;X<320;X+=8)
		{
		SPI_FLASH_BufferRead(&P[0],Y++*240+40*Page*240,240);
		Draw_Full_List(&P[0],240,X,0,Color_Red,Color_Blue,Color_Green);
		}		
		KeyData=MUI_GetKey(0);
		if(KeyData==Key_Left)if(Page>0)Page--;
		if(KeyData==Key_Right)Page++;
		if(KeyData==Key_Down)break;
		if(KeyData==Key_Up)break;
	}
	Mema_Free(P);
	Task_RePaint();
}
void Task_ShowTos()
{
	uint8 i=10;
	MUI_ReSetTitle("About TOS");
	for(i=6;i;i--)
	{
	MGUI_DrawBitMap((LCD_XSIZE-TOSLogoLib[0])/2,(LCD_YSIZE-TOSLogoLib[1])/2,(uintbus)TOSLogoLib,i);
	Tos_TaskDelay(300);	
	}
	MUI_SetDrawClear(True);
	MGUI_DrawBitMap((LCD_XSIZE-TOSLogoLib[0])/2,(LCD_YSIZE-TOSLogoLib[1])/2,(uintbus)TOSLogoLib,0);
}
void MUI_Init(const MUI_MenuStruct *CurMenu)
{
	uint8 i;
	MenuValue->DrawMode=Draw_All;
	MenuValue->Title=" ";
	for(i=0;i<5;i++)MenuValue->DeepRecord[i]=0;
	MenuValue->Index=0;//菜单深度
	MenuValue->CurMenu=CurMenu;
	TGUI_Set_Font(Run_Value.MenuCFG.MenuFont);
	MenuValue->StartX=0;
	MenuValue->StartY=50;
	MenuValue->EndX=LCD_XSIZE;
	MenuValue->EndY=LCD_YSIZE;
	MenuValue->RowCount=(MenuValue->EndY-MenuValue->StartY)/Tos_GetFontHandle()->SizeEH;
	MenuValue->ColumnCount=2;
	TGUI_Set_LineN(1);
}

void MUI_Task(void *Tags)
{
	uint8 i,KeyData;
	Tos_ArrayMessage *ArrayMsg;
	MGUI_KeyMsg *KeyMsg;
	const MUI_MenuStruct *CurMenu=(MUI_MenuStruct *)Tags;
	MenuValue=Mema_Malloc(sizeof(MUI_MenuValue));
	KeyMsg=Mema_Malloc(sizeof(MGUI_KeyMsg)*3);
	ArrayMsg=Mema_Malloc(sizeof(Tos_ArrayMessage)*3);
	for(i=0;i<3;i++)ArrayMsg[i].Qmsg_Addr=&KeyMsg[i];
	Tos_TaskCreateQmsg(ArrayMsg,3);
	TGUI_DefaultStyle();
	Tos_TaskGetKeyBoard();
	Tos_TaskGetIO_InPut();
	Task_LoadConfigs(0);
	MUI_SetDrawClear(True);
	TGUI_Clear_Show();
	MUI_Init(CurMenu);
	while(1)
	{
		MenuValue->Title=(char *)MUI_GetCurMenuInfor()->Option;
		MUI_Draw(MenuValue->CurMenu);
		MenuValue->OptionLength=MUI_OptionLength(MenuValue->CurMenu);
		KeyData=MUI_GetKey(0);
		if(KeyData==Key_Down)if((MenuValue->OptionLength-1)>MenuValue->DeepRecord[MenuValue->Index])MenuValue->DeepRecord[MenuValue->Index]++;else MenuValue->DeepRecord[MenuValue->Index]=0;
		if(KeyData==Key_Up)if(MenuValue->DeepRecord[MenuValue->Index])MenuValue->DeepRecord[MenuValue->Index]--;else MenuValue->DeepRecord[MenuValue->Index]=MenuValue->OptionLength-1;
		if(KeyData==Key_Right)MUI_Enter(MenuValue->CurMenu+MenuValue->DeepRecord[MenuValue->Index]);
		if(KeyData==Key_Left)MenuValue->DeepRecord[MenuValue->Index]=0;
		MenuValue->DrawMode|=Draw_Menu;
		Task_OutAction();
	}
}

