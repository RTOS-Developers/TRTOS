#ifndef _CS1180_DRIVER_H_
#define _CS1180_DRIVER_H_
#include <Include.h>

#define CS1180_SDI_SET 	GPIOA->BSRR=S4		
#define CS1180_SDI_CLR 	GPIOA->BRR =S4


#define CS1180_CLK_SET 	GPIOA->BSRR=S2	
#define CS1180_CLK_CLR 	GPIOA->BRR=S2

#define CS1180_CS_SET   GPIOA->BSRR=S5
#define CS1180_CS_CLR   GPIOA->BRR=S5

#define CS1180_SDO_DAT 	(GPIOA->IDR&S3)
#define CS1180_DRDY_DAT (GPIOA->IDR&S1)


#define COMM_RDATA      0X01 
#define COMM_RDATAC 0X03 
#define COMM_STOPC      0x0f 
#define COMM_RREG       0x10 
#define COMM_WREG       0x05 
#define COMM_CALSELF    0xf0 
#define COMM_OCALSELF   0xf1 
#define COMM_SLFGCAL    0xf2 
#define COMM_OCALSYS    0xf3 
#define COMM_GCALSYS    0xf4 
#define COMM_WAKEUP             0xfb 
#define COMM_SYNC               0xfc 
#define COMM_SLEEP              0xfd 
#define COMM_RESET              0xfe 

#define CS1180_DRDYPIN_USE   0
extern uint16 CS1180_ADC1;

void CS1180_Init(void);
uint32 CS1180_ReadADC(void);
void IniCS1180(void) ;
uint32 CS1180_ReadADC(void);
void Printf_CS1180Reg(void);
uint16 CS1180_ReadFiter(void);

#endif
