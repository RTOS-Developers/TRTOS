#ifndef _TGUI_INCLUDE_H_
#define _TGUI_INCLUDE_H_
#include <include.h>
#include <GUI_Include.h>

#ifdef GUI_ENABLE
#include <TGUI_2D_Driver.c>
#include <TGUI_GBK_Driver.c>
#endif

#ifdef   TGUI_ENABLE
#include <TGUI_Base.c>
#include <TGUI.c>
#include <TGUI_Button.c>
#include <TGUI_3D_Driver.c>
#include <TGUI_3DCobe.c>
#include <TGUI_Excel.c>
#include <TGUI_Label.c>
#include <TGUI_Processbar.c>
#include <TGUI_Softkeyboard.c>
#include <TGUI_Textbox.c>
#include <TGUI_Window.c>
#endif

#ifdef   MGUI_ENABLE  //MGUI
#include <MGUI_Lib.c>
#include <MGui.c>
//#include <LGUI.c>
#include <MGUI_Message.c>
#endif

#endif
