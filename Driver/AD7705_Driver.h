#ifndef _AD7705_DRIVER_H_
#define _AD7705_DRIVER_H_
#include <Include.h>

#define AD7705_DIN_SET    {GPIOA->BSRR=S7;}
#define AD7705_DIN_CLR    {GPIOA->BRR =S7;}
#define AD7705_CLK_SET    {GPIOA->BSRR=S2;}
#define AD7705_CLK_CLR    {GPIOA->BRR =S2;}
#define AD7705_RST_SET    {GPIOA->BSRR=S4;}
#define AD7705_RST_CLR    {GPIOA->BRR =S4;}
#define AD7705_CS_SET     {GPIOA->BSRR=S3;}
#define AD7705_CS_CLR     {GPIOA->BRR =S3;}


#define AD7705_OUT_DAT    (GPIOA->IDR&S6)
#define AD7705_DEDY_DAT    (GPIOA->IDR&S5)

void AD7705_Init(uint8 CH);
uint16 AD7705_GetADC(void);
uint16 AD7705_ReadFitel(void);


#endif
