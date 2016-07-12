
<<<<<<<<<<<在对本TOS系统及TGUI进行修改或升级必须遵循以下规范>>>>>>>>>>>>
【版本声明】
随着新版本的增加和简化，会对旧工程产生局部冲突
【结构规范】
代码组织结构必须按照原结构对号入座，不得擅自更改,结构调整过的代码禁止上传到svn.
代码依赖关系尽可能独立，以确保系统的可裁剪性和移植性.

【版本规范】
版本号规范 Ver3.43
Version=3.0版本,在更改目录结构和框架结构的时候主版本号升级.
Subedition=43版本,在增加或修改驱动等非框架结构副版本号升级.


【编程注意】
在中断内部不允许对任务进行切换，只能对任务进行唤醒.

【共用体的用法】
typedef union 
{
	uint16 W;
	struct
	{
		uint8 a;
		uint8 b;
	}b;
}Word2Byte;

【使用四线下载禁用jlink模式】
Remap_SWJ_JTAGDisable();

【BOOTLoader】
应用程序开始必须	设置中断向量地址
NVIC_SetVectorTable(0x08005000,0);
【代码规范】
所有子函数注释最少保证三条，功能介绍(Func)、最后修改日期(Date)、备注或描述(Note).
可新增注释 作者(Writer)、版本(Ver).


【新建工程】
新建工程时需要对工程作如下配置
修改C/C+选项卡中define的值定义板子唯一类型、更改output 中的obj输出到工程用户子目录、
根据情况修改cpu类型及程序大小和程序空间地址规划

	宏			  MCU型号		FLASH大小      
			     STM32F101xx	  			   |
STM32F10X_LD     STM32F102xx	16~32Kbytes    |
			     STM32F103xx				   |
-----------------------------------------------|				 
				 STM32F101xx				   |
STM32F10X_MD	 STM32F102xx	64~128Kbytes   |
				 STM32F103xx				   |
-----------------------------------------------|
			     STM32F101xx				   |
STM32F10X_HD     STM32F102xx    256~512KBytes  |
				 STM32F103xx 				   |
-----------------------------------------------|
				 STM32F101xx  				   |
STM32F10X_CL     STM32F102xx    			   |
				 STM32F103xx				   |
-----------------------------------------------|

动态内存分配的使用
首先要对内存堆进行初始化
执行初始化函数	
	static uint8 MemBuf[40000];//堆申请可以使用局部静态变量也可以使用全局变量，库函数使用的居然是任务局部变量
	Mema_Init(&MemBuf[0]);
使用是先任意类型指针 void *P;
P=Mema_Malloc(内存大小);
接着就可以使用P指针指向的泛型空间实体了
使用完成可以对内存进行释放
Mema_Free（P);必须特别申明的是P指针的地址值不得在中途进行修改

对于原先的这种通用接口仍未废弃，但操作起来较为麻烦还不如直接操作
void Task0(void *Tags)
{
	uint8 Out;

	DeBug_OutSet(DeBug_USART2);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	while(1)
	{
		Out=0x05;
		//if(!SIG1)Joystick_Send(0x51,0x03,0,0);
		Tos_Device_Tab[DeviceId_IO].Write((void *)&Out,Null,Null);
		Tos_TaskDelay(1000);
		Out=0x02;
		Tos_Device_Tab[DeviceId_IO].Write((void *)&Out,Null,Null);
		Tos_TaskDelay(1000);
	}
}

void _IAP_Command(void *Tags)
{
	uint16 Length;
	uint16 MasterAddr;
	uint16 OffSet;
	uint8 Buf[1500];
	BufferFill(&Buf[0],0xff,sizeof(Buf));
	while(1)
	{
		ClearWithBuffer(&USART1_RX_Buffer[0],sizeof(USART1_RX_Buffer));
		Length=USART1_ReadDatas(Null,sizeof(USART1_RX_Buffer),0);
		MasterAddr=GetWordFromBytes(&USART1_RX_Buffer[0],1);
		if(ZigBee_CheckCrc(&USART1_RX_Buffer[0]))
		{
			WaitDownLoadCount=50;
			Length=5;
			MasterAddr=GetWordFromBytes(&USART1_RX_Buffer[OSE_DevAddr],1);
			switch(USART1_RX_Buffer[OSE_Cmd])
			{
				case SCFF_Write64Byte:
					OffSet=USART1_RX_Buffer[OSE_DataStart];
					OffSet*=64;
					if(OffSet>=sizeof(Buf))break;
					BufferCoppy(&USART1_RX_Buffer[Length],&Buf[OffSet],64);
					break;
				case SCFF_Read1024Byte:
		      IAP_ReadDatas(ApplicationAddress+FLASH_PAGESIZE*GetWordFromBytes(&USART1_RX_Buffer[OSE_DataStart],1),(uint16 *)&Buf[0],512);
				  Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],MasterAddr,SCFF_Read64Byte,&Buf[0],64);
				  USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length);
					break;
				case SCFF_Read64Byte:
					OffSet=USART1_RX_Buffer[OSE_DataStart];
					OffSet*=64;
				  if(OffSet>=sizeof(Buf))break;
					Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],MasterAddr,SCFF_Read64Byte,&Buf[OffSet],64);
				  USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length);
					break;
				case SCFF_Write1024Byte:
				  if(Tools_GetCRC16(&Buf[0],FLASH_PAGESIZE)==GetWordFromBytes(&USART1_RX_Buffer[OSE_DataStart],1))
					{
						FLASH_WriteBank(ApplicationAddress+FLASH_PAGESIZE*(GetWordFromBytes(&USART1_RX_Buffer[OSE_DataStart+2],1)),&Buf[0],1024);
						Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],MasterAddr,SCFF_Write1024Byte,&USART1_RX_Buffer[OSE_DataStart+2],2);
						USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
					}
					BufferFill(&Buf[0],0xff,sizeof(Buf));
				  break;
					case SCFF_GoToBootLoader:
					FLASH_ProgramDone();
					Application_Run(ApplicationAddress);
				  break;
				case SCFF_EraseFlase:
				  	FLASH_ProgramStart(Uint32FromByte(&USART1_RX_Buffer[OSE_DataStart]));
				    Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],MasterAddr,SCFF_EraseFlase,&USART1_RX_Buffer[OSE_DataStart],4);
						USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length); 
				  break;
			}
		}
	}
}

【编程帮助】

/*************************************************************
Func:  掉电参数处理
Date:  2015-7-27
Note:  参数结构体
*************************************************************/
typedef struct{
	ConfigHeadStruct CHS;
	uint8 Name[11];
	uint8 Age;
	f32 Weight;
	f32 Height;
	uint16 eHeight;
}UserInfor;
UserInfor UI;
/*************************************************************
Func:  掉电参数处理
Date:  2015-7-27
Note:  使用
*************************************************************/
void Task1(void *Tags)
{
	char *m;
	StringCoppy("TomCat ba",&UI.Name[0]);
	UI.Age=99;
	UI.Weight=112.11;
	UI.Height=33.33;
	UI.CHS.Size=sizeof(UI);
  //Save_AppConfig(&UI,0);
	while(1)
	{
		if(Load_AppConfig(&UI,0)){
	    DeBug("UI.Age=%d",UI.Age,Infor_Infor);
		DeBug("UI.Name=%s",UI.Name,Infor_Infor);
		DeBug("UI.Weight=%.2f",UI.Weight,Infor_Infor);
		DeBug("UI.Height=%.2f",UI.Height,Infor_Infor);
		DeBug("UI.Size=%d",UI.CHS.Size,Infor_Infor);
		}
		Tos_TaskDelay(100);
	}
}
/*************************************************************
Func:  消息队列的用法
Date:  2014-9-10
Note:  接收方
*************************************************************/
void TaskReviceQMsg(void *Tags)
{
	uint8 Buf[100];
	uint16 i;
	Tos_ArrayMessage *CurMsg,Msg[10];
	TQFS_Open_File NewOpenFile;
	CanRxMsg *RxMessage;	
	CanRxMsg RxMessageBuf[10];	
	i=TQFS_FindKeyPageMark(FT_FileHead,FT_FileHead);
	TQFS_SetCursor(&NewOpenFile,i,0);
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&RxMessageBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	while(1)
	{
		Tos_TaskDelay(10);
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
		RxMessage=CurMsg->Qmsg_Addr;
		DeBug("RxMessage->Data[0]=%d",RxMessage->Data[0],Infor_Infor);
		}
	}
}
/*************************************************************
Func:  消息队列的用法
Date:  2014-9-10
Note:  发送方
*************************************************************/
void TaskSendQMsg(void *Tags)
{
    uint8 i;
	CanRxMsg RxMessage;	
	while(1)
	{
		Tos_TaskDelay(100);
		RxMessage.Data[0]=i++;
		Tos_TaskPutQmsg(2,0,sizeof(CanRxMsg),&RxMessage);
	}
}

/*************************************************************
Func:  Nrf2401使用
Date:  2014-9-22
Note:  发送方
*************************************************************/
int main(void)
{
  
   SPI_NRF_Init();
   
  /* 串口1初始化 */
	USART1_Config();

	printf("\r\n 这是一个 NRF24L01 无线传输实验 \r\n");
   	printf("\r\n 这是无线传输 主机端 的反馈信息\r\n");
	printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");

	 /*检测NRF模块与MCU的连接*/
   	status = NRF_Check(); 

	/*判断连接状态*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      NRF与MCU连接成功！\r\n");  
   else	  
   	     printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");


 while(1)
		{
	   	printf("\r\n 主机端 进入自应答发送模式\r\n"); 
	 	NRF_TX_Mode();
		 	
			/*开始发送数据*/	
		status = NRF_Tx_Dat(txbuf);	  
		  
		  /*判断发送状态*/
		  switch(status)
		  	{
		  	  case MAX_RT:
			 	 	printf("\r\n 主机端 没接收到应答信号，发送次数超过限定值，发送失败。 \r\n");
				 break;
	
			  case ERROR:
			  	 	printf("\r\n 未知原因导致发送失败。 \r\n");
				 break;
	
			  case TX_DS:
			  		printf("\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");	 		
				 break;  								
		  	}			   	
	
	 	printf("\r\n 主机端 进入接收模式。 \r\n");	
		NRF_RX_Mode();
	
			/*等待接收数据*/
		status = NRF_Rx_Dat(rxbuf);
	
			/*判断接收状态*/
			switch(status)
			{
			 case RX_DR:
			 	for(i=0;i<4;i++)
				{					
					printf("\r\n 主机端 接收到 从机端 发送的数据为：%d \r\n",rxbuf[i]);
					txbuf[i] =rxbuf[i];
				}
				  break;
	
			 case ERROR:
				  	printf("\r\n 主机端 接收出错。   \r\n");
				  break;  		
			}
		} 

}


void Task_Face(void *Tags)
{
	uint8 Buf[100];
	IpPortStruct IPPort;
	BEEP_ON;
	Tos_TaskDelay(4);
	BEEP_OFF;
	while(1)
	{
		RTC_Get();
		Ht1621_DrawHome();
		DHT11_Read(&UIShowValue.CH_Value[0],&UIShowValue.CH_Value[1]);
		Tos_TaskDelay(500);
//		IpPortFromString(&IPPort,"121.121.10.121:8080");
//		DeBug_Get();
//		DeBug("%d.%d.%d.%d:%d",IPPort.Ip[0],IPPort.Ip[1],IPPort.Ip[2],IPPort.Ip[3],IPPort.Port,Infor_Error);
//		IpPortToString(&IPPort,&Buf[0]);
//		DeBug("%s",&Buf[0],Infor_Error);
		
	}
}

【系统定时器用法】
void Tasdk()
{
	DeBug("ZZZZZZZZZZZZZZZZZZZ",4);
}
void Tasdk1()
{
	DeBug("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",4);
}


void Task_1(void *Tags)
{
	Tos_Timer *P,*P2;
	P=Tos_CreateTimer(1,TTF_FOREVER,Tasdk);
	P2=Tos_CreateTimer(4,TTF_FOREVER,Tasdk1);
	Tos_TaskDelay(10000);
	Tos_DeleteTimer(P);
	Tos_DeleteTimer(P2);
	P=Tos_CreateTimer(1,TTF_FOREVER,Tasdk);
	while(1)
	{
		Tos_TaskDelay(100);
	}
}

