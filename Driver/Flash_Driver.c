#include <Include.h>
#include <flash_driver.h>
#define FLASH_ADDR_BASE		0x08000000
uint8 DeviceId_FLASH=DeviceNull;
uint8 FlashAddr_Index;
/**************************************************************************************
 Func: 从flash中读取数据
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
void FLASH_ReadDatas(uint16 Addr,uint16 *Buffer,uint8 Length)
{		
	uint16 *FlashAddr;
	FlashAddr=(uint16 *)(FLASH_ADDR_BASE+FLASH_PARAM_PAGE*1024+Addr);
	while(Length--)*Buffer++=*FlashAddr++;
}

/**************************************************************************************
 Func: 等待空闲状态
 Time: 2014-6-12
 Ver.: V1.0
 Note;
**************************************************************************************/
uint8 FLASH_WaitBusy(void)
{
	uint16 T=1000;
	do{
		if(!(FLASH->SR&FLASH_SR_BSY))return 0;
	}while(--T);
	return 0xFF;
}
void Delayy(uint32 i)
{
	uint16 u;
	while(i--)for(u=0;u<100;u++);
}
uint8 _FLASH_WriteDatas(uint16 Addr,uint16 *Buffer,uint16 Length)
{
	FLASH_Status Status;
	DIS_INT;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	Status=FLASH_ErasePage(FLASH_ADDR_BASE+FLASH_PARAM_PAGE*1024);
	Delayy(1000);
	while(Length--)
	{
		Status=FLASH_ProgramHalfWord((FLASH_ADDR_BASE+FLASH_PARAM_PAGE*1024+Addr++),*Buffer++);  //flash  为一个字节存储
		FLASH_WaitBusy();
		if(Status>0)goto OpError;
		Delayy(1000);
	}
  FLASH_Lock();EN_INT;return 0;
OpError:FLASH_Lock();EN_INT;return Status;
}

/**************************************************************************************
 Func: 保存配置文件
 Time: 2012-8-2	  2012-8-3
 Ver.: V1.0		  V1.1		  	
 Note: 			  新增加错误返回处理
 				  return 0/OK   >0/ERROR
**************************************************************************************/
uint8 FLASH_WriteDatas(uint16 Addr,uint16 *Buffer,uint16 Length)
{
	uint8 R;
	uint16 TM,*FlashAddr;
	uint32 T;
	Tos_TaskGetDev(DeviceId_FLASH,0);
	FlashAddr=(uint16 *)(FLASH_ADDR_BASE+FLASH_PARAM_PAGE*1024+Addr);
	if(Length==0)return 1;
	if(FLASH->CR&FLASH_CR_LOCK){
		FLASH->KEYR=0x45670123;
		FLASH->KEYR=0xCDEF89AB;
	}
	//擦除flash	
	R=FLASH_WaitBusy();	if(R>0)goto OpError;
	FLASH->CR|=FLASH_CR_PER;
	FLASH->AR=FLASH_ADDR_BASE+FLASH_PARAM_PAGE*1024;
	FLASH->CR|=FLASH_CR_STRT;
	R=FLASH_WaitBusy();	if(R>0)goto OpError;
	TM=0;
	do{
		T=FLASH->SR;
		TM++;
		if(T&FLASH_SR_EOP)break;
	}while(TM<0xFFFF);
	if(!(T&FLASH_SR_EOP))goto OpError;
	FLASH->SR|=FLASH_SR_EOP;
	FLASH->CR&=(~(FLASH_CR_STRT|FLASH_CR_PER));
	while(Length--)
	{
		//写过程
		R=FLASH_WaitBusy();	if(R>0)goto OpError;
		FLASH->CR|=FLASH_CR_PG;
		*FlashAddr++=*Buffer++;
		R=FLASH_WaitBusy();	if(R>0)goto OpError;
	    FLASH->CR&=~FLASH_CR_PG;
		T=FLASH->SR;
		if(!(T&FLASH_SR_EOP))goto OpError;
		FLASH->SR|=FLASH_SR_EOP;
	}
	FLASH->CR|=FLASH_CR_LOCK;
	Tos_TaskDropDev(DeviceId_FLASH);
	return 0;
 OpError:			//操作错误
 	FLASH->CR|=FLASH_CR_LOCK;
	Tos_TaskDropDev(DeviceId_FLASH);
	return 0xFF;
}
/**************************************************************************************
 Func: TOS设备管理的标准读接口
 Time: 2014-11-13
 Ver.:	  	
 Note: 
**************************************************************************************/
void Flash_Read(void *Addr,void *Buffer,void *Length)
{
	uint16 *P_Buffer=(uint16 *)Buffer;
	uint16 *P_Addr=(uint16 *)Addr;
	uint16 *P_Length=(uint16 *)Length;
	FLASH_ReadDatas(*P_Addr,P_Buffer,*P_Length);
}
/**************************************************************************************
 Func: TOS设备管理的标准写接口
 Time: 2014-11-13
 Ver.:	  	
 Note: 
**************************************************************************************/
void Flash_Write(void *Addr,void *Buffer,void *Length)
{
	uint16 *P_Buffer=(uint16 *)Buffer;
	uint16 *P_Addr=(uint16 *)Addr;
	uint16 *P_Length=(uint16 *)Length;
	FLASH_WriteDatas(*P_Addr,P_Buffer,*P_Length);
}









