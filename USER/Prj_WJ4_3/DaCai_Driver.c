#include <..\USER\Prj_WJ4_3\DaCai_Driver.h>


RunValueStruct Run_Value;


uint8 DaCai_FillHead(uint8 *Buf,uint8 Type,uint16 ScreenID,uint16 ControlID)
{
	uint8 i=0;
	Buf[i++]=0xEE;
	Buf[i++]=0xB1;
	Buf[i++]=Type;
	Buf[i++]=ScreenID/0xff;
	Buf[i++]=ScreenID&0xff;
	Buf[i++]=ControlID/0xff;
	Buf[i++]=ControlID&0xff;
	return i;
	
}
uint8 DaCai_FillEnd(uint8 *Buf,uint8 OffSet)
{
	uint8 i=0;
	Buf[i++]=0xFF;
	Buf[i++]=0xFC;
	Buf[i++]=0xFF;
	Buf[i++]=0xFF;
	return i;
}
uint16 String_Coppy(void *S,void *Out,uint8 Length)
{
	uint16 i=0;
	INT8U *PS,*PD;
	PS=(INT8U *)S;
	PD=(INT8U *)Out;
	while (*PS)
	{
		(*PD++)=(*PS++);
		i++;
		if(Length--==0)return i;
	}
	*PD=0;
	return i;
}
void DaCai_SetTextBox(uint16 ScreenID,uint16 ControlID,uint8 *String,uint8 Length)
{
	uint8 i=0;
	uint8 Buf[50];
	BufferFill(&Buf[0],0,sizeof(Buf));
	i=DaCai_FillHead(&Buf[0],0x10,ScreenID,ControlID);
	i+=BufferCoppy(&String[0],&Buf[i],Length);//Buf[i++]=0;
	i+=DaCai_FillEnd(&Buf[i],i);
	USART1_DMAWriteDatas(&Buf[0],i);
	Tos_TaskDelay(10);
}
void DaCai_TextBox(uint16 ScreenID,uint16 ControlID,char *fmt,...)
{
  va_list ap;
	char String[256];
	va_start(ap,fmt);
	vsprintf(String,fmt,ap);
	DaCai_SetTextBox(ScreenID,ControlID,(uint8 *)String,GetTextLength(String));
	va_end(ap);
	
}
void DaCai_SetPage(uint8 Page)
{
	uint8 i=0,Buffer[18];
	Buffer[i++]=0xee;
	Buffer[i++]=0xB1;
	Buffer[i++]=0x00;
	Buffer[i++]=0x00;
	Buffer[i++]=Page;
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFC;
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFF;
	USART1_DMAWriteDatas(&Buffer[0],i);
}
void DaCai_SetSpk()
{
	//EE 61 0A 27 A8 FF FC FF FF 
	uint8 i=0,Buffer[18];
	Buffer[i++]=0xee;
	Buffer[i++]=0x61;
	Buffer[i++]=0x0a;
	Buffer[i++]=0x27;
	Buffer[i++]=0xa8;
	Buffer[i++]=0xff;
	Buffer[i++]=0xFC;
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFF;
	USART1_DMAWriteDatas(&Buffer[0],i);
}
void DaCai_SetDateTime(Date_Time *DateTime)
{
	uint8 i=0,Buffer[18];
	Buffer[i++]=0xee;
	Buffer[i++]=0x81;
	Buffer[i++]=Decimal_Bcd(DateTime->Sec);
	Buffer[i++]=Decimal_Bcd(DateTime->Min);
	Buffer[i++]=Decimal_Bcd(DateTime->Hour);
	Buffer[i++]=Decimal_Bcd(DateTime->Day);
	Buffer[i++]=0x00;
	Buffer[i++]=Decimal_Bcd(DateTime->Mon);
	Buffer[i++]=Decimal_Bcd(DateTime->Year);
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFC;
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFF;
	USART1_DMAWriteDatas(&Buffer[0],i);
}
void DaCai_SetDateLight(uint8 Light)
{
	uint8 i=0,Buffer[18];
	if(Light>210)Light=210;
	Buffer[i++]=0xee;
	Buffer[i++]=0x60;
  Buffer[i++]=Light;
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFC;
	Buffer[i++]=0xFF;
	Buffer[i++]=0xFF;
	USART1_DMAWriteDatas(&Buffer[0],i);
}
void DaCai_ShowWidget(uint16 ScreenID,uint16 ControlID,uint8 En)
{
	uint8 i=0;
	uint8 Buf[50];
	BufferFill(&Buf[0],0,sizeof(Buf));
	i=DaCai_FillHead(&Buf[0],0x10,ScreenID,ControlID);
	Buf[i++]=En;
	i+=DaCai_FillEnd(&Buf[i],i);
	USART1_DMAWriteDatas(&Buf[0],i);
	Tos_TaskDelay(10);
}
void DaCai_Printf(char *fmt,...)
{
  va_list ap;
	char String[256];
	va_start(ap,fmt);
	vsprintf(String,fmt,ap);
	DaCai_SetTextBox(0x00,0x11,(uint8 *)String,GetTextLength(String));
	va_end(ap);
	
}
