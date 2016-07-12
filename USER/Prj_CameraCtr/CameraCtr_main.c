#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_CAMERACTR\IO_Driver.c>
#include <..\USER\Prj_CAMERACTR\Moto.c>
#include <..\USER\Prj_CAMERACTR\SCA100t.c>
#include <..\USER\Prj_CAMERACTR\UsartCmd.c>
void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART2|DeBug_USART1);
	DeBug_SetType(Infor_All);
	while(1)
	{
		LED1_OFF;
		Tos_TaskDelay(500);
		LED1_ON;
		Tos_TaskDelay(500);
	}
}
void Task_Moto(void *Tags)
{
	uint16 Door=1700;
	while(1)
	{
		uint16 i =0;
	MOTOR_EN(0);
	if(AngleXY[0]<(Door-10))MOTOR_DIR(MOVE_UP);
	else if(AngleXY[0]>(Door+10))MOTOR_DIR(MOVE_DN);
	else MOTOR_EN(1); 
	for(i=0; i < 2 ;i++)
	{
	 MOTOR_PLUSE(0);
	 Tos_TaskDelay(PULSE_DELAY);
	 MOTOR_PLUSE(1);  
	 Tos_TaskDelay(PULSE_DELAY);
	}
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
				case 0x60:
			    SensorWeightADC=GetWordFromBytes(&RxMessage->Data[0],0);
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
	Tos_TaskCreate(1,Task_Moto,Null,"Tast0",1000);
	Tos_TaskCreate(2,TaskCanRead,Null,"Tast0",1000);
	Tos_TaskCreate(3,Usart_Cmd,Null,"Tast0",1000);
	Tos_Start();
	while(1);
}


