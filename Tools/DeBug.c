#include <Include.h>
#include <Tos_Include.h>
#include <Usart_Driver.h>
#include <text.h>
#include <DeBug.h>

DeBug_CallBack DeBug_CB;

uint8 DeBug_EN=0xff;//哪些线程可以使用打印
uint8 DeBug_Out=0x00;//通过哪些方式打印
uint8 DeBug_Type=0xff;//调试类型

/**************************************************************************************
 Func: 调试信息管道独享
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void DeBug_Get()
{
	DeBug_EN=Tos_TaskCurrent;
}
/**************************************************************************************
 Func: 调试信息管道
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void DeBug_Drop()
{
	DeBug_EN=0xff;
}
/**************************************************************************************
 Func: 控制抑制调试输出
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void DeBug_OutSet(uint8 OutSet)
{
	DeBug_Out=OutSet;
}
/**************************************************************************************
 Func: 选择输出调试消息内容
 Time: 2014-6-19
 Ver.: V1.0
 Note;
**************************************************************************************/
void DeBug_SetType(uint8 Type)
{
	DeBug_Type=Type;
}
void DeBug_SetCallBack(DeBug_CallBack DBCB)
{
	DeBug_CB=DBCB;
}
/**************************************************************************************
 Func: 寻找统计记录类型
 Time: 2015-3-23
 Ver.: V1.0
 Note;这么多年才发现这个问题
**************************************************************************************/
uint16 GetChar_List(uint8 *Str1,uint8 *List)
{
	uint16 Count=0,c=0;
	BOOL En=False;
	uint8 *Str=Str1;
	while(*Str)
	{
		if(*(Str)=='%'){En=True;c++;}
		if(En==True)
		{
			if(*(Str)=='d'){List[Count++]=0x01;En=False;}
			if(*(Str)=='f'){List[Count++]=0x02;En=False;}
			if(*(Str)=='s'){List[Count++]=0x03;En=False;}
			if(*(Str)=='c'){List[Count++]=0x04;En=False;}
		}
		Str++;
	}
	return c;
}

/**************************************************************************************
 Func: 调试接口
 Time: 2014-6-12
 Ver.: V1.0
 Note;日到狗了，缓冲区太小导致的重启不止一次
**************************************************************************************/
void DeBug(char *fmt,...)
{
	
#ifdef COMPILE_MODE_DEBUG
	va_list ap;
	uint8 i=0;
	int Temp=1;
	int TempOk=0;
  char *Buf;
	uint8 Buf1[20];
	Buf=Mema_Malloc(1024);
	if((DeBug_EN==0xff)|(DeBug_EN==Tos_TaskCurrent))
	{
	
  va_start(ap,fmt);
  vsprintf(Buf,fmt,ap);
  va_end(ap);
	va_start(ap,fmt);
	Temp=GetChar_List((string)fmt,&Buf1[0]);
	while(Temp--)
		{
			switch(Buf1[i++])
			{
				case 0x01:TempOk=va_arg(ap,int);break;
				case 0x02:TempOk=va_arg(ap,double);break;
				case 0x03:TempOk=va_arg(ap,int);break;
				default:TempOk=va_arg(ap,int);break;
			}
		}
	TempOk=va_arg(ap,int);
	va_end(ap);
	if((DeBug_Type==Infor_All)|(DeBug_Type>=TempOk))
	{
		StringEndingAppend("\r\n",&Buf[0]);
		if(DeBug_Out&DeBug_USART1)if(Device_Ok(DeviceId_USART_TX[0]))USART_WriteDatas(USART1,(uint8 *)&Buf[0],GetTextLength(Buf));
		if(DeBug_Out&DeBug_USART2)if(Device_Ok(DeviceId_USART_TX[1]))USART_WriteDatas(USART2,(uint8 *)&Buf[0],GetTextLength(Buf));
		if(DeBug_CB)DeBug_CB((uint8 *)&Buf[0],GetTextLength(Buf));
	}
	Mema_Free(Buf);
	}
#endif
}
