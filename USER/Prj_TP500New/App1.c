void TaskFunc(void *Tags)
{
	uint8 i;
	Tos_ArrayMessage Msg[2];
	MGUI_KeyMsg *KeyMsg,KeyMsgBuf[2];	
	Tos_TaskGetHandKey();
  for(i=0;i<2;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],2);
	i=0;
	if(UIShowValue.SysWorkMode==0xff)
	while(1)
	{
		Ht1621_DrawMenu(i);
		Tos_TaskDelay(10);
		KeyMsg=MUI_GetKey(0);
		if(KeyMsg)
		{
			if(KeyMsg->KeyValue==Key_Printf)break;
			if(KeyMsg->KeyValue==Key_UpDown)i++;
			if(i>4)i=0;
		}
	}
	UIShowValue.SysWorkMode=i;
	while(1)
	{
		Tos_TaskDelay(10);
		KeyMsg=MUI_GetKey(0);
		if(KeyMsg)
		{
			if(KeyMsg->KeyValue==Key_Printf)if(KeyMsg->KeyType==Key_OneClick)
			{
				if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)Task_PrintfLog();
				else {if(UIShowValue.RunFlag&WRF_BeepDisable)UIShowValue.RunFlag&=~WRF_BeepDisable;else UIShowValue.RunFlag|=WRF_BeepDisable;}
			}
			if(KeyMsg->KeyValue==Key_Printf)if(KeyMsg->KeyType==Key_DownTimeOut)
			{
				if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)UIShowValue.SaveValue.WorkBit&=~WCB_RecordEN;else UIShowValue.SaveValue.WorkBit|=WCB_RecordEN;
				BEEP_Double();
				UIShowValue.RecordCount=0;
			  W25Q16_Erase(0,sizeof(NandHistoryRecord)*0xff);
				Task_SaveConfigs();
			}
			if(KeyMsg->KeyValue==Key_UpDown)if(KeyMsg->KeyType==Key_OneClick)
			{
				if(UIShowValue.RunFlag&WRF_BeepDisable)UIShowValue.RunFlag&=~WRF_BeepDisable;else UIShowValue.RunFlag|=WRF_BeepDisable;
			}
			if(KeyMsg->KeyValue==Key_WkUp)if(KeyMsg->KeyType==Key_DownTimeOut)
			{
				BEEP_Double();
				GPIO_DeInit(GPIOA);
				GPIO_DeInit(GPIOB);
				GPIO_DeInit(GPIOC);
				GPIO_DeInit(GPIOD);
				GPIO_DeInit(GPIOE);
				GPS_OFF;
				Standby_Init();
			}
			if(KeyMsg->KeyValue==Key_UpDown)if(KeyMsg->KeyType==Key_DownTimeOut)
			{
				BEEP_Double();
				UIShowValue.SysWorkMode=8;
				while(1)
				{
					Ht1621_DrawUpDown(UIShowValue.SysWorkMode-8);
					Tos_TaskDelay(10);
					KeyMsg=MUI_GetKey(0);
					if(KeyMsg)
					{
						if(KeyMsg->KeyValue==Key_UpDown)if(KeyMsg->KeyType==Key_OneClick)
						{
							UIShowValue.SysWorkMode++;
							if(UIShowValue.SysWorkMode>12)UIShowValue.SysWorkMode=8;
						}
						if(KeyMsg->KeyValue==Key_UpDown)if(KeyMsg->KeyType==Key_DownTimeOut)
							{
						  BEEP_Double();
							UIShowValue.SysWorkMode=0;
							break;
							}
						if(KeyMsg->KeyValue==Key_Printf)if(KeyMsg->KeyType==Key_OneClick)
							{
						  BEEP_Double();
							UIShowValue.SysWorkMode=0;
							break;
							}
					}
				}
			}
				
		}
	}
}

void TaskPlan(void *Tags)
{
	static float CHValue[2];
	ADC1_Init((uintbus)&UIShowValue.ADC[0]);
	ISL1208_Init((void *)IO_ISL1208_P);
	Sensor_Init();
	Tos_TaskDelay(3000);
	ISL1208_GetDateTime(&UIShowValue.DateTime);	
	DS_Read_Value(&UIShowValue.CH_Value[0],&UIShowValue.CH_Value[1]);
	DS_Read_Value(&UIShowValue.CH_Value[0],&UIShowValue.CH_Value[1]);
	UIShowValue.RecordCount=Get_PrintfRecordCount();
	while(1)
	{
		ISL1208_GetDateTime(&UIShowValue.DateTime);	
		Tos_TaskDelay(500);
		DS_Read_Value(&CHValue[0],&CHValue[1]);
		UIShowValue.CH_Value[0]=UIShowValue.SaveValue.CorrectValue[0]+CHValue[0];
		UIShowValue.CH_Value[1]=UIShowValue.SaveValue.CorrectValue[1]+CHValue[1];
		//UIShowValue.CH_Value[1]=UIShowValue.SaveValue.CorrectValue[1]+CHValue[1]+(CHValue[0]-(int)CHValue[0]);
		Task_Plan();
		if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)Task_Record();
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))UIShowValue.RunFlag|=WRF_USB_Connected;else UIShowValue.RunFlag&=~WRF_USB_Connected;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))UIShowValue.RunFlag|=WRF_EXP_Connected;else UIShowValue.RunFlag&=~WRF_EXP_Connected;
	}
}
void TaskBigShow()
{
	BEEP_ON;
	LED1_ON;
	LED2_ON;
	Tos_TaskDelay(100);
	BEEP_OFF;
	LED1_OFF;
	LED2_OFF;
	Tos_TaskDelay(100);
	BEEP_ON;
	LED1_ON;
	LED2_ON;
	Tos_TaskDelay(100);
	BEEP_OFF;
	LED1_OFF;
	LED2_OFF;
}

