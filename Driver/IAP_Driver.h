#ifndef _IAP_DRIVER_H_
#define _IAP_DRIVER_H_
#include <include.h>

#define PAGE_SIZE                         (512)  
#define EnValueOffset      0x4c00
#define ApplicationOffset  0x5000
#define BootLoaderAddress  NVIC_VectTab_FLASH
#define ApplicationAddress  (BootLoaderAddress+ApplicationOffset)//应用程序必须放在这个地址才能运行
#define  ENVALUE_ADDR_BASE  (FLASH_ADDR_BASE+EnValueOffset)
typedef enum 
{
	AFT_SPINand=1,//从SPI nandflash上获取固件
	AFT_IIC=2,//从iic总线设备上获取固件
	AFC_FLASH=3,//从自身内存上获取固件
}AppFromType;
typedef struct
{
	ConfigHeadStruct CHS;
	uint32 AppToAddr;//App将要下载到的决定地址
	uint32 AppFromAddr;//App将由哪个地址获取
	AppFromType AppFromType;//App来源介质类型
	uint32 AppSize;//应用大小(bytes)
	uint8 UpdateApp;//True有固件需要升级，由应用置True由boot置假
	uint8 UpdateOver;//升级完置True，跳转到应用后由应用置False，应用没置假，直接在下次开机时回滚
	uint16 Version;
}EnValue;

#endif
