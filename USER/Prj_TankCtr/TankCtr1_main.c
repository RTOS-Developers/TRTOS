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
	}
}
void Task_CAN_SIGMY(void *Tags)
{
	uint8 Data[10];
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
				default:break;
			}
}
uint8 MemaBuf[40000];
int main(void)
{
	Mema_Init(&MemaBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Control();//挂在输出设备
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Can1();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(2,App_TankButton,Null,"Tast2",3000);
	Tos_TaskCreate(3,Task_CAN_SIGMY,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}





















