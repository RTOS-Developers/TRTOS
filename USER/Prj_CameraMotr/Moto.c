#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <..\USER\Prj_CAMERACTR\Moto.h>
#include <..\USER\Prj_CAMERAMOTR\SCA100t.h>

static void TIM1_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void TIM1_Mode_Config(uint16 TIM_Pulse)
{
	uint16 TIM_Period=0xffff;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x7;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xffff;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_Pulse =TIM_Period-TIM_Pulse;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_Pulse =TIM_Pulse;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
  TIM_Cmd(TIM1, ENABLE);
  if(TIM_Pulse!=0x8000)TIM_CtrlPWMOutputs(TIM1, ENABLE);
	else TIM_CtrlPWMOutputs(TIM1, DISABLE);
}

/*
 * 函数名：TIM1_PWM_Init
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM1_PWM_Init(void)
{
	TIM1_GPIO_Config();
	TIM1_Mode_Config(0x8000);	
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
int16 DefValue=1200;
uint32 Poo;
uint8 Poi;
void Motor_Ctrl(uint8 dir,uint16 num)
{
	  int16 Center;
		int16 dd;
	if(DefValue>1800)DefValue=1800;
	if(DefValue<200)DefValue=200;
	  Center=DefValue;
	  Center-=AngleXY[0];
	  if(abs(Center)<20){TIM1_Mode_Config(0x8000);LED2_OFF;return;}
    LED2_ON;
	  //if(K90){TIM1_Mode_Config(0x8000);return;}
if(Center>0)dd=900;
else dd=-900;
if(Center>300)Center=300;
if(Center<-300)Center=-300;
		TIM1_Mode_Config(FlowPoolFilter(&Poo,0x8000+Center*20+dd,&Poi,5));
}
PID Pid;
void delay()
{
	uint16 i=355;
	while(i--);
}
void Task_Moto(void *Tags)
{
	//TIM1_PWM_Init();
	while(1)
	{
		// Tos_TaskDelay(10);
		delay();
		MOTOB_OFF;
		MOTOF_ON;
		delay();
	MOTOB_ON;
		MOTOF_OFF;
	}
}
