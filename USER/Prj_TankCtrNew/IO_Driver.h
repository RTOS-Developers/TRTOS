#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_
#include <Include.h>

#define CTR1_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_6) 
#define CTR1_ON GPIO_SetBits(GPIOC,GPIO_Pin_6) 
#define CTR2_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_7) 
#define CTR2_ON GPIO_SetBits(GPIOC,GPIO_Pin_7) 
#define CTR3_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_8) 
#define CTR3_ON GPIO_SetBits(GPIOC,GPIO_Pin_8) 
#define CTR4_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_9) 
#define CTR4_ON GPIO_SetBits(GPIOC,GPIO_Pin_9) 
#define CTR5_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_8) 
#define CTR5_ON GPIO_SetBits(GPIOA,GPIO_Pin_8) 
#define CTR6_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_15) 
#define CTR6_ON GPIO_SetBits(GPIOA,GPIO_Pin_15)//这个使用了调试端口 
#define CTR7_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_10) 
#define CTR7_ON GPIO_SetBits(GPIOC,GPIO_Pin_10) 
#define CTR8_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_11) 
#define CTR8_ON GPIO_SetBits(GPIOC,GPIO_Pin_11) 
#define CTR9_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_12) 
#define CTR9_ON GPIO_SetBits(GPIOC,GPIO_Pin_12) 

#define CTR10_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_0) 
#define CTR10_ON GPIO_SetBits(GPIOD,GPIO_Pin_0) 
#define CTR11_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_1) 
#define CTR11_ON GPIO_SetBits(GPIOD,GPIO_Pin_1) 
#define CTR12_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_2) 
#define CTR12_ON GPIO_SetBits(GPIOD,GPIO_Pin_2) 
#define CTR13_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_3) 
#define CTR13_ON GPIO_SetBits(GPIOD,GPIO_Pin_3) 
#define CTR14_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_4) 
#define CTR14_ON GPIO_SetBits(GPIOD,GPIO_Pin_4) 
#define CTR15_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_5) 
#define CTR15_ON GPIO_SetBits(GPIOD,GPIO_Pin_5) 
#define CTR16_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_6) 
#define CTR16_ON GPIO_SetBits(GPIOD,GPIO_Pin_6) 
#define CTR17_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_7) 
#define CTR17_ON GPIO_SetBits(GPIOD,GPIO_Pin_7) 

#define CTR18_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_3) //这个使用了调试端口
#define CTR18_ON GPIO_SetBits(GPIOB,GPIO_Pin_3) 
#define CTR19_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_4) //这个使用了调试端口
#define CTR19_ON GPIO_SetBits(GPIOB,GPIO_Pin_4) 
#define CTR20_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_5) 
#define CTR20_ON GPIO_SetBits(GPIOB,GPIO_Pin_5) 
#define CTR21_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_6) 
#define CTR21_ON GPIO_SetBits(GPIOB,GPIO_Pin_6) 
#define CTR22_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_7) 
#define CTR22_ON GPIO_SetBits(GPIOB,GPIO_Pin_7) 
#define CTR23_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_8) 
#define CTR23_ON GPIO_SetBits(GPIOB,GPIO_Pin_8) 
#define CTR24_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_9) 
#define CTR24_ON GPIO_SetBits(GPIOB,GPIO_Pin_9) 

#define  RS485_RXD GPIO_ResetBits(GPIOA,GPIO_Pin_1) 
#define  RS485_TXD GPIO_SetBits(GPIOA,GPIO_Pin_1) 

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


#define LED1_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define LED1_OFF GPIO_SetBits(GPIOE,GPIO_Pin_2) 
#define LED2_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_3)
#define LED2_OFF GPIO_SetBits(GPIOE,GPIO_Pin_3) 
#define LED3_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define LED3_OFF GPIO_SetBits(GPIOE,GPIO_Pin_4) 
extern uint32 CTR_DATABIT;
void GPIO_Control_Init(void);
void DeviceMount_Control(void);
void DeviceMount_Signel(void);
void CTR_SetBIT(uint8 Bit);
void CTR_ClrBIT(uint8 Bit);
void CTR_SetBIT32(uint32 Bit);//不影响标志
void CTR_SetUint32(uint32 Bit);
uint32 SIG_GetUint32(void);
void CTR_Test(uint8 i);

#endif
