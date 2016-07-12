#include <Include.h>
#include <..\USER\Prj_TankCtrNew\IO_Driver.h>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.h>

uint8 DeviceId_CONTROL=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;
void GPIO_Control_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	CTR_SetBIT32(0x00);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7| GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);//指示灯
	
	Remap_SWJ_JTAGDisable();
}
void CTR_SetBIT32(uint32 Bit)
{
	uint8 i=0;
	if(Bit&BIT(i++))CTR1_ON;else CTR1_OFF;
	if(Bit&BIT(i++))CTR2_ON;else CTR2_OFF;
	if(Bit&BIT(i++))CTR3_ON;else CTR3_OFF;
	if(Bit&BIT(i++))CTR4_ON;else CTR4_OFF;
	if(Bit&BIT(i++))CTR5_ON;else CTR5_OFF;
	if(Bit&BIT(i++))CTR6_ON;else CTR6_OFF;
	if(Bit&BIT(i++))CTR7_ON;else CTR7_OFF;
	if(Bit&BIT(i++))CTR8_ON;else CTR8_OFF;
	if(Bit&BIT(i++))CTR9_ON;else CTR9_OFF;
	if(Bit&BIT(i++))CTR10_ON;else CTR10_OFF;
	if(Bit&BIT(i++))CTR11_ON;else CTR11_OFF;
	if(Bit&BIT(i++))CTR12_ON;else CTR12_OFF;
	if(Bit&BIT(i++))CTR13_ON;else CTR13_OFF;
	if(Bit&BIT(i++))CTR14_ON;else CTR14_OFF;
	if(Bit&BIT(i++))CTR15_ON;else CTR15_OFF;
	if(Bit&BIT(i++))CTR16_ON;else CTR16_OFF;
	if(Bit&BIT(i++))CTR17_ON;else CTR17_OFF;
	if(Bit&BIT(i++))CTR18_ON;else CTR18_OFF;
	if(Bit&BIT(i++))CTR19_ON;else CTR19_OFF;
	if(Bit&BIT(i++))CTR20_ON;else CTR20_OFF;
	if(Bit&BIT(i++))CTR21_ON;else CTR21_OFF;
	if(Bit&BIT(i++))CTR22_ON;else CTR22_OFF;
	if(Bit&BIT(i++))CTR23_ON;else CTR23_OFF;
	if(Bit&BIT(i++))CTR24_ON;else CTR24_OFF;
}
void CTR_SetUint32(uint32 Bit)
{
	DevRun_Value.Host_Ctr_Data=Bit;
	CTR_SetBIT32(DevRun_Value.Host_Ctr_Data);
}
void CTR_SetBIT(uint8 Bit)
{
	DevRun_Value.Host_Ctr_Data|=BIT(Bit);
	CTR_SetBIT32(DevRun_Value.Host_Ctr_Data);
}
void CTR_ClrBIT(uint8 Bit)
{
	DevRun_Value.Host_Ctr_Data&=~BIT(Bit);
	CTR_SetBIT32(DevRun_Value.Host_Ctr_Data);
}
uint32 SIG_GetUint32()
{
	uint32 Temp=0x00000000;
	if(SIG1)Temp|=S0;
	if(SIG2)Temp|=S1;
	if(SIG3)Temp|=S2;
	if(SIG4)Temp|=S3;
	if(SIG5)Temp|=S4;
	if(SIG6)Temp|=S5;
	if(SIG7)Temp|=S6;
	if(SIG8)Temp|=S7;
	if(SIG9)Temp|=S8;
	if(SIG10)Temp|=S9;
	if(SIG11)Temp|=S10;
	if(SIG12)Temp|=S11;
	if(SIG13)Temp|=S12;
	if(SIG14)Temp|=S13;
	if(SIG15)Temp|=S14;
	if(SIG16)Temp|=S15;
	if(SIG17)Temp|=S16;
	if(SIG18)Temp|=S17;
	if(SIG19)Temp|=S18;
	if(SIG20)Temp|=S19;
	if(SIG21)Temp|=S20;
	if(SIG22)Temp|=S21;
	if(SIG23)Temp|=S22;
	if(SIG24)Temp|=S23;
	if(SIG25)Temp|=S24;
	if(SIG26)Temp|=S25;
	if(SIG27)Temp|=S26;
	if(SIG28)Temp|=S27;
	if(SIG29)Temp|=S28;
	if(SIG30)Temp|=S29;
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
void GPIO_Signal_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8;    //PA.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化 IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;    //PA.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化 IO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7;    //PA.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //初始化 IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化 IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化 IO
	
}
void DeviceMount_Control()
{
	DeviceId_CONTROL=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceId=DeviceId_CONTROL;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceName="CTR";
	Tos_Device_Tab[DeviceId_CONTROL].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_CONTROL].DeviceVirtue=DV_Task;
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
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_SIGNAL].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_SIGNAL].Init=GPIO_Signal_Init;
	Tos_Device_Tab[DeviceId_SIGNAL].Write=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Read=Null;
	Tos_Device_Tab[DeviceId_SIGNAL].Exit=Null;
}

