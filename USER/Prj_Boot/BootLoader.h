#ifndef _BOOTLOADER_H_
#define _BOOTLOADER_H_

#include <Include.h>
#include <STM32LIB_Include.h>

#define PAGE_SIZE                         (512)  
#define ApplicationOffset  0x5000
#define BootLoaderAddress  NVIC_VectTab_FLASH
#define ApplicationAddress  (BootLoaderAddress+ApplicationOffset)//应用程序必须放在这个地址才能运行
#define FLASH_PAGESIZE  1024

typedef enum
{
	OSE_DevAddr=0,
	OSE_Cmd=2,
	OSE_Length=3,
	OSE_DataStart=4,
}OffSetEnum;


void Application_Run(uint32_t App_Addr);
void IAP_Command(void *Tags);
void TaskCommandRead(void *Tags);
void BootCheckDownLoad(uint8 *);
#endif
