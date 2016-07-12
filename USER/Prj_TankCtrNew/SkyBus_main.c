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
RS485ReadStruct SkyBus_Value;
void Task0(void *Tags)
{
	

	LED2_OFF;
	LED3_OFF;
	Remap_SWJ_JTAGDisable();
	CTR_SetUint32(0);
	DevRun_Value.AllStatus=0;
	LED1_ON;//系统灯
	LED2_ON;//通信指示
	LED3_ON;//Can通信灯
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(500);
		LED1_OFF;
		Tos_TaskDelay(500);
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
				  break;
				case 0x90:
					TankRun_Value.SIG_Value=~Uint32FromByte(&RxMessage->Data[0]);
					break;
				default:break;
			}
}

void TaskGetValue(void *Tags)
{
	while(1)
	{
		Tos_TaskDelay(200);
		DevRun_Value.Host_Sig_Data=~SIG_GetUint32();
	}
}

uint8 MemaBuf[40000];
void RS485_Send(USART_TypeDef* USARTx,uint16 Addr,uint16 Cmd,RS485ReadStruct *CCRS)
{
	CCRS->Addr=Addr;
	CCRS->Cmd=Cmd;
	CCRS->WaterID=0x01;
	CCRS->Note=0xaa;
	Create_Message(CCRS,sizeof(RS485ReadStruct));
	RS485_TXD;
	USART_WriteDatas(USARTx,(uint8 *)CCRS,sizeof(RS485ReadStruct));
	Tos_TaskDelay(1);
}
void RS485_Revice(USART_TypeDef* USARTx,RS485WriteStruct *RRS)
{
	uint8 Length;
	RS485ReadStruct *P;
	uint8 UsartCH=USART_GetIndex(USARTx);
	Length=USART_ReadDatas(USARTx,USART_TX_Buffer[UsartCH],sizeof(RS485WriteStruct),0);
	if(Export_Message(USART_TX_Buffer[UsartCH],Length))
		{
			P=(RS485ReadStruct *)USART_TX_Buffer[UsartCH];
			switch(P->Addr)
			{
				case 0x02:switch(P->Cmd)
									{
										case 0x01:BufferCoppy(USART_TX_Buffer[UsartCH],RRS,sizeof(RS485WriteStruct));
										Tos_TaskDelay(10);
										CTR_SetUint32(RRS->AllStatus);
										RS485_TXD;
										SkyBus_Value.AllStatus=SIG_GetUint32();
										RS485_Send(USARTx,0x01,0x01,&SkyBus_Value);//天车通信
										default:break;
									}break;
				case 0xff:switch(P->Cmd)
									{
										case 0x01:BufferCoppy(USART_TX_Buffer[UsartCH],RRS,sizeof(RS485ReadStruct));
										default:break;
									}break;
				default:break;
			}
			
		}
}

void Task485(void *Taga)
{
	USART_TypeDef *USARTx=(USART_TypeDef*)Taga;
	while(1)
	{
		Tos_TaskDelay(1);
		RS485_RXD;
		RS485_Revice(USARTx,&RS485WiteToSkyBus_Value);
	}
}
int main(void)
{
	Mema_Init(&MemaBuf[0]);
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_Control();//挂在输出设备
	DeviceMount_Signel();//挂在信号采集设备
	DeviceMount_Can1();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",2000);
	Tos_TaskCreate(1,TaskGetValue,Null,"GetValue",2000);
	Tos_TaskCreate(2,Task485,(uintbus)USART2,"GetValue",2000);
	Tos_Start();
	while(1);
}









