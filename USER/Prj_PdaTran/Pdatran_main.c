#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <App_Pdatran.h>
#include <App_Pdatran.c>

/****************************************************************************
Date:2014-11-12
Func:主任务一般不用来用作处理事务
Note:可以通过指示灯来判断程序运行状态
****************************************************************************/
void Task0(void *Tags)
{
		LED_ON;
		Tos_TaskDelay(1000);
	while(1)
	{
		LED_ON;
		Tos_TaskDelay(100);
		LED_OFF;
		Tos_TaskDelay(900);
		if(Pda_Status)
		{
			Pda_Status=0;
			LED_ON;
			Tos_TaskDelay(10);
			LED_OFF;
			Tos_TaskDelay(10);
			LED_ON;
			Tos_TaskDelay(10);
			LED_OFF;
			Tos_TaskDelay(10);
		}
	}
}

int main(void)
{
	RCC_Configuration();
	DeviceList_Init();
	DeviceMount_Led();
	DeviceMount_Flash();
	DeviceMount_Usart1();
	DeviceMount_Usart2();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Task1",500);
	Tos_TaskCreate(1,Task_PdaTran,Null,"Task2",1500);
	Tos_TaskCreate(2,Task_PdaGetDev,Null,"Task3",1500);
	Tos_Start();
	while(1);
}


