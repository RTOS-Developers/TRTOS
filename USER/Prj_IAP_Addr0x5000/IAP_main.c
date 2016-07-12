#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Usart_Driver.h>
#include <Tos_Mema.h>
#include <IAP_Driver.c>
#include <W25Q16_Driver.c>
int main1(void)
{
	uint32 i=0;
	RCC_Configuration();
	Mema_Init(MemBuf);
	SPI_FLASH_Init();
	Load_EnvConfig();
	Frist_CheckEnv();
	if(enValue.UpdateApp==True)
	{
		//在这里放置搬运程序
		enValue.UpdateApp=False;
		Save_EnvConfig();
	}
	if(enValue.UpdateOver==True)
	{
		//在这里放置回滚程序
		enValue.UpdateOver=False;
		Save_EnvConfig();
	}
	
	USART_Configuration(0);
	for(i=0;i<10000;i++)
	{
		FLASH_ProgramStart(SPI_FLASH_SectorErase,i*1024,1024);
		Flash_Coppy(Flash_ReadData,0,W25Q16_Write,i*1024,1024);
		Flash_Printf(W25Q16_Read,i*1024);
	}
	//Flash_Printf(Flash_ReadData,0);
	//Application_Run();
	while(1);
}


int main(void)
{
	uint32 i=0;
	RCC_Configuration();
	//Mema_Init(MemBuf);
	//USART_Configuration(0);
	//USART_WriteLine(USART1,"fdsafdsafsaf");
	Application_Run();
	while(1);
}
