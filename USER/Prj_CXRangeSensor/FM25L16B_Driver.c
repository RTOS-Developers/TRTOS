#include <..\USER\Prj_CXRangeSensor\FM25L16B_Driver.h>

uint8 DeviceId_FRAM=DeviceNull;
void FM_Delay(uint8 i)
{
	//while(i--);
}

void Fram_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ÍÆÍìÊä³ö
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void FramInit(void)
{  
	Fram_GPIO_Init();
	FM_WP_L;
  FM_CS_H;
  FM_SCK_L;
}

uint8 SendReadByte(uint8 data)
{
	uint8 i;
	FM_SCK_L;
	FM_Delay(10);
	for (i=0; i<8; i++)
	{
		if(data&0x80)FM_MOSI_H;else FM_MOSI_L;
		FM_Delay(10);
		data	<<=	1;
		FM_SCK_H;
		FM_Delay(10);
		data	|= FM_MISO;
		FM_SCK_L;
		FM_Delay(10);
  }
	return(data);
}
void FMWriteMemory(uint16 add, uint8	*dat, uint16 len)
{
  uint16 i;
  FM_CS_L;
  SendReadByte(WREN);
  FM_CS_H;
  FM_CS_L;
  SendReadByte(WRITE);
  SendReadByte((uint8)(add>>8));
  SendReadByte((uint8)(add));
  for(i	=	0; i	< len; i++)SendReadByte(*(dat	+ i));  
  FM_CS_H;
}
void FMReadMemory(uint16 add, uint8	*dat, uint16 len)
{
  uint16 i;
  FM_CS_L;
  SendReadByte(READ);
  SendReadByte((uint8)(add>>8));
  SendReadByte((uint8)(add));
  for(i	=	0;i<len;i++){*(dat	+ i)	= SendReadByte(0xaa);}
  FM_CS_H;   
}
void DeviceMount_Fram()
{
	DeviceId_FRAM=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_FRAM].DeviceId=DeviceId_FRAM;
	Tos_Device_Tab[DeviceId_FRAM].DeviceName="ÌúµçFRAM";
	Tos_Device_Tab[DeviceId_FRAM].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_FRAM].DeviceVirtue=ReadOnly|WriteOnly|CharDerice;
	Tos_Device_Tab[DeviceId_FRAM].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_FRAM].Init=FramInit;
	Tos_Device_Tab[DeviceId_FRAM].Write=Null;
	Tos_Device_Tab[DeviceId_FRAM].Read=Null;
	Tos_Device_Tab[DeviceId_FRAM].Exit=Null;
}



