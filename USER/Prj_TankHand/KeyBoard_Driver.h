#ifndef _HANDKEY_DRIVER_H_
#define _HANDKEY_DRIVER_H_
#include <include.h>


void KeyBoard_Configuration(void);

uint8 HandKey_GetValue(void);
void Tos_TaskGetHandKey(void);
void Task_GetHandKey(void *Tags);
void Standby_Init(void);
void AutoCorrect_TankHand(void);
#endif
