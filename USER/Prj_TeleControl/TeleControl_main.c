#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <Tos_Mema.h>
#include <AppConfig.h>
#include <..\USER\Prj_TeleControl\IO_Driver.c>
#include <..\USER\Prj_TeleControl\ADC_Driver.c>
/****************************************************************************
Date:2015-01-23
Func:ÈÎÎñÒ»
Note:
****************************************************************************/
void Task0(void *Tags)
{
	LED2_OFF;
	BEEP_OFF;
	Tos_TaskDelay(10);
	BEEP_ON;
	CTR1_OFF;
	CTR2_OFF;
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(100);
	}
}
typedef struct
{
	ConfigHeadStruct CHS;
	uint8 Cmd;
	uint8 Buf[100];
}TOSBUS;

void Task_Send(void *Tags)
{
	TOSBUS TB;
	TB.Cmd=0x00;
	Create_Message(&TB,sizeof(TB));
	while(1)
	{
		Tos_TaskDelay(100);
		USART_WriteDatas(USART3,(uint8 *)&TB,sizeof(TB));
		USART_WriteDatas(USART2,(uint8 *)&TB,sizeof(TB));
	}
}
void CAN_Istr(CanRxMsg *RxMessage)
{
	switch(RxMessage->StdId)
			{
				case 0x060:
					break;
				case 0x061:LED2_ON;
					break;
				case 0x062:
					break;
				case 0x063:
					break;
				case 0x2a0:LED2_ON;
					break;
				case 0x1C0:LED2_ON;
				  break;
				case 0x90:LED2_ON;
					break;
				default:break;
			}
}
void Task_Revice(void *Tags)
{
	uint8 Buf[100];
	TOSBUS *TB;
	uint8 Length;
	while(1)
	{
		Tos_TaskDelay(100);
	  Length=USART_ReadDatas(USART3,&Buf[0],100,0);
		if(Export_Message(&Buf[0],Length))
		{
			TB=(TOSBUS *)&Buf[0];
			switch(TB->Cmd)
			{
				case 0x00:break;
				case 0x01:break;
				default:break;
			}
		}
		LED2_ON;
		Tos_TaskDelay(100);
		LED2_OFF;
		Tos_TaskDelay(100);
	}
}
const TaskInitList TaskList[]={
{Task0,Null,"Task0",2000},
{Task_Send,Null,"Task_Send",2000},
//{Task_Revice,Null,"Task_Revice",2000},
{Null},
};
uint8 MemBuf[10000];
int main(void)
{
	Remap_SWJ_JTAGDisable();
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Control();
	DeviceMount_Usart(USART2);
	DeviceMount_Usart(USART3);
  DeviceMount_Signel();
	DeviceMount_Can1();
	Tos_ListCreate(TaskList);
	while(1);
}



