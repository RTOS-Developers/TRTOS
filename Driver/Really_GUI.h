#ifndef _REALL_GUI_H_
#define _REALL_GUI_H_
#include <include.h>
#include <Really_Driver.h>
#ifdef BOARD_REALLY

#define ReallyID 0xff
#define Color_Back 208
#define Window_BackColor 0xcf


#define	RRM_Base 0x01
#define RRM_Data 0x02
#define RRM_Clear 0x04

extern uint8 Really_DevIndex,ReallyPageIndex;
void Really_TextButton(uint16 x,uint16 y,char *String,BOOL ClickTrue);
void Really_CreateBase(uint8 *Title);
void Really_Task(void *Tags);
void Really_ShowFloat(uint16 x,uint16 y,float Value,uint8 DotSize);




#endif

#endif

