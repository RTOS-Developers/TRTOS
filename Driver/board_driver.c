#include <Include.h>
#include <DeBug.h>
#include <Board_Driver.h>
/**************************************************************************************
 Func: 禁用JTAG仅适用SWD调试
 Time: 2015-9-02
 Ver.: V1.0
 Note;
**************************************************************************************/
void Remap_SWJ_JTAGDisable()
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	while(1)
	{	DeBug_Get();
		DeBug("ErrorFile%s->Line%d",file,line,Infor_Infor);
		Tos_TaskDelay(1000);
	}
}
#endif
/**************************************************************************************
 Func: 端口清单初始化
 Time: 2015-9-02
 Ver.: V1.0
 Note;
**************************************************************************************/
void GPIO_InitList(const GPIO_InitStruct *List)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	uint32_t RCC_APB2Periph_GPIOx; 
	while(List->GPIO_Speed)
	{
		switch((uintbus)List->GPIOx)
	{
		case (uintbus)GPIOA:RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOA;break;
		case (uintbus)GPIOB:RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOB;break;
		case (uintbus)GPIOC:RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;break;
		case (uintbus)GPIOD:RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOD;break;
		case (uintbus)GPIOE:RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOE;break;
		case (uintbus)GPIOF:RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOF;break;
	}
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Speed=(GPIOSpeed_TypeDef)List->GPIO_Speed;
	GPIO_InitStructure.GPIO_Pin=List->GPIO_Pin; 
	GPIO_InitStructure.GPIO_Mode=List->GPIO_Mode;
	GPIO_Init(List->GPIOx, &GPIO_InitStructure);
	List++;
	}
}

void BareDelay(uint32 i)
{
	uint32 m;
	while(i--)
	{
		m=72000000000/Tos_Cpu_Freq;
		while(m--);
	}
}
void Standby_Init()
{
	GPIO_InitTypeDef   GPIO_InitStructure;      
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;    //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化 IO
	//使用外部中断方式
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
	//中断线 0 连接 GPIOA.0
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;  //设置按键所有的外部线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //设外外部中断模
	//式:EXTI 线路为中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);  //  初始化外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级 2 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化 NVIC
	Sys_Enter_Standby();
	
}
void EXTI0_IRQHandler(void)
{                          
	
	EXTI_ClearITPendingBit(EXTI_Line0); //  清除 LINE10 上的中断标志位   
	MCU_Reset();	
} 
void Sys_Standby(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);  //使能 PWR 外设时钟
	PWR_WakeUpPinCmd(ENABLE);   //使能唤醒管脚功能
	PWR_BackupAccessCmd(ENABLE);
	//PWR_EnterSTANDBYMode();    //进入待命（STANDBY）模式       
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
}
//系统进入待机模式
void Sys_Enter_Standby(void)
{     
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);  //复位所有 IO 口
	Sys_Standby();
}
void MCU_Reset(void)
{
	__disable_fault_irq();      // STM32 软复位  
	NVIC_SystemReset();
}

void IWDG_Configuration(void)
{
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 写入0x5555,用于允许狗狗寄存器写入功能 */
 IWDG_SetPrescaler(IWDG_Prescaler_256); /* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
 IWDG_SetReload(0xff0);/* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
 IWDG_ReloadCounter();/* 喂狗*/
 IWDG_Enable(); /* 使能狗狗*/
}
void IWDG_Reload()
{
	IWDG_ReloadCounter();//系统狗/使能休眠功能后需要关闭系统狗
}
