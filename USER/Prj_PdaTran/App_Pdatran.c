#include <Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <App_Pdatran.h>
#ifdef BOARD_PDATRAN
char *String_Hello="Hello!";
uint16 HostAddr;
uint8 Pda_Status=0;
uint8 PDA_Buf[PDA_Buf_Size];
/****************************************************************************
Date:2014-11-12
Func:获取PDA配置参数
Note:
****************************************************************************/
void Load_Pdatran_Configs(uint8 Offset)
{
	FLASH_ReadDatas(Offset,(uint16 *)&HostAddr,sizeof(HostAddr)/2);
}
/****************************************************************************
Date:2014-11-12
Func:保存PDA参数
Note:
****************************************************************************/
void Save_Pdatran_Configs(uint8 Offset)
{
	FLASH_WriteDatas(Offset,(uint16 *)&HostAddr,sizeof(HostAddr)/2);
}
/****************************************************************************
Date:2014-11-12
Func:PDA转换
Note:
****************************************************************************/
void Task_PdaTran(void *Tags)
{
	uint8 Length;
	uint8 Offset;
	uint8 Buf[100];
	ModBus_Message ModBusMsg;
	uint8 SENSOR_CFG_OffSet=FlashConfigNull;
	for(Length=0;Length<100;Length++)PDA_Buf[Length]=Length;
	FlashGetConfigRoom(&SENSOR_CFG_OffSet,sizeof(HostAddr)/2);
	Load_Pdatran_Configs(SENSOR_CFG_OffSet);
	ModBus_Create(&ModBusMsg,0xfe,0x02,WorkMode_Slave,MBCM_Crc);
	while(1)
	{
		ClearWithBuffer(&USART2_RX_Buffer[0],100);
		Length=USART2_ReadDatas(Null,100,100);
		Length=ModBus_Expend(&USART2_RX_Buffer[0],Length,&ModBusMsg);
		if(ModBusMsg.ErrorFlag==ModBus_Ok)
		{
			switch(ModBusMsg.MsgFlag)
			{
				case PMFF_TestCmd:Length=StringCoppy(String_Hello,&Buf[0]);
						 break;
				case PMFF_GetAddr:
						 break;
				case PMFF_SetAddr:
						 break;
				case PMFF_GetUint16:
						 break;
				case PMFF_GetUint8:
						 break;
				case PMFF_GetBuf:
						 Offset=USART2_RX_Buffer[Length];
					   Length=USART2_RX_Buffer[Length+1];
						 if(Offset<PDA_Buf_Size)if(Length<(PDA_Buf_Size-Offset))
						 {
							 BufferCoppy(&PDA_Buf[Offset],&Buf[0],Length);
						 }
						 break;
				case PMFF_SetBuf:
						 break;
				default:Length=0;
						 break;
			}
			Length=ModBus_CreateMsg(&USART2_TX_Buffer[0],&ModBusMsg,0x01,0x01,0x01,&Buf[0],Length);
			USART2_WriteDatas(&USART2_TX_Buffer[0],Length);
		}else if(ModBusMsg.ErrorFlag==ModBus_Broad)
		{
			//USART2_WriteDatas("ModBus_Broad",10);
		}
		else if(ModBusMsg.ErrorFlag==ModBus_FlagError)
		{
			//USART2_WriteDatas("ModBus_FlagError",10);
		}
		//else USART2_WriteDatas("Other_Error",10);
		
	}
}
/****************************************************************************
Date:2014-11-12
Func:PDA获取数据任务
Note:
****************************************************************************/
void Task_PdaGetDev(void *Tags)
{
	uint8 Length;
	uint8 Buf[8]={0x02,0x03,0x00,0x00,0x00,0x22,0xC5,0xE0};
	uint16 CrcValue,Temp;
	while(1)
	{
		USART1_DMAWriteDatas(&Buf[0],8);
		Length=USART1_ReadDatas(Null,100,1000);
		if(Length>20)
		{
			if(USART1_RX_Buffer[0]==0x02)
			if(USART1_RX_Buffer[1]==0x03)
			if(USART1_RX_Buffer[2]<PDA_Buf_Size)
			{
				Pda_Status=1;
				CrcValue=USART1_RX_Buffer[USART1_RX_Buffer[2]+4];
				CrcValue<<=8;
				CrcValue|=USART1_RX_Buffer[USART1_RX_Buffer[2]+3];
				Temp=Tools_GetCRC16(&USART1_RX_Buffer[0],USART1_RX_Buffer[2]+3);
				if(Temp==CrcValue)BufferCoppy(&USART1_RX_Buffer[3],&PDA_Buf[0],USART1_RX_Buffer[2]);
			}
		}
		Tos_TaskDelay(1000);
	}
}
#endif
