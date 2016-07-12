#include <Include.h>
#include <Cs1168_Driver.h>

#define CS1168_CLK_SET 	GPIOA->BSRR=S2	
#define CS1168_CLK_CLR 	GPIOA->BRR=S2
#define CS1168_DAT 	   (GPIOA->IDR&S1)

int16 CS1168_ADC;
/*******************************************************************************
Func:CS1168专用延时
Date:2014-10-9
Note:
*******************************************************************************/
void CS1168_Delay(uint8 i)
{
	i*=20;
	while(i--);
}
/*******************************************************************************
Func:CS1168端口初始化
Date:2014-10-9
Note:
*******************************************************************************/
void CS1168_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*******************************************************************************
FuncCS11685初始化
Date:2014-10-9
Note:
*******************************************************************************/
uint8 IniCS1168()
{
	u16	count;
	count=65535;
	CS1168_GPIO_Init();
	CS1168_CLK_CLR;
	CS1168_Delay(1);
	CS1168_CLK_SET;
	CS1168_Delay(1);
	while(!CS1168_DAT)
	{					
		count--;
		if(count==0)
			return	1;
	}
	count=65535;
	while(CS1168_DAT)
	{
		count--;
		if(count==0)
			return	1;			
	}
	CS1168_CLK_CLR;   
	return	0; 
}
/*******************************************************************************
Func:CS1168读取数据
Date:2014-10-9
Note:
*******************************************************************************/
uint8 CS1168_ReceiveByte(void)
{
	u8 i;
	u8 rdata = 0;
	
	for(i = 8; i!=0; i--)
	{
		rdata <<= 1;
		CS1168_CLK_CLR;
		CS1168_Delay(1);
		CS1168_CLK_SET;
		CS1168_Delay(1);
		if(CS1168_DAT)
		{
			rdata += 1;
		}
	}        
	return(rdata);
}
/*******************************************************************************
Func:CS1168读取ADC
Date:2014-10-9
Note:
*******************************************************************************/
int16 CS1168_ReadADC()
{
	uint8  i;
	uint32 Count=1000;
	static uint16 dat;
	uint8 Buf[3];	
	while(CS1168_DAT&Count--)
	{
		//return	1;
	}
	if(Count<1)return dat;
	CS1168_CLK_CLR;
	CS1168_Delay(1);
	CS1168_CLK_SET;					//第一个SCLK 信号为响应信号
	CS1168_Delay(1);
	for(i = 0; i < 2; i++)
	{
		Buf[i] = CS1168_ReceiveByte();
	}
	CS1168_ReceiveByte();
	CS1168_CLK_CLR;				  
	dat=Buf[0]*256+Buf[1];
	return	(int16)dat; 
}
