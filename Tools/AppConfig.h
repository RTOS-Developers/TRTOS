#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_
#include <Include.h>


typedef struct
{
	uint16 Size;
	uint16 CrcValue;
}ConfigHeadStruct;

#define FLASH_ADDR_BASE		0x08000000
#ifndef CONFIG_SECTOR_SPACE
	#define CONFIG_SECTOR_SPACE 2048
#endif
#define CONFIGS_ADDR_BASE  (FLASH_ADDR_BASE+FLASH_PARAM_PAGE*1024)
void Save_VoidConfig(void(*Save)(uint32 Addr,void *Data,uint16 Size),uint32 Addr,void *Data,uint16 Sector);
BOOL Load_VoidConfig(void(*Load)(uint32 Addr,void *Data,uint16 Size),uint32 Addr,void *Data,uint16 Sector);
void Save_AppConfig(void *Data,uint16 Size);
BOOL Load_AppConfig(void *Data,uint16 Size);
void Flash_WriteData(uint32 Addr,void *Data,uint16 Size);
void Flash_ReadData(uint32 Addr,void *Data,uint16 Size);
void Create_Message(void *Buf,uint8 Length);
BOOL Export_Message(void *Buf,uint8 Length);
#endif


