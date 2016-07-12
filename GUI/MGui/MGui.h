#ifndef _MGUI_H_
#define _MGUI_H_
#include <include.h>
#include <TGui_GBk_Driver.h>
 
typedef struct
{
	TGUI_FontType Title_Font;
	TGUI_FontType Menu_Font;
	
	
}MGUI_Style;
typedef enum MGUIOptionType
{
	MGOT_Menu=1,//菜单选项
	MGOT_OptionBool=3,//布尔类型
	MGOT_OptionU32=4,//
	MGOT_OptionF32=5,//
	MGOT_OptionASCII=6,//
	MGOT_OptionFunc=0,
}MGUIOptionType;
typedef enum MGUIDrawMode
{
	MGDM_ReDraw=1,//重绘
	MGDM_ClrDrawOnly=2,//清除绘制
	MGDM_ClrDraw=3,//清除所有
}MGUIDrawMode;

typedef struct MGUI_MenuStruct
{
	MGUIOptionType  OptionType;
	BOOL   SelectTrue;//选中为真
	char  *OptionName;//选项名
	uintbus OptionValue;//选项值
	struct MGUI_MenuStruct *NextMenu;//下一级菜单
	struct MGUI_MenuStruct *SubMenu;//子菜单
	struct MGUI_MenuStruct *ParentMenu;//父菜单
	struct MGUI_MenuStruct *PrevMenu;//上一个节点
}MGUI_MenuStruct;
void MGUI_MenuReDraw(MGUIDrawMode Mode);
void MGUI_TestTask(void *Tags);
void MGUI_GetPutIn(void);

#endif
