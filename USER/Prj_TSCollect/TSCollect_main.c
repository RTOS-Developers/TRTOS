#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.h>
#include <GUI_Include.h>
#include <FuncTask_Include.h>
#include <Sensor_Driver.h>
#include <..\USER\Prj_TankCtrNew\TankCom.h>
#include <..\Driver\SZZigBee_Driver.c>
#include <..\USER\Prj_TankCtrNew\IO_Driver.c>
#include <..\USER\Prj_TankCtrNew\TankCom.c>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.c>
void Task0(void *Tags)
{
	

	LED2_OFF;
	LED3_ON;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	Remap_SWJ_JTAGDisable();
	LED1_ON;
	Tos_TaskDelay(2000);
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(900);
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
		CAN_SIG_SendMsg(&Data[0],0x1C0);
		Tos_TaskDelay(250);
		LED3_OFF;
		Tos_TaskDelay(250);
	}
}
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Led();
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Flash();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_CAN_SIGMY,Null,"Tast2",3000);
	Tos_TaskCreate(2,TaskReadCanToCom,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}



