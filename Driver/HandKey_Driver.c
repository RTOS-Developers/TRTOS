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
#if defined(BOARD_TANK_Hand)
	GPIO_InitTypeDef   GPIO_InitStructure;      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;    //PA.0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//PB2为调试端口使用就不可以调试
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化 IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;    //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);  //初始化 IO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能 GPIOA 和复用功能时钟
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_14|GPIO_Pin_15;    //PA.0
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);  //初始化 IO
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	#else
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
#endif
	
}
void _Standby_Init()
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
#if defined(BOARD_TANK_Hand)
	//if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0){Value=Key_Enter;}//老板子的这个端口是下载调试口不能使用
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0){Value=Key_Left;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0){Value=Key_Right;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0){Value=Key_Up;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==0){Value=Key_Down;}
  if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)==0){Value=Key_A;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)==0){Value=Key_B;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)==0){Value=Key_C;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==0){Value=Key_D;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==0){Value=Key_E;}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)==0){Value=Key_1;}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)==0){Value=Key_2;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0){Value=Key_3;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==0){Value=Key_4;}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0){Value=Key_5;}
	
	
#else
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0){Value|=Key_Left;}//Sys_Standby();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)Value|=Key_Up;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)Value|=Key_Right;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)Value|=Key_Down;
#endif
	return Value;
}
extern uint16 ADCForTankHand_ReadFitel(uint8 CH);

void AutoCorrect_TankHand()
{
	CenterYADC=ADCForTankHand_ReadFitel(0);
	CenterXADC=ADCForTankHand_ReadFitel(1);
}
int8 GetXYSpeed(uint8 KeyValue)
{
	uint8 i;
	int8 Value=0;
	uint16 CenterValue;
	uint16 RockerBarSpase=300;
	uint16 Temp;
	if(KeyValue==Key_X){Temp=ADCForTankHand_ReadFitel(1);CenterValue=CenterXADC;}
	if(KeyValue==Key_Y){Temp=ADCForTankHand_ReadFitel(0);CenterValue=CenterYADC;}
	for(i=0;i<5;i++)
	{
		if(Temp>(CenterValue+i*RockerBarSpase))Value=0-i;
	}
	for(i=0;i<5;i++)
	{
		if(Temp<(CenterValue-i*RockerBarSpase))Value=i;
	}
		return Value;
}
BOOL HandKey_GetRockerBar(MGUI_KeyMsg *KeyMsg)
{
	BOOL OK=False;
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)==0){KeyMsg->KeyValue=Key_X;OK=True;}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)==0){KeyMsg->KeyValue=Key_Y;OK=True;}
	if(OK){Tos_TaskDelay(100);KeyMsg->KeyTime=GetXYSpeed(KeyMsg->KeyValue);}
	return OK;
}
extern void ADCForTankHand_Init(void);
/********************************************************************************
Func:获取按键任务
Date:2014-11-20
Note:
********************************************************************************/
void Task_GetHandKey(void *Tags)
{
	uint16 Count=0;
	uint8 Old_Key;
	int8 Old_Time;
	MGUI_KeyMsg KeyMsg;
	KeyBoard_Configuration();
	ADCForTankHand_Init();
	Tos_TaskDelay(200);//等待XY轴模拟量稳定
	AutoCorrect_TankHand();//自动校准中值  
	while(1)
	{
		Tos_TaskDelay(10);
		KeyMsg.KeyValue=HandKey_GetValue();
		if(KeyMsg.KeyValue!=0){KeyMsg.KeyType=KeyBoard_Hand;if(Old_Key!=KeyMsg.KeyValue)if(KeyMsg.KeyValue)Tos_TaskPutQmsg(HandKey_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);}
		KeyMsg.KeyTime=0;
		if(HandKey_GetRockerBar(&KeyMsg))
		{
			Count++;
			KeyMsg.KeyType=KeyBoard_RockerBar;
			if(Old_Time!=KeyMsg.KeyTime)
			{
				if(KeyMsg.KeyTime!=0)Tos_TaskPutQmsg(HandKey_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
			}else if(Count>10)Tos_TaskPutQmsg(HandKey_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
		}else Count=0;
		Old_Time=KeyMsg.KeyTime;
		Old_Key=KeyMsg.KeyValue;
		KeyMsg.KeyType=Key_DownIng;
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
