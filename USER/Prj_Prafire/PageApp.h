#ifndef _PAGEAPP_H_
#define _PAGEAPP_H_
#include <Include.h>
#include <AppConfig.h>
#include <TGui_GBk_Driver.h>
#include <..\USER\Prj_Prafire\Menu_Base.h>
typedef struct
{
	ConfigHeadStruct CH;
	char Name[10];
	uint8 Age;
	uint8 Hight;
	f32 Weight;
	MUI_MenuConfigs MenuCFG;
	BOOL DeBugEn;
	BOOL LED[3];
	uint8 LifePlayBuf[10];
	uint32 IP;
	uint32 LocalHost;
	uint32 SubnetMask;
	uint32 GateWay;
}App_Value;

extern App_Value Run_Value;
void Task_SaveConfigs(uint8 D);
void Task_LoadConfigs(uint8 D);
void Task_ShowTos(void);
void ROM_ReadMap(void);
void Nand_ReadMap(void);
void RAM_ReadMap(void);
void Task_ShowInfor(uint8 D);
void Task_ShowAsk(void);
void Task_ShowYesOrNo(void);
void Task_Waiting(uint8 D);
void Task_TaskManage(uint8 D);
void Task_ReSave(void);
void Task_SetTitleFont(TGUI_FontType TFT);
void Task_SetMenuFont(TGUI_FontType TFT);
void Task_SetMsgFont(TGUI_FontType TFT);
void Task_OutAction(void);
void Task_SaveConfigsToNand(uint8 Page);
void Task_LoadConfigsFromNand(uint8 Page);
void MUI_Task(void *Tags);
#endif

