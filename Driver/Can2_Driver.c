#include <include.h>
#include <Can2_Driver.h>


void _CAN1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Struct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Struct.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOA,&GPIO_Struct);

	GPIO_Struct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Struct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Struct.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOA,&GPIO_Struct);
	
}
void _CAN1_Init()
{
	
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;	

	_CAN1_GPIO_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	//CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;/*重新同步跳宽*/
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;/*时间段1*/
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;/*时间段2*/
	CAN_InitStructure.CAN_Prescaler=45;/*波特率预分频数*/
	CAN_Init(CAN1,&CAN_InitStructure);
	
	//允许所有报文通过
	CAN_FilterInitStructure.CAN_FilterNumber=0; //设置过滤器组0，范围为0~13
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//指定过滤器被设置为标识符屏蔽模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//给出过滤器位宽为32位
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;//为过滤器分配缓存 此处到//fifo 因为 CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
void _CAN2_NVIC_Init()
{
		NVIC_InitTypeDef NVIC_InitStructure;
	
  //  NVIC_InitStructure.NVIC_IRQChannel ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void _CAN_SendMsg(uint8 *Buf,uint8 Length)
{
	static uint8 TestValue;
	u8 TransmitMailbox=0,i=0;
	CanTxMsg  TxMessage;
	CanRxMsg  RxMessage;
	TxMessage.StdId=0x00;//接收板地址
	TxMessage.RTR=CAN_RTR_DATA;//发送数据帧
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.DLC=2;//发送数据的长度
	TxMessage.Data[0]=TestValue;
	TxMessage.Data[1]=0xFE;
	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
	while((CAN_TransmitStatus(CAN1,TransmitMailbox)!=CANTXOK)&&(i!=0xff))
	{
	i++;
	}
	if(i==0xff)DeBug("Can Send TimeOut2",Infor_Infor);
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.Data[0]==TestValue%0xff)
	{
		DeBug("Can_PoolBack_Ok TestValue=%d",TestValue,Infor_Infor);
	}
	TestValue++;
}





void CAN2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_Struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_Struct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Struct.GPIO_Pin=GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_Struct);

	GPIO_Struct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Struct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Struct.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOB,&GPIO_Struct);
	
	
	//GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);
}

void CAN2_Init(void)
{
	CAN_InitTypeDef CAN_Struct;
	CAN_FilterInitTypeDef CAN_FilterStruct;

	CAN2_GPIO_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);
	CAN_Struct.CAN_TTCM=DISABLE;
	CAN_Struct.CAN_ABOM=DISABLE;
	CAN_Struct.CAN_AWUM=DISABLE;
	CAN_Struct.CAN_NART=DISABLE;
	CAN_Struct.CAN_RFLM=DISABLE;
	CAN_Struct.CAN_TXFP=DISABLE;
	CAN_Struct.CAN_Mode=CAN_Mode_LoopBack;
	CAN_Struct.CAN_SJW=CAN_SJW_1tq;
	CAN_Struct.CAN_BS1=CAN_BS1_8tq;
	CAN_Struct.CAN_BS2=CAN_BS2_7tq;
	CAN_Struct.CAN_Prescaler=5;
	CAN_Init(CAN2,&CAN_Struct);

	CAN_FilterStruct.CAN_FilterNumber=0;
	CAN_FilterStruct.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterStruct.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterStruct.CAN_FilterIdHigh=0x0000;
	CAN_FilterStruct.CAN_FilterIdLow=0x0000;
	CAN_FilterStruct.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterStruct.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterStruct.CAN_FilterFIFOAssignment=0;
	CAN_FilterStruct.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterStruct);
}   
