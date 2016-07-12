#include <..\USER\Prj_TP502T\UpgradeByGPRS.h>

u8 UpgradeServer_Connect()
{
	return GPRS_Connect("AT+QIOPEN=\"TCP\",\"113.99.66.73\",9112\r\n");
}
u8 UpgradeServer_Read(uint8 *ReBuf,uint16 Addr,uint8 Cmd,uint8 *Data,uint16 Length)
{
	uint8 Buf[300];
	BufferFill(USART_RX_Buffer[1],0x00,USART_RX_BUFFER_SIZE);
	BufferFill(&ReBuf[0],0x00,USART_RX_BUFFER_SIZE);
	Length=ZBUS_SendMsg(&Buf[0],Addr,Cmd,&Data[0],Length);
	USART_WriteLine(USART2,"AT+QISEND=%d\r\n",Length);
	if(!GPRS_Ack(">",2000))return 1;
	USART_WriteDatas(USART2,&Buf[0],Length);
	if(!GPRS_Ack("SEND OK",1000))return 2;
	Length=USART_ReadDatas(USART2,&ReBuf[0],USART_RX_BUFFER_SIZE,2000);
	if(Length)
	{
		if(ZBUS_CheckCrc(USART_RX_Buffer[1]))
		{
			QuectelM26Printf_Bytes("R:",USART_RX_Buffer[1],Length);
			return 0;
		}else DeBug("校验失败",Infor_Warning);
	}
	DeBug("返回超时",Infor_Warning);
	return 3;
}
void Task_DownConfig(void *Tags)
{
	uint16 i=0,Length,c=0;
	uint8 sta;
	uint8 Buf[USART_RX_BUFFER_SIZE],Data[]={0x00,100},WR[sizeof(UIShowValue.SaveValue)];
	UIShowValue.RSSI=99;
	UIShowValue.UpgradePogress=0;
	sta=QuectelM26_Init();
	if(sta)DeBug("初始化失败[%d]",sta,Infor_Warning);
	sta=UpgradeServer_Connect();
	if(sta)DeBug("连接服务器[%d]",sta,Infor_Warning);
	DeBug("自动配置",Infor_Warning);
	while(1)
	{
		if(!UpgradeServer_Read(&Buf[0],0x0000,UCG_GetDateTime,&Data[0],0))
		{
			UIShowValue.DateTime.Year=Buf[5];
			UIShowValue.DateTime.Mon=Buf[6];
			UIShowValue.DateTime.Day=Buf[7];
			UIShowValue.DateTime.Hour=Buf[8];
			UIShowValue.DateTime.Min=Buf[9];
			UIShowValue.DateTime.Sec=Buf[10];
			UIShowValue.DateTime.Week=Buf[11];
			ISL1208_SetDateTime(&UIShowValue.DateTime);	
			DeBug("同步时间完毕",Infor_Warning);;
		}
		if(!UpgradeServer_Read(&Buf[0],0x0001,UCG_SetBinCFGFile,&Data[0],2))
		{
			c=0;
			while(1)
			{
				if(!UpgradeServer_Read(&Buf[0],i,UGC_ReadBinFile,&Data[0],0))
				{
					i++;
					Length=Buf[3];
					Length<<=8;
					Length|=Buf[4];
					if((c+Length)<=sizeof(WR))c+=BufferCoppy(&Buf[5],&WR[c],Length);
					QuectelM26Printf_Bytes("Bin:",&Buf[5],Length);
					UIShowValue.UpgradePogress=(uint16)c*100/sizeof(WR);
					if(Length==0)
					{
						QuectelM26Printf_Bytes("CFG:",&WR[0],c);
						if(c==sizeof(UIShowValue.SaveValue))
						{
							Save_AppConfig(&WR[0],0);
							Task_LoadConfigs();
							DeBug("下载配置完成",Infor_Warning);
						}else DeBug("配置文件长度错误",Infor_Warning);
							GPRS_CloseNet();
							BEEP_Double();
						return;
					}
				}
				Tos_TaskDelay(1000);
			}
		};
	//	W25Q16_Write(0x3432,&Buf[0],100);
		Buf[0]=0;
		//W25Q16_Read(0x3432,&Buf[0],100);
		//DeBug("读取数据[%d]",Buf[0],Infor_Warning);
		Tos_TaskDelay(20000);
	}
}
void Task_Upgrade(void *Tags)
{
	uint16 i=0;
	uint8 sta;
	uint8 Buf[USART_RX_BUFFER_SIZE],Data[]={0x01,15};
	UIShowValue.RSSI=99;
	UIShowValue.UpgradePogress=0;
	sta=QuectelM26_Init();
	if(sta)DeBug("初始化失败[%d]",sta,Infor_Warning);
	sta=UpgradeServer_Connect();
	if(sta)DeBug("连接服务器[%d]",sta,Infor_Warning);
	DeBug("应用升级",Infor_Warning);
	while(1)
	{
		Buf[0]=23;
		if(!UpgradeServer_Read(&Buf[0],0x0002,UGC_SetBinFile,&Data[0],2))
		{
			while(1)
			{
				if(!UpgradeServer_Read(&Buf[0],i,UGC_ReadBinFile,&Data[0],0))
				{
					i++;
					W25Q16_Write(i*271,&Buf[5],271);
					DeBug("写入nand[%d]",i*271,Infor_Warning);
				}
				Tos_TaskDelay(20000);
			}
		};
	//	W25Q16_Write(0x3432,&Buf[0],100);
		Buf[0]=0;
		//W25Q16_Read(0x3432,&Buf[0],100);
		//DeBug("读取数据[%d]",Buf[0],Infor_Warning);
		Tos_TaskDelay(20000);
	}
}
uint8 UpConfig_Create(uint8 *Buf,uint16 Space,uint16 index)
{
	uint16 Length,i;
	uint16 SpaceSize=Space;
	Length=sizeof(UIShowValue.SaveValue);
	i=(Length-index*SpaceSize)/SpaceSize;//剩余包数包含当前包
	if(Length<=index*SpaceSize){SpaceSize=0;goto OK;}//剩余长度不够
  if(i==0){SpaceSize=Length-index*SpaceSize;goto OK;}//剩余半包
OK:
	i=0;
	i+=BufferCoppy(&((uint8 *)&UIShowValue.SaveValue)[index*Space],&Buf[i],SpaceSize);
	DeBug("数据长度[%d]",i,Infor_Warning);
	return i;
}

void Task_UpConfig(void *Tags)
{
	uint16 i=0;
	uint8 sta,length;
	uint8 Buf[USART_RX_BUFFER_SIZE],Data[USART_RX_BUFFER_SIZE];
	UIShowValue.RSSI=99;
	UIShowValue.UpgradePogress=0;
	sta=QuectelM26_Init();
	if(sta)DeBug("初始化失败[%d]",sta,Infor_Warning);
	sta=UpgradeServer_Connect();
	if(sta)DeBug("连接服务器[%d]",sta,Infor_Warning);
	DeBug("上传配置",Infor_Warning);
	while(1)
	{
		Buf[0]=23;
		i=0;
		while(1)
		{
			length=UpConfig_Create(&Data[0],100,i);
			while(1)
			{
			Tos_TaskDelay(1000);
			if(!UpgradeServer_Read(&Buf[0],0x0002,UCG_SendBinCFG,&Data[0],length))break;
			UIShowValue.UpgradePogress=(uint16)i*100/(sizeof(UIShowValue.SaveValue)/100);
			Tos_TaskDelay(1000);
			}
			i++;
			if(length==0)break;
		}
		QuectelM26Printf_Bytes("SaveData:",&((uint8 *)&UIShowValue.SaveValue)[0],sizeof(UIShowValue.SaveValue));
		GPRS_CloseNet();
		BEEP_Double();
		DeBug("上传配置完毕",Infor_Warning);return;
	}
}

void Task_UpTextConfig(void *Tags)
{
	uint16 i=0,Length;
	uint8 MenuLength;
	const COM_MenuStruct *Menu=MenuList;
	uint8 sta;
	uint8 Buf[USART_RX_BUFFER_SIZE],Data[40];
	UIShowValue.RSSI=99;
	UIShowValue.UpgradePogress=0;
	sta=QuectelM26_Init();
	if(sta)DeBug("初始化失败[%d]",sta,Infor_Warning);
	sta=UpgradeServer_Connect();
	if(sta)DeBug("连接服务器[%d]",sta,Infor_Warning);
	DeBug("更新配置",Infor_Warning);
	while(1)
	{
		i=0;
		if(!UpgradeServer_Read(&Buf[0],0x0000,UCG_GetDateTime,&Data[0],0))
		{
			UIShowValue.DateTime.Year=Buf[5];
			UIShowValue.DateTime.Mon=Buf[6];
			UIShowValue.DateTime.Day=Buf[7];
			UIShowValue.DateTime.Hour=Buf[8];
			UIShowValue.DateTime.Min=Buf[9];
			UIShowValue.DateTime.Sec=Buf[10];
			UIShowValue.DateTime.Week=Buf[11];
			ISL1208_SetDateTime(&UIShowValue.DateTime);	
			DeBug("同步时间完毕",Infor_Warning);
		}
		BEEP_Double();
		
		
Set:
		i=0;
		sta=BufferCoppy((uint8 *)0x1ffff7e8,&Data[0],12);
		Menu=MenuList;
		MenuLength=Menu_OptionLength(Menu);
		if(!UpgradeServer_Read(&Buf[0],0xff00+MenuLength,UCG_SetMenuList,&Data[0],sta))
		while(Menu->Type!=VT_End)
		{
			MUI_GetOptionString((char *)&Data[0],Menu);
			sta=GetTextLength(Data);
			DeBug("上传菜单:%s",&Data[0],Infor_Warning);
			if(!UpgradeServer_Read(&Buf[0],i,UCG_SetMenuList,&Data[0],sta))
			{
				i++;
				UIShowValue.UpgradePogress=i*100/MenuLength;
				Menu++;
			}
			Tos_TaskDelay(10);
		}
		i=0;
		Menu=MenuList;
		sta=Menu_OptionLength(Menu)-2;
		while(1)
		{
			
			if(!UpgradeServer_Read(&Buf[0],i,UCG_CheckNewMenu,&Data[0],0))
			{
				Length=Buf[3];
				Length<<=8;
			  Length|=Buf[4];
				if(Length>0)if(Buf[5]==3)goto Over;
				if(Length>0)if(Buf[5]==2)goto Set;
				if(Length>0)if(Buf[5]==1)
			while(1)
			{
				if(!UpgradeServer_Read(&Buf[0],i,UCG_GetMenuList,&Data[0],0))
				{
						Length=Buf[3];
						Length<<=8;
						Length|=Buf[4];
					if(Length>0)
					{
						COM_Menu_Enter(Buf[1],&Buf[5]);
						DeBug("下载菜单:%s",&Buf[5],Infor_Warning);
					}
					if(i++>=sta){BEEP_Double();Task_SaveConfigs();goto Set;}
					UIShowValue.UpgradePogress=i*100/MenuLength;
			  }
				Tos_TaskDelay(10);
			}
			}
			Tos_TaskDelay(5000);
		}
Over:		
		GPRS_CloseNet();
		BEEP_Double();
		DeBug("更新配置完毕",Infor_Warning);return;
	}
}
void Task_DownTextConfig(void *Tags)
{
	uint16 i=0;
	uint8 sta,maxpog;
	uint8 Buf[USART_RX_BUFFER_SIZE],Data[]={0x01,15};
	UIShowValue.RSSI=99;
	UIShowValue.UpgradePogress=0;
	sta=QuectelM26_Init();
	if(sta)DeBug("初始化失败[%d]",sta,Infor_Warning);
	sta=UpgradeServer_Connect();
	if(sta)DeBug("连接服务器[%d]",sta,Infor_Warning);
	DeBug("更新配置",Infor_Warning);
	while(1)
	{
		Buf[0]=23;
		i=0;
		if(!UpgradeServer_Read(&Buf[0],0x0000,UCG_GetDateTime,&Data[0],0))
		{
			UIShowValue.DateTime.Year=Buf[5];
			UIShowValue.DateTime.Mon=Buf[6];
			UIShowValue.DateTime.Day=Buf[7];
			UIShowValue.DateTime.Hour=Buf[8];
			UIShowValue.DateTime.Min=Buf[9];
			UIShowValue.DateTime.Sec=Buf[10];
			UIShowValue.DateTime.Week=Buf[11];
			ISL1208_SetDateTime(&UIShowValue.DateTime);	
			DeBug("同步时间完毕",Infor_Warning);
		}
		if(!UpgradeServer_Read(&Buf[0],0x0001,UGC_SetTextFile,&Data[0],2))
		{
			Tos_TaskDelay(2000);
			if(!UpgradeServer_Read(&Buf[0],0x0001,UGC_GetTextFileInfor,0,0))
			{
				maxpog=Buf[5];
				while(1)
			{
				if(!UpgradeServer_Read(&Buf[0],i,UGC_ReadTextFile,&Data[0],0))
				{
					i++;
					UIShowValue.UpgradePogress=(uint16)i*100/maxpog;
					DeBug("ReadLine[%d]:%s",i,&Buf[5],Infor_Warning);
					if(i>=maxpog)
					{
						GPRS_CloseNet();
						BEEP_Double();
						DeBug("更新配置完毕",Infor_Warning);return;
					}
				}
				Tos_TaskDelay(100);
			}
			}
		}
		Buf[0]=0;
		Tos_TaskDelay(40000);
	}
}
