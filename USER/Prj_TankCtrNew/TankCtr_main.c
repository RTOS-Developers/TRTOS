#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <Tos_Mema.h>
#include <Sensor_Driver.h>
#include <..\USER\Prj_TankCtrNew\TankCom.h>
#include <..\Driver\SZZigBee_Driver.c>
#include <..\USER\Prj_TankCtrNew\IO_Driver.c>
#include <..\USER\Prj_TankCtrNew\TankCom.c>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.c>
#include <..\USER\Prj_TankCtrNew\Communication.c>

void Task0(void *Tags)
{
	DevRun_Value.AllStatus=0;
	TankRun_Value.SIG_Value=0xffffffff;
	LED1_ON;//系统灯
	LED2_ON;//通信指示
	LED3_ON;//Can通信灯
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(500);
		LED1_OFF;
		Tos_TaskDelay(500);
		if(LinkCount--==0);LED2_OFF;
		LED3_OFF;
	}
}
void CAN_Istr(CanRxMsg *RxMessage)
{
	uint8 i;
	LED3_ON;
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
				  i=~RxMessage->Data[0];//车子罐在位置检测
         if(i&S0)DevRun_Value.AllStatus|=WSA_Tank1Place;else DevRun_Value.AllStatus&=~WSA_Tank1Place;
		     if(i&S2)DevRun_Value.AllStatus|=WSA_Tank2Place;else DevRun_Value.AllStatus&=~WSA_Tank2Place;
					DevRun_Value.AllStatus|=WSA_SensorIOTOK;
				  break;
				case 0x90:
					if(DevRun_Value.AllStatus&WSA_TankPowerOn)
					{
						TankRun_Value.SIG_Value=~Uint32FromByte(&RxMessage->Data[0]);
					}else TankRun_Value.SIG_Value=0xffffffff;
					DevRun_Value.AllStatus|=WSA_SensorTankOK;
					break;
				default:break;
			}
}
//全局变量交换
void TaskGetValue(void *Tags)
{
	uint32 ADALL;
	uint32 Pool[2];
	uint8 Index[2];
	DevRun_Value.ADC_Value[0]=0;
	DevRun_Value.ADC_Value[1]=0;
	while(1)
	{
		Tos_TaskDelay(200);
		DevRun_Value.Host_Sig_Data=~SIG_GetUint32();
	  DevRun_Value.SensorValue[0]=Sensor_GetSensor(0,FlowPoolFilter(&Pool[0],DevRun_Value.ADC_Value[0],&Index[0],10));//计算传感器重量
		DevRun_Value.SensorValue[1]=Sensor_GetSensor(1,FlowPoolFilter(&Pool[1],DevRun_Value.ADC_Value[1],&Index[1],10));//计算幅度
		if(DevRun_Value.SensorValue[0]>=DevRun_Value.FullWeight)DevRun_Value.AllStatus|=WSA_DownLoadOver;
	  if(DevRun_Value.SensorValue[0]<=(DevRun_Value.FullWeight-1))DevRun_Value.AllStatus&=~WSA_DownLoadOver;
		if(DevRun_Value.Host_Sig_Data&S0)DevRun_Value.AllStatus|=WSA_SysPowerOn;else DevRun_Value.AllStatus&=~WSA_SysPowerOn;
		if(DevRun_Value.Host_Sig_Data&S1)DevRun_Value.AllStatus|=WSA_SysError;else DevRun_Value.AllStatus&=~WSA_SysError; 
		if(TankRun_Value.SIG_Value&S0)DevRun_Value.AllStatus|=WSA_TankPowerCheckRealy;else DevRun_Value.AllStatus&=~WSA_TankPowerCheckRealy;
		if(!(TankRun_Value.SIG_Value&S1))DevRun_Value.AllStatus|=WSA_TapOpenRealy;else DevRun_Value.AllStatus&=~WSA_TapOpenRealy;	
		if(!(TankRun_Value.SIG_Value&S2))DevRun_Value.AllStatus|=WSA_TapCloseRealy;else DevRun_Value.AllStatus&=~WSA_TapCloseRealy;	
    if(!(TankRun_Value.SIG_Value&S5))DevRun_Value.AllStatus|=WSA_SealedInPlace;else DevRun_Value.AllStatus&=~WSA_SealedInPlace;
		ADALL=DevRun_Value.AD6x[0]+DevRun_Value.AD6x[1]+DevRun_Value.AD6x[2]+DevRun_Value.AD6x[3];
		DevRun_Value.ADC_Value[0]=ADALL/4;
		RS485Wite_Value.Distance=DevRun_Value.SensorValue[1];
		RS485Wite_Value.Weight=DevRun_Value.SensorValue[0];
		RS485Wite_Value.AllStatus=DevRun_Value.AllStatus;
		if((DevRun_Value.SensorValue[1]<5)&&(DevRun_Value.Host_Sig_Data&S2)&&(DevRun_Value.Host_Sig_Data&S3))RS485WiteToSkyBus_Value.AllStatus|=S0;else RS485WiteToSkyBus_Value.AllStatus&=~S0;
	}
}

int main(void)
{
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_Can1();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",2000);
	Tos_TaskCreate(1,Sensor_Command,Null,"Tast1",2000);
	Tos_TaskCreate(2,App_BusButton,Null,"Tast2",2000);
	Tos_TaskCreate(3,TaskGetValue,Null,"GetValue",2000);
	Tos_TaskCreate(4,Task_SendValue,Null,"GetValue",2000);
	Tos_TaskCreate(5,Task485,(uintbus)USART2,"GetValue",2000);
	Tos_Start();
	while(1);
}









