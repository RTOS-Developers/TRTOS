#include <Include.h>
#include <Tools.h>
#include <..\USER\Prj_Prafire\Page.h>
#include <..\USER\Prj_Prafire\PageApp.h>

const MUI_MenuStruct Menu0x1100[]={
	{"(1)BACK",(void *)&Menu0x1000[0],VT_Menu,1},
	{"(2)Uint8:%d",(void *)&Run_Value.Age,VT_Uint8,3},
	{"(3)Uint8:%d",(void *)&Run_Value.Hight,VT_Uint8,3},
	{"(4)Int8:%d",(void *)&Run_Value.Hight,VT_Int8,4},
	{"(5)Float:%.3f",(void *)&Run_Value.Weight,VT_Float,5},
	{"(6)IP:%d.%d.%d.%d",(void *)&Run_Value.IP,VT_IP,},
	{"(7)String:%s",(void *)&Run_Value.Name[0],VT_Str,sizeof(Run_Value.Name)-1},
	{"(8)SaveConfigs",(void *)&Task_SaveConfigs,VT_TaskUint8,1},
	{"(9)LoadConfigs",(void *)&Task_LoadConfigs,VT_TaskUint8,1},
	{"UpPowerData",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x1200[]={
	{"(1)BACK",(void *)&Menu0x1000[0],VT_Menu,1},
	{"(2)Infor",(void *)&Task_ShowInfor,VT_TaskUint8,10},
	{"(3)Ask",(void *)&Task_ShowAsk,VT_Task,0},
	{"(3)YesOrNo",(void *)&Task_ShowYesOrNo,VT_Task,0},
  {"(3)Waiting",(void *)&Task_Waiting,VT_TaskUint8,3},
	{"MessageShow",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x1300[]={
	{"(1)BACK",(void *)&Menu0x1000[0],VT_Menu,1},
	{"(2)Name:%s",(void *)&Run_Value.Name[0],VT_Str,sizeof(Run_Value.Name)-1},
  {"(3)Delete",(void *)&Task_TaskManage,VT_TaskUint8,0},
	{"(4)Create",(void *)&Task_TaskManage,VT_TaskUint8,1},
	{"(5)Suspeed",(void *)&Task_TaskManage,VT_TaskUint8,2},
  {"(6)Resume",(void *)&Task_TaskManage,VT_TaskUint8,3},
	{"(7)Use_Ram:%d",(void *)&Tos_CPUInfor.Use_Ram,VT_ROMUint32,0},
	{"TaskManage",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x1000[]={
	{"(1)BACK",(void *)&MenuHome[0],VT_Menu,0},
	{"(2)UpPowerData",(void *)&Menu0x1100[0],VT_Menu,2},
	{"(3)MessageShow",(void *)&Menu0x1200[0],VT_Menu,2},
	{"(4)TosTask",(void *)&Menu0x1300[0],VT_Menu,2},
	{"FuncTask",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x2100[]={
	{"(1)BACK",(void *)&Menu0x2000[0],VT_Menu,1},
	{"(2)LED1:%s",(void *)&Run_Value.LED[0],VT_BOOL,1},
	{"(3)LED2:%s",(void *)&Run_Value.LED[1],VT_BOOL,1},
	{"(4)LED3:%s",(void *)&Run_Value.LED[2],VT_BOOL,1},
	{"LED",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x2200[]={
	{"(1)BACK",(void *)&Menu0x2000[0],VT_Menu,1},
	{"(2)WriteCFGToPage1",(void *)&Task_SaveConfigsToNand,VT_TaskUint8,0},
	{"(3)ReadCFGFromPage1",(void *)&Task_LoadConfigsFromNand,VT_TaskUint8,0},
	{"(4)WriteCFGToPage2",(void *)&Task_SaveConfigsToNand,VT_TaskUint8,1},
	{"(5)ReadCFGFromPage2",(void *)&Task_LoadConfigsFromNand,VT_TaskUint8,1},
	{"W25Q16",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x2000[]={
	{"(1)BACK",(void *)&MenuHome[0],VT_Menu,0},
	{"(2)LED",(void *)&Menu0x2100[0],VT_Menu,2},
	{"(3)W25Q16",(void *)&Menu0x2200[0],VT_Menu,2},
	{"HandTest",Null,VT_End,Null},
};
uint32 _Tos_Cpu_Freq=Tos_Cpu_Freq;
const MUI_MenuStruct Menu0x3100[]={
	{"(1)BACK",(void *)&Menu0x3000[0],VT_Menu,1},
  {"(2)CPU_Model:%s",(void *)CPU_Model,VT_ROMStr,0},
	{"(3)Tos_Cpu_Freq:%d",(void *)&_Tos_Cpu_Freq,VT_ROMUint32,0},
	{"(4)BoardName:%s",(void *)Board_Name,VT_ROMStr,0},
	{"Hand",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x3200[]={
	{"(1)BACK",(void *)&Menu0x3000[0],VT_Menu,1},
	{"(2)Use_CPU:%d  ",(void *)&Tos_CPUInfor.Userate_CPU,VT_ROMUint8,0},
  {"(3)Use_Ram:%d",(void *)&Tos_CPUInfor.Use_Ram,VT_ROMUint32,0},
	{"(3)DeviceCount:%d",(void *)&DeviceId_Index,VT_ROMUint8,0},
	{"(4)TaskCount:%d",(void *)&GetTosTask_Count,VT_Uint8Task,0},
	{"Soft",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x3300[]={
	{"(1)BACK",(void *)&Menu0x3000[0],VT_Menu,1},
	{"(2)NandFlash",(void *)&Nand_ReadMap,VT_Task,3},
	{"(3)InsideROM",(void *)&ROM_ReadMap,VT_Task,3},
	{"(4)InsideRAM",(void *)&RAM_ReadMap,VT_Task,3},
	{"ReadMap",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x3000[]={
	{"(1)BACK",(void *)&MenuHome[0],VT_Menu,0},
	{"(2)AboutHand",(void *)&Menu0x3100[0],VT_Menu,2},
	{"(3)AboutSoft",(void *)&Menu0x3200[0],VT_Menu,2},
	{"(4)ReadMap",(void *)Menu0x3300,VT_Menu,2},
	{"(5)AboutTOS",(void *)&Task_ShowTos,VT_Task,0},
	{"About",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4100[]={
	{"(1)BACK",(void *)&Menu0x4000[0],VT_Menu,2},
  {"(2)B_Color:%.03d.%.03d.%.03d",(void *)&Run_Value.MenuCFG.B_Color,VT_Color,5},
	{"(3)F_Color:%.03d.%.03d.%.03d",(void *)&Run_Value.MenuCFG.F_Color,VT_Color,5},
	{"(4)S_Color:%.03d.%.03d.%.03d",(void *)&Run_Value.MenuCFG.S_Color,VT_Color,5},
	{"(5)A_Color:%.03d.%.03d.%.03d",(void *)&Run_Value.MenuCFG.A_Color,VT_Color,5},
	{"(5)K_Color:%.03d.%.03d.%.03d",(void *)&Run_Value.MenuCFG.K_Color,VT_Color,5},
	{"(6)SaveConfigs",(void *)&Task_SaveConfigs,VT_TaskUint8,1},
	{"Color",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4210[]={
	{"(1)BACK",(void *)&Menu0x4200[0],VT_Menu,2},
	{"(2)FontType12X12",(void *)&Task_SetTitleFont,VT_TaskUint8,TGUI_FontType_12X12},
	{"(3)FontType16X16",(void *)&Task_SetTitleFont,VT_TaskUint8,TGUI_FontType_16X16},
	{"(4)FontType16X24",(void *)&Task_SetTitleFont,VT_TaskUint8,TGUI_FontType_16X24},
	{"(5)FontType24X24",(void *)&Task_SetTitleFont,VT_TaskUint8,TGUI_FontType_24X24},
	{"(6)FontType32X32",(void *)&Task_SetTitleFont,VT_TaskUint8,TGUI_FontType_32X32},
	{"(7)FontType48X48",(void *)&Task_SetTitleFont,VT_TaskUint8,TGUI_FontType_48X48},
	{"TitleFont",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4220[]={
	{"(1)BACK",(void *)&Menu0x4200[0],VT_Menu,2},
	{"(2)FontType12X12",(void *)&Task_SetMenuFont,VT_TaskUint8,TGUI_FontType_12X12},
	{"(3)FontType16X16",(void *)&Task_SetMenuFont,VT_TaskUint8,TGUI_FontType_16X16},
	{"(4)FontType16X24",(void *)&Task_SetMenuFont,VT_TaskUint8,TGUI_FontType_16X24},
	{"(5)FontType24X24",(void *)&Task_SetMenuFont,VT_TaskUint8,TGUI_FontType_24X24},
  {"(6)FontType32X32",(void *)&Task_SetMenuFont,VT_TaskUint8,TGUI_FontType_32X32},
	{"(7)FontType48X48",(void *)&Task_SetMenuFont,VT_TaskUint8,TGUI_FontType_48X48},
	{"MenuFont",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4230[]={
	{"(1)BACK",(void *)&Menu0x4200[0],VT_Menu,2},
	{"(2)FontType12X12",(void *)&Task_SetMsgFont,VT_TaskUint8,TGUI_FontType_12X12},
	{"(3)FontType16X16",(void *)&Task_SetMsgFont,VT_TaskUint8,TGUI_FontType_16X16},
	{"(4)FontType16X24",(void *)&Task_SetMsgFont,VT_TaskUint8,TGUI_FontType_16X24},
	{"(5)FontType24X24",(void *)&Task_SetMsgFont,VT_TaskUint8,TGUI_FontType_24X24},
  {"(6)FontType32X32",(void *)&Task_SetMsgFont,VT_TaskUint8,TGUI_FontType_32X32},
	{"(7)FontType48X48",(void *)&Task_SetMsgFont,VT_TaskUint8,TGUI_FontType_48X48},
	{"MsgFont",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4200[]={
	{"(1)BACK",(void *)&Menu0x4000[0],VT_Menu,1},
	{"(2)TitleFont",(void *)&Menu0x4210[0],VT_Menu,3},
	{"(3)MenuFont",(void *)&Menu0x4220[0],VT_Menu,3},
	{"(3)MsgFont",(void *)&Menu0x4230[0],VT_Menu,3},
	{"(4)SaveConfigs",(void *)&Task_SaveConfigs,VT_TaskUint8,1},
	{"Font",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4300[]={
	{"(1)BACK",(void *)&Menu0x4000[0],VT_Menu,1},
	{"(2)LocalHost:%03d.%03d.%03d.%03d",(void *)&Run_Value.LocalHost,VT_IP,5},
	{"(3)GateWay:%03d.%03d.%03d.%03d",(void *)&Run_Value.GateWay,VT_IP,5},
	{"(2)Subnet Mask:%03d.%03d.%03d.%03d",(void *)&Run_Value.SubnetMask,VT_IP,5},
	{"(4)SaveConfigs",(void *)&Task_SaveConfigs,VT_TaskUint8,1},
	{"IPConfig",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x4000[]={
	{"(1)BACK",(void *)&MenuHome[0],VT_Menu,0},
	{"(2)Color",(void *)&Menu0x4100[0],VT_Menu,2},
	{"(3)Font",(void *)&Menu0x4200[0],VT_Menu,2},
	{"(4)IPConfig",(void *)&Menu0x4300[0],VT_Menu,2},
	{"(5)DeBugEn:%s",(void *)&Run_Value.DeBugEn,VT_BOOL,1},
	{"Set",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x5000[]={
	{"(1)BACK",(void *)&MenuHome[0],VT_Menu,0},
	{"(2)File Browser",(void *)Null,VT_Null,2},
	{"(3)Zigbee Control",(void *)Null,VT_Null,2},
	{"(4)USB Communication",(void *)Null,VT_Null,1},
	{"(4)Web Server",(void *)Null,VT_Null,1},
	{"(4)Camera",(void *)Null,VT_Null,1},
	{"Appliance",Null,VT_End,Null},
};
const MUI_MenuStruct Menu0x6000[]={
	{"(1)BACK",(void *)&MenuHome[0],VT_Menu,0},
	{"(2)PictureMosaic",(void *)&Task_PictureMosaic,VT_Task,1},
	{"(3)CramSnake",(void *)&Task_CramSnake,VT_Task,1},
	{"(4)LifePlay",(void *)&Task_LifePlay,VT_Task,1},
	{"(5)Tetris",(void *)Task_Tetris,VT_Task,1},
	{"Game",Null,VT_End,Null},
};
const MUI_MenuStruct MenuHome[]={
	{"(1)FuncTask",(void *)&Menu0x1000[0],VT_Menu,1},
	{"(2)HandTest",(void *)&Menu0x2000[0],VT_Menu,1},
	{"(3)About",(void *)&Menu0x3000[0],VT_Menu,1},
	{"(4)Set",(void *)&Menu0x4000[0],VT_Menu,1},
	{"(5)App",(void *)&Menu0x5000[0],VT_Menu,1},
	{"(6)Game",(void *)&Menu0x6000[0],VT_Menu,1},
	{"(7)Reset",(void *)&MCU_Reset,VT_Task,1},
	{"Home",Null,VT_End,Null},
};































