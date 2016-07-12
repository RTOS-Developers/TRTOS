#include <Include.h>
#ifdef BOARD_REALLY
#include <App_Really.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Really_GUI.h>
#include <KeyBoard.h>
uint8 PDA_Buf[PDA_Dev_Size][PDA_Buf_Size];
uint8 PDA_Tos_TaskID;
uint16 SleepCount=0;
void Task_GetPDA(void *Tags)
{
	uint16 Length;
	uint8 Buf[100],TempDevIndex;
	ModBus_Message ModBusMsg;
	MGUI_KeyMsg KeyMsg;
	Tos_TaskDelay(1000);
	while(1)
	{
		Length=2;
		Buf[0]=0;//Æ«ÒÆ
		Buf[1]=40;//³¤¶È
	  BufferFill(&USART2_TX_Buffer[0],0,100);
		BufferFill(&USART2_RX_Buffer[0],0,100);
		Tos_TaskDelay(1000);
		TempDevIndex=Really_DevIndex;
		ModBus_Create(&ModBusMsg,0xfe,TempDevIndex,WorkMode_Master,MBCM_Crc);
		Length=ModBus_CreateMsg(&USART2_TX_Buffer[0],&ModBusMsg,PMFF_GetBuf,0x01,0x01,&Buf[0],Length);
		USART2_WriteDatas(&USART2_TX_Buffer[0],Length);
		Length=USART2_ReadDatas(Null,100,1000);
		//Really_ShowFloat(0,0,Length,0);
		if(Length)
		{
			//Really_ShowFloat(0,0,Length,0);
			Length=ModBus_Expend(&USART2_RX_Buffer[0],Length,&ModBusMsg);
			if(ModBusMsg.ErrorFlag==ModBus_Ok)
			{
				if(ModBusMsg.DataLength<=PDA_Buf_Size)BufferCoppy(&USART2_RX_Buffer[Length],&PDA_Buf[TempDevIndex][0],ModBusMsg.DataLength);
				KeyMsg.KeyValue=Null;
				Tos_TaskPutQmsg(PDA_Tos_TaskID,Null,sizeof(MGUI_KeyMsg),&KeyMsg);
			}else
			{
		//	BufferFill(&PDA_Buf[Really_DevIndex][0],0,PDA_Buf_Size);
			//print_string_xy(ReallyID,80,0,&USART2_RX_Buffer[Length]);
			}
			
		}
	}
}
void Tos_TaskGetPDA()
{
	PDA_Tos_TaskID=Tos_TaskCurrent;
}
#endif
