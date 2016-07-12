#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_WJ4_3\IO_Driver.c>
#include <..\USER\Prj_WJ4_3\DaCai_Driver.c>
#include <..\USER\Prj_WJ4_3\ADC_Driver.c>
void Task0(void *Tags)
{
	LEDLINK_OFF;
	LEDSRART_OFF;
	LED3_RG;
	LED2_R;
	LED3_R;
	LED1_R;
	Tos_TaskDelay(2);
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	while(1)
	{
		LEDRUN_OFF;
		Tos_TaskDelay(1000);
		LED1_G;
		LEDRUN_ON;
		LEDSRART_ON;
		LEDLINK_OFF;
		Tos_TaskDelay(100);
		LED1_R;
		LEDSRART_OFF;
		LEDLINK_ON;
	}
}
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Control();
	DeviceMount_Signel();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_Start();
	while(1);
}


