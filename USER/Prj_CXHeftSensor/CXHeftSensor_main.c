#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.h>
#include <GUI_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_CXHeftSensor\Can_Driver.c>
#include <..\USER\Prj_CXHeftSensor\ADC_Driver.c>
#include <..\USER\Prj_CXHeftSensor\IO_Driver.c>

uint16 AD620_Value;
uint8 IO_Value;
void Task0(void *Tags)
{
	
	LED2_ON;
	LED1_ON;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	Tos_TaskDelay(2000);
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(900);
	}
}

/****************************************************************************
Date:2015-01-23
Func:应用任务
Note:
****************************************************************************/
void Task1(void *Tags)
{
	while(1)
	{
		LED2_ON;
		IO_Value=SIG_GetUint8();
		Tos_TaskDelay(100);
		LED2_OFF;
		Tos_TaskDelay(300);
	}
}
void SIG_Delay(uint8 D)
{
	Tos_TaskDelay(D);
	LED3_OFF;
	Tos_TaskDelay(D);
	LED3_ON;
}
void Task2(void *Tags)
{     
	uint16 ADC_Value;
	uint8 Data[10];
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{
		BufferCoppy(&ADCBuffer[0],&Data[0],2);
		CAN_SIG_SendMsg(&Data[0],0x1A1);//Printf_Bytes(&Data[0],8);
		SIG_Delay(200);
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
	DeviceMount_Led();
	DeviceMount_Flash();
	DeviceMount_Control();
	DeviceMount_Signel();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_ADCFitel,Null,"ADC-Reader",3000);
	Tos_TaskCreate(2,Task2,Null,"ADC-Reader",3000);
	Tos_Start();
	while(1);
}



