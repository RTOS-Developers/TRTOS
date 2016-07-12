#include <Include.h>
#include <..\USER\Prj_TankHand\IO_Driver.h>
#include <..\USER\Prj_TankHand\App_TankHand.h>

uint8 DeviceId_CTR=DeviceNull;

void Lamp_OutBIT()
{
	if(Run_Value.LAMPDAT&S0)LAMP1_ON;else LAMP1_OFF;
	if(Run_Value.LAMPDAT&S1)LAMP2_ON;else LAMP2_OFF;
	if(Run_Value.LAMPDAT&S2)LAMP3_ON;else LAMP3_OFF;
	if(Run_Value.LAMPDAT&S3)LAMP4_ON;else LAMP4_OFF;
	if(Run_Value.LAMPDAT&S4)LAMP5_ON;else LAMP5_OFF;
	if(Run_Value.LAMPDAT&S5)LAMP6_ON;else LAMP6_OFF;
	if(Run_Value.LAMPDAT&S6)LAMP7_ON;else LAMP7_OFF;
	if(Run_Value.LAMPDAT&S7)LAMP8_ON;else LAMP8_OFF;
	if(Run_Value.LAMPDAT&S8)LAMP9_ON;else LAMP9_OFF;
	if(Run_Value.LAMPDAT&S9)LAMP10_ON;else LAMP10_OFF;
	if(Run_Value.LAMPDAT&S10)LAMP11_ON;else LAMP11_OFF;
	if(Run_Value.LAMPDAT&S11)LAMP12_ON;else LAMP12_OFF;
	if(Run_Value.LAMPDAT&S12)LAMP13_ON;else LAMP13_OFF;
	if(Run_Value.LAMPDAT&S13)LAMP14_ON;else LAMP14_OFF;
}
void Lamp_SetUint32(uint32 Bit)
{
	Run_Value.LAMPDAT=Bit;
	Lamp_OutBIT();
}
void Lamp_SetBIT(uint8 Bit)
{
	Run_Value.LAMPDAT|=BIT(Bit);
	Lamp_OutBIT();
}
void Lamp_ClrBIT(uint8 Bit)
{
	Run_Value.LAMPDAT&=~BIT(Bit);
	Lamp_OutBIT();
}
void Lamp_Test(uint8 i)
{
	while(i--)
	{
		Lamp_SetUint32(0x00000000);
		Tos_TaskDelay(500);
		Lamp_SetUint32(0xffffffff);
		Tos_TaskDelay(500);
	}
}
void GPIO_Ctr_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能PortB
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); //使能PortB
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void DeviceMount_Ctr()
{
	DeviceId_CTR=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_CTR].DeviceId=DeviceId_CTR;
	Tos_Device_Tab[DeviceId_CTR].DeviceName="CTR";
	Tos_Device_Tab[DeviceId_CTR].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_CTR].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_CTR].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_CTR].Init=GPIO_Ctr_Init;
	Tos_Device_Tab[DeviceId_CTR].Write=Null;
	Tos_Device_Tab[DeviceId_CTR].Read=Null;
	Tos_Device_Tab[DeviceId_CTR].Exit=Null;
}
