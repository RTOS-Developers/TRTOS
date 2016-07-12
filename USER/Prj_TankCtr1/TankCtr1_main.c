#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.h>
#include <GUI_Include.h>
#include <FuncTask_Include.h>
#include <Sensor_Driver.h>
#include <..\USER\Prj_TankCtr\TankCom.h>
#include <..\USER\Prj_TankCtr\IO_Driver.c>
//#include <..\USER\Prj_TankCtr\TankCom.c>
#include <..\USER\Prj_TankCtr\App_TankCtr1.c>
#include <..\Driver\SZZigBee_Driver.c>
uint8 LinkCount;
void Task0(void *Tags)
{
	LED2_OFF;
	LED3_OFF;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	Remap_SWJ_JTAGDisable();
	//CTR_Test(3);
	CTR_SetUint32(0);
	DevRun_Value.AllStatus=0;
	LED1_ON;
	Tos_TaskDelay(2000);
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(500);
		LED1_OFF;
		Tos_TaskDelay(500);
//		DevRun_Value.SIG_DATABIT=~SIG_GetUint32();
//		if(DevRun_Value.SIG_DATABIT&S7)TankRun_Value.AllStatus|=WSA_IfTankRunDownTrue;else TankRun_Value.AllStatus&=~WSA_IfTankRunDownTrue;
//		if(DevRun_Value.SIG_DATABIT&S0)TankRun_Value.AllStatus|=WSA_TankPowerCheckRealy;else TankRun_Value.AllStatus&=~WSA_TankPowerCheckRealy;
//		if(TankRun_Value.AllStatus&WSA_IfTankRunDownTrue)
//		{
//		if(DevRun_Value.SIG_DATABIT&S3)TankRun_Value.AllStatus|=WSA_TapOpenRealy;else TankRun_Value.AllStatus&=~WSA_TapOpenRealy;
//		if(DevRun_Value.SIG_DATABIT&S4)TankRun_Value.AllStatus|=WSA_TapCloseRealy;else TankRun_Value.AllStatus&=~WSA_TapCloseRealy;
//		if(DevRun_Value.SIG_DATABIT&S6)TankRun_Value.AllStatus|=WSA_SealedInPlace;else TankRun_Value.AllStatus&=~WSA_SealedInPlace;
//		}else
//		{
//		if(DevRun_Value.SIG_DATABIT&S1)TankRun_Value.AllStatus|=WSA_TapOpenRealy;else TankRun_Value.AllStatus&=~WSA_TapOpenRealy;
//		if(DevRun_Value.SIG_DATABIT&S2)TankRun_Value.AllStatus|=WSA_TapCloseRealy;else TankRun_Value.AllStatus&=~WSA_TapCloseRealy;
//		if(DevRun_Value.SIG_DATABIT&S5)TankRun_Value.AllStatus|=WSA_SealedInPlace;else TankRun_Value.AllStatus&=~WSA_SealedInPlace;
//		}
	}
}
void Task_CAN_SIGMY(void *Tags)
{
	uint8 Data[10];
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{		
		LED3_ON;
		Uint32ToByte(&Data[0],SIG_GetUint32());
		CAN_SIG_SendMsg(&Data[0],0x90);
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(100);
	}
}
void CAN_Istr(CanRxMsg *RxMessage)
{
	switch(RxMessage->StdId)
			{
			case 0x91:App_SetButtonDelay(RxMessage->Data[0]);break;
			}
}
void TaskCanRead(void *Tags)
{
	uint8 i;
	Tos_ArrayMessage *CurMsg,Msg[10]; 
  CanRxMsg *RxMessage,RxMessageBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&RxMessageBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetCAN();
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{
		
		Tos_TaskDelay(1);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			LED2_OFF;
			RxMessage=CurMsg->Qmsg_Addr;
			switch(RxMessage->StdId)
			{
				case 0x91:LED2_ON;App_SetButtonDelay(RxMessage->Data[0]);
			}
		}
		Tos_TaskDelay(100);
		
	}
}
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Control();//挂在输出设备
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Can1();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(2,App_TankButton,Null,"Tast2",3000);
	Tos_TaskCreate(3,Task_CAN_SIGMY,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}









