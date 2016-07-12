#include <Include.h>
#include <IO_Driver.h>

uint8 DeviceId_IO=DeviceNull;
const GPIO_InitStruct *IO_Driver;

void IO_Set(uint8 Index,BOOL Vol)
{
	if(Vol)GPIO_SetBits(IO_Driver[Index].GPIOx,IO_Driver[Index].GPIO_Pin);
	else GPIO_ResetBits(IO_Driver[Index].GPIOx,IO_Driver[Index].GPIO_Pin);
}
BOOL IO_Get(uint8 Index)
{
	uint8 R;
	 R=GPIO_ReadInputDataBit(IO_Driver[Index].GPIOx,IO_Driver[Index].GPIO_Pin);
	return (BOOL)R;
}
void DeviceMount_IO(const GPIO_InitStruct *IOList)
{
	IO_Driver=IOList;
	DeviceId_IO=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_IO].DeviceId=DeviceId_IO;
	Tos_Device_Tab[DeviceId_IO].DeviceName="IO";
	Tos_Device_Tab[DeviceId_IO].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_IO].DeviceVirtue=DV_IOList;
	Tos_Device_Tab[DeviceId_IO].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_IO].Init=GPIO_InitList;
	Tos_Device_Tab[DeviceId_IO].Note=(uintbus)IOList;
	Tos_Device_Tab[DeviceId_IO].Write=Null;
	Tos_Device_Tab[DeviceId_IO].Read=Null;
	Tos_Device_Tab[DeviceId_IO].Exit=Null;
}



