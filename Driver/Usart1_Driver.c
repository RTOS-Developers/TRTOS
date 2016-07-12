#include <Include.h>
#include <STM32LIB_Include.h>
#include <Tools_Include.h>
#include <Usart1_Driver.h>
uint8 DeviceId_USART1=DeviceNull;
uint8 DeviceId_USART1_RX=DeviceNull;
uint8  USART1_TX_TaskID=Tos_TaskTotal;
uint8  USART1_RX_TaskID=Tos_TaskTotal;
uint8  USART1_RX_Buffer[USART_RX_BUFFER_SIZE];
uint8  USART1_TX_Buffer[USART_TX_BUFFER_SIZE];
uint16 USART1_RX_Count=Null;
uint16 USART1_RX_Index;

/******************************************************************************
 Func: 串口1的端口设置
 Time: 2014-7-23
 Ver.: V1.0
 Note:
******************************************************************************/
void USART1_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
#ifdef USART1_GPIO_Remap
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//使能端口重映射 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//USART1_RX配置 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	//USART1_参数设置
#else
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART1_RX配置 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//USART1_参数设置
#endif
}
void USART1_Struct_Init(uint32 Baud)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = Baud; 
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
}
void USART1_RX_Enable(FunctionalState En)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;               //通道设置为串口1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //中断占先等级0  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;              //中断响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = En;                 //打开中断  
	NVIC_Init(&NVIC_InitStructure);   
}
/**************************************************************************************
 Func: 初始化Tos
 Time: 2014-1-31
 Ver.: V1.0
 Note; 用于初始化系统变量
**************************************************************************************/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART1_GPIO_Init();
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);  
#ifdef USART1_BaudRate
	USART_InitStructure.USART_BaudRate = USART1_BaudRate; 
#else 
	USART_InitStructure.USART_BaudRate = 115200; 
#endif
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE); 
	//配置UART1中断    
	USART1_RX_Enable(ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

/**************************************************************************************
 Func: 串口数据中断
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART1_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART1->DR;	  
		if(USART1_RX_Index<USART1_RX_Count)
		{
			(USART1_RX_Buffer[USART1_RX_Index++])=c;
			Tos_TaskTimeout(USART1_RX_TaskID,10);
		}
	} 
}
/**************************************************************************************
 Func: 使用DMA方式传输数据
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART1_DMAConfiguration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	USART1_Configuration();
	  
	//启动DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMA发送中断设置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//DMA1通道4配置
	DMA_DeInit(DMA1_Channel4);
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32)USART1_TX_Buffer;//地址未设置请自行设置
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize =sizeof(USART1_TX_Buffer);//长度未设定请自行设置
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//设置DMA的2个memory中的变量互相访问
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
}
/**************************************************************************************
 Func: DMA传输中断
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void DMA1_Channel4_IRQHandler(void)//串口1DMA发送中断函数  
{  
    DMA_ClearFlag(DMA1_FLAG_TC4);  
	if(Tos_TaskGetState(USART1_TX_TaskID)==_TTS_DelayWait)return;
	if(Tos_TaskGetState(USART1_TX_TaskID)==_TTS_Suspend)Tos_TaskRecover(USART1_TX_TaskID);
	else Tos_TaskTimeout(USART1_TX_TaskID,10);//防止在任务没有挂起就发生dma发送中断了
}  

void USART1_WriteDatas(u8* Data,u16 Length)
{
 	 u16 i;
 	 for (i=0; i<Length; i++){
 	USART1_SendByte(Data[i]);
 	}
}


void Usart1_Write(void *Addr,void *Data,void *Length)
{
	USART1_TX_TaskID=Tos_TaskCurrent;
	DMA1_Channel4->CMAR=(uint32)Data;	
	DMA1_Channel4->CNDTR=(uint32)Length;
	DMA_Cmd(DMA1_Channel4, ENABLE);
	Tos_TaskStop(USART1_TX_TaskID); 
	DMA_Cmd(DMA1_Channel4,DISABLE);
}

void DeviceMount_Usart1()
{
	DeviceMount_Usart(USART1);
}
void USART1_DMAWriteDatas(u8* Data,u16 Length)
{
	BOOL R;
	R=Tos_TaskGetDev(DeviceId_USART1,0);
  if(R)
	{
		USART1_TX_TaskID=Tos_TaskCurrent;
		DMA1_Channel4->CMAR=(uint32)Data;	
		DMA1_Channel4->CNDTR=(uint32)Length;
		DMA_Cmd(DMA1_Channel4, ENABLE);
		Tos_TaskStop(USART1_TX_TaskID); 
		DMA_Cmd(DMA1_Channel4,DISABLE);
	  Tos_TaskDropDev(DeviceId_USART1);
	}
	
}	
uint16 USART1_ReadDatas(uint8 *Buf,uint16 Length,uint32 TimeOut)
{
	BOOL R;
	R=Tos_TaskGetDev(DeviceId_USART1_RX,0);
	USART1_RX_Enable(ENABLE);
  if(R)
	{
		USART1_RX_TaskID=Tos_TaskCurrent;
		USART1_RX_Count=Length;
		USART1_RX_Index=0;
		if(TimeOut)Tos_TaskDelay(TimeOut);
		else Tos_TaskSuspend(USART1_RX_TaskID); 
	  Tos_TaskDropDev(DeviceId_USART1_RX);
		USART1_RX_Enable(DISABLE);
	}
	
	return USART1_RX_Index;
}

/**************************************************************************************
 Func: 串口发送单个数据
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
}
/**************************************************************************************
 Func: 串口发送字符串
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART1_Printf(u8 *Data)
{
	USART1_WriteDatas(Data,GetTextLength(Data));
}
/**************************************************************************************
 Func: 串口主动接收数据
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
u16 USART1_ReciveByte()
{
   while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
   return (USART_ReceiveData(USART1)); 
}

