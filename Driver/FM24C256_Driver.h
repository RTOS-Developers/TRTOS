#ifndef _FM24C256_DRIVER_H_
#define _FM24C256_DRIVER_H_
#include <Include.h>


#define FM24C256_SDA_SET 	{GPIOB->BSRR=S11;}		//	PB11
#define FM24C256_SDA_CLR 	{GPIOB->BRR =S11;}
#define FM24C256_SCL_SET 	{GPIOB->BSRR=S10;}		//	PB10
#define FM24C256_SCL_CLR 	{GPIOB->BRR=S10;}

#define FM24C256_SDA_DAT 	(GPIOB->IDR&S11)

#define FM24C256_WRITE_DIS 	{GPIOE->BSRR=S2;}		//	PB10
#define FM24C256_WRITE_EN 	{GPIOE->BRR=S2;}

uint8 FM24C256_IOCONTROL(uint8 ConMode);
void FM24C256_GPIO_Init(void);
void FM24C256_Init(void);
BOOL FM24C256_Write(uint16 Addr,uint8 *Buf,uint16 Length);
BOOL FM24C256_Read(uint16 Addr,uint8 *Buf,uint16 Length);



#endif


