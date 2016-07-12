#ifndef _IO_DRIVER_H_
#define _IO_DRIVER_H_
#include <Include.h>


#define LED1_ON     GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define LED1_OFF    GPIO_SetBits(GPIOB,GPIO_Pin_14) 
#define LED2_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define LED2_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define CTR1_ON    GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define CTR1_OFF   GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define CTR2_ON    GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define CTR2_OFF   GPIO_SetBits(GPIOC,GPIO_Pin_14)

#define K90        GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define K00        GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

#define MOTOB_ON     GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define MOTOB_OFF    GPIO_SetBits(GPIOB,GPIO_Pin_15) 
#define MOTOF_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define MOTOF_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_14)

#define RS485_ON    GPIO_ResetBits(GPIOA,GPIO_Pin_1) 
#define RS485_OFF   GPIO_SetBits(GPIOA,GPIO_Pin_1) 

#define EN_ON     GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define EN_OFF    GPIO_SetBits(GPIOA,GPIO_Pin_6) 

#define DIR_ON    GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define DIR_OFF   GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define PUL_ON    GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define PUL_OFF   GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define BEEP_ON    GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define BEEP_OFF   GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define  MOTOR_EN(x)        (x?GPIO_SetBits(GPIOA, GPIO_Pin_6):GPIO_ResetBits(GPIOA, GPIO_Pin_6))
#define  MOTOR_DIR(x)       (x?GPIO_SetBits(GPIOA, GPIO_Pin_5):GPIO_ResetBits(GPIOA, GPIO_Pin_5))
#define  MOTOR_PLUSE(x)     (x?GPIO_SetBits(GPIOA, GPIO_Pin_4):GPIO_ResetBits(GPIOA, GPIO_Pin_4))    

void DeviceMount_Control(void);
void DeviceMount_Signel(void);
void KC_EXTI_Init(void);
void DeviceMount_CodeKey(void);
void Tos_TaskGetKeyCode(void);

#endif
