#include <Include.h>
#include <..\USER\Prj_TSCollect\APP_TS_Collect.h>
#include <..\Driver\SZZigBee_Driver.h>
DevRun_ValueStruct DevRun_Value;

void TaskReadCanToCom(void *Tags)
{
	uint16 Length;
	uint8 i;
	Tos_ArrayMessage *CurMsg,Msg[10]; 
  CanRxMsg *RxMessage,RxMessageBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&RxMessageBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetCAN();
	CAN_SIG_Init();
	CAN_SIG_Filter_Configs();
	while(1)
	{
		DeBug_Get();
		Tos_TaskDelay(100);
		//CurMsg=Tos_TaskGetQMsg();
		Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],0x01,0xfe,(uint8 *)RxMessage,sizeof(CanRxMsg));
		USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
		if(CurMsg)
		{
			RxMessage=CurMsg->Qmsg_Addr;
			switch(RxMessage->StdId)
			{
				//case 0x060:
//					Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],20,0xff,(uint8 *)RxMessage->Data,8);
//		      USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
//				  break;
				
			//	case 0x1c0:
//					Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],20,0xff,(uint8 *)RxMessage->Data,8);
//		      USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
	//			  break;
				default:
					Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],20,0xfe,(uint8 *)RxMessage,sizeof(CanRxMsg));
		      USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
				  break;
			}
		}
		LED3_ON;
		Tos_TaskDelay(10);
		LED3_OFF;
	}
}
