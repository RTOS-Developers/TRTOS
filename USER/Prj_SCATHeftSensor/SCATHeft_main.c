#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Sensor_Driver.h>
#include <..\USER\Prj_SCATHeftSensor\IO_Driver.c>
#include <..\USER\Prj_SCATHeftSensor\IO_Driver.h>
#include <..\USER\Prj_SCATHeftSensor\ADC_Driver.c>
#include <..\USER\Prj_SCATHeftSensor\ADC_Driver.h>
#include <..\Driver\SZZigBee_Driver.c>
void Task0(void *Tags)
{
	

	LED2_OFF;
	LED3_ON;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	Remap_SWJ_JTAGDisable();
	LED1_ON;
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(500);
		LED1_OFF;
		Tos_TaskDelay(500);
	}
}




void Task_CAN_SIGMY(void *Tags)
{
	uint8 Data[10];
	uint16 Temp;
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{		
		LED3_ON;
		//Uint32ToByte(&Data[0],SIG_GetUint32());
		BufferCoppy(&ADC_Buffer[0],&Data[0],2);
		CAN_SIG_SendMsg(&Data[0],0x7C0);
		Temp=Data[1];
		Temp<<=8;
		Temp|=Data[0];
		DeBug("%d",Temp);
		Tos_TaskDelay(250);
		LED3_OFF;
		Tos_TaskDelay(250);
	}
}
void Sensor_Command(void *Tags)
{
	uint16 Length;
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(&USART1_RX_Buffer[0],100);
		Length=USART1_ReadDatas(Null,100,0);
		DeBug_Get();
		if(ZigBee_CheckCrc(&USART1_RX_Buffer[0]))
		{
			Length=4;
			switch(USART1_RX_Buffer[Length-2])
			{
				case SCFF_EraseFlase:if(USART1_RX_Buffer[8]==0x19)if(USART1_RX_Buffer[9]==0x89)if(USART1_RX_Buffer[10]==0x11)if(USART1_RX_Buffer[11]==0x06)MCU_Reset();break;
				default:break;
			}
			LED2_ON;
		}
	}
}
void TaskReadCanToCom(void *Tags)
{
	uint16 Length;
	uint8 i;
	Tos_ArrayMessage *CurMsg,Msg[10]; 
  CanRxMsg *RxMessage,RxMessageBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&RxMessageBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetCAN();
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{
		//DeBug_Get();
		Tos_TaskDelay(100);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			RxMessage=CurMsg->Qmsg_Addr;
			Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],20,0xfe,(uint8 *)RxMessage,sizeof(CanRxMsg));
			USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
		}
		LED3_ON;
		Tos_TaskDelay(10);
		LED3_OFF;
	}
}
int main(void)
{
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Control();
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Flash();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_CAN_SIGMY,Null,"Tast2",3000);
	Tos_TaskCreate(2,Task_ADCFitel,Null,"Tast2",3000);
	Tos_TaskCreate(3,TaskReadCanToCom,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}



