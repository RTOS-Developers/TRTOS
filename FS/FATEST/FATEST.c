#include <include.h>

uint8 DeviceId_FATEST=DeviceNull;

#include <..\FS\FATEST\sdio_sdcard.c>
#include <..\FS\FATEST\ff9\diskio.c>
//#include <..\FS\FATEST\ff9\option\cc936.c>
#include <..\FS\FATEST\ff9\ff.c>


void FS_Configs()
{
	NVIC_SD_Configuration();
	SD_Init();
}



void DeviceMount_FS()
{
	DeviceId_FATEST=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_FATEST].DeviceName="FATEST";
	Tos_Device_Tab[DeviceId_FATEST].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_FATEST].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_FATEST].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_FATEST].Init=FS_Configs;
	Tos_Device_Tab[DeviceId_FATEST].Write=Null;
	Tos_Device_Tab[DeviceId_FATEST].Read=Null;
	Tos_Device_Tab[DeviceId_FATEST].Exit=Null;
}
