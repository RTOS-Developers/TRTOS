#include <Include.h>
#ifdef BOARD_SENSOR
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <Driver_Include.h>
#include <App_Sensor.h>
#include <App_Sensor.c>
void Task0(void *Tags)
{

	DeBug_OutSet(DeBug_USART);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();

// DeBug("Tos_CPUInfor.Userate_CPU=%d",FLASH_WriteDatas(0,&Buf[0],10),Infor_Infor);
	while(1)
	{
		LED_ON;
		Tos_TaskDelay(30);
		LED_OFF;
		Tos_TaskDelay(30);	
		LED_ON;
		Tos_TaskDelay(30);
		LED_OFF;
		Tos_TaskDelay(910);
	
	//	EN_INT;
	}
}


void Task1(void *Tags)
{
	while(1)
	{
		
		Tos_TaskDelay(100);
	}
}
void TaskReadADC(void *Tags)
{
	int16 ADC;
	IniCS1168();
	//AD7705_Init(1);
	Tos_TaskCreate(2,Sensor_Command,(uintbus)&ADC,"Sensor_Command",1000);
	while(1)
	{
		Tos_TaskDelay(500);
		ADC=CS1168_ReadADC();
		//ADC=AD7705_ReadFitel();
		DeBug("CS1180_ADC1=%x",GetLockCode(),Infor_Infor);
		
	}
}
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Led();
	DeviceMount_Flash();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Task_System",1000);
	//Tos_TaskCreate(1,Task_CAN_Slave,1000);
	Tos_TaskCreate(1,Sensor_Command,Null,"Task_System",1000);
	Tos_TaskCreate(3,TaskReadADC,Null,"TaskReadADC",2000);
	Tos_Start();
	while(1);
}


#endif

