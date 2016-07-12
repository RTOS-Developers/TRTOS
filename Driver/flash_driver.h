#ifndef _FLASH_DRIVER_H_
#define _FLASH_DRIVER_H_
#include <Include.h>

extern uint8 DeviceId_FLASH;
extern uint8 FlashAddr_Index;
void FLASH_ReadDatas(uint16 Addr,uint16 *Buffer,uint8 Length);
uint8 FLASH_WriteDatas(uint16 Addr,uint16 *Buffer,uint16 Length);
void DeviceMount_Flash(void);
void FlashConfigRoom_Init(void);
void FlashGetConfigRoom(uint8 *Offset,uint8 Size);
#endif

