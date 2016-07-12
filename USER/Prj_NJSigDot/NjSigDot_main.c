#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Tos_Mema.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_TankCtr\App_TankCtr.c>
#include <..\USER\Prj_TankCtr\IO_Driver.c>

void Task0(void *Tags)
{
	

	LED2_ON;
	LED3_ON;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	LED1_ON;
	Tos_TaskDelay(2000);
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(900);
	}
}




void ConVet(uint8 *Data,uint8 Value)
{
	uint8 n;
	Data[7]=0;
	for(n=0;n<6;n++)
	{
		if(Value&BIT(n))Data[n]=1;else Data[n]=0;
		Data[7]+=Data[n];
	}
	Data[6]=0;
}
/****************************************************************************
Date:2014-5-12
Func:CAN从机任务
Note:第一第三组正常第二组取反
****************************************************************************/
void Task_CAN_SIGWJ(void *Tags)
{
	uint8 SIG_Value;
	uint8 Data[10];
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{		
		LED3_ON;
		SIG_Value=0xff&SIG_GetUint32();
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C0);
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(200);
		LED3_ON;
		
		SIG_Value=0xff&(SIG_GetUint32()>>6); 
		SIG_Value=~SIG_Value;
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C1);
		
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(200);
		LED3_ON;
		SIG_Value=0xff&(SIG_GetUint32()>>12);
		ConVet(&Data[0],SIG_Value);
		SIG_Value=0xff&(SIG_GetUint32()>>18);
		if(SIG_Value&S0)Data[6]=1;else Data[6]=0;
		Data[7]+=Data[6];
		CAN_SIG_SendMsg(&Data[0],0x1C2);Printf_Bytes(&Data[0],8);
		LED3_ON;
		Tos_TaskDelay(100);
		LED3_OFF;
		
		

		Tos_TaskDelay(100);
	}
}
/****************************************************************************
Date:2014-5-12
Func:CAN从机任务
Note:三个一组，第一组第二组正常，第三组最后四个接有源信号
****************************************************************************/
void Task_CAN_SIGWJNew(void *Tags)
{
	uint8 SIG_Value;
	uint8 RunBuf[2];
	uint8 Data[10];
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{		
		LED3_ON;
		SIG_Value=0xff&SIG_GetUint32();
		RunBuf[0]=(SIG_Value&S0);
		RunBuf[1]=(SIG_Value&S1);
		SIG_Value&=~S1;SIG_Value&=~S0;
		if(RunBuf[0]&RunBuf[1]){SIG_Value|=S1;SIG_Value&=~S0;}
    if(RunBuf[0]){SIG_Value&=~S1;SIG_Value|=S0;}
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C0);
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(200);
		LED3_ON;
		
		SIG_Value=0xff&(SIG_GetUint32()>>6); 
	//	SIG_Value=~SIG_Value;
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C1);
		
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(200);
		LED3_ON;
		SIG_Value=0xff&(SIG_GetUint32()>>12);
		ConVet(&Data[0],SIG_Value);
		SIG_Value=0xff&(SIG_GetUint32()>>18);
		if(SIG_Value&S0)Data[6]=1;else Data[6]=0;
		Data[7]+=Data[6];
		CAN_SIG_SendMsg(&Data[0],0x1C2);Printf_Bytes(&Data[0],8);
		LED3_ON;
		Tos_TaskDelay(100);
		LED3_OFF;
		
		

		Tos_TaskDelay(100);
	}
}
/****************************************************************************
Date:2014-5-12
Func:CAN从机任务
Note:三个一组，第一组第二组正常，第三组最后四个接有源信号
****************************************************************************/
void Task_CAN_SIGWJThree(void *Tags)
{
	uint8 SIG_Value;
	uint8 Data[10];
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{		
		LED3_ON;
		SIG_Value=0xff&SIG_GetUint32();
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C0);
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(100);
		LED3_ON;
		SIG_Value=0xff&(SIG_GetUint32()>>6); 
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C1);
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(100);
		LED3_ON;
		SIG_Value=0xff&(SIG_GetUint32()>>12);
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C2);
		LED3_ON;
		Tos_TaskDelay(100);
		LED3_OFF;
		SIG_Value=0xff&(SIG_GetUint32()>>18);
		ConVet(&Data[0],SIG_Value);
		CAN_SIG_SendMsg(&Data[0],0x1C3);
		LED3_ON;
		Tos_TaskDelay(100);
		LED3_OFF;
		Tos_TaskDelay(100);
	}
}
void Task_CAN_SIGMY(void *Tags)
{
	uint8 Data[10];
	Data[0]=OC_UpDoorOpen;
	//CAN_SIG_SendMsg(&Data[0],0x91);
	while(1)
	{		
		LED3_ON;
		Uint32ToByte(&Data[0],SIG_GetUint32());
		CAN_SIG_SendMsg(&Data[0],0x1C0);
		Tos_TaskDelay(250);
		
		LED3_OFF;
		Tos_TaskDelay(250);
	}
}
void CAN_Istr(CanRxMsg *RxMessage)
{
	uint8 i;
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
				  break;
				case 0x90:
					TankRun_Value.SIG_Value=~Uint32FromByte(&RxMessage->Data[0]);
					break;
				case 0x91:

					break;
				default:break;
			}
}

int main(void)
{
	RCC_Configuration();
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Control();
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Can1();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_CAN_SIGWJ,Null,"Tast2",3000);
	Tos_Start();
	while(1);
}












