#include <Include.h>
#include <..\USER\Prj_TeleControl\IO_Driver.h>

uint8 DeviceId_CONTROL=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;
uint8 DeviceId_CODEKEY=DeviceNull;
uint8 KeyCode_Tos_TaskID=Tos_TaskNull;
const GPIO_InitStruct IO_Control_List[]=
{
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_14|GPIO_Pin_15},
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_13|GPIO_Pin_14},
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOB,GPIO_Pin_11|GPIO_Pin_9},
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_8},
	{0},
};

const GPIO_InitStruct IO_Signel_List[]=
{
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOB,GPIO_Pin_11|GPIO_Pin_9},
	{0},
};
void DeviceMount_Control()
{
	DeviceId_CONTROL=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceId=DeviceId_CONTROL;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceName="CTR";
	Tos_Device_Tab[DeviceId_CONTROL].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceVirtue=DV_IOList;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_CONTROL].Init=GPIO_InitList;
	Tos_Device_Tab[DeviceId_CONTROL].Note=(uintbus)&IO_Control_List;
	Tos_Device_Tab[DeviceId_CONTROL].Write=Null;
	Tos_Device_Tab[DeviceId_CONTROL].Read=Null;
	Tos_Device_Tab[DeviceId_CONTROL].Exit=Null;
}
void DeviceMount_Signel()
{
	DeviceId_SIGNAL=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceId=DeviceId_SIGNAL;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceName="SIG";
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceVirtue=DV_IOList;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_SIGNAL].Init=GPIO_InitList;
	Tos_Device_Tab[DeviceId_SIGNAL].Note=(uintbus)&IO_Signel_List;
	Tos_Device_Tab[DeviceId_SIGNAL].Write=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Read=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Exit=Null;
	
}




