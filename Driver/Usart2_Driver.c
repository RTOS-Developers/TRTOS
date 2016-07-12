#include <Include.h>
#include <STM32LIB_Include.h>
#include <Tools_Include.h>
#include <Usart2_Driver.h>
uint8 DeviceId_USART2=DeviceNull;
uint8 DeviceId_USART2_RX=DeviceNull;
uint8  USART2_TaskID;
uint8  USART2_RX_TaskID;
uint8  USART2_RX_Buffer[USART_RX_BUFFER_SIZE];
uint8  USART2_TX_Buffer[USART_TX_BUFFER_SIZE];
uint16 USART2_RX_Count=Null;
uint16 USART2_RX_Index;
void USART2_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//USART2_TX配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART2_RX配置 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//USART2_参数设置
 
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
#ifdef USART2_BaudRate
	USART_InitStructure.USART_BaudRate = USART2_BaudRate; 
#else 
	USART_InitStructure.USART_BaudRate = 115200; 
#endif
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE); 
	//配置UART1中断  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;               //通道设置为串口1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //中断占先等级0  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;              //中断响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断  
	NVIC_Init(&NVIC_InitStructure);    
	
}
void USART2_DMAConfiguration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	USART2_Configuration();
	  
	//启动DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMA发送中断设置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//DMA1通道4配置
	DMA_DeInit(DMA1_Channel4);
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32)USART2_TX_Buffer;//地址未设置请自行设置
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize =sizeof(USART2_TX_Buffer);//长度未设定请自行设置
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
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
}
void DMA1_Channel5_IRQHandler(void)//串口1DMA发送中断函数  
{  
    DMA_ClearFlag(DMA1_FLAG_TC5);  
	if(Tos_TaskGetState(USART2_TaskID)==_TTS_Stop)Tos_TaskRecover(USART2_TaskID);
	else Tos_TaskTimeout(USART2_TaskID,10);//防止在任务没有挂起就发生dma发送中断了
}  
void USART2_DMAWriteDatas(u8* Data,u16 Length)
{
	BOOL R;
	R=Tos_TaskGetDev(DeviceId_USART2,0);
  if(R)
	{
		USART2_TaskID=Tos_TaskCurrent;
		DMA1_Channel5->CMAR=(uint32)Data;	
		//BufferCoppy(Data,&USART1_TX_Buffer[0],Length);
		DMA1_Channel5->CNDTR=(uint32)Length;
		DMA_Cmd(DMA1_Channel5, ENABLE);
		Tos_TaskStop(USART2_TaskID); 
		DMA_Cmd(DMA1_Channel5,DISABLE);
	  Tos_TaskDropDev(DeviceId_USART2);
	}
}	
void _USART2_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART2->DR;	  
		if(USART2_RX_Index<USART2_RX_Count)
		{
			(USART2_RX_Buffer[USART2_RX_Index++])=c;
			Tos_TaskTimeout(USART2_RX_TaskID,100);
		}
	} 
}
void USART2_SendByte(u16 Data)
{ 
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);	 
}
void USART2_WriteDatas(u8* Data,u16 Length)
{
 	 u16 i;
 	 for (i=0; i<Length; i++){
 	USART2_SendByte(Data[i]);
 	}
}
uint16 USART2_ReadDatas(uint8 *Buf,uint16 Length,uint32 TimeOut)
{
	BOOL R;
	R=Tos_TaskGetDev(DeviceId_USART2_RX,0);
  if(R)
	{
		USART2_RX_TaskID=Tos_TaskCurrent;
		USART2_RX_Count=Length;
		USART2_RX_Index=0;
		if(TimeOut)Tos_TaskDelay(TimeOut);
		else Tos_TaskStop(USART2_RX_TaskID); 
	  Tos_TaskDropDev(DeviceId_USART2_RX);
	}
	return USART2_RX_Index;
}
void DeviceMount_Usart2()
{
	DeviceId_USART2=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_USART2].DeviceId=DeviceId_USART2;
	Tos_Device_Tab[DeviceId_USART2].DeviceName="USART2";
	Tos_Device_Tab[DeviceId_USART2].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_USART2].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_USART2].DeviceState=Ready;
	Tos_Device_Tab[DeviceId_USART2].Init=USART2_Configuration;
	Tos_Device_Tab[DeviceId_USART2].Write=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART2].Read=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART2].Exit=Null;
	
	DeviceId_USART2_RX=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_USART2_RX].DeviceId=DeviceId_USART2_RX;
	Tos_Device_Tab[DeviceId_USART2_RX].DeviceName="USART2_RX";
	Tos_Device_Tab[DeviceId_USART2_RX].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_USART2_RX].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_USART2_RX].DeviceState=Ready;
	Tos_Device_Tab[DeviceId_USART2_RX].Init=Null;
	Tos_Device_Tab[DeviceId_USART2_RX].Write=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART2_RX].Read=Null;//读写通用函数接口
	Tos_Device_Tab[DeviceId_USART2_RX].Exit=Null;
}
