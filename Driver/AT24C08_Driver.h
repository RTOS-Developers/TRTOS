#ifndef _AT24C08_DRIVER_H_
#define _AT24C08_DRIVER_H_
#include <Include.h>


#define AT24C08_SDA_SET  GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define AT24C08_SDA_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define AT24C08_SCL_SET  GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define AT24C08_SCL_CLR  GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define AT24C08_SDA_DAT GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

#define AT24C08_WRITE_DIS 	GPIO_SetBits(GPIOE,GPIO_Pin_0)
#define AT24C08_WRITE_EN 	  GPIO_ResetBits(GPIOE,GPIO_Pin_0)

#define AT24CXX_ADDR 		0x50
#define AT24CXX_PAGESIZE 	16
#define AT24CXX_MEMSIZE		0x03FF		//24C02

uint8 AT24C08_IOCONTROL(uint8 ConMode);
void AT24C08_Init(void);
BOOL AT24C08_Write(uint16 Addr,uint8 *Buf,uint16 Length);
BOOL AT24C08_Read(uint16 Addr,uint8 *Buf,uint16 Length);



#endif


