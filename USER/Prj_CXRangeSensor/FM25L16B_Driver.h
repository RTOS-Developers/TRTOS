#ifndef _FM25L16B_DRIVER_H_
#define _FM25L16B_DRIVER_H_
#include <Include.h>


//PA7  CS
//PA6  SO
//PA5  WP
//PA4  CLK
//PA3  SI

#define WREN	0x06
#define WRDI	0x04
#define RDSR	0x05
#define WRSR	0x01
#define READ	0x03
#define WRITE	0x02

#define FM_WRITE     	0
#define FM_READ      	1
#define FM_WRITE_DEFAULT	2
#define FM_READ_DEFAULT  	3

#define FM_CS_H GPIO_SetBits(GPIOA,GPIO_Pin_7) 
#define FM_CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define FM_WP_H GPIO_SetBits(GPIOA,GPIO_Pin_5) 
#define FM_WP_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define FM_SCK_H GPIO_SetBits(GPIOA,GPIO_Pin_4) 
#define FM_SCK_L GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define FM_MOSI_H GPIO_SetBits(GPIOA,GPIO_Pin_3) 
#define FM_MOSI_L GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define FM_MISO GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)


void FramInit(void);
uint8 SendReadByte(uint8 data);
void FMWriteMemory(uint16 add, uint8	*dat, uint16 len);
void FMReadMemory(uint16 add, uint8	*dat, uint16 len);
void DeviceMount_Fram(void);

#endif

