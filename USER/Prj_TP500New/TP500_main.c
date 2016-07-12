#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Tos_Mema.h>
#include <FuncTask_Include.h>
#include <isl1208_driver.h>
#include <..\USER\Prj_TP500New\IO_Driver.c>
#include <..\USER\Prj_TP500New\Ht1621_Driver.c>
#include <..\USER\Prj_TP500New\Ht1621_Face.c>
#include <..\USER\Prj_TP502T\TempHui_Driver.c>
#include <..\USER\Prj_TP502T\App_Sensor.c>
#include <SZZigBee_Driver.c>
#include <W25Q16_Driver.c>
#include <isl1208_driver.c>
#include <..\USER\Prj_TP502T\App1.h>
#include <..\USER\Prj_TP502T\GB2312_Unicode.c>
#include <..\USER\Prj_TP502T\ADC_Driver.c>
#include <..\USER\Prj_TP502T\GPS_Driver.c>
#include <AT_Command.c>
#include <..\USER\Prj_TP502T\QuectelM26New.c>
#include <..\USER\Prj_TP502T\UpgradeByGPRS.c>
#include <..\USER\Prj_TP502T\Auto_Register.c>
#include <..\USER\Prj_TP502T\App2.c>
#include <..\USER\Prj_TP500New\App1.c>

void Task0(void *Tags)
{
	Task0_Init();
	UIShowValue.SysWorkMode=0;
	LED2_OFF;
	LED1_OFF;
	while(1)
	{
		
	  switch(UIShowValue.SysWorkMode)
		{
			case 0:
				Tos_TaskDelay(1000);
				LED1_OFF;
				LED2_OFF;
			if(!(UIShowValue.RunFlag&WRF_BeepDisable))
			{
				if(UIShowValue.RunFlag&WRF_SensorCH1Connected)
				if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH1Min)){LED1_ON;BEEP_ON;Tos_TaskDelay(10);BEEP_OFF;LED1_OFF;}
				if(UIShowValue.RunFlag&WRF_SensorCH2Connected)
				if(UIShowValue.RunFlag&(WRF_CH2Max|WRF_CH2Min)){LED2_ON;BEEP_ON;Tos_TaskDelay(10);BEEP_OFF;LED2_OFF;}
				Tos_TaskDelay(1000);
			}else Tos_TaskDelay(100);
				break;
			default:
				Tos_TaskDelay(100);
				LED2_ON;
				LED1_OFF;
				Tos_TaskDelay(100);
				LED1_ON;
				LED2_OFF;break;
		}
	}
}


void Task_Face(void *Tags)
{
	BEEP_ON;
	Tos_TaskDelay(1);
	BEEP_OFF;
	E3V3_ON;
	Tos_TaskDelay(100);
	HT1621_Init();
	Tos_TaskDelay(1000);
	while(1)
	{
		 switch(UIShowValue.SysWorkMode)
		{
			case 0:Ht1621_DrawHome();break;
			case 1:Ht1621_DrawUpgrade();break;
			case 2:Ht1621_DrawUpgrade();break;
			case 3:Ht1621_DrawUpgrade();break;
			case 4:Ht1621_DrawUpgrade();break;
			case 8:Ht1621_DrawUpDown(0);break;
			case 9:Ht1621_DrawUpDown(1);break;
			case 10:Ht1621_DrawUpDown(2);break;
			case 11:Ht1621_DrawUpDown(3);break;
			case 12:Ht1621_DrawUpDown(4);break;
			case 0xff:break;
		}
		Tos_TaskDelay(1000);
	}
}
void Task_GPRSSwitch(void *Tags)
{
	while(1)
	{
		  switch(UIShowValue.SysWorkMode)
			{
				case 0:Task_M26(Null);break;
				case 1:Task_DownConfig(Null);UIShowValue.SysWorkMode=0;break;
				case 2:Task_UpConfig(Null);UIShowValue.SysWorkMode=0;break;
				case 3:Task_Upgrade(Null);UIShowValue.SysWorkMode=0;break;
				case 4:Task_Upgrade(Null);UIShowValue.SysWorkMode=0;break;
				case 0xff:break;
				default:break;
			}
			Tos_TaskDelay(1000);
	}
}
const TaskInitList TaskList[]={
{Task0,Null,"Task0",2000},
{TaskFunc,Null,"TaskFunc",2000},
{Task_Face,Null,"Task_Face",2000},
{Task_KeyBoard,Null,"Task_KeyBoard",2000},
{Sensor_Command,USART1,"Sensor_Command",2000},
{TaskPlan,Null,"TaskPlan",2000},
{Task_M26,Null,"Task_GPRSSwitch",4000},
{Task_Arrary,Null,"Task_Arrary",2000},
{Task_Gps,Null,"Task_Gps",2000},
{Task_HistoryRecord,Null,"Task_HistoryRecord",2000},
{Null},
};
const GPIO_InitStruct IO_ISL1208_List[]=
{
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOC,GPIO_Pin_3},//SDA
	{GPIO_Speed_50MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_2},//SCL
	{0},
}; 
const GPIO_InitStruct IO_Control_List[]=
{
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_2|GPIO_Pin_12|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_13},
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_9|GPIO_Pin_8},
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOC,GPIO_Pin_6|GPIO_Pin_7},
	{GPIO_Speed_50MHz,GPIO_Mode_IPD,GPIOA,GPIO_Pin_0},
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOA,GPIO_Pin_8},
	{GPIO_Speed_50MHz,GPIO_Mode_IPU,GPIOC,GPIO_Pin_1|GPIO_Pin_0},
	{GPIO_Speed_50MHz,GPIO_Mode_IN_FLOATING,GPIOB,GPIO_Pin_4|GPIO_Pin_5},
	{0},
};
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000); 
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_Usart(USART3);
	DeviceMount_ISL1208(IO_ISL1208_List);
	DeviceMount_IO(IO_Control_List);
	DeviceMount_W25Q16();
	DeviceMount_HT1621();
	Tos_ListCreate(TaskList);
	while(1);
}



