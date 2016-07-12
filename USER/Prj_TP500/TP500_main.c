#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Tos_Mema.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_TP500\IO_Driver.c>
#include <..\USER\Prj_TP500\Ht1621_Driver.c>
#include <..\USER\Prj_TP500\Ht1621_Face.c>
#include <..\USER\Prj_TP500\DHT11_Driver.c>
#include <..\USER\Prj_TP500\App_Sensor.c>
#include <Rtc_Driver.c>
#include <..\USER\Prj_TP500\GU900_MMS.c>
#include <..\USER\Prj_TP500\App2.c>
#include <..\USER\Prj_TP500\App1.c>
void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_Error);//只打印Infor类型的消息
	Printf_SysInfor();
	UIShowValue.RunFlag=0;
	while(1)
	{
		Tos_TaskDelay(1000);
		if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH1Min)){LED1_ON;BEEP_ON;Tos_TaskDelay(10);BEEP_OFF;}
		if(UIShowValue.RunFlag&(WRF_CH2Max|WRF_CH2Min)){LED2_ON;BEEP_ON;Tos_TaskDelay(10);BEEP_OFF;}
		LED3_ON;
		Tos_TaskDelay(1000);
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
	}
}

void Task_Face(void *Tags)
{
	BEEP_ON;
	Tos_TaskDelay(1);
	BEEP_OFF;
	while(1)
	{
		RTC_Get();
		Ht1621_DrawHome();
		DHT11_Read(&UIShowValue.CH_Value[0],&UIShowValue.CH_Value[1]);
		Tos_TaskDelay(500);	
	}
}
const TaskInitList TaskList[]={
{Task0,Null,"Task0",4000},
{TaskFunc,Null,"Task2",4000},
{Task_Face,Null,"Task_Face",4000},
{Task_KeyBoard,Null,"Task3",4000},
{Sensor_Command,USART1,"Task3",4000},
{TaskPlan,Null,"Task3",4000},
{Task_GPRS,Null,"Task3",8000},
{Null},
};
int main(void)
{
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_HT1621();
	DeviceMount_RTC();
	Tos_ListCreate(TaskList);
	while(1);
}



