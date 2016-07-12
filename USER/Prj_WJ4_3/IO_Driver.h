#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_
#include <Include.h>


#define LEDRUN_ON     GPIO_ResetBits(GPIOD,GPIO_Pin_6)
#define LEDRUN_OFF    GPIO_SetBits(GPIOD,GPIO_Pin_6) 
#define LEDLINK_ON    GPIO_ResetBits(GPIOD,GPIO_Pin_7)
#define LEDLINK_OFF   GPIO_SetBits(GPIOD,GPIO_Pin_7) 
#define LEDSRART_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define LEDSRART_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_3) 

#define LED3_RG {GPIO_ResetBits(GPIOB,GPIO_Pin_3);GPIO_ResetBits(GPIOD,GPIO_Pin_7);}
#define LED3_G {GPIO_ResetBits(GPIOB,GPIO_Pin_3);GPIO_SetBits(GPIOD,GPIO_Pin_7);}
#define LED3_R {GPIO_SetBits(GPIOB,GPIO_Pin_3);GPIO_ResetBits(GPIOD,GPIO_Pin_7);}
#define LED3_OFF {GPIO_SetBits(GPIOB,GPIO_Pin_3);GPIO_SetBits(GPIOD,GPIO_Pin_7);}

#define LED2_RG {GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_5);}
#define LED2_G {GPIO_ResetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_5);}
#define LED2_R {GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_5);}
#define LED2_OFF {GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_5);}

#define LED1_RG {GPIO_ResetBits(GPIOB,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_6);}
#define LED1_G {GPIO_ResetBits(GPIOB,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_6);}
#define LED1_R {GPIO_SetBits(GPIOB,GPIO_Pin_7);GPIO_ResetBits(GPIOB,GPIO_Pin_6);}
#define LED1_OFF {GPIO_SetBits(GPIOB,GPIO_Pin_7);GPIO_SetBits(GPIOB,GPIO_Pin_6);}

#define KC1 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)
#define KC2 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)
void DeviceMount_Control(void);
void DeviceMount_Signel(void);
void KC_EXTI_Init(void);
void DeviceMount_CodeKey(void);

#endif
