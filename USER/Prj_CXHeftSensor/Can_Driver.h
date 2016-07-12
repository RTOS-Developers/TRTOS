#ifndef _CAN_DRIVER_H_
#define _CAN_DRIVER_H_

#include <include.h>

#define CAN_GROUP   0x0000

void CAN_SendMsg(void *Msg,uint8 Length);
void CAN_Filter_Configs(uint16 LocalAddr,BOOL Host);
#endif


