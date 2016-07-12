#include <Include.h>
#include <..\USER\Prj_TP500\IO_Driver.h>

uint8 DeviceId_CONTROL=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;

uint8 KeyCode_Tos_TaskID=Tos_TaskNull;
const GPIO_InitStruct IO_Control_List[]=
{
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_0},
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_3},
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOC,GPIO_Pin_1|GPIO_Pin_0},
	{0},
};

const GPIO_InitStruct IO_Signel_List[]=
{
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOC,GPIO_Pin_1|GPIO_Pin_0},
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
uint8 HandKey_GetValue()
{
	uint8 Value=0x00;
  if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0){Value=Key_UpDown;}
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==0){Value=Key_Printf;}
	return Value;
}
void Task_KeyBoard(void *Tags)
{
	MGUI_KeyMsg KeyMsg;
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
void Tos_TaskGetHandKey()
{
	KeyCode_Tos_TaskID=Tos_TaskCurrent;
}





