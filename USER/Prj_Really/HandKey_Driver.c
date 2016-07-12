#include <include.h>
#include <Driver_Include.h>
#include <HandKey_Driver.h>
#include <KeyBoard.h>
uint8 HandKey_Tos_TaskID;
uint16 CenterXADC,CenterYADC;
/********************************************************************************
Func:键盘初始化
Date:2014-11-20
Note:
********************************************************************************/
void KeyBoard_Configuration()
{
	GPIO_InitTypeDef   GPIO_InitStructure;      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;    //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化 IO
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能PortC
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
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
		//Sys_Enter_Standby(); 
	MCU_Reset();	
} 
/********************************************************************************
Func:获取按下按键
Date:2014-11-20
Note:创星科技
********************************************************************************/
uint8 HandKey_GetValue()
{
	uint8 Value=0x00;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0){Value|=Key_Left;}//Sys_Standby();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)Value|=Key_Up;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)Value|=Key_Right;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)Value|=Key_Down;
	return Value;
}


/********************************************************************************
Func:获取按键任务
Date:2014-11-20
Note:
********************************************************************************/
void Task_GetHandKey(void *Tags)
{
  MGUI_KeyMsg KeyMsg;
	KeyBoard_Configuration();
	while(1)
	{
		Tos_TaskDelay(10);
		KeyMsg.KeyValue=HandKey_GetValue();
		KeyMsg.KeyType=Key_DownIng;
		if(KeyMsg.KeyValue)Tos_TaskPutQmsg(HandKey_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
	}
}
/********************************************************************************
Func:键盘钩子
Date:2014-11-20
Note:
********************************************************************************/
void Tos_TaskGetHandKey()
{
	HandKey_Tos_TaskID=Tos_TaskCurrent;
}
