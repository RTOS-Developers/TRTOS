
void TaskFunc(void *Tags)
{
	uint8 i;
	Tos_ArrayMessage Msg[2];
	MGUI_KeyMsg *KeyMsg,KeyMsgBuf[2];	
	Tos_TaskGetHandKey();
  for(i=0;i<2;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],2);
	Task_LoadConfigs();
	while(1)
	{
		Tos_TaskDelay(10);
		KeyMsg=MUI_GetKey(0);
		if(KeyMsg)
		{
			if(KeyMsg->KeyValue==Key_Printf)if(KeyMsg->KeyType==Key_OneClick)
			{
				if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)Task_PrintfLog();
			}
			if(KeyMsg->KeyValue==Key_Printf)if(KeyMsg->KeyType==Key_DownTimeOut)
			{
				if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)UIShowValue.SaveValue.WorkBit&=~WCB_RecordEN;else UIShowValue.SaveValue.WorkBit|=WCB_RecordEN;
				BEEP_Double();
				Task_SaveConfigs();
			}
		}
	}
}

void TaskPlan(void *Tags)
{
	Tos_TaskDelay(5000);
	while(1)
	{
		Tos_TaskDelay(500);
		if(UIShowValue.CH_Value[0]>=UIShowValue.SaveValue.CH_Limit[0].Max)UIShowValue.RunFlag|=WRF_CH1Max;else UIShowValue.RunFlag&=~WRF_CH1Max;
		if(UIShowValue.CH_Value[0]<=UIShowValue.SaveValue.CH_Limit[0].Min)UIShowValue.RunFlag|=WRF_CH1Min;else UIShowValue.RunFlag&=~WRF_CH1Min;
		if(UIShowValue.CH_Value[1]>=UIShowValue.SaveValue.CH_Limit[1].Max)UIShowValue.RunFlag|=WRF_CH2Max;else UIShowValue.RunFlag&=~WRF_CH2Max;
		if(UIShowValue.CH_Value[1]<=UIShowValue.SaveValue.CH_Limit[1].Min)UIShowValue.RunFlag|=WRF_CH2Min;else UIShowValue.RunFlag&=~WRF_CH2Min;
	}
}
