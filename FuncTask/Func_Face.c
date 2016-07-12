#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Func_Face.h>
uint8 TaskArray_Tos_TaskID=Tos_TaskNull;
/******************************************************************************
 Func:加载&更新屏幕通过ModBus协议
 Time: 2014-7-20
 Ver.: V2.0
 Note:
******************************************************************************/
#ifdef LCD_YSIZE
void Task_GetScreen(void *Tags)
{
	uint16 x;
	uint16 y;
	uint16 Temp;
	uint16 ReadLine;
	uint8 Buf[600];
	ModBus_Message ModBusMsg;
	ModBus_Create(&ModBusMsg,0x01,0x94,WorkMode_Slave,MBCM_Xor);
	while(1)
	{
	  x=USART_ReadDatas(USART1,Null,600,0);
	 if(x)
		{
			DeBug_Get();
 			x=ModBus_Expend(USART_RX_Buffer[0],x,&ModBusMsg);
			if(ModBusMsg.MsgFlag==0x02)
			{
			ReadLine=GetWordFromBytes(&USART_RX_Buffer[0][x],1);
			for(y=0;y<LCD_YSIZE;y++){
				x=LCD_Get_Pixel(ReadLine,y);
				Buf[y*2]=x>>8;
				Buf[y*2+1]=x&0xff;
			}
			x= ModBus_CreateMsg(USART_TX_Buffer[0],&ModBusMsg,0x01,0x01,0x01,&Buf[0],LCD_YSIZE*2);
			USART_WriteDatas(USART1,USART_TX_Buffer[0],x);
		  }else if(ModBusMsg.MsgFlag==0x04)
			{
				ReadLine=GetWordFromBytes(&USART_RX_Buffer[0][x],1);
				for(y=0;y<LCD_YSIZE;y++)
				{
				Temp=USART_RX_Buffer[0][y*2+x+2];
				Temp<<=8;
			  Temp|=USART_RX_Buffer[0][y*2+1+x+2];
				LCD_Set_Pixel(ReadLine,y,Temp);
			  }
			}
		}
		Tos_TaskDelay(10);
	}
}
#endif
/****************************************************************************
Date:2014-5-12
Func:串口调试用
Note:
****************************************************************************/
void Usart_Terminal(void *Tags)
{
	uint8 x;
	DeBug_OutSet(DeBug_USART1|DeBug_LCD);//调试信息通过串口打印
	DeBug_Get();
	while(1)
	{
		
	  x=USART_ReadDatas(USART1,Null,600,0);
	 if(x)
		{
			DeBug("%s",USART_RX_Buffer[0],Infor_Infor);
		}
		Tos_TaskDelay(10);
	}
}
/****************************************************************************
Date:2014-5-12
Func:串口调试用
Note:
****************************************************************************/
void Usart_Usart(void *Tags)
{
	uint8 x;
	DeBug_OutSet(DeBug_USART1|DeBug_LCD);//调试信息通过串口打印
	DeBug_Get();
	while(1)
	{
	  x=USART_ReadDatas(USART1,Null,600,0);
	 if(x)
		{
			DeBug("%s",USART_RX_Buffer[0],Infor_Infor);
		}
		Tos_TaskDelay(10);
	}
}
/****************************************************************************
Date:2014-7-25
Func:加入任务到队列
Note:任务队列
****************************************************************************/
void Task_Join(void *CurTask,FuncType Type,uintbus Note)
{
	TaskArrayStruct TS;
	TS.Task=CurTask;
	TS.Type=Type;
	TS.Note=Note;
	Tos_TaskPutQmsg(TaskArray_Tos_TaskID,0,sizeof(TaskArrayStruct),&TS);
}
/****************************************************************************
Date:2014-7-25
Func:任务处理任务
Note:任务队列
****************************************************************************/
void Task_Arrary(void *Tags)
{
	uint8 i;
	typedef void(*VoidFuncVoid)(void);
	typedef void(*VoidFuncUint8)(uint8);
	typedef void(*VoidFuncUint32)(uint32);
	Tos_ArrayMessage *CurMsg,Msg[10];
	TaskArrayStruct *TaskMsg,TaskMsgBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&TaskMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	TaskArray_Tos_TaskID=Tos_TaskCurrent;
	while(1)
	{
		Tos_TaskDelay(10);
		 CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			TaskMsg=(TaskArrayStruct *)CurMsg->Qmsg_Addr;
			switch((uint8)TaskMsg->Type)
			{
				case FT_VoidFuncVoid:((VoidFuncVoid)(TaskMsg->Task))();break;
				case FT_VoidFuncUint8:((VoidFuncUint8)(TaskMsg->Task))(TaskMsg->Note);break;
				case FT_VoidFuncUint32:((VoidFuncUint32)(TaskMsg->Task))(TaskMsg->Note);break;
			}
		}
	}
}
