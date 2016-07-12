#include <include.h>
#include <Tools_Include.h>
#include <..\USER\Prj_CXHeftSensor\Can_Driver.h>

void CAN_Filter_Configs(uint16 LocalAddr,BOOL Host)
{
	if(Host)
	{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		DeBug("Configs Filter For Master",Infor_Infor);
	CAN_FilterInitStructure.CAN_FilterNumber=0;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x00;			// id高位包含
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=(uint16)(CAN_GROUP<<3)|CAN_RTR_DATA|CAN_ID_EXT;;		//id低位包含		
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	}
	else
	{
		
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		DeBug("Configs Filter For Slave Addr=0x%x",LocalAddr,Infor_Infor);
	CAN_FilterInitStructure.CAN_FilterNumber=0;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=LocalAddr;			
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;		// 掩码
	
	CAN_FilterInitStructure.CAN_FilterIdLow=(uint16)(CAN_GROUP<<3)|CAN_RTR_DATA|CAN_ID_EXT;		//id低位包含		
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xffff;   //掩码
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber=1;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0xffff;			// 设备可以接到广播地址
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xffff;		// 掩码
	
	CAN_FilterInitStructure.CAN_FilterIdLow=(uint16)(CAN_GROUP<<3)|CAN_RTR_DATA|CAN_ID_EXT;		//id低位包含		
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xffff;   //掩码
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	}
}
/*************************************************************
Func:  CAN 中断配置
Date:  2014-8-26
Note:  
*************************************************************/
void CAN_NVIC_Cfg(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable CAN1 RX0 interrupt IRQ channel */
   NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

} 

/*************************************************************
Func:  CAN 发送消息
Date:  2014-8-26
Note:  
*************************************************************/
void CAN_SendMsg(void *Msg,uint8 Length)
{
	static uint8 TestValue='0';
	uint16 TransmitMailbox=0,i=0;
	CanTxMsg  TxMessage;
	TxMessage.ExtId=0x11<<13;//接收板地址
	TxMessage.RTR=CAN_RTR_DATA;//发送数据帧
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.DLC=Length+1;//发送数据的长度
	BufferCoppy(Msg,&TxMessage.Data[0],Length);
	TxMessage.Data[Length]=TestValue;
	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
	while((CAN_TransmitStatus(CAN1,TransmitMailbox)!=CANTXOK)&&(i!=0xff)) 
	{
	i++;
		Tos_TaskDelay(1);
	}
	if(i==0xff)DeBug("Can Send TimeOut1",Infor_Infor);
	else DeBug("Can Send Succeed1",Infor_Infor);
	if(TestValue<'9')TestValue++;
	else TestValue='0';
}






















