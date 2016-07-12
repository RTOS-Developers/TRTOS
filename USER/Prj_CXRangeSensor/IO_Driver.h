#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_
#include <Include.h>


#define SIG1 	(GPIOB->IDR&S0)
#define SIG2 	(GPIOB->IDR&S1)
#define SIG3 	(GPIOB->IDR&S2)

#define LED1_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LED1_ON GPIO_SetBits(GPIOB,GPIO_Pin_5) 
#define LED2_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define LED2_ON GPIO_SetBits(GPIOB,GPIO_Pin_6) 
#define LED3_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define LED3_ON GPIO_SetBits(GPIOB,GPIO_Pin_7) 
#define LEDCAN_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define LEDCAN_ON GPIO_SetBits(GPIOB,GPIO_Pin_7) 

void DeviceMount_Control(void);
void DeviceMount_Signel(void);
uint8 SIG_GetUint8(void);

#endif
