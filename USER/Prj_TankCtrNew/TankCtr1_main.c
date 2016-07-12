#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.h>
#include <GUI_Include.h>
#include <FuncTask_Include.h>
#include <Sensor_Driver.h>
#include <..\USER\Prj_TankCtr\TankCom.h>
#include <..\USER\Prj_TankCtr\IO_Driver.c>
//#include <..\USER\Prj_TankCtr\TankCom.c>
#include <..\USER\Prj_TankCtr\App_TankCtr1.c>
#include <..\Driver\SZZigBee_Driver.c>
uint8 LinkCount;
void Task0(void *Tags)
{
	LED2_OFF;
	LED3_OFF;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	Remap_SWJ_JTAGDisable();
	//CTR_Test(3);
	CTR_SetUint32(0);
	DevRun_Value.AllStatus=0;
	LED1_ON;
	Tos_TaskDelay(2000);
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(30);
		LED1_OFF;
		Tos_TaskDelay(30);
		LED1_ON;
		Tos_TaskDelay(30);
		LED1_OFF;
		Tos_TaskDelay(400);
		if(LinkCount++>3)LED2_OFF;
		DevRun_Value.SIG_DATABIT=~SIG_GetUint32();
		if(DevRun_Value.SIG_DATABIT&S7)TankRun_Value.AllStatus|=WSA_IfTankRunDownTrue;else TankRun_Value.AllStatus&=~WSA_IfTankRunDownTrue;
		if(DevRun_Value.SIG_DATABIT&S0)TankRun_Value.AllStatus|=WSA_TankPowerCheckRealy;else TankRun_Value.AllStatus&=~WSA_TankPowerCheckRealy;
		if(TankRun_Value.AllStatus&WSA_IfTankRunDownTrue)
		{
		if(DevRun_Value.SIG_DATABIT&S3)TankRun_Value.AllStatus|=WSA_TapOpenRealy;else TankRun_Value.AllStatus&=~WSA_TapOpenRealy;
		if(DevRun_Value.SIG_DATABIT&S4)TankRun_Value.AllStatus|=WSA_TapCloseRealy;else TankRun_Value.AllStatus&=~WSA_TapCloseRealy;
		if(DevRun_Value.SIG_DATABIT&S6)TankRun_Value.AllStatus|=WSA_SealedInPlace;else TankRun_Value.AllStatus&=~WSA_SealedInPlace;
		}else
		{
		if(DevRun_Value.SIG_DATABIT&S1)TankRun_Value.AllStatus|=WSA_TapOpenRealy;else TankRun_Value.AllStatus&=~WSA_TapOpenRealy;
		if(DevRun_Value.SIG_DATABIT&S2)TankRun_Value.AllStatus|=WSA_TapCloseRealy;else TankRun_Value.AllStatus&=~WSA_TapCloseRealy;
		if(DevRun_Value.SIG_DATABIT&S5)TankRun_Value.AllStatus|=WSA_SealedInPlace;else TankRun_Value.AllStatus&=~WSA_SealedInPlace;
		}
	}
}

void TaskCommandRead(void *Tags)
{
	uint16 Length;
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(&USART1_RX_Buffer[0],100);
		Length=USART1_ReadDatas(Null,100,0);
		DeBug_Get();
		TankRun_Value.MasterAddr=GetWordFromBytes(&USART1_RX_Buffer[0],1);
		if(ZigBee_CheckCrc(&USART1_RX_Buffer[0]))
		{
			Length=4;
			switch(USART1_RX_Buffer[Length-2])
			{
				case SCFF_ButtonClick:App_SetButtonDelay(USART1_RX_Buffer[Length]);
					   Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],TankRun_Value.MasterAddr,SCFF_RefurBishSet1,(uint8 *)&TankRun_Value,sizeof(TankRun_Value));
	           USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length);
				     LED2_OFF;
				     LinkCount=0;
				  break;
			}
			LED2_ON;
			LinkCount=2;
		}
	}
}

int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Control();//挂在输出设备
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Flash();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,TaskCommandRead,Null,"Tast1",3000);
	Tos_TaskCreate(2,App_TankButton,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}









