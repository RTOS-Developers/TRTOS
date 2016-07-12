#ifndef _APP1_TANKHAND_H_
#define _APP1_TANKHAND_H_

#include <Include.h>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.h>

extern DevRun_ValueStruct DevRun_Value;
float Get_BatteryVoltage(void);
void Click_CorrectWeight(void);
void Click_ClearWeight(void);
void Click_CorrectRange(void);
void Click_CorrectFullWeight(void);
void Click_ClearRange(void);
BOOL Send_ButtonCommand(uint8 Com);
void Draw_Work(void);
void Draw_Home(void);
void Click_ShowRange(void);
void Click_ShowWeight(void);
void Click_ResetFrq(void);
void Load_Configs(void);
void Save_Configs(void);
void Save_Consig(void);
void Set_Brightness(void);
void Click_SaveValue(void);
void Beep_WithKey(void);
void LampOutShow(void);
void TankPowerClick(void);
void TankRelieveClick(void);
BOOL Send_TankButtonCommand(uint8 Com,BOOL ShowEn);
BOOL CheckPowerLow(float Vol);
void StopSys_Doing(void);
void Click_ChangePassword(void);
void Click_SetPassword(void);
BOOL Admin_GetRoot(void);
void Click_ReadValue(void);
void Click_RegLic(void);
BOOL Start_CheackLic(void);
void Send_OnlyCommand(uint8 Cmd);
BOOL StartCheck(void);
#endif
