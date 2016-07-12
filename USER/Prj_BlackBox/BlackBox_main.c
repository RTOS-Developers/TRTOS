#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_BlackBox\IO_Driver.c>
#include <..\USER\Prj_BlackBox\SCA100t.c>
#include <stdlib.h>
#include <..\Driver\SZZigBee_Driver.c>
#include <..\USER\Prj_Boot\BootLoader.c>
typedef struct{
	ConfigHeadStruct CHS;
	uint8 Name[11];
	uint8 Age;
	f32 Weight;
	f32 Height;
	uint16 eHeight;
}UserInfor;
UserInfor UI;

void Task0(void *Tags)
{
	DeBug_OutSet(DeBug_USART2);
	DeBug_SetType(Infor_All);
	while(1)
	{
		LED1_OFF;
		Tos_TaskDelay(100);
		LED1_ON;
		Tos_TaskDelay(100);
	}
}
void Task1(void *Tags)
{
	StringCoppy("TomCat ba",&UI.Name[0]);
	UI.Age=99;
	UI.Weight=112.11;
	UI.Height=33.33;
	UI.CHS.Size=sizeof(UI);
  Save_AppConfig(&UI,0);
	while(1)
	{
//		if(Load_AppConfig(&UI,0)){
//	  DeBug("UI.Age=%d",UI.Age,Infor_Infor);
//		DeBug("UI.Name=%s",UI.Name,Infor_Infor);
//		DeBug("UI.Weight=%.2f",UI.Weight,Infor_Infor);
//		DeBug("UI.Height=%.2f",UI.Height,Infor_Infor);
//		DeBug("UI.Size=%d",UI.CHS.Size,Infor_Infor);
//		}
		Tos_TaskDelay(1000);
	}
}
void TaskCanRead(void *Tags)
{
  CanRxMsg *RxMessage;
	Tos_ArrayMessage *CurMsg;
	Tos_TaskGetCAN();
	while(1)
	{
		Tos_TaskDelay(10);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			RxMessage=CurMsg->Qmsg_Addr;
			switch(RxMessage->StdId)
			{
				case 0x60:
			//    SensorWeightADC=GetWordFromBytes(&RxMessage->Data[0],0);
					break;
				default:break;
			}
		}
		Tos_TaskDelay(10);
	}
}
void TaskCommand(void *Tags)
{
	uint16 Length;
	USART_TypeDef *USARTx=(USART_TypeDef *)Tags;
	uint8 UsartCH=USART_GetIndex(USARTx);
	while(1)
	{
		Tos_TaskDelay(1);//不能不用。系统狗在此期间休息，免得累死
		ClearWithBuffer(&USART_RX_Buffer[UsartCH][0],sizeof(USART_RX_Buffer[UsartCH]));
		Length=USART_ReadDatas(USARTx,Null,sizeof(USART_RX_Buffer[UsartCH]),0);
		if(Length)BootCheckDownLoad(&USART_RX_Buffer[UsartCH][0]);
	}
}
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x5000);
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart(USART1);
	DeviceMount_Usart(USART2);
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_Can1();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",2000);
	Tos_TaskCreate(1,Task1,Null,"Tast0",2000);
	Tos_TaskCreate(2,TaskCommand,(uintbus)USART2,"Tast0",2000);
	Tos_Start();
	while(1);
}


