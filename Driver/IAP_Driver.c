#include <Include.h>
#include <AppConfig.h>
#include <CheakOut.h>
#include <IAP_Driver.h>
#include <AppConfig.h>

pFunction Jump_To_Application;
uint32_t JumpAddress;
EnValue enValue;
void Application_Run()
{
	JumpAddress = *(volatile u32*) (ApplicationAddress + 4); 
	Jump_To_Application = (pFunction) JumpAddress;
	Jump_To_Application();
}
u32 FLASH_WriteBank(u32 addr,u8 *pData,  u16 size)
{
    vu16 *pDataTemp = (vu16 *)pData;
    vu32 temp = addr;
    for (; temp < (addr + size); pDataTemp++, temp += 2)
    {
        FLASH_ProgramHalfWord(temp, *pDataTemp);
        if (*pDataTemp != *(vu16 *)temp)
        {
            return False;
        }
    }

    return True;
}

void FLASH_ProgramDone(void)
{
    FLASH_Lock();
}
void FLASH_ProgramStart(void(*Erase)(uint32 Addr),uint32 Addr,uint32 ApplicationSize)
{
	u32 NbrOfPage = 0;
	u32 EraseCounter = 0;
	FLASH_Unlock();
	NbrOfPage=ApplicationSize/PAGE_SIZE;
	if(ApplicationSize%PAGE_SIZE>0)NbrOfPage++;
	for (; (EraseCounter < NbrOfPage); EraseCounter++)
	{
	Erase(Addr + (PAGE_SIZE * EraseCounter));
	}
}
/**************************************************************************************
 Func: 存储环境变量
 Time: 2016-04-16
 Ver.:	1.0  	
 Note: 
**************************************************************************************/
void Save_EnvConfig()
{
	enValue.CHS.Size=sizeof(enValue);
	Save_VoidConfig(Flash_WriteData,ENVALUE_ADDR_BASE,&enValue,0);
}

/**************************************************************************************
 Func: 获取环境变量
 Time: 2016-04-16
 Ver.:	1.0  	
 Note: 
**************************************************************************************/
BOOL Load_EnvConfig()
{
	return Load_VoidConfig(Flash_ReadData,ENVALUE_ADDR_BASE,&enValue,0);
}
void Frist_CheckEnv()
{
	if((enValue.UpdateApp>1)&(enValue.UpdateOver>1))
	{
		BufferFill(&enValue,0x00,sizeof(enValue));
		Save_EnvConfig();
	}
}
void Flash_Coppy(void(*Load)(uint32 Addr,void *Data,uint16 Size),uint32 S,void(*Save)(uint32 Addr,void *Data,uint16 Size),uint32 D,uint32 Length)
{
	uint16 i=0,BufSize=256;
	uint8 *Buf;
	Buf=Mema_Malloc(BufSize);
	while(Length/BufSize)
	{
		Load(S+(uint32)i*BufSize,&Buf[0],BufSize);
		Save(D+(uint32)i*BufSize,&Buf[0],BufSize);
		i++;Length-=BufSize;
	}
	if(Length%BufSize)
	{
		Load(S+(uint32)i*BufSize,&Buf[0],Length%BufSize);
		Save(D+(uint32)i*BufSize,&Buf[0],Length%BufSize);
	}
	Mema_Free(Buf);
}

void Flash_Printf(void(*Load)(uint32 Addr,void *Data,uint16 Size),uint32 Addr)
{
	
	uint16 i,BufSize=1024;
	uint8 *TBuf;
	TBuf=Mema_Malloc(BufSize);
	Load(Addr,&TBuf[0],BufSize);
	for(i=0;i<BufSize;i++)
	{
		USART_WriteLine(USART1,"[%d]%02X ",i,TBuf[i]);
	}
	Mema_Free(TBuf);
}
