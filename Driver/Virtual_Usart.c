#include <Virtual_Usart.h>
extern uint8 KeyBoard_Tos_TaskID;
void VisualCOM_Writes(char *Buf,uint16 Length)
{
	uint16 i=0,Count;
	while(Length)
	{
		while(count_in)Tos_TaskDelay(1);
		if(Length>=64){Length-=64;UserToPMABufferCopy((uint8 *)&Buf[i], ENDP1_TXADDR, 64);Count=64;i+=64;}
		else if(Length<64){UserToPMABufferCopy((uint8 *)&Buf[i], ENDP1_TXADDR, Length);Count=Length;Length=0;}
		SetEPTxCount(ENDP1, Count);
		SetEPTxValid(ENDP1);
	}
}
uint16 VisualCOM_WriteLine(char *fmt,...)
{
	va_list ap;
	uint16 Length;
  char *Buf;
	Buf=Mema_Malloc(1024);
  va_start(ap,fmt);
  vsprintf(Buf,fmt,ap);
  va_end(ap);
	Length=GetTextLength(Buf);
	VisualCOM_Writes((char *)&Buf[0],Length);
	Mema_Free(Buf);
	return Length;
}
uint16 VisualCOM_Reads(uint8 *Buf)
{
		uint8 Length;
		if ((count_out != 0) && (bDeviceState == CONFIGURED))
    {
			BufferCoppy(buffer_out,Buf,count_out);
			Length=count_out;
      count_out = 0;
			return Length;
    }
		return 0;
}

void Task_GetKeyVirtual(void *Tags)
{
	uint16 Length;
	uint8 Buf[100];
	MGUI_KeyMsg KeyMsg;
	while(1)
		
	{
		Tos_TaskDelay(10);
		ClearWithBuffer(&Buf[0],100);
		Length=VisualCOM_Reads(&Buf[0]);
		if(Length)if(Buf[0])
		{
			KeyMsg.KeyValue=Buf[0];
			Tos_TaskPutQmsg(KeyBoard_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
		}
	}
}
