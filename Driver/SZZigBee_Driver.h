#ifndef _SZZIGBEE_DRIVER_H_
#define _SZZIGBEE_DRIVER_H_
#include <Include.h>

BOOL ZigBee_CheckCrc(uint8 *Buf);
uint8 ZigBee_SendMsg(uint8 *Buf,uint16 OnlyAddr,uint8 Cmd,uint8 *Data,uint16 DataLegnth);


#endif
