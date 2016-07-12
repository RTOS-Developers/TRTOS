#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <ComKeyBoard.h>
#include <..\Driver\SZZigBee_Driver.h>
uint8 KeyBoard_Tos_TaskID=Tos_TaskNull;
/******************************************************************************
 Func:获取串口传过来的按键信息
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:通过ModBus协议
******************************************************************************/
void Task_GetKeyBoard(void *Tags)
{
	uint16 Length;
	uint8 Buf[20];
	ModBus_Message ModBusMsg;
	MGUI_KeyMsg KeyMsg;
	USART_TypeDef *USARTx=(USART_TypeDef *)Tags;
	uint8 UsartCH=USART_GetIndex(USARTx);
	ModBus_Create(&ModBusMsg,0x01,0xfe,WorkMode_Slave,MBCM_Crc);
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(&USART_RX_Buffer[UsartCH][0],50);
		Length=USART_ReadDatas(USARTx,Null,sizeof(USART_RX_Buffer[UsartCH]),0);
		Length=ModBus_Expend(&USART_RX_Buffer[UsartCH][0],Length,&ModBusMsg);
		if(ModBusMsg.ErrorFlag==ModBus_Ok)
		{
			switch(ModBusMsg.MsgFlag)
			{
				case KCFF_IsKeyMsg:
					   KeyMsg.KeyValue=USART_RX_Buffer[UsartCH][Length];
				     KeyMsg.KeyType=USART_RX_Buffer[UsartCH][Length+1];
					   Length=StringCoppy("OK",&Buf[0]);
					   Tos_TaskPutQmsg(KeyBoard_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
				     break;
				default:Length=0;
						 break;
			}
			Length=ModBus_CreateMsg(&USART_TX_Buffer[UsartCH][0],&ModBusMsg,0x01,0x01,0x01,&Buf[0],Length);
			USART_WriteDatas(USARTx,&USART_TX_Buffer[UsartCH][0],Length);
		}
	}
}
/******************************************************************************
 Func:钩子
 Time: 2014年国庆节前夕
 Ver.: V1.0
 Note:获取键盘
******************************************************************************/
void Tos_TaskGetKeyBoard()
{
	KeyBoard_Tos_TaskID=Tos_TaskCurrent;
}
void Task_GetKeyBoardZigbee(void *Tags)
{
	uint16 Length;
	MGUI_KeyMsg KeyMsg;
	USART_TypeDef *USARTx=(USART_TypeDef *)Tags;
	uint8 UsartCH=USART_GetIndex(USARTx);
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(&USART_RX_Buffer[UsartCH][0],USART_RX_BUFFER_SIZE);
		Length=USART_ReadDatas(USARTx,Null,USART_RX_BUFFER_SIZE,0);
		if(MyCom_CheckCrc(&USART_RX_Buffer[UsartCH][0]))
		{
			Length=4;
			switch(USART_RX_Buffer[UsartCH][Length-2])
			{
				case 1:
						 KeyMsg.KeyValue=USART_RX_Buffer[UsartCH][Length];
				     KeyMsg.KeyType=USART_RX_Buffer[UsartCH][Length+1];
					   Tos_TaskPutQmsg(KeyBoard_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
						 break;
			}
		}
	}
}
