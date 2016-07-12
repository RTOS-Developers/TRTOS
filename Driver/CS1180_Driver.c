#include <Include.h>
#include <Tools_Include.h>
#include <CS1180_Driver.h>
uint16 CS1180_ADC1;
void CS1180_Delay(uint32 i)
{
	i*=8;
	while(i--);
}
void CS1180_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void CS1180_SPI_clock(void) 
{ 
	CS1180_CLK_CLR;
	CS1180_Delay(1); 
	CS1180_CLK_SET;
	CS1180_Delay(1);  
} 
void CS1180_SPI_Send(unsigned char SPI_data) 
{ 
	unsigned char bitCount; 
	for (bitCount=0; bitCount<8; bitCount++) 
	{ 
	if(SPI_data & 0x80)CS1180_SDI_SET;else CS1180_SDI_CLR;
	SPI_data <<= 1; 
	CS1180_SPI_clock(); 
	}
} 
uint8 CS1180_SPI_Receive(void) 
{ 
	unsigned char bitCount; 
	unsigned char SPI_data=0; 
	for (bitCount=0; bitCount<8; bitCount++) 
	{ 
	SPI_data *= 2; 
	CS1180_CLK_SET;
	CS1180_Delay(1);
	CS1180_CLK_CLR; 
	CS1180_Delay(1);
	if (CS1180_SDO_DAT) 
	{ 
	SPI_data++; 
	} 
	} 
	return (SPI_data); 
} 
uint32 CS1180_ReadADC(void) 
{ 
	uint16 Temp;
	unsigned char i; 
	uint8 Buf[3];
	CS1180_CS_CLR;
	while (CS1180_DRDY_DAT)                            //当DRDY为高时，不读取数据 
	CS1180_Delay(1);
	CS1180_SPI_Send(COMM_RDATA); 
	CS1180_Delay(50);                           //在发送读指令后，延时100us才能接收数据 
	for (i=0;i<3;i++) 
	{ 
	Buf[i] = CS1180_SPI_Receive(); 
	} 
	CS1180_CS_SET;
	Temp=Buf[0];
	Temp<<=8;
	Temp+=Buf[1];
	return ((int16)Temp+32768);
} 
void IniCS1180(void) 
{ 
	unsigned char m; 
  uint8 buf[16];
	CS1180_GPIO_Init();
	CS1180_CLK_SET;
	CS1180_SDI_SET; 
	CS1180_Delay(1); 
	CS1180_CLK_CLR; 
	Tos_TaskDelay(1); 
	CS1180_SPI_Send(COMM_RESET);                           //rst 1180 
	Tos_TaskDelay(10);
	CS1180_CLK_SET; 
	CS1180_Delay(100);    
	CS1180_CLK_CLR; 
	CS1180_Delay(1);
	CS1180_SPI_Send(0x50); 
	CS1180_SPI_Send(0x02);                                 //发送3个指令 
	CS1180_SPI_Send(0x07);                                 //0x00 pga = 7 
	CS1180_SPI_Send(0x01);                                 //0x01 此处必须为01 
	CS1180_SPI_Send(0x04);                                 //0x02 输出频率 15hz，参考电压5V 
	Tos_TaskDelay(10); 
	CS1180_CS_CLR;
	CS1180_Delay(1);  
	CS1180_SPI_Send(COMM_CALSELF); 
	CS1180_Delay(1); 
	CS1180_CS_SET;
	while (CS1180_DRDY_DAT);
	while (!CS1180_DRDY_DAT);
	Tos_TaskDelay(10); 
	while (CS1180_DRDY_DAT);                                    //扔调一次数据 ; 
	CS1180_CS_CLR;                                       //自校正 
	CS1180_Delay(1);                                       //做通讯检测用，主要是读取寄存器的值看是否与写入的值相符合 
	CS1180_SPI_Send(0x10); 
	CS1180_Delay(1);
	CS1180_SPI_Send(15); 
	CS1180_Delay(100); 
	for (m=0;m<16;m++) 
	{ 
		buf[m]=CS1180_SPI_Receive(); 
	}    
  CS1180_CS_SET;  
	Printf_Bytes(&buf[0],16);
} 
uint16 CS1180_ReadFiter()
{
	static uint32 Pool;
	static uint8 Index;
	uint16 Temp;
	Temp=FlowPoolFilter(&Pool,CS1180_ReadADC(),&Index,6);
	return Temp;
}


