#include <Include.h>
#include <STM32LIB_Include.h>
#include <Tools_Include.h>
#include <Usart3_Driver.h>
uint8 DeviceId_USART3=DeviceNull;
uint8 DeviceId_USART3_RX=DeviceNull;
uint8  USART3_TX_TaskID=Tos_TaskTotal;
uint8  USART3_RX_TaskID=Tos_TaskTotal;
uint8  USART3_RX_Buffer[USART_RX_BUFFER_SIZE];
uint8  USART3_TX_Buffer[USART_TX_BUFFER_SIZE];
uint16 USART3_RX_Count=Null;
uint16 USART3_RX_Index;

/******************************************************************************
 Func: 串口1的端口设置
 Time: 2014-7-23
 Ver.: V1.0
 Note:
******************************************************************************/
void USART3_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);   
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口时钟配置 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//USART1_RX配置 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	//USART1_参数设置
}

void USART3_Struct_Init(uint32 Baud)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = Baud; 
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
}
void USART3_RX_Enable(FunctionalState En)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;               //通道设置为串口1中断  
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
void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART3_GPIO_Init();
 RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);  
#ifdef USART3_BaudRate
	USART_InitStructure.USART_BaudRate = USART3_BaudRate; 
#else 
	USART_InitStructure.USART_BaudRate = 115200; 
#endif
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE); 
	//配置UART1中断    
	USART3_RX_Enable(ENABLE);
}

/**************************************************************************************
 Func: 串口数据中断
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART3_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART3->DR;	  
		if(USART3_RX_Index<USART3_RX_Count)
		{
			(USART3_RX_Buffer[USART3_RX_Index++])=c;
			Tos_TaskTimeout(USART3_RX_TaskID,10);
		}
	} 
}
/**************************************************************************************
 Func: 串口发送单个数据
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART3_SendByte(u16 Data)
{ 
  while((USART3->SR & USART_FLAG_TXE) == 0);
   USART3->DR = (Data & (uint16_t)0x01FF);	 
}
/**************************************************************************************
 Func: DMA传输中断
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void DMA1_Channel6_IRQHandler(void)//串口1DMA发送中断函数  
{  
    DMA_ClearFlag(DMA1_FLAG_TC4);  
	if(Tos_TaskGetState(USART3_TX_TaskID)==_TTS_Suspend)return;
	if(Tos_TaskGetState(USART3_TX_TaskID)==_TTS_Stop)Tos_TaskRecover(USART3_TX_TaskID);
	else Tos_TaskTimeout(USART3_TX_TaskID,10);//防止在任务没有挂起就发生dma发送中断了
}  
/**************************************************************************************
 Func: 使用DMA方式传输数据
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void USART3_DMAConfiguration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	USART3_Configuration();
	  
	//启动DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMA发送中断设置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//DMA1通道4配置
	DMA_DeInit(DMA1_Channel6);
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32)USART3_TX_Buffer;//地址未设置请自行设置
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize =sizeof(USART3_TX_Buffer);//长度未设定请自行设置
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
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
}
void USART3_WriteDatas(u8* Data,u16 Length)
{
 	 u16 i;
 	 for (i=0; i<Length; i++){
 	USART3_SendByte(Data[i]);
 	}
}
void USART3_DMAWriteDatas(u8* Data,u16 Length)
{
	BOOL R;
	
	R=Tos_TaskGetDev(DeviceId_USART3,0);
  if(R)
	{
		USART3_TX_TaskID=Tos_TaskCurrent;
		DMA1_Channel6->CMAR=(uint32)Data;	
		DMA1_Channel6->CNDTR=(uint32)Length;
		DMA_Cmd(DMA1_Channel6, ENABLE);
		Tos_TaskStop(USART3_TX_TaskID); 
		DMA_Cmd(DMA1_Channel6,DISABLE);
	  Tos_TaskDropDev(DeviceId_USART3);
	}
	
}	
uint16 USART3_ReadDatas(uint8 *Buf,uint16 Length,uint32 TimeOut)
{
	BOOL R;
	R=Tos_TaskGetDev(DeviceId_USART3_RX,0);
	USART3_RX_Enable(ENABLE);
  if(R)
	{
		USART3_RX_TaskID=Tos_TaskCurrent;
		USART3_RX_Count=Length;
		USART3_RX_Index=0;
		if(TimeOut)Tos_TaskDelay(TimeOut);
		else Tos_TaskStop(USART3_RX_TaskID); 
	  Tos_TaskDropDev(DeviceId_USART3_RX);
		USART3_RX_Enable(DISABLE);
	}
	
	return USART3_RX_Index;
}
void DeviceMount_Usart3()
{
	DeviceId_USART3=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_USART3].DeviceId=DeviceId_USART3;
	Tos_Device_Tab[DeviceId_USART3].DeviceName="USART3";
	Tos_Device_Tab[DeviceId_USART3].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_USART3].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_USART3].DeviceState=Ready;
	Tos_Device_Tab[DeviceId_USART3].Init=USART3_Configuration;
	Tos_Device_Tab[DeviceId_USART3].Write=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART3].Read=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART3].Exit=Null;
	
	DeviceId_USART3_RX=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_USART3_RX].DeviceId=DeviceId_USART3_RX;
	Tos_Device_Tab[DeviceId_USART3_RX].DeviceName="USART3_RX";
	Tos_Device_Tab[DeviceId_USART3_RX].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_USART3_RX].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_USART3_RX].DeviceState=Ready;
	Tos_Device_Tab[DeviceId_USART3_RX].Init=Null;
	Tos_Device_Tab[DeviceId_USART3_RX].Write=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART3_RX].Read=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART3_RX].Exit=Null;
}
