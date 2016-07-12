#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_

#include <Include.h>

#define SIG1 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)
#define SIG2 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)
#define CTR1_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define CTR1_OFF GPIO_SetBits(GPIOC,GPIO_Pin_3) 
#define CTR2_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define CTR2_OFF GPIO_SetBits(GPIOC,GPIO_Pin_4) 
#define CTR3_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define CTR3_OFF GPIO_SetBits(GPIOC,GPIO_Pin_5) 

#define  RS485_RXD GPIO_ResetBits(GPIOA,GPIO_Pin_0) 
#define  RS485_TXD GPIO_SetBits(GPIOA,GPIO_Pin_0) 

#define  WS2812_L GPIO_ResetBits(GPIOA,GPIO_Pin_5) 
#define  WS2812_H GPIO_SetBits(GPIOA,GPIO_Pin_5) 

void DeviceMount_IO(void);
void Tos_TaskGetIO_InPut(void);
#endif
