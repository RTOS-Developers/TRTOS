#ifndef _CRC_H_
#define _CRC_H_
#include <Define.h>

uint16 Tools_GetCRC16(uint8 *DataSource,uint16 DataLen);
uint8 Tools_GetXOR8(string Ptr,uint16 Length);
uint16 Tools_GetXOR16(uint16 *Ptr,uint16 Length);
uint16 SmallCrc(uint8 *ptr, uint16 len);
uint8 Tools_GetCRC8(uint8 *ptr,uint16 len);
#endif
