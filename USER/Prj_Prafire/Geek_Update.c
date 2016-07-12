#include <Geek_Update.h>
#include <W25Q16_Driver.h>

void Geek_Send(USART_TypeDef* USARTx,uint8 Addr,uint8 Cmd,uint32 Tags1,uint32 Tags2,uint8 *Buf,uint16 Length)
{
	Geek_DatStruct *GDS;
	GDS=(Geek_DatStruct *)Mema_Malloc(sizeof(Geek_DatStruct)+Length);
	GDS->Addr=Addr;
	GDS->Cmd=Cmd;
	GDS->Tags1=Tags1;
	GDS->Tags2=Tags2;
	GDS->Length=Length;
	BufferCoppy(&Buf[0],GDS->Buf,Length);
	Create_Message(GDS,sizeof(Geek_DatStruct)+Length);
	USART_WriteDatas(USARTx,(uint8 *)&GDS,sizeof(Geek_DatStruct)+Length);
	Mema_Free(GDS);
}
void Geek_CheckUpdate(USART_TypeDef* USARTx)
{
	uint8 Length;
	Geek_DatStruct *GDS;
	uint8 Buf[100];
	uint8 UsartCH=USART_GetIndex(USARTx);
	Length=USART_ReadDatas(USARTx,USART_TX_Buffer[UsartCH],USART_RX_BUFFER_SIZE,SPASETIME);
	Geek_Send(USARTx,0x01,0xaa,Null,Null,Null,0);
	if(Export_Message(USART_TX_Buffer[UsartCH],Length))
	{
		GDS=(Geek_DatStruct *)USART_TX_Buffer[UsartCH];
		switch(GDS->Cmd)
		{
			case GC_Erase:break;
			case GC_Stop:break;
			case GC_Run:break;	
			case GC_Write:SPI_FLASH_BufferWrite(GDS->Buf,GDS->Tags1,GDS->Length);
										Geek_Send(USARTx,HOSTADDRESS,GC_Success,Null,Null,Null,0);break;
			case GC_Read:SPI_FLASH_BufferRead(&Buf[0],GDS->Tags2,GDS->Tags2);
									 Geek_Send(USARTx,HOSTADDRESS,GC_Data,GDS->Tags2,GDS->Tags2,&Buf[0],GDS->Tags2);break;
		}
	}
}
