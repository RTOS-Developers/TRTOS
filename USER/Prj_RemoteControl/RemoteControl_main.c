#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_RemoteControl\IO_Driver.c>
#include <..\USER\Prj_RemoteControl\ADC_Driver.c>
#include <..\USER\Prj_RemoteControl\24L01_Driver.c>
void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	LED2_OFF;
	LED4_ON;
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(100);
	}
}



/****************************************************************************
Date:2015-01-23
Func:主函数
Note:
****************************************************************************/
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Flash();
	DeviceMount_Control();
	DeviceMount_Signel();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",2000);
	Tos_TaskCreate(1,Nrf_Task,Null,"Tast0",2000);
	Tos_TaskCreate(2,Task_ADCFitel,Null,"Tast0",2000);
	Tos_Start();
	while(1);
}



