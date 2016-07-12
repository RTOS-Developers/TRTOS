#ifndef _BOARD_DRIVER_H_
#define _BOARD_DRIVER_H_

#include <Include.h>


typedef struct 
{
	uint8 GPIO_Speed;
	GPIOMode_TypeDef GPIO_Mode;
	GPIO_TypeDef *GPIOx;
	uint16 GPIO_Pin;
}GPIO_InitStruct;

void Sys_Standby(void);
void BareDelay(uint32 i);
void MCU_Reset(void);
void Sys_Enter_Standby(void);

void assert_failed(uint8_t* file, uint32_t line);
void Remap_SWJ_JTAGDisable(void);
void GPIO_InitList(const GPIO_InitStruct *List);
void IWDG_Configuration(void);
void IWDG_Reload(void);
void Standby_Init(void);
#endif
