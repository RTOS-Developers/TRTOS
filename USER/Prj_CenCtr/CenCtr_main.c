#include <Include.h>
#ifdef BOARD_CENCTR

void Task0(void *Tags)
{
	Printf_SysInfor();
	while(1)
	{
		LED_ON;
		Tos_TaskDelay(30);
		LED_OFF;
		Tos_TaskDelay(100);	
		LED_ON;
		Tos_TaskDelay(30);
		LED_OFF;
		Tos_TaskDelay(1000);	
	}
}


void Task1(void *Tags)
{

	while(1) 
	{
		Tos_TaskDelay(1000);
		
	}
}
void Task2(void *Tags)
{

	while(1)
	{
		Tos_TaskDelay(10);
	}
}
void Task3(void *Tags)
{
	while(1)
	{
		Tos_TaskDelay(1000);
	}
}

void Task4(void *Tags)
{
	while(1)
	{
		Tos_TaskDelay(50);
	}
}

int main(void)
{
	RCC_Configuration();//时钟控制器配置
	DeBug_Init();//调试功能初始化
	DeviceList_Init();//设备列表初始化
	DeviceMount_Usart1();//挂载Usart串口1
	DeviceMount_Led();//挂载led设备（led其实可以不加入设备管理）
	DeviceMount_Flash();//挂载flash
	DeviceMount_TQFS();//挂载TQFS文件系统
	Device_Init();//对挂载的设备进行初始化
	Tos_Init();//Tos系统初始化
	Tos_TaskCreate(0,Task0,3000);
	Tos_TaskCreate(1,Task1,3000);
	Tos_TaskCreate(2,Task2,2000);
	Tos_TaskCreate(3,Task3,2000);
	Tos_TaskCreate(4,Task4,2000);
	Tos_Start();//Tos系统启动
	while(1);
}
#endif

