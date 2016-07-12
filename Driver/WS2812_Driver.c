#include <WS2812_Driver.h>

void WS2812_Delay(uint16 u)
{
	while(u--);
}
void WS2812_Fill(uint32 *Color,uint8 Index,uint8 Mode)
{
	uint8 i;
	switch(Mode)
	{
		case 0:Color[Index]|=0xff0000;break;
		case 1:Color[Index]|=0x00ff00;break;
		case 2:Color[Index]|=0x0000ff;break;
		case 3:Color[Index]|=0x00ff00;Color[31-Index]|=0x0000ff;break;
		case 4:for(i=0;i<30;i++)Color[i]=0xff00ff;break;
		case 5:Color[Index]=0xaacc00;Color[Index+1]=0xaacc33;break;
		default:Color[Index]=0xffffff;break;
	}
}

void Taskfuck(void *Tags)
{
	uint32  Color[38];
	uint8 i,j,p=0;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	while(1)
	{
//		if(j++>30){j=0;p++;if(p>10)p=0;for(i=0;i<30;i++)Color[i]=0x000000;}
//		WS2812_Fill(Color,j,p);
//		WS2812_SendList(Color);
		Tos_TaskDelay(100);
	}
}

void WS2812_SendList(uint32 *D)
{
	uint16 x,y;
	uint32 Data;
	uint32 *P;
	P=D;
	for(x=0;x<30;x++)
	{
		Data=*P++;
		for(y=0;y<24;y++)
		{
			if(Data&S0)
			{
				GPIOA->BSRR=GPIO_Pin_5;
				WS2812_Delay(2);
				GPIOA->BRR=GPIO_Pin_5;
				WS2812_Delay(1);
			}else
			{
				GPIOA->BSRR=GPIO_Pin_5;
				WS2812_Delay(1);
				GPIOA->BRR=GPIO_Pin_5;
				WS2812_Delay(2);
			}
			Data>>=1;
		}
	}
	GPIOA->BRR=GPIO_Pin_5;
}
void WS2812_WriteDot(GPIO_TypeDef *GPIOx,uint16_t Port,uint32 *Color)
{
	uint16 x,y;
	uint32 Data;
	for(x=0;x<30;x++)
	{
		Data=0xffffff;//*P++;
		for(y=0;y<24;y++)
		{
			if(Data&S0)
			{
				GPIOx->BSRR=Port;
				WS2812_Delay(2);
				GPIOx->BRR=Port;
				WS2812_Delay(1);
			}else
			{
				GPIOx->BSRR=Port;
				WS2812_Delay(1);
				GPIOx->BRR=Port;
				WS2812_Delay(2);
			}
			Data>>=1;
		}
	}
}
