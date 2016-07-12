#include <Include.h>
#include <..\USER\Prj_CAMERAMRC\IO_Driver.h>

uint8 DeviceId_CONTROL=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;
uint8 DeviceId_CODEKEY=DeviceNull;
uint8 KeyCode_Tos_TaskID=Tos_TaskNull;
void GPIO_Control_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14|GPIO_Pin_15;//指示灯
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	Remap_SWJ_JTAGDisable();
}
void CTR_SetBIT32(uint32 Bit)
{
//	uint8 i=0;
//	if(Bit&BIT(i++))CTR1_ON;else CTR1_OFF;
//	if(Bit&BIT(i++))CTR2_ON;else CTR2_OFF;
//	if(Bit&BIT(i++))CTR3_ON;else CTR3_OFF;
//	if(Bit&BIT(i++))CTR4_ON;else CTR4_OFF;
//	if(Bit&BIT(i++))CTR5_ON;else CTR5_OFF;
}
void CTR_SetUint32(uint32 Bit)
{
//	DevRun_Value.CTR_DATABIT=Bit;
//	CTR_SetBIT32(DevRun_Value.CTR_DATABIT);
}
void CTR_SetBIT(uint8 Bit)
{
//	DevRun_Value.CTR_DATABIT|=BIT(Bit);
	//CTR_SetBIT32(DevRun_Value.CTR_DATABIT);
}
void CTR_ClrBIT(uint8 Bit)
{
	//DevRun_Value.CTR_DATABIT&=~BIT(Bit);
	//CTR_SetBIT32(DevRun_Value.CTR_DATABIT);
}
uint32 SIG_GetUint32()
{
	uint32 Temp=0x00000000;
//	if(SIG1)Temp|=S0;
//	if(SIG2)Temp|=S1;
//	if(SIG3)Temp|=S2;
//	if(SIG4)Temp|=S3;
	return Temp;
}
void CTR_Test(uint8 i)
{
	while(i--)
	{
	CTR_SetUint32(0xffffffff);
	Tos_TaskDelay(1000);
	CTR_SetUint32(0x00000000);
	Tos_TaskDelay(1000);
	}
}



void Tos_TaskGetKeyCode()
{
	KeyCode_Tos_TaskID=Tos_TaskCurrent;
}

void GPIO_Signal_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化 IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化 IO
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
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceName="CTR";
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceVirtue=ReadOnly|WriteOnly|CharDerice;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_SIGNAL].Init=GPIO_Signal_Init;
	Tos_Device_Tab[DeviceId_SIGNAL].Write=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Read=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Exit=Null;
}


