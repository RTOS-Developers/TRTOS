#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_CAMERAMOTR\IO_Driver.c>
#include <..\Driver\SZZigBee_Driver.c>
#include <..\USER\Prj_CAMERAMOTR\Moto.c>
#include <..\USER\Prj_CAMERAMOTR\SCA100t.c>
#include <..\USER\Prj_CAMERAMOTR\UsartCmd.c>


void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART2|DeBug_USART1);
	DeBug_SetType(Infor_All);
	while(1)
	{
		LED1_OFF;
		Tos_TaskDelay(100);
		LED1_ON;
		Tos_TaskDelay(100);
	//	if(K90)LED2_OFF;else LED2_ON;
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
				case 0x90:
					DefValue=GetWordFromBytes(&RxMessage->Data[0],0);
					break;
				default:break;
			}
		}
		Tos_TaskDelay(10);
	}
}
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_Can1();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_Moto,Null,"Tast0",3000);
	//Tos_TaskCreate(2,Task_GetAngle,Null,"Tast0",1000);
//	Tos_TaskCreate(3,Usart_Moto,Null,"Tast0",2000);
//	Tos_TaskCreate(4,TaskCanRead,Null,"Tast0",2000);
	Tos_Start();
	while(1);
}


