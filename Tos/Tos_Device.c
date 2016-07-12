#include <Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <Tos_device.h>
#include <Tos_Mema.h>
uint8 DeviceId_Index;
//设备列表   Tos的设备里包含
DeviceInfor	*Tos_Device_Tab;
/*******************************************************************************
Func设备信息列表初始化
Date:2014-5-22
Note:
*******************************************************************************/
void DeviceList_Init()
{
	uint8 i;
	DeviceId_Index=0;
	Tos_Device_Tab=Mema_Malloc(sizeof(DeviceInfor)*Tos_DeviceTotal);
	for(i=0;i<USART_DEF_CH_SIZE;i++)
	{
		DeviceId_USART_TX[i]=DeviceNull;
		DeviceId_USART_RX[i]=DeviceNull;
	}
	for(i=0;i<Tos_DeviceTotal;i++)
	{
		Tos_Device_Tab[i].Init=Null;
		Tos_Device_Tab[i].DeviceState=Ready;
		Tos_Device_Tab[i].Read=Null;
		Tos_Device_Tab[i].Write=Null;
		Tos_Device_Tab[i].Exit=Null;
	}
}
BOOL Device_Ok(uint8 ID)
{
	if(ID==DeviceNull)return False;
	if(Tos_Device_Tab[ID].Init==Null)return False;
	return True;
}

/*******************************************************************************
Func:任务获取设备
Date:2014-5-22
Note:
*******************************************************************************/
BOOL Tos_TaskGetDev(uint8 ID,uint32 Timeout)
{
GoGet:
	switch(Tos_Device_Tab[ID].DeviceState)
	{
		case Delete:
			return False;
		case Busy:
			if(Tos_Device_Tab[ID].DeviceOwnerId==Tos_TaskCurrent)return True;
			else {Tos_WaitDev(ID);goto GoGet;}
		case NotBusy:
			Tos_Device_Tab[ID].DeviceState=Busy;
			Tos_Device_Tab[ID].DeviceOwnerId=Tos_TaskCurrent;
			return True;
		case Ready:
			Tos_Device_Tab[ID].DeviceState=Busy;
		  Tos_Device_Tab[ID].DeviceOwnerId=Tos_TaskCurrent;
		  return True;
		default :return False;
	}
}
/*******************************************************************************
Func任务放开设备
Date:2014-5-22
Note:
*******************************************************************************/
BOOL Tos_TaskDropDev(uint8 ID)
{
	if(Tos_Device_Tab[ID].DeviceState==Delete)return False;
	if(Tos_Device_Tab[ID].DeviceOwnerId!=Tos_TaskCurrent)return False;
	Tos_Device_Tab[ID].DeviceState=NotBusy;
	return True;
}
/*******************************************************************************
Func各设备初始化及设备注册
Date:2014-5-22
Note:设备在注册登记后需要对设备列表里的设备进行初始化
*******************************************************************************/
void Device_Init()
{
	uint8 i;
typedef   void 					(*FuncTaskTags)(void);
typedef   void 					(*FuncArrayTags)(uint8 ID);
typedef   void 					(*FuncIOListTags)(const GPIO_InitStruct *List);
typedef   void 					(*FuncValueTags)(void *);
	DeBug("Tos Device Init..........",Infor_Infor);
	for(i=0;i<Tos_DeviceTotal;i++)
	{
			if(Tos_Device_Tab[i].Init)
			{
				switch(Tos_Device_Tab[i].DeviceVirtue)
				{
					case DV_Array:((FuncArrayTags)(Tos_Device_Tab[i].Init))(Tos_Device_Tab[i].Note);break;
					case DV_Task:((FuncTaskTags)Tos_Device_Tab[i].Init)();break;
					case DV_IOList:((FuncIOListTags)Tos_Device_Tab[i].Init)((const GPIO_InitStruct *)Tos_Device_Tab[i].Note);break;
					case DV_TaskValue:((FuncValueTags)Tos_Device_Tab[i].Init)((void *)Tos_Device_Tab[i].Note);break;
				}
			}
	}
}
uint8 Device_Count()
{
	uint8 i,count=0;;
	for(i=0;i<Tos_DeviceTotal;i++)
	{
		if(Tos_Device_Tab[i].DeviceName!=Null)count++;
	}
	return count;
}
void DeviceList_Printf()
{
uint8 i;
	for(i=0;i<Tos_DeviceTotal;i++)
	{
		if(Tos_Device_Tab[i].DeviceName!=Null)
		{
			switch(Tos_Device_Tab[i].DeviceVirtue)
				{
					case DV_Array:DeBug("Device[%d]Name:%s[%d]",i,Tos_Device_Tab[i].DeviceName,Tos_Device_Tab[i].Note,Infor_Infor);break;
					case DV_Task:DeBug("Device[%d]Name:%s",i,Tos_Device_Tab[i].DeviceName,Infor_Infor);break;
					case DV_IOList:DeBug("Device[%d]Name:%s",i,Tos_Device_Tab[i].DeviceName,Infor_Infor);break;
					case DV_TaskValue:DeBug("Device[%d]Name:%s",i,Tos_Device_Tab[i].DeviceName,Infor_Infor);break;
					default:DeBug("Device[%d]Name:%s",i,Tos_Device_Tab[i].DeviceName,Infor_Infor);break;
				}
		}
	}
}
BOOL DeviceUnMount(uint8 ID)
{
	typedef   void 					(*FuncNotTags)(void);
	if(Tos_Device_Tab[ID].Exit){((FuncNotTags)Tos_Device_Tab[ID].Exit)();return True;}
	return False;
}
