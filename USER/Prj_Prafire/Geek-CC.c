#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.c>
#include <Tos_Mema.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_Prafire\IO_Driver.c>
#include <..\USER\Prj_Prafire\Geek-Sample.c>
#include <..\USER\Prj_Prafire\Geek_Update.c>
#include <W25Q16_Driver.c>
void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART2);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	while(1)
	{
		//Geek_Export(Geek_String);
		Geek_CheckUpdate(USART2);
		Tos_TaskDelay(1000);
	}
}
const TaskInitList TaskList[]={
{Task0,Null,"Task0",4000},
{Null},
};

int main(void)
{
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Lcd();
	DeviceMount_Usart(USART2);
	DeviceMount_IO();
	Tos_ListCreate(TaskList);
	while(1);
}



