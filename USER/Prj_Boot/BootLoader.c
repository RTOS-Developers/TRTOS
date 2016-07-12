#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <..\USER\Prj_Boot\BootLoader.h>
#include <..\Driver\SZZigBee_Driver.h>
#include <Sensor_Driver.h>
#include <IAP_Driver.h>
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint8 WaitDownLoadCount=1;
EnValue enValue;
/********************************************************************************
 Func: 系统直接跳到指定地址运行
 Time: 2014-4-7
 Ver.: V2.0
 Note; 从bootloader 跳转到用户应用程序
********************************************************************************/
void Application_Run(uint32_t App_Addr)
{
	  JumpAddress = *(volatile u32*) (App_Addr + 4); 
    Jump_To_Application = (pFunction) JumpAddress;
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x5000);
    Jump_To_Application();
}
static u32 IAP_PagesMask(vu32 Size)
{
    u32 pagenumber = 0x0;
    u32 size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;
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

void FLASH_ProgramStart(uint32 ApplicationSize)
{
    FLASH_Status FLASHStatus = FLASH_COMPLETE;
    u32 NbrOfPage = 0;
    vu32 EraseCounter = 0;
    FLASH_Unlock();
    NbrOfPage = IAP_PagesMask(ApplicationSize);
    for (; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(ApplicationAddress + (PAGE_SIZE * EraseCounter));
    }
}
void IAP_ReadDatas(uintbus Addr,uint16 *Buffer,uint16 Length)
{		
	uint16 *FlashAddr;
	FlashAddr=(uint16 *)(Addr);
	while(Length--)*Buffer++=*FlashAddr++;
}
uint8 GetCodeStr18(uint8 *Buf)
{
	uint32 CpuID[3];
	uint8 i,c=0;
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[2]=*(vu32*)(0x1ffff7ec);
	CpuID[1]=*(vu32*)(0x1ffff7f0);
	for(i=0;i<3;i++)
	{
		while(CpuID[i])
		{
			Buf[c++]=ToHex64(CpuID[i]%62);
			CpuID[i]/=62;
		}
	}
	return c;
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
	if(Load_EnvConfig())return;
	if(Load_EnvConfig())return;
	enValue.UpdateApp=True;
	Save_EnvConfig();
}
void DownLoad_Command(void *Tags)
{
	uint16 Length,DataStart,z,i=0;
	char Buf[200];
	ModBus_Message ModBusMsg;
	USART_TypeDef *USARTx=(USART_TypeDef *)Tags;
	uint8 UsartCH=USART_GetIndex(USARTx);
	ModBus_Create(&ModBusMsg,0x01,0x05,WorkMode_Slave,MBCM_Crc);
	while(1)
	{
		ClearWithBuffer(USART_RX_Buffer[UsartCH],USART_RX_BUFFER_SIZE);
		Length=USART_ReadDatas(USARTx,Null,USART_RX_BUFFER_SIZE,0);
		Length=ModBus_Expend(USART_RX_Buffer[UsartCH],Length,&ModBusMsg);
		DataStart=Length;
		if(ModBusMsg.ErrorFlag==ModBus_Ok)
		{
			switch(ModBusMsg.MsgFlag)
			{
				case SCFF_Write1024Byte:
					WaitDownLoadCount=60;
					if(USART_RX_Buffer[UsartCH][DataStart]==i)
					{
						FLASH_WriteBank(ApplicationAddress+FLASH_PAGESIZE*USART_RX_Buffer[UsartCH][DataStart],&USART_RX_Buffer[UsartCH][DataStart+1],1024);
						if(enValue.UpdateApp)Length=GetStringByFmt((char *)&Buf[0],"Page[%d] OK\r\n",i++);
					else Length=GetStringByFmt((char *)&Buf[0],"Not Erase[%d] Fail\r\n",i);
					}else
					{
						if(enValue.UpdateApp)Length=GetStringByFmt((char *)&Buf[0],"Page[%d] OK\r\n",i);
						else Length=GetStringByFmt((char *)&Buf[0],"Not Erase[%d] Fail\r\n",i);
					}
				break;
				case SCFF_EraseFlase:
					i=0;
					enValue.UpdateApp=True;
					Save_EnvConfig();
					WaitDownLoadCount=60;
					FLASH_ProgramStart(Uint32FromByte(&USART_RX_Buffer[UsartCH][DataStart]));
					z=Uint32FromByte(&USART_RX_Buffer[UsartCH][DataStart])/1024;
					Length=GetStringByFmt((char *)&Buf[0],"Erase[%d]Kb OK\r\n",z);
					break;
				case SCFF_GotoApplication:
					if((i-1)!=z)break;
					FLASH_ProgramDone();
					enValue.UpdateApp=False;
					Save_EnvConfig();
					Application_Run(ApplicationAddress);
				  break;
				case SCFF_GoToBootLoader:
					WaitDownLoadCount=60;
					Length=GetStringByFmt(&Buf[0],"Find ");
					Length+=GetCodeStr18((uint8 *)&Buf[Length]);
					Length+=GetStringByFmt(&Buf[Length]," OK \r\nTRTOS Boot Ver2.0\r\n");
					break;
				default: Length=StringCoppy("固件错误",&Buf[0]);
				break;
			}
			ModBus_Create(&ModBusMsg,USART_RX_Buffer[UsartCH][2],0x05,WorkMode_Slave,MBCM_Crc);
			Length=ModBus_CreateMsg(USART_TX_Buffer[UsartCH],&ModBusMsg,0x01,0x01,0x01,(uint8 *)&Buf[0],Length);
			USART_WriteDatas(USARTx,USART_TX_Buffer[UsartCH],Length);
		}else if(ModBusMsg.ErrorFlag==ModBus_Broad)
		{
			
		}
		else if(ModBusMsg.ErrorFlag==ModBus_FlagError)
		{
			DeBug("%s",USART_RX_Buffer[UsartCH],Infor_Error);
		}else DeBug("%d",ModBusMsg.ErrorFlag,Infor_Infor);
	}
}
