#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.c>
#include <GUI_Include.h>
#include <FuncTask_Include.h>
#include <Sensor_Driver.h>
#include <Tos_Mema.h>
#include <..\Driver\FM24C256_Driver.c>
#include <..\USER\Prj_TankHand\IO_Driver.c>
#include <..\Driver\SZZigBee_Driver.c>
#include <..\USER\Prj_TankHand\Local_GBKLib.c>
#include <..\Driver\ADCForTankHand_Driver.c>
#include <..\USER\Prj_TankHand\KeyBoard_Driver.c>
#include <..\USER\Prj_TankHand\Oled_GUI.c>
#include <..\USER\Prj_TankHand\App_TankHand.c>
#include <..\USER\Prj_TankHand\App1_TankHand.c>

void Task0(void *Tags)
{
	SPK_ON;
	LED2_OFF;
	LED3_OFF;
	HANDPOWER_ON;
	ADCForTankHand_Init();
	Tos_TaskDelay(20);
	SPK_OFF;
	Remap_SWJ_JTAGDisable();
	Lamp_Test(2);
	Send_OnlyCommand(SCFF_RefurBishSet);
	while(1)
	{
		LampOutShow();
		Run_Value.Voltage=Get_BatteryVoltage();
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(400);
		if(Run_Value.LinkCount++>1)LAMP13_ON;
		if(CheckPowerLow(Run_Value.Voltage)){Sys_Standby();Sys_Enter_Standby();}
	}
}
void TaskCommandRead(void *Tags)
{
	uint16 Length;
	MGUI_KeyMsg KeyMsg;
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(USART_RX_Buffer[0],300);
		Length=USART_ReadDatas(USART1,Null,300,0);
		if(ZigBee_CheckCrc(USART_RX_Buffer[0]))
		{
			Length=4;
			switch(USART_RX_Buffer[0][Length-2])
			{
				case SCFF_RefurBishSet:
					    BufferCoppy(&USART_RX_Buffer[0][Length],&DevRun_Value,sizeof(DevRun_Value));
				      LAMP13_OFF;
				      Run_Value.LinkCount=0;
				      KeyMsg.KeyType=KeyBoard_Null;
				      Tos_TaskPutQmsg(HandKey_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
						 break;
				default:break;
			}
		}
	}
}
uint8 MemaBuf[40000];
int main(void)
{
	Mema_Init(&MemaBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Lcd();
	DeviceMount_Ctr();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_GetHandKey,Null,"Tast1",3000);
	Tos_TaskCreate(2,TaskCommandRead,Null,"Tast2",3000);	
	Tos_TaskCreate(3,TaskFor_SetWork,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}


