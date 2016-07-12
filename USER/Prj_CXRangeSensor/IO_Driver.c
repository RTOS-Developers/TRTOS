#include <Include.h>
#include <..\USER\Prj_CXRangeSensor\IO_Driver.h>

uint8 DeviceId_CONTROL=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;
void GPIO_Control_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能PortC
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
void GPIO_Signal_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;    //PA.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化 IO
	
}
uint8 SIG_GetUint8()
{
	uint8 Temp=0x00;
	if(!SIG1)Temp|=S0;
	if(!SIG2)Temp|=S1;
	if(!SIG3)Temp|=S2;
	return Temp;
}
void DeviceMount_Control()
{
	DeviceId_CONTROL=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceId=DeviceId_CONTROL;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceName="CTR";
	Tos_Device_Tab[DeviceId_CONTROL].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceVirtue=ReadOnly|WriteOnly|CharDerice;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_CONTROL].Init=GPIO_Control_Init;
	Tos_Device_Tab[DeviceId_CONTROL].Write=Null;
	Tos_Device_Tab[DeviceId_CONTROL].Read=Null;
	Tos_Device_Tab[DeviceId_CONTROL].Exit=Null;
}
void DeviceMount_Signel()
{
	DeviceId_SIGNAL=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceId=DeviceId_SIGNAL;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceName="SIG";
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceVirtue=ReadOnly|WriteOnly|CharDerice;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_SIGNAL].Init=GPIO_Signal_Init;
	Tos_Device_Tab[DeviceId_SIGNAL].Write=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Read=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Exit=Null;
}

