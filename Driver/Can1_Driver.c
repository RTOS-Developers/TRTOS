#include <include.h>
#include <Tools_Include.h>
#include <Can1_Driver.h>
uint8 CAN_Tos_TaskID=Tos_TaskNull;
uint8 DeviceId_CAN1=DeviceNull;
Tos_ArrayMessage Msg[10]; 
CanRxMsg RxMessageBuf[10];
/*************************************************************
Func:  CAN 发送中断
Date:  2014-8-26
Note:  在这里没用使用
*************************************************************/
void USB_HP_CAN0_TX_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN1, CAN_IT_TME)!=RESET)	 //FIFO空中断
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);      //清除中断标记
                         
	}	
}
extern void CAN_Istr(CanRxMsg *RxMessage);
extern void USB_Istr(void);
/*************************************************************
Func:  CAN 接收中断
Date:  2014-8-26
Note:  之前就是因为这里出现了
*************************************************************/

#if defined(USBHID_ENABLE)
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}

#elif defined(CAN_ISTR_EN)
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;	
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0)!=RESET)	 //FIFO1消息挂号状态标志 
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);     //清除中断标记
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); 	 //把FIFO1中的报文另存到临时结构体中
		CAN_Istr(&RxMessage);
	}	
}
#endif

/*************************************************************
Func:  CAN GPIO初始化
Date:  2014-8-26
Note:  
*************************************************************/
void CAN1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_Struct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Struct.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOA,&GPIO_Struct);

	GPIO_Struct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Struct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Struct.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOA,&GPIO_Struct);
	
}

/*************************************************************
Func:  CAN 中断配置
Date:  2014-8-26
Note:  
*************************************************************/
void CAN1_NVIC_Cfg(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable CAN1 RX0 interrupt IRQ channel */
   NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

}

void CAN_SIG_Filter_Configs()
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CAN_FilterInitStructure.CAN_FilterNumber=0;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 掩码
	
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;   //掩码
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
}
/*************************************************************
Func:  CAN 初始化
Date:  2014-8-26
Note:  
*************************************************************/
void CAN_SIG_Init(void)
{
	CAN_InitTypeDef        CAN_InitStructure;

	CAN1_NVIC_Cfg();
	CAN1_GPIO_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		
	CAN_DeInit(CAN1);//恢复到默认值
	CAN_StructInit(&CAN_InitStructure);//复位所有设置   

	CAN_InitStructure.CAN_TTCM=DISABLE;//禁止时间触发通信模式
	CAN_InitStructure.CAN_ABOM=DISABLE;//，软件对CAN_MCR寄存器的INRQ位进行置1随后清0后，一旦硬件检测
                                     //到128次11位连续的隐性位，就退出离线状态。
	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过清除CAN_MCR寄存器的SLEEP位，由软件唤醒
	CAN_InitStructure.CAN_NART=ENABLE;//DISABLE;CAN报文只被发送1次，不管发送的结果如何（成功、出错或仲裁丢失）
	CAN_InitStructure.CAN_RFLM=DISABLE;//在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有
	CAN_InitStructure.CAN_TXFP=DISABLE;//发送FIFO优先级由报文的标识符来决定
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
#ifdef HSI_USER
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_1tq;
	CAN_InitStructure.CAN_Prescaler=9;//36/8/16
#else 
CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=9;//36/8/16
#endif
	
	// CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
	CAN_Init(CAN1,&CAN_InitStructure);										
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_SIG_Filter_Configs();
	
} 

void CAN_SIG_SendMsg(uint8 *Data,uint32 Addr)
{
	uint16 TransmitMailbox=0,i=0;
	CanTxMsg  TxMessage;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.StdId=Addr;
	TxMessage.RTR=CAN_RTR_DATA;//发送数据帧
	BufferCoppy(Data,&TxMessage.Data[0],8);
	TxMessage.DLC=8;
	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
	while(CAN_TransmitStatus(CAN1,TransmitMailbox)!=CANTXOK) 
	{
	i++;
	if(i>=0xff){CAN_SIG_Init();return;}
	Tos_TaskDelay(1);
	}
}
///*************************************************************
//Func:  CAN 发送消息
//Date:  2014-8-26
//Note:  
//*************************************************************/
//void CAN1_SendMsg(void *Msg,uint8 Length)
//{
//	static uint8 TestValue='0';
//	uint16 TransmitMailbox=0,i=0;
//	CanTxMsg  TxMessage;
//	TxMessage.ExtId=0x11<<13;//接收板地址
//	TxMessage.RTR=CAN_RTR_DATA;//发送数据帧
//	TxMessage.IDE=CAN_ID_EXT;
//	TxMessage.DLC=Length+1;//发送数据的长度
//	BufferCoppy(Msg,&TxMessage.Data[0],Length);
//	TxMessage.Data[Length]=TestValue;
//	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
//	while((CAN_TransmitStatus(CAN1,TransmitMailbox)!=CANTXOK)&&(i!=0xff)) 
//	{
//	i++;
//		Tos_TaskDelay(1);
//	}
//	if(i==0xff)DeBug("Can Send TimeOut1",Infor_Infor);
//	else DeBug("Can Send Succeed1",Infor_Infor);
//	if(TestValue<'9')TestValue++;
//	else TestValue='0';
//}
void DeviceMount_Can1()
{
	DeviceId_CAN1=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_CAN1].DeviceName="CAN1";
	Tos_Device_Tab[DeviceId_CAN1].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_CAN1].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_CAN1].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_CAN1].Init=CAN_SIG_Init;
	Tos_Device_Tab[DeviceId_CAN1].Write=Null;
	Tos_Device_Tab[DeviceId_CAN1].Read=Null;
	Tos_Device_Tab[DeviceId_CAN1].Exit=Null;
}






















