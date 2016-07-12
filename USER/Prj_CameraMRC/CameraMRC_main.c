#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_CAMERAMRC\IO_Driver.c>
#include <..\USER\Prj_CAMERAMRC\UsartCmd.c>
void Task0(void *Tags)
{
		uint8 Data[10];
	DeBug_OutSet(DeBug_USART2|DeBug_USART1);
	DeBug_SetType(Infor_All);
	while(1)
	{
		LED1_OFF;
		Tos_TaskDelay(100);
		LED1_ON;
		Tos_TaskDelay(100);
		SetWordToBytes(&Data[0],400,0);
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
				case 0x60:
			//    SensorWeightADC=GetWordFromBytes(&RxMessage->Data[0],0);
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
	DeviceMount_Usart1();
	DeviceMount_Usart2();
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_Can1();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_Start();
	while(1);
}


