#include <Include.h>
#include <..\USER\Prj_CAMERAMRC\UsartCmd.h>
uint16 AngleXY[2];
uint32 BufToUint32(uint8 *D)
{
	uint8 n=4;
	uint64_t Data=0;
	while(n--){Data<<=8;Data|=*D++;}
	return Data;
}
uint8 BufFromUint32(uint8 *D,uint32 Data)
{
	uint8 n=4;
	while(n--){(*D++)=(Data>>24);Data<<=8;}
	return 4;
}
void PD_SendAngle()
{
//	uint8 Buf[10],i=0;
//	Buf[i++]=0xff;
//	Buf[i++]=0x00;
//	i+=SetWordToBytes(&Buf[i],AngleXY[0],0);
//	i+=SetWordToBytes(&Buf[i],AngleXY[1],0);
//	Buf[i++]=Buf[1]+Buf[2]+Buf[3]+Buf[4]+Buf[5];
//	USART1_WriteDatas(&Buf[0],i);
	Tos_TaskDelay(10);
}
uint8 PD_Create(uint8 *Buf,uint8 Address,uint32 Data)
{
	uint8 i=0;
	Buf[i++]=0xff;
	Buf[i++]=Address;
	i+=BufFromUint32(&Buf[i],Data);
	Buf[i++]=Buf[1]+Buf[2]+Buf[3]+Buf[4]+Buf[5];
	return i;
}
const PELCO_DStruct PDConst[]=
{
	{0x01000000,(void *)&PD_SendAngle,VT_Task,Null},
  {0x00},
};

BOOL Pelco_Export(uint8 *Buf,uint32 *Ref)
{
	uint8 i=0,n;
	uint8 Temp=0x00;
	if(Buf[i++]!=0xff)return False;
	if(Buf[i++]!=0x00)return False;
	for(n=1;n<6;n++)Temp+=Buf[n];
	if(Temp!=Buf[6])return False;
	*Ref=BufToUint32(&Buf[i]);
	return True;
}
void EnterTask(const PELCO_DStruct *Menu,uint8 *Addr)
{
typedef   void (*TaskFunc)(uint8 d);
	switch(Menu->Type)
	{
			case VT_Task:if(Menu->Value)((Func)Menu->Value)();break;  
		  case VT_Str:ClearWithBuffer(Menu->Value,Menu->Note);StringCoppy(Addr,Menu->Value);break;  
			case VT_Float:*(float *)(Menu->Value)=GetStringToNumber(Addr);break;
		  case VT_Uint8:*(uint8 *)(Menu->Value)=GetStringToNumber(Addr);break;
			case VT_TaskUint8:if(Menu->Value)((TaskFunc)Menu->Value)(Menu->Note);break;  
	}
}

void Usart_Cmd(void *Tags)
{
	uint8 i;
	uint32 Data;
	uint8 Buf[40];
	DeBug_OutSet(DeBug_USART1|DeBug_USART2);//调试信息通过串口打印
	DeBug_Get();
	while(1)
	{
		Tos_TaskDelay(100);
		RS485_OFF;
		i=PD_Create(&Buf[0],0x02,0x01000000);
		USART2_WriteDatas(&Buf[0],i);
		Tos_TaskDelay(4);
		RS485_ON;
		i=USART2_ReadDatas(Null,50,100);
		if(i)
		{
			if(Pelco_Export(&USART2_RX_Buffer[0],&Data))
			{
			AngleXY[0]=GetWordFromBytes(&USART2_RX_Buffer[2],0);
			AngleXY[1]=GetWordFromBytes(&USART2_RX_Buffer[4],0);
				RS485_OFF;
			LED2_OFF;
			Tos_TaskDelay(10);
			LED2_ON;
			Tos_TaskDelay(10);
			}
		}
	}
}
