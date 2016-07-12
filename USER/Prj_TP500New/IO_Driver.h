#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_
#include <Include.h>


#define LED1_ON     GPIO_ResetBits(GPIOB,GPIO_Pin_2)
#define LED1_OFF    GPIO_SetBits(GPIOB,GPIO_Pin_2) 

#define LED2_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define LED2_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_12)


#define BEEP_OFF    GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define BEEP_ON   GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define E3V3_ON    GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define E3V3_OFF   GPIO_SetBits(GPIOC,GPIO_Pin_9)

#define GPRS_ON    GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define GPRS_OFF   GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define GPS_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define GPS_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define PWKEY_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define PWKEY_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define WKUP      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define GPRS_VDDEXT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)

typedef enum
{
	Key_Printf=0x01,
	Key_UpDown=0x02,
	Key_WkUp=0x04,
}KeyCode;
void DeviceMount_IO(const GPIO_InitStruct *List);
void KC_EXTI_Init(void);
void DeviceMount_CodeKey(void);
void Tos_TaskGetKeyCode(void);

#endif
