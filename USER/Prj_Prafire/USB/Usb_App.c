#include <include.h>
#include <..\USER\Prj_Prafire\USB\Usb_All.h>
uint8 DeviceId_USB=DeviceNull;



void USB_Configs()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	USB_Cable_Config(ENABLE);
	USB_Interrupts_Config();    
	Set_USBClock();  
	USB_Init();	
}



void DeviceMount_Usb()
{
	DeviceId_USB=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_USB].DeviceName="USB";
	Tos_Device_Tab[DeviceId_USB].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_USB].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_USB].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_USB].Init=USB_Configs;
	Tos_Device_Tab[DeviceId_USB].Write=Null;
	Tos_Device_Tab[DeviceId_USB].Read=Null;
	Tos_Device_Tab[DeviceId_USB].Exit=Null;
}
