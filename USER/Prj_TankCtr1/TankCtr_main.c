#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <Sensor_Driver.h>
#include <..\USER\Prj_TankCtr\TankCom.h>
#include <..\Driver\SZZigBee_Driver.c>
#include <..\USER\Prj_TankCtr\IO_Driver.c>
#include <..\USER\Prj_TankCtr\TankCom.c>
#include <..\USER\Prj_TankCtr\App_TankCtr.c>

void Task0(void *Tags)
{
	

	LED2_OFF;
	LED3_OFF;
	DeBug_OutSet(Null);//调试信息通过串口打印
	DeBug_SetType(Null);//只打印Infor类型的消息
	//Printf_SysInfor();
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
		Tos_TaskDelay(900);
		DevRun_Value.SIG_DATABIT=~SIG_GetUint32();
		if(LinkCount--==0);LED2_OFF;
		if(DevRun_Value.SIG_DATABIT&S0)DevRun_Value.AllStatus|=WSA_SysPowerOn;else DevRun_Value.AllStatus&=~WSA_SysPowerOn;
		if(DevRun_Value.SIG_DATABIT&S1)DevRun_Value.AllStatus|=WSA_SysError;else DevRun_Value.AllStatus&=~WSA_SysError; 
	}
}
void TaskCanRead(void *Tags)
{
	uint8 i;
	uint32 ADALL;
	Tos_ArrayMessage *CurMsg,Msg[10]; 
  CanRxMsg *RxMessage,RxMessageBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&RxMessageBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetCAN();
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	DevRun_Value.ADC_Value[0]=0;
	DevRun_Value.ADC_Value[1]=0;
	while(1)
	{
		Tos_TaskDelay(10);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			RxMessage=CurMsg->Qmsg_Addr;
			switch(RxMessage->StdId)
			{
				case 0x060:
					DevRun_Value.AD6x[0]=GetWordFromBytes(&RxMessage->Data[0],1);
				  DevRun_Value.AllStatus|=WSA_SensorG1Ok;
					break;
				case 0x061:
				  DevRun_Value.AD6x[1]=GetWordFromBytes(&RxMessage->Data[0],1);
				  DevRun_Value.AllStatus|=WSA_SensorG2Ok;
					break;
				case 0x062:
					DevRun_Value.AD6x[2]=GetWordFromBytes(&RxMessage->Data[0],1);
				  DevRun_Value.AllStatus|=WSA_SensorG3Ok;
					break;
				case 0x063:
				  DevRun_Value.AD6x[3]=GetWordFromBytes(&RxMessage->Data[0],1);
				  DevRun_Value.AllStatus|=WSA_SensorG4Ok;
					break;
				case 0x2a0:
				  DevRun_Value.ADC_Value[1]=GetWordFromBytes(&RxMessage->Data[0],1);
				  DevRun_Value.AllStatus|=WSA_SensorR1Ok;
					break;
				case 0x1C0:
					i=~RxMessage->Data[0];
					if(i&S0)DevRun_Value.AllStatus|=WSA_Tank1Place;else DevRun_Value.AllStatus&=~WSA_Tank1Place;
				  if(i&S2)DevRun_Value.AllStatus|=WSA_Tank2Place;else DevRun_Value.AllStatus&=~WSA_Tank2Place;
				  break;
				default:break;
			}
			ADALL=DevRun_Value.AD6x[0]+DevRun_Value.AD6x[1]+DevRun_Value.AD6x[2]+DevRun_Value.AD6x[3];
			DevRun_Value.ADC_Value[0]=ADALL/4;
			//DeBug("Tos_CPUInfor.Userate_CPU=%x",RxMessage->StdId,Infor_Infor);
		}
		LED3_ON;
		Tos_TaskDelay(10);
		LED3_OFF;
	}
}

void TaskGetValue(void *Tags)
{
	while(1)
	{
		Tos_TaskDelay(500);
	  DevRun_Value.SensorValue[0]=Sensor_GetSensor(0,DevRun_Value.ADC_Value[0]);
		DevRun_Value.SensorValue[1]=Sensor_GetSensor(1,DevRun_Value.ADC_Value[1]);
	}
}
//int main(void)
//{
//	RCC_Configuration();
//	DeBug_Init(); 
//	DeviceList_Init();
//	DeviceMount_Usart1();
//	//DeviceMount_Usart(USART2);
//	DeviceMount_Control();//挂在输出设备
//	DeviceMount_Signel();//挂在信号采集设备
//	DeviceMount_Flash();
//	Device_Init();
//	Tos_Init();
// 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
//	Tos_TaskCreate(1,Sensor_Command,Null,"Tast1",3000);
//	Tos_TaskCreate(2,App_BusButton,Null,"Tast2",3000);
//	Tos_TaskCreate(3,TaskCanRead,Null,"Tast2",3000);
//	Tos_TaskCreate(4,TaskGetValue,Null,"GetValue",1000);
//	Tos_Start();
//	while(1);
//}
const TaskInitList TaskList[]={
{Task0,Null,"Task0",2000},
{Null},
};

int main(void)
{
	static uint8 MemBuf[40000];
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Lcd();
	DeviceMount_Usart(USART2);
	//DeviceMount_Usart(USART1);
	//DeviceMount_ZigBee();
	//DeviceMount_Usb();
	DeviceMount_IO();
	Tos_ListCreate(TaskList);
	while(1);
}








