#include <Include.h>
#include <..\USER\Prj_RS485ANGLE\UsartCmd.h>
uint32 BufToUint32(uint8 *D)
{
	uint8 n=4;
	uint64_t Data=0;
	while(n--){Data<<=8;Data|=*D++;}
	return Data;
}
void PD_SendAngle()
{
	uint8 Buf[10],i=0;
	Buf[i++]=0xff;
	Buf[i++]=0x00;
	i+=SetWordToBytes(&Buf[i],AngleXY[0],0);
	i+=SetWordToBytes(&Buf[i],AngleXY[1],0);
	Buf[i++]=Buf[1]+Buf[2]+Buf[3]+Buf[4]+Buf[5];
	Tos_TaskDelay(1);
	USART1_WriteDatas(&Buf[0],i);
	Tos_TaskDelay(10);
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
	if(Buf[i++]!=0x02)return False;
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
	uint8 x,i;
	uint32 Data;
	DeBug_OutSet(DeBug_USART1|DeBug_LCD);//调试信息通过串口打印
	DeBug_Get();
	while(1)
	{
		
		RS485_ON;
	  x=USART1_ReadDatas(Null,50,0);
	 if(x)
		{
			LED2_OFF;
			RS485_OFF;
			if(Pelco_Export(&USART1_RX_Buffer[0],&Data))
			{
			i=0;
			while(PDConst[i].Key)
			{
				if(PDConst[i].Key==Data)EnterTask(&PDConst[i],&USART1_RX_Buffer[8]);i++;
			}
		  }
		  LED2_ON;
		}
		RS485_ON;
		Tos_TaskDelay(10);
	}
}
