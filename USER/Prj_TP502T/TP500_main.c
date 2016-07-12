#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Tos_Mema.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_TP502T\isl1208_driver.h>
#include <IO_Driver.c>
#include <..\USER\Prj_TP502T\Ht1621_Driver.c>
#include <..\USER\Prj_TP500New\Ht1621_Face.c>
#include <..\USER\Prj_TP502T\TempHui_Driver.c>
#include <..\USER\Prj_TP502T\App_Sensor.c>
#include <SZZigBee_Driver.c>
#include <W25Q16_Driver.c>
#include <..\USER\Prj_TP502T\isl1208_driver.c>
#include <..\USER\Prj_TP502T\App1.h>
#include <..\USER\Prj_TP502T\ADC_Driver.c>
#include <..\USER\Prj_TP502T\GPS_Driver.c>
#include <..\USER\Prj_TP502T\GB2312_Unicode.c>
#include <AT_Command.c>
#include <..\USER\Prj_TP502T\QuectelM26New.c>
#include <..\USER\Prj_TP502T\Auto_Register.c>
#include <..\USER\Prj_TP502T\UpgradeByGPRS.c>
#include <..\USER\Prj_TP502T\App2.c>
#include <..\USER\Prj_TP502T\App1.c>

void Task0(void *Tags)
{
	
	Task0_Init();
	IO_Set(0,True);
	while(1)
	{
			if(UIShowValue.RunFlag&WRF_GPRS_Connected)
			{
				IO_Set(1,True);
				Tos_TaskDelay(2000);
				IO_Set(1,False);
				Tos_TaskDelay(10);
				IO_Set(1,True);
				Tos_TaskDelay(100);
				IO_Set(1,False);
				Tos_TaskDelay(10);
			}else
			if(UIShowValue.RSSI<99&UIShowValue.RSSI>8)
			{
				IO_Set(1,True);
				Tos_TaskDelay(1000);
				IO_Set(1,False);
				Tos_TaskDelay(10);
			}else 
			{
				
				IO_Set(1,True);
				Tos_TaskDelay(1000);
				IO_Set(1,False);
				Tos_TaskDelay(1000);
			}
	}
}

const GPIO_InitStruct IO_ISL1208_List[]=
{
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOB,GPIO_Pin_11},//SDA
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_10},//SCL
	{0},
};
const GPIO_InitStruct IO_IO_List[]=
{
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_7},
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_6},
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_13},
	{GPIO_Speed_50MHz,GPIO_Mode_IPD,GPIOA,GPIO_Pin_0},
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_8|GPIO_Pin_9},
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_3|GPIO_Pin_2},
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOC,GPIO_Pin_1|GPIO_Pin_0},
	{0},
};
const TaskInitList TaskList[]={
{Task0,Null,"Task0",2000},
{Sensor_Command,USART1,"Sensor_Command",2000}, 
{TaskPlan,Null,"TaskPlan",2000},
{Task_M26,Null,"Task_M26",6000},
{Task_Arrary,Null,"Task_Arrary",2000},
{Task_HistoryRecord,Null,"Task_HistoryRecord",2000},
{Null},
};
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000); 
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_ISL1208(IO_ISL1208_List);
	DeviceMount_IO(IO_IO_List);
	DeviceMount_W25Q16();
	Tos_ListCreate(TaskList);
	while(1);
}



