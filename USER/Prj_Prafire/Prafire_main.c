#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <..\USER\Prj_Prafire\GBK_Lib.c>
#include <LCD_Driver.c>
#include <GUI_Include.c>
#include <TGui_3DCube.c>
#include <Tos_Mema.h>
//#include <..\FS\FATEST\FATEST.c>
#include <FuncTask_Include.h>
#include <SZZigBee_Driver.c>
//#include <I2C1_Driver.c>
#include <..\USER\Prj_Prafire\IO_Driver.c>
#include <W25Q16_Driver.c>
//#include <..\USER\Prj_Prafire\USB\Usb_All.c>
#include <..\USER\Prj_Prafire\24L01_Driver.c>
#include <..\USER\Prj_Prafire\PageApp.c>
#include <..\USER\Prj_Prafire\Menu_Base.c>
//#include <..\USER\Prj_Prafire\App_Prafire.c>
#include <..\USER\Prj_Prafire\LifePlay.c>
#include <..\USER\Prj_Prafire\Tetris.c>
#include <..\USER\Prj_Prafire\PictureMosaic.c>
#include <..\USER\Prj_Prafire\CramSnake.c>
#include <..\USER\Prj_Prafire\Page.c>
#include <..\USER\Prj_Prafire\NET\Net.c>
#include <WS2812_Driver.c>



void Task0(void *Tags)
{
	uint32  Color[38];
	uint8 i,j,p=0;
	while(1)
	{

		Tos_TaskDelay(100);
	}
}

void Task2(void *Tags)
{
	Coordinate_3D  *Tem=(Coordinate_3D  *)Tags;
	while(1)
	{
		TGui_3DCobe(Tem->X,Tem->Y,Tem->Z,Tem->D);
	}
}

const TaskInitList TaskList[]={
{Task0,Null,"Task0",4000},
{Web_App,Null,"Web_App",2000},
{MUI_Task,(void *)&MenuHome[0],"MUI_Task",2000},
{Task2,(void *)&C3D_Default,"CUBE",2000},
{Task_GetKeyBoardZigbee,(void *)USART2,"Task_GetKeyBoardZigbee",2000},
{Task_GetKey,Null,"Task_GetKey",2000},
{Task_Arrary,Null,"Task_Arrary",2000},
//{Task_SDFAT,Null,"Task_SDFAT",10000},
{Null},
};

int main(void)
{
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Lcd();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_W25Q16();
//	DeviceMount_FS();
	//DeviceMount_EEPROM();
//	DeviceMount_Usb();
	DeviceMount_IO();
	Tos_ListCreate(TaskList);
	while(1);
}




