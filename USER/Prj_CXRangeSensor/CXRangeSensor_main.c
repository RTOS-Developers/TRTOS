#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.h>
#include <GUI_Include.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_CXRangeSensor\ADC_Driver.c>
#include <..\USER\Prj_CXRangeSensor\IO_Driver.c>
#include <..\USER\Prj_CXRangeSensor\FM25L16B_Driver.c>
#include <..\USER\Prj_CXRangeSensor\MLX91204_Driver.c>
uint16 AD620_Value;
uint8 IO_Value;
void Task0(void *Tags)
{
	uint8 i;
	
	uint8 Buf[12];
	LED2_ON;
	LED1_ON;
	LED3_ON;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	LED3_OFF;
	while(1)
	{
		LED1_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		Tos_TaskDelay(900);
//		for(i=0;i<10;i++)Buf[i]=i;
//		FMWriteMemory(0,&Buf[0],10);
//		for(i=0;i<10;i++)Buf[i]=0;
//		FMReadMemory(0,&Buf[0],10);
//		SendReadByte(0x05);
//		Buf[2]=SendReadByte(0x05);
//		Tos_TaskDelay(300);
//		Tos_TaskDelay(300);
	
	}
}

/****************************************************************************
Date:2015-01-23
Func:应用任务
Note:  
****************************************************************************/
void Task1(void *Tags)
{
	
	while(1)
	{
		LED2_ON;
		IO_Value=SIG_GetUint8();
		Tos_TaskDelay(100);
		LED2_OFF;
		Tos_TaskDelay(300);
	}
}
void SIG_Delay(uint8 D)
{
	Tos_TaskDelay(D);
	LED3_OFF;
	Tos_TaskDelay(D);
	LED3_ON;
}
void Task2(void *Tags)
{     
	float Temp;
	uint8 Data[10];
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	MLX_ClearMaxMin();
	while(1)
	{
//		BufferFill(&Data[0],0,8);
//		ADC_Value=ADCBuffer[1]-ADCBuffer[0];
////		BufferCoppy(&ADC_Value,&Data[0],2);
////			ADC_Value=ADCBuffer[2]-ADCBuffer[1];
////		BufferCoppy(&ADC_Value,&Data[2],2);
////	//	BufferCoppy(&IO_Value,&Data[2],1);
//		BufferCoppy(&ADCBuffer[0],&Data[0],2);
//		BufferCoppy(&ADCBuffer[1],&Data[2],2);
//		BufferCoppy(&ADCBuffer[2],&Data[4],2);
//		BufferCoppy(&ADC_Value,&Data[6],2);
		MLX_GetMaxMin(ADC_Buffer[0],ADC_Buffer[2]);
		Temp=MLX_ConverterAngle();
		BufferCoppy(&Temp,&Data[0],4);
	//	BufferCoppy(&MLX_Value.CenValue,&Data[0],2);
	//	BufferCoppy(&MLX_Value.CenValue1,&Data[2],2);
	//	BufferCoppy(&MLX_Value.CenValue,&Data[4],2);
		CAN_SIG_SendMsg(&Data[0],0x1A1);//Printf_Bytes(&Data[0],8);
		SIG_Delay(200);
	}
}



/****************************************************************************
Date:2015-01-23
Func:主函数
Note:
****************************************************************************/
int main(void)
{
	//NVIC_SetVectorTable(0x08005000,0);
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Led();
	DeviceMount_Flash();
	DeviceMount_Control();
	DeviceMount_Signel();
	DeviceMount_Fram();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Tast0",3000);
	Tos_TaskCreate(1,Task_ADCFitel,Null,"ADC-Reader",3000);
	Tos_TaskCreate(2,Task2,Null,"ADC-Reader",3000);
	Tos_Start();
	while(1);
}



