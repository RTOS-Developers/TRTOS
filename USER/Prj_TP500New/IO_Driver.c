#include <Include.h>
#include <..\USER\Prj_TP500New\IO_Driver.h>

uint8 DeviceId_IO=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;

uint8 KeyCode_Tos_TaskID=Tos_TaskNull;


void DeviceMount_IO(const GPIO_InitStruct *List)
{
	DeviceId_IO=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_IO].DeviceId=DeviceId_IO;
	Tos_Device_Tab[DeviceId_IO].DeviceName="IO";
	Tos_Device_Tab[DeviceId_IO].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_IO].DeviceVirtue=DV_IOList;
	Tos_Device_Tab[DeviceId_IO].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_IO].Init=GPIO_InitList;
	Tos_Device_Tab[DeviceId_IO].Note=(uintbus)List;
	Tos_Device_Tab[DeviceId_IO].Write=Null;
	Tos_Device_Tab[DeviceId_IO].Read=Null;
	Tos_Device_Tab[DeviceId_IO].Exit=Null;
}
uint8 HandKey_GetValue()
{
	uint8 Value=0x00;
  if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)==0){Value=Key_UpDown;}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)==0){Value=Key_Printf;}
	if(WKUP==1){Value=Key_WkUp;}
	return Value;
}
void Task_KeyBoard(void *Tags)
{
	MGUI_KeyMsg KeyMsg;
	Tos_TaskDelay(2000);
	while(1)
	{
		Tos_TaskDelay(10);
		KeyMsg.KeyValue=HandKey_GetValue();
		KeyMsg.KeyTime=0;
		if(KeyMsg.KeyValue!=0)
		{
			BEEP_ON;
	    Tos_TaskDelay(4);
			BEEP_OFF;
			while(HandKey_GetValue())
			{
				Tos_TaskDelay(10);
				KeyMsg.KeyTime++;
				if(KeyMsg.KeyTime>50)break;
			}
			if(KeyMsg.KeyTime>50)KeyMsg.KeyType=Key_DownTimeOut;
			else KeyMsg.KeyType=Key_OneClick;
			Tos_TaskPutQmsg(KeyCode_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
			while(HandKey_GetValue())Tos_TaskDelay(10);
		}
	}	
}

MGUI_KeyMsg * MUI_GetKey(uint16 D)
{
	Tos_ArrayMessage *CurMsg;
	MGUI_KeyMsg *CurKeyMsg;
	if(D)CurMsg=Tos_TaskGetQMsgDelay(D);
	else CurMsg=Tos_TaskGetQMsg();
	if(CurMsg)
	{
		CurKeyMsg=(MGUI_KeyMsg *)CurMsg->Qmsg_Addr;
		return CurKeyMsg;
	}
	return Null;
}

void Tos_TaskGetHandKey()
{
	KeyCode_Tos_TaskID=Tos_TaskCurrent;
}





