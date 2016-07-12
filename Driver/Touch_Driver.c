#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <GUI_Include.h>
#include <Touch_driver.h>

uint8 DeviceId_TOUCH=DeviceNull;
uint8 TOUCH_CFG_OffSet=FlashConfigNull;

#define TOUCH_DOWNDELAYCOUNT 4
#define TOUCH_UPDELAYCOUNT 10
uint8  Touch_Update;
uint8 TOUCH_Tos_TaskID;
uint8 TOUCH_Correct_TaskID;
_Touch_Event Touch_Event;
uint8 Touch_Screen_UserTaskID=Tos_TaskNull;

const uint16 TOUCH_CROSS_ADDR[5][2]={//校准点的位置
{TOUCH_CROSS_GAUGE,TOUCH_CROSS_GAUGE},//左上角校准点位置
{LCD_XSIZE-TOUCH_CROSS_GAUGE,TOUCH_CROSS_GAUGE},//右上角校准点位置
{LCD_XSIZE-TOUCH_CROSS_GAUGE,LCD_YSIZE-TOUCH_CROSS_GAUGE},//右下角校准点位置
{TOUCH_CROSS_GAUGE,LCD_YSIZE-TOUCH_CROSS_GAUGE},//左下角校准点位置
{LCD_XSIZE/2,LCD_YSIZE/2}//中间校准点位置
};
uint16 TOUCH_CROSS_DATA[5][2];//校准校准数据临时存放点

volatile uint16  TOUCH_ADC_Value[TOUCH_ADC_BUFSize][TOUCH_ADC_CHN]; //触摸屏ADC采集池  
uint16 TOUCH_ADC_XValue,TOUCH_ADC_YValue;//触摸屏ADC模数值
uint16 TOUCH_XValue,TOUCH_YValue;

/**************************************************************************************
 Func: 触摸屏中断配置
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void NVIC_ADC1_2_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
   
    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************
 Func: ADC模拟狗配置
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void ADC_WatchdogConfig(void)
{
  NVIC_ADC1_2_Configuration(); //设置模拟狗相关
	ADC_AnalogWatchdogSingleChannelConfig(ADC1,ADC_Channel_15);	//设置模拟狗相关
	ADC_AnalogWatchdogThresholdsConfig(ADC1,0xf,0);//设置模拟狗相关
	ADC_AnalogWatchdogCmd(ADC1,ADC_AnalogWatchdog_SingleRegEnable);	//设置模拟狗相关
	ADC_ITConfig(ADC1,ADC_IT_AWD,DISABLE); //设置模拟狗相关
}
/**************************************************************************************
 Func: ADC中断函数
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void ADC1_2_IRQHandler(void) //模拟狗相关
{
	ADC_ITConfig(ADC1,ADC_IT_AWD,DISABLE);
	if(SET == ADC_GetFlagStatus(ADC1,ADC_FLAG_AWD))
	{
		ADC_ClearFlag(ADC1,ADC_FLAG_AWD);
		ADC_ClearITPendingBit(ADC1,ADC_IT_AWD);
		if(Tos_TaskGetState(TOUCH_Tos_TaskID)==_TTS_Stop)Tos_TaskRecover(TOUCH_Tos_TaskID);
	else Tos_TaskTimeout(TOUCH_Tos_TaskID,10);//防止在任务没有挂起就发生dma发送中断了
	}
	
}
/**************************************************************************************
 Func: 配置端口读取Y轴AD值
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Touch_ADCY_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//强拉接地
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PCout(5)=0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_5); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//强拉接VCC
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PBout(1)=1;
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 
}
/**************************************************************************************
 Func: 配置端口读取X轴AD值
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Touch_ADCX_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PCout(4)=0;
	GPIO_ResetBits(GPIOC,GPIO_Pin_4); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PBout(0)=1;
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	
}
/**************************************************************************************
 Func: 配置端口等待中断发生
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Touch_XIRQ()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//PCout(4)=1;
	GPIO_SetBits(GPIOC,GPIO_Pin_4); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PBout(0)=1;
	GPIO_SetBits(GPIOB,GPIO_Pin_0); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*************************************************************
Func:  松开动作事件
Date:  2014 端午节
Note:  测试
*************************************************************/
void Touch_UpIngEvent()
{
	if(Touch_Event.UpIng_Event)
	{
#ifdef COMPILE_MODE_DEBUG
		DeBug("TOUCH_EVENT_UPING",Infor_Infor);
#endif
		Touch_Event.UpIng_Event=Null;
	}
}
/*************************************************************
Func:  按下动作事件
Date:  2014 端午节
Note:  测试
*************************************************************/
void Touch_DownIngEvent()
{
	if(Touch_Event.DownIng_Event)
	{
#ifdef COMPILE_MODE_DEBUG
		DeBug("TOUCH_EVENT_DOWNING",Infor_Infor);
#endif
		Touch_Event.DownIng_Event=Null;
	}
}
/*************************************************************
Func:  松开超时事件
Date:  2014 端午节
Note:  测试
*************************************************************/
void Touch_UpDelayEvent()
{
	if(Touch_Event.Up_Event)
	{
#ifdef COMPILE_MODE_DEBUG
		DeBug("TOUCH_EVENT_UP",Infor_Infor); 
#endif
		Touch_Event.Up_Event=Null;
	}
}
/*************************************************************
Func:  按下超时事件
Date:  2014 端午节
Note:  测试
*************************************************************/
void Touch_DownDelayEvent()
{
	if(Touch_Event.Down_Event)
	{
#ifdef COMPILE_MODE_DEBUG
		DeBug("TOUCH_EVENT_DOWN",Infor_Infor);
#endif
		Touch_Event.Down_Event=Null;
	}
}
/*************************************************************
Func:  触摸屏监视任务
Date:  2014 端午节
Note:  测试
*************************************************************/
void Task_GetTouch(void *Tags)
{
  uint32 Temp_ADC1,Temp_ADC2,Temp_ADC11;
	TGUI_TouchMsg  TouchMsg;
	TOUCH_Tos_TaskID=Tos_TaskCurrent;
	ADC_WatchdogConfig();
	while(1)
	{
		
		Tos_TaskDelay(30); 
		Touch_ADCX_Config();
		Tos_TaskDelay(10);
		Temp_ADC11=AvrFilter_ByWords((uint16 *)&TOUCH_ADC_Value[0][1],TOUCH_ADC_BUFSize,TOUCH_ADC_BUFSize);
		if(Temp_ADC11>100)
		{
			Tos_TaskDelay(10); 
			Temp_ADC1=AvrFilter_ByWords((uint16 *)&TOUCH_ADC_Value[0][1],TOUCH_ADC_BUFSize,TOUCH_ADC_BUFSize);
			Touch_ADCY_Config();
			Tos_TaskDelay(10); 
			Temp_ADC2=AvrFilter_ByWords((uint16 *)&TOUCH_ADC_Value[0][0],TOUCH_ADC_BUFSize,TOUCH_ADC_BUFSize);
			if(__fabs(Temp_ADC11-Temp_ADC1)<20&Temp_ADC2>400)//消抖
			{
			  TOUCH_ADC_YValue=Temp_ADC2; 
				TOUCH_ADC_XValue=Temp_ADC1;
				if(Touch_Update==TOUCH_DOWNING){Touch_Update=TOUCH_DOWN;}
				if(Touch_Update!=TOUCH_DOWN)
					{
					Touch_Update=TOUCH_DOWNING;
						Touch_Event.DownIng_Event=Touch_DownIngEvent;//事件测试
						Touch_Event.Down_Event=Touch_DownDelayEvent;//事件测试
					if(Touch_Event.DownIng_Event)Touch_Event.DownIng_Event();//当存在按下事件时触发
						Touch_Event.UpIng_Event=Touch_UpIngEvent;//////事件测试
					Touch_Event.DownDelay_Count=0;
					}	
				if(Touch_Update==TOUCH_DOWN)
					{
						if(Touch_Event.DownDelay_Count<TOUCH_DOWNDELAYCOUNT)
						{
							Touch_Event.DownDelay_Count++;
						}else if(Touch_Event.Down_Event)Touch_Event.Down_Event();//当存在持续按下事件时触发
					}
				
				TOUCH_XValue=Opera_WhithADC(TOUCH_ADC_XValue,TOUCH_CROSS_DATA[0][0],TOUCH_CROSS_ADDR[0][0],TOUCH_CROSS_DATA[1][0],TOUCH_CROSS_ADDR[1][0],LCD_XMAX);
	      TOUCH_YValue=Opera_WhithADC(TOUCH_ADC_YValue,TOUCH_CROSS_DATA[0][1],TOUCH_CROSS_ADDR[0][1],TOUCH_CROSS_DATA[3][1],TOUCH_CROSS_ADDR[3][1],LCD_YMAX);	
#ifdef COMPILE_MODE_DEBUG
			  DeBug("X=%d Y=%d",TOUCH_ADC_XValue,TOUCH_ADC_YValue,Infor_Infor);
#endif
				TouchMsg.TouchValueX=TOUCH_XValue;
				TouchMsg.TouchValueY=TOUCH_YValue;
				TouchMsg.TouchType=Touch_Update;
				TouchMsg.TouchTime=0;	
				if(Touch_Update==TOUCH_DOWN)TouchMsg.TouchTime=Touch_Event.DownDelay_Count;
				if(Touch_Update==TOUCH_UP)TouchMsg.TouchTime=Touch_Event.UpDelay_Count;
				Tos_TaskPutQmsg(Touch_Screen_UserTaskID,TGUI_MsgType_TouchScreen,sizeof(TGUI_TouchMsg),&TouchMsg);
		}
	}else
	{
		if(Touch_Update==TOUCH_UPING){Touch_Update=TOUCH_UP;}
		if(Touch_Update!=TOUCH_UP)//触摸屏第一次按下触发
			{
			Touch_Update=TOUCH_UPING;	
			if(Touch_Event.UpIng_Event)Touch_Event.UpIng_Event();//当存在放下动作时触发
			Touch_Event.UpDelay_Count=0;//按下第一次时清除按下时间计数
			Touch_Event.Up_Event=Touch_UpDelayEvent;//////事件测试
			}
		if(Touch_Update==TOUCH_UP)//触摸屏一直按下
		{
			if(Touch_Event.UpDelay_Count<TOUCH_UPDELAYCOUNT)
			{
			
				Touch_Event.UpDelay_Count++;
			}else if(Touch_Event.Up_Event)Touch_Event.Up_Event();//当存在持续未按下触发
		}
	}
	} 
}
/**************************************************************************************
 Func: 四线触摸端口配置
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Touch_ADC1_Configuration(void)
{
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;	//模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换关闭
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel =TOUCH_ADC_CHN;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC1, ENABLE);	   //使能指定的ADC1
	ADC_ResetCalibration(ADC1);	  //复位指定的ADC1的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1复位校准寄存器的状态,设置状态则等待
	ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
	while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
}
/**************************************************************************************
 Func: 触摸ADC读取使用DMA方式，不停转换，根据需求使用X还是Y
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Touch_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&TOUCH_ADC_Value;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = TOUCH_ADC_BUFSize*TOUCH_ADC_CHN;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); 
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);	 
}
/**************************************************************************************
 Func: 触摸屏初始化接口
 Time: 2014-6-18
 Ver.: V1.0
 Note;设备管理接口
**************************************************************************************/
void Touch_ADC_Init()
{
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
	Touch_ADC1_Configuration();
	Touch_DMA_Configuration();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE); 
	Touch_Event.UpIng_Event=Null;
	Touch_Event.Up_Event=Null;
	Touch_Event.DownIng_Event=Null;
	Touch_Event.Down_Event=Null;
	Touch_Event.UpDelay_Count=0;//延迟周期数后执行延迟任务
	Touch_Event.DownDelay_Count=0;//延迟周期数后执行延迟任务
	Load_Touch_Configs();//读取触摸屏配置
	TouchScreen_Correct();
}

/**************************************************************************************
 Func: 设备挂载
 Time: 2014-6-18
 Ver.: V1.0
 Note;设备管理接口
**************************************************************************************/
void DeviceMount_Touch()
{
	DeviceId_TOUCH=DeviceId_Index++;
	FlashGetConfigRoom(&TOUCH_CFG_OffSet,10);
	Tos_Device_Tab[DeviceId_TOUCH].DeviceId=DeviceId_TOUCH;
	Tos_Device_Tab[DeviceId_TOUCH].DeviceName="TOUCH";
	Tos_Device_Tab[DeviceId_TOUCH].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_TOUCH].DeviceVirtue=ReadOnly|WriteOnly|CharDerice;
	Tos_Device_Tab[DeviceId_TOUCH].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_TOUCH].Init=Touch_ADC_Init;
	Tos_Device_Tab[DeviceId_TOUCH].Write=Null;
	Tos_Device_Tab[DeviceId_TOUCH].Read=Null;
	Tos_Device_Tab[DeviceId_TOUCH].Exit=Null;
}




/**************************************************************************************
 Func: 在输入校准点后要做的事
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Draw_Cross(uint16 X,uint16 Y,uint16 R,uint16 C,uint8 Index)
{
  TGUI_DefaultStyle();
	TGUI_Set_Font(TGUI_FontType_12X12);
	TGUI_Set_Color(Color_White,Color_Green,TGUI_Button_ForeColor,Null);
	TGUI_Set_Region(X-20,Y-23,X+20,Y,Align_CenterTop);
	Printf("X=%d",TOUCH_CROSS_DATA[Index][0]);
	TGUI_Set_Region(X-20,Y,X+20,Y+12,Align_CenterBottom);
	Printf("Y=%d",TOUCH_CROSS_DATA[Index][1]);
	TGui_Draw_Line(X-R,Y,X+R,Y,C);
	TGui_Draw_Line(X,Y-R,X,Y+R,C);
	TGUI_DrawRound(X,Y,R,C);
	
}
void TOUCH_DrawRound(uint8 Index)
{
	TGUI_DrawRound(TOUCH_CROSS_ADDR[Index][0],TOUCH_CROSS_ADDR[Index][1],10,Color_Red);
	LCD_Set_Pixel(TOUCH_CROSS_ADDR[Index][0],TOUCH_CROSS_ADDR[Index][1],Color_White);
}
#define ABS(x) ( (x)>0?(x):-(x) ) 

BOOL Cheak_Correct()
{
	uint16 X,Y;
	uint16 Error=0;//容差
#ifdef COMPILE_MODE_DEBUG
	DeBug("Cheak_TouchCorrect:",Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[1][1]-TOUCH_CROSS_DATA[0][1]);
#ifdef COMPILE_MODE_DEBUG
DeBug("Tolerance=%d",Error,Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[2][1]-TOUCH_CROSS_DATA[3][1]);
#ifdef COMPILE_MODE_DEBUG
  DeBug("Tolerance=%d",Error,Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[3][0]-TOUCH_CROSS_DATA[0][0]);
#ifdef COMPILE_MODE_DEBUG
	DeBug("Tolerance=%d",Error,Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[2][0]-TOUCH_CROSS_DATA[1][0]);
#ifdef COMPILE_MODE_DEBUG
	DeBug("Tolerance=%d",Error,Infor_Infor);
#endif
	if(Error>50){
#ifdef COMPILE_MODE_DEBUG
							DeBug("Correct Fail!",Infor_Infor);
#endif
	return False;}
  Error=ABS(TOUCH_CROSS_DATA[1][0]-TOUCH_CROSS_DATA[0][0]);
#ifdef COMPILE_MODE_DEBUG
	DeBug("Discrete=%d",Error,Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[2][0]-TOUCH_CROSS_DATA[3][0]);
#ifdef COMPILE_MODE_DEBUG
	DeBug("Discrete=%d",Error,Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[3][1]-TOUCH_CROSS_DATA[0][1]);
#ifdef COMPILE_MODE_DEBUG
	DeBug("Discrete=%d",Error,Infor_Infor);
#endif
	Error+=ABS(TOUCH_CROSS_DATA[2][1]-TOUCH_CROSS_DATA[1][1]);
#ifdef COMPILE_MODE_DEBUG
	DeBug("Discrete=%d",Error,Infor_Infor);
#endif
  if(Error<1000){
#ifdef COMPILE_MODE_DEBUG
	DeBug("Correct Fail!",Infor_Infor);
#endif
	return False;}
	X=Opera_WhithADC(TOUCH_CROSS_DATA[4][0],TOUCH_CROSS_DATA[0][0],TOUCH_CROSS_ADDR[0][0],TOUCH_CROSS_DATA[1][0],TOUCH_CROSS_ADDR[1][0],LCD_XMAX);
	Y=Opera_WhithADC(TOUCH_CROSS_DATA[4][1],TOUCH_CROSS_DATA[0][1],TOUCH_CROSS_ADDR[0][1],TOUCH_CROSS_DATA[3][1],TOUCH_CROSS_ADDR[3][1],LCD_YMAX);
	DeBug("X=%d  Y=%d",X,Y,Infor_Infor);
	return True;
}


void TOUCH_SaveCorrect(uint8 Index)
{
	TOUCH_CROSS_DATA[Index][0]=TOUCH_ADC_XValue;
	TOUCH_CROSS_DATA[Index][1]=TOUCH_ADC_YValue;

}
/**************************************************************************************
 Func: 触摸屏获取校准点
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void TouchScreen_GetTouch(uint8 Index)
{
	while(1)
	{
		Tos_TaskDelay(10);
		if(Touch_Update==TOUCH_DOWN)
			{			
				while(1)
				{
					if(Touch_Update==TOUCH_UP)
					{
						TOUCH_SaveCorrect(Index);
						return;
					}
					Tos_TaskDelay(10);
				}
			}
	}
}
/**************************************************************************************
 Func: 触摸屏校准
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void TouchScreen_Correct()
{
	uint8 TOUCH_Index=0;
	if(Cheak_Correct()){
#ifdef COMPILE_MODE_DEBUG
	DeBug("Load Touch Configs Ok!",Infor_Infor);
#endif
		return;}
	LCD_Clear_Screen(Color_Black);
	TOUCH_Correct_TaskID=Tos_TaskCurrent;
	TOUCH_DrawRound(TOUCH_Index);
	DeBug_Get();
	while(TOUCH_Index<5)
	{
		TOUCH_DrawRound(TOUCH_Index);
		TouchScreen_GetTouch(TOUCH_Index);
		Draw_Cross(TOUCH_CROSS_ADDR[TOUCH_Index][0],TOUCH_CROSS_ADDR[TOUCH_Index][1],5,Color_White,TOUCH_Index);
		TOUCH_Index++;
	}
	if(Cheak_Correct())Save_Touch_Configs();
	DeBug_Drop();
}

/**************************************************************************************
 Func:读取触摸配置
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Load_Touch_Configs()
{
	FLASH_ReadDatas(TOUCH_CFG_OffSet,&TOUCH_CROSS_DATA[0][0],10);
}
/**************************************************************************************
 Func: 保存触摸配置
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Save_Touch_Configs()
{
	FLASH_WriteDatas(TOUCH_CFG_OffSet,&TOUCH_CROSS_DATA[0][0],10);
}

/**************************************************************************************
 Func: 任务获取触摸屏
 Time: 2014-6-18
 Ver.: V1.0
 Note;
**************************************************************************************/
void Tos_TaskGetTouch()
{
	Touch_Screen_UserTaskID=Tos_TaskCurrent;
}




