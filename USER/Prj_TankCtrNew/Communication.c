#include <Include.h>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.h>
//向遥控传送实时数据
void Task_SendValue(void *Tags)
{
	uint8 Length;
	while(1)
	{
		Tos_TaskDelay(1000);
		LED2_ON;
		Length=ZigBee_SendMsg(&USART_TX_Buffer[0][0],TankRun_Value.MasterAddr,SCFF_RefurBishSet,(uint8 *)&DevRun_Value,sizeof(DevRun_Value));
	  USART_WriteDatas(USART1,&USART_TX_Buffer[0][0],Length);
		LED2_OFF;
		CAN_SIG_SendMsg((uint8 *)&DevRun_Value.AllStatus,0x81);
	}
}

void RS485_Send(USART_TypeDef* USARTx,uint16 Addr,uint16 Cmd,RS485WriteStruct *CCRS)
{
	CCRS->Addr=Addr;
	CCRS->Cmd=Cmd;
	CCRS->WaterID=0x01;
	CCRS->Note=0xaa;
	Create_Message(CCRS,sizeof(RS485WriteStruct));
	RS485_TXD;
	USART_WriteDatas(USARTx,(uint8 *)CCRS,sizeof(RS485WriteStruct));
	Tos_TaskDelay(2);
}
#define SPASETIME 1000
void RS485_Revice(USART_TypeDef* USARTx,RS485ReadStruct *RRS)
{
	uint8 Length;
	RS485ReadStruct *P;
	uint8 UsartCH=USART_GetIndex(USARTx);
	RS485_RXD;
	Length=USART_ReadDatas(USARTx,USART_TX_Buffer[UsartCH],sizeof(RS485ReadStruct),SPASETIME);
	if(Export_Message(USART_TX_Buffer[UsartCH],Length))
		{
			P=(RS485ReadStruct *)USART_TX_Buffer[UsartCH];
			switch(P->Addr)
			{
				case 0x01:switch(P->Cmd)
									{
										case 0x01:BufferCoppy(USART_TX_Buffer[UsartCH],RRS,sizeof(RS485ReadStruct));break;
										case 0x02:App_SetButtonDelay(P->Note);break;
										default:break;
									}break;
				case 0xff:switch(P->Cmd)
									{
										case 0x01:BufferCoppy(USART_TX_Buffer[UsartCH],RRS,sizeof(RS485ReadStruct));
										default:break;
									}break;
				default:break;
			}
			
		}
}
//向天车发送数据
void Task485(void *Tags)
{
	USART_TypeDef *USARTx=(USART_TypeDef*)Tags;
	SkyBus_Value.AllStatus=0xffffffff;
	RS485WiteToSkyBus_Value.AllStatus=0x00000000;
	while(1)
	{
		
		RS485_Send(USARTx,0x02,0x01,&RS485WiteToSkyBus_Value);//天车通信
		RS485_Revice(USARTx,&SkyBus_Value);
		Tos_TaskDelay(SPASETIME);
		RS485_Send(USARTx,0x03,0x01,&RS485Wite_Value);//中控室通信
		RS485_Revice(USARTx,&CtrRoom_Value);
		Tos_TaskDelay(SPASETIME);
	}
}
