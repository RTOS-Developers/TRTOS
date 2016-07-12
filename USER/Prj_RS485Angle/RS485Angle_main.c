#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_RS485ANGLE\IO_Driver.c>
#include <..\USER\Prj_RS485ANGLE\SCA100t.c>
#include <..\USER\Prj_RS485ANGLE\UsartCmd.c>
void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART1);
	DeBug_SetType(Infor_All);
	GPIO_SCA100T_Init();
	SCA_Init();
	while(1)
	{
		LED1_OFF;
		Tos_TaskDelay(10);
		LED1_ON;
		Tos_TaskDelay(90);
	}
}

void TaskKey(void *Tags)
{
	uint16 i;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetKeyCode();
	while(1)
	{		
		Tos_TaskDelay(10);
		 CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
		}
	}
}
void TaskCanRead(void *Tags)
{
  CanRxMsg *RxMessage;
	Tos_ArrayMessage *CurMsg;
	Tos_TaskGetCAN();
	while(1)
	{
		Tos_TaskDelay(10);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			RxMessage=CurMsg->Qmsg_Addr;
			switch(RxMessage->StdId)
			{
				case 0x1c0:
//			    Run_Value.Sig=Uint32FromByte(&RxMessage->Data[0]);
					break;
				default:break;
			}
		}
		Tos_TaskDelay(10);
	}
}

int main(void)
{
	RCCRC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_Can1();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",1000);
	Tos_TaskCreate(1,Usart_Cmd,Null,"Tast0",1000);
	Tos_TaskCreate(2,TaskCanRead,Null,"Tast0",1000);
	Tos_TaskCreate(3,Task_GetAngle,Null,"Tast0",1000);
	Tos_Start();
	while(1);
}


