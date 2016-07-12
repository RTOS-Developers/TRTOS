#ifndef _I2C1_DRIVER_H_
#define _I2C1_DRIVER_H_
#include <include.h>

#define I2C_Speed              10000
#define I2C1_SLAVE_ADDRESS7    0xA0
#define EEPROM_ADDRESS         0xA0

#define I2C_PageSize           4


void I2C1_Configuration(void);
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);//需要两个起始信号;
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);

void EEPROM_SaveConfig(void *Data,uint8 Sector);
BOOL EEPROM_LoadConfig(void *Data,uint8 Sector);
void DeviceMount_EEPROM(void);
#endif
