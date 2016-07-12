#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_
#include <Include.h>



#define LED1_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_13) 
#define LED1_ON GPIO_SetBits(GPIOC,GPIO_Pin_13) 
#define LED2_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_14) 
#define LED2_ON GPIO_SetBits(GPIOC,GPIO_Pin_14) 
#define LED3_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_15) 
#define LED3_ON GPIO_SetBits(GPIOC,GPIO_Pin_15) 



#define SIG1 	(GPIOD->IDR&S15)
#define SIG2 	(GPIOD->IDR&S14)
#define SIG3 	(GPIOD->IDR&S13)

#define SIG4 	(GPIOD->IDR&S12)
#define SIG5 	(GPIOD->IDR&S11)
#define SIG6 	(GPIOD->IDR&S10)
#define SIG7 	(GPIOD->IDR&S9)
#define SIG8 	(GPIOD->IDR&S8)

#define SIG9 	  (GPIOB->IDR&S15)
#define SIG10 	(GPIOB->IDR&S14)
#define SIG11 	(GPIOB->IDR&S13)
#define SIG12 	(GPIOB->IDR&S12)

#define SIG13 	(GPIOE->IDR&S15)
#define SIG14 	(GPIOE->IDR&S14)
#define SIG15 	(GPIOE->IDR&S13)
#define SIG16 	(GPIOE->IDR&S12)
#define SIG17 	(GPIOE->IDR&S11)
#define SIG18 	(GPIOE->IDR&S10)
#define SIG19 	(GPIOE->IDR&S9)
#define SIG20 	(GPIOE->IDR&S8)
#define SIG21 	(GPIOE->IDR&S7)

#define SIG22 	(GPIOB->IDR&S2)
#define SIG23 	(GPIOB->IDR&S1)
#define SIG24 	(GPIOB->IDR&S0)
#define SIG25 	(GPIOC->IDR&S5)
#define SIG26 	(GPIOC->IDR&S4)

#define SIG27 	(GPIOA->IDR&S7)
#define SIG28 	(GPIOA->IDR&S6)
#define SIG29 	(GPIOA->IDR&S5)
#define SIG30 	(GPIOA->IDR&S4)

void DeviceMount_Control(void);
void DeviceMount_Signel(void);

#endif
