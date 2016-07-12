#include <Include.h>
#include <..\USER\Prj_7LCDSJJ\IO_Driver.h>

uint8 DeviceId_CONTROL=DeviceNull;
uint8 DeviceId_SIGNAL=DeviceNull;
uint8 DeviceId_CODEKEY=DeviceNull;
uint8 KeyCode_Tos_TaskID=Tos_TaskNull;
void GPIO_Control_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
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
void CodeKey_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                          //选择中断分组2
	NVIC_InitStructure.NVIC_IRQChannel =EXTI0_IRQn;     //选择中断通道2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel =EXTI1_IRQn;            //选择中断通道3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占式中断优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应式中断优先级设置为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                   //使能中断
	NVIC_Init(&NVIC_InitStructure);
}


void CodeKey_IO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;                                             //选择引脚2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //输出频率最大50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                  //带上拉电阻输出
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                             //选择引脚2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //输出频率最大50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                  //带上拉电阻输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

void CodeKey_EXTI_Init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource1);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1; //
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设置为中断请求，非事件请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //设置中断触发方式为上下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                          //外部中断使能
	EXTI_Init(&EXTI_InitStructure);
}
void CodeKey_Init()
{
	CodeKey_IO_Init();
	CodeKey_NVIC_Init();
	CodeKey_EXTI_Init();
}
void EXTI0_IRQHandler(void)
{
	static uint8 keya,keyb;
	MGUI_KeyMsg KeyMsg;
	EXTI_ClearITPendingBit(EXTI_Line0);
	if(KC2==0)
	{
		if(KC1==0)keya=Key_Up;
		else keya=Key_Down;
	}else
	{
		if(KC1==1)keyb=Key_Up;
		else keyb=Key_Down;
	}
	if(keya==keyb)
	{
		KeyMsg.KeyValue=keya;
	Tos_TaskPutQmsg(KeyCode_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
	}
}
void Tos_TaskGetKeyCode()
{
	KeyCode_Tos_TaskID=Tos_TaskCurrent;
}
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void GPIO_Signal_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化 IO
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
void DeviceMount_CodeKey()
{
	DeviceId_CODEKEY=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_CODEKEY].DeviceId=DeviceId_CODEKEY;
	Tos_Device_Tab[DeviceId_CODEKEY].DeviceName="CODEKEY";
	Tos_Device_Tab[DeviceId_CODEKEY].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_CODEKEY].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_CODEKEY].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_CODEKEY].Init=CodeKey_Init;
	Tos_Device_Tab[DeviceId_CODEKEY].Write=Null;
	Tos_Device_Tab[DeviceId_CODEKEY].Read=Null;
	Tos_Device_Tab[DeviceId_CODEKEY].Exit=Null;
}


