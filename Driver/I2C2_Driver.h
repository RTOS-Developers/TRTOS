#ifndef _I2C2_DRIVER_H_
#define _I2C2_DRIVER_H_
#include <include.h>

#define I2C_Speed              100
#define I2C2_SLAVE_ADDRESS7    0xA0

#define I2C_PageSize           4


void I2C2_Configuration(void);
void I2C2_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void I2C2_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);//需要两个起始信号;
void I2C2_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);

#endif
