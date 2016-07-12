#include <..\USER\Prj_TP502T\App2.h>
void BEEP_Double()
{
	BEEP_ON;
	Tos_TaskDelay(10);
	BEEP_OFF;
	Tos_TaskDelay(40);
	BEEP_ON;
	Tos_TaskDelay(10);
	BEEP_OFF;
}
void Task_BeepDouble()
{
	Task_Join(BEEP_Double,FT_VoidFuncVoid,Null);
}
void Task_SaveConfigs()
{
	UIShowValue.SaveValue.CH.Size=sizeof(UIShowValue.SaveValue);
	Save_AppConfig(&UIShowValue.SaveValue,0);
	Save_AppConfig(&UIShowValue.SaveValue,1);
}
void Task_SaveNandConfig()
{
	UIShowValue.SaveValue.CH.Size=sizeof(UIShowValue.SaveValue);
	W25Q16_SaveConfig(&UIShowValue.SaveValue,0);
}
void Task_LoadNandConfig()
{
	W25Q16_LoadConfig(&UIShowValue.SaveValue,0);
}
void Task_BackConfigs()
{
	UIShowValue.SaveValue.CH.Size=sizeof(UIShowValue.SaveValue);
	Save_AppConfig(&UIShowValue.SaveValue,1);
}
BOOL BOOL_LoadConfigs()
{
	BOOL Ok;
	Ok=Load_AppConfig(&UIShowValue.SaveValue,0);
	if(!Ok)
	{
		DeBug("配置参数片区一错误",Infor_Warning);
		Ok=Load_AppConfig(&UIShowValue.SaveValue,1);
	}
	return Ok;
}
void System_ResetDelay()
{
	Tos_TaskDelay(10);
	MCU_Reset();
}
void System_SaveConfig()
{
	Tos_TaskDelay(10);
	Task_Join(Task_SaveConfigs,FT_VoidFuncVoid,Null);
}
void System_Reset()
{
	Task_Join(System_ResetDelay,FT_VoidFuncVoid,Null);
}
void Task_LoadConfigs()
{
	BOOL_LoadConfigs();
}

void Task_ReBackConfigs()
{
	Load_AppConfig(&UIShowValue.SaveValue,1);
}
uint8 Get_PrintfRecordCount()
{
	uint8 i=0;
	NandHistoryRecord NHR;
	while(1)
	{
		W25Q16_Read(i*sizeof(NandHistoryRecord)+RECORDADDR,&NHR,sizeof(NandHistoryRecord));
		if(NHR.NextAddr==0xffffffff)break;
		else i++;
		Tos_TaskDelay(1);
		if(i>=0xff)break;
	}
	DeBug("打印记录索引%d",i,Infor_Warning);
	return i;
}
void Task_Record()
{
	static uint8 OldMin=0xff;
	NandHistoryRecord NHR;
	if((UIShowValue.DateTime.Min%UIShowValue.SaveValue.RecordCycle==0)|(OldMin==0xff))
	{	
	if(OldMin!=UIShowValue.DateTime.Min)if(UIShowValue.RecordCount<0xff)
	{
	BufferCoppy(&UIShowValue.DateTime,&NHR.HR.DT,sizeof(UIShowValue.DateTime));
	BufferCoppy(&UIShowValue.CH_Value,&NHR.HR.CH_Value,sizeof(UIShowValue.CH_Value));
	NHR.NextAddr=(uint32)UIShowValue.RecordCount*sizeof(NandHistoryRecord)+RECORDADDR+sizeof(NandHistoryRecord);
	SPI_FLASH_BufferWrite((uint8*)&NHR, (uint32)UIShowValue.RecordCount*sizeof(NandHistoryRecord)+RECORDADDR, sizeof(NandHistoryRecord));
	DeBug("此刻记录被存储[%d]",UIShowValue.RecordCount,Infor_Warning);
	UIShowValue.RecordCount++;
	}
	OldMin=UIShowValue.DateTime.Min;
	}
}
void Task_PrintfLog()
{
	uint8 i=0;
	NandHistoryRecord NHR;
	DeBug("用户:%s",&UIShowValue.SaveValue.PrintfName[0],Infor_Error);
	DeBug("标题:%s",&UIShowValue.SaveValue.PrintfTitle[0],Infor_Error);
	DeBug("********************************",Infor_Error);
	if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)DeBug("记录时间           温度   湿度  ",0);
	else DeBug("记录时间           温度   温度  ",0);
	while(1)
	{
		W25Q16_Read((uint32)i*sizeof(NandHistoryRecord)+RECORDADDR,&NHR,sizeof(NandHistoryRecord));
		if(NHR.NextAddr==0xffffffff)break;
		else {i++;DeBug("%02d-%02d-%02d %02d:%02d     %.2f   %.2f",NHR.HR.DT.Year,NHR.HR.DT.Mon,NHR.HR.DT.Day,NHR.HR.DT.Hour,NHR.HR.DT.Min,NHR.HR.CH_Value[0],NHR.HR.CH_Value[1],Infor_Error);}
		if(i%50==49)Tos_TaskDelay(4000);
		if(i>=0xff)break;
	}
	DeBug("********************************",Infor_Error);
	DeBug("                                ",Infor_Error);
	DeBug("签字:                     时间: ",Infor_Error);
	DeBug("                                ",Infor_Error);
	DeBug("                                ",Infor_Error);
	DeBug("                                ",Infor_Error);
	DeBug("备注:                           ",Infor_Error);
	DeBug("\r\n\r\n\r\n                    ",Infor_Error);
}
void Task0_Init()
{
 if(!BOOL_LoadConfigs())UIShowValue.SaveValue.DeBugCFG=0xff;
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(UIShowValue.SaveValue.DeBugCFG);//只打印Infor类型的消息
	Printf_SysInfor();
	UIShowValue.RunFlag=0;
	UIShowValue.SysWorkMode=0;
	UIShowValue.RecordCount=0;
	UIShowValue.CH_Hold[0].Max=-200;
	UIShowValue.CH_Hold[1].Max=-200;
	UIShowValue.CH_Hold[0].Min=200;
	UIShowValue.CH_Hold[1].Min=200;
//	if(HandKey_GetValue()==Key_UpDown)UIShowValue.SysWorkMode=0xff;
	E3V3_ON;
	GPS_OFF;
}
void Task_Plan()
{
	static uint8 i=0,c=0,b=0;
	static uint32 Pool;
	uint16 T;
	T=FlowPoolFilter(&Pool,UIShowValue.ADC[0],&b,10);
	UIShowValue.CH_Value[2]=T*2.5/2048;
	c++;
	if(c>20)
	{
		c--;
		if(UIShowValue.CH_Value[0]>UIShowValue.CH_Hold[0].Max)UIShowValue.CH_Hold[0].Max=UIShowValue.CH_Value[0];
		if(UIShowValue.CH_Value[1]>UIShowValue.CH_Hold[1].Max)UIShowValue.CH_Hold[1].Max=UIShowValue.CH_Value[1];
		if(UIShowValue.CH_Value[0]<UIShowValue.CH_Hold[0].Min)UIShowValue.CH_Hold[0].Min=UIShowValue.CH_Value[0];
		if(UIShowValue.CH_Value[1]<UIShowValue.CH_Hold[1].Min)UIShowValue.CH_Hold[1].Min=UIShowValue.CH_Value[1];
	}
	if(UIShowValue.RunFlag&WRF_SensorCH1Connected)
	{
	if(UIShowValue.CH_Value[0]>=UIShowValue.SaveValue.CH_Limit[0].Max)UIShowValue.RunFlag|=WRF_CH1Max;else UIShowValue.RunFlag&=~WRF_CH1Max;
	if(UIShowValue.CH_Value[0]<=UIShowValue.SaveValue.CH_Limit[0].Min)UIShowValue.RunFlag|=WRF_CH1Min;else UIShowValue.RunFlag&=~WRF_CH1Min;
	}
	if(UIShowValue.RunFlag&WRF_SensorCH2Connected)
	{
	if(UIShowValue.CH_Value[1]>=UIShowValue.SaveValue.CH_Limit[1].Max)UIShowValue.RunFlag|=WRF_CH2Max;else UIShowValue.RunFlag&=~WRF_CH2Max;
	if(UIShowValue.CH_Value[1]<=UIShowValue.SaveValue.CH_Limit[1].Min)UIShowValue.RunFlag|=WRF_CH2Min;else UIShowValue.RunFlag&=~WRF_CH2Min;
	}
	if(UIShowValue.CH_Value[2]<3.3)i++;
	else i=0;
	#ifndef BOARD_TP502T
	if(i>200)
		{
				BEEP_Double();
				GPIO_DeInit(GPIOA);
				GPIO_DeInit(GPIOB);
				GPIO_DeInit(GPIOC);
				GPIO_DeInit(GPIOD);
				GPIO_DeInit(GPIOE);
				Standby_Init();
		}
		#endif
}
void Task_HistoryRecord(void *Tags)
{
	uint32 i=0;
	NandHistoryRecord NHR;
	while(1)
	{
		W25Q16_Read(i*sizeof(NandHistoryRecord)+HISTORYADDR,&NHR,sizeof(NandHistoryRecord));
		if(NHR.NextAddr==0xffffffff)break;
		else i++;
		Tos_TaskDelay(1);
	}
	DeBug("记录索引%d",i,Infor_Warning);
	Tos_TaskDelay(5000);
	while(1)
	{
		Tos_TaskDelay(5*60000);
		BufferCoppy(&UIShowValue.DateTime,&NHR.HR.DT,sizeof(UIShowValue.DateTime));
		BufferCoppy(&UIShowValue.CH_Value,&NHR.HR.CH_Value,sizeof(UIShowValue.CH_Value));
		NHR.NextAddr=i*sizeof(NandHistoryRecord)+HISTORYADDR+sizeof(NandHistoryRecord);
		SPI_FLASH_BufferWrite((uint8*)&NHR, i*sizeof(NandHistoryRecord)+HISTORYADDR, sizeof(NandHistoryRecord));i++;
		DeBug("此刻数据被记录%d",i,Infor_Warning);
	}
}
void Task_PrintfHistory()
{
	uint32 i=0;
	NandHistoryRecord NHR;
	DeBug_Get();
	if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)DeBug("日期,温度,湿度",0);
	else DeBug("日期,温度,温度",0);
	while(1)
	{
	W25Q16_Read(i*sizeof(NandHistoryRecord)+HISTORYADDR,&NHR,sizeof(NandHistoryRecord));
	if(NHR.NextAddr==0xffffffff)break;
	else {i++;DeBug("20%02d-%02d-%02d %02d:%02d,%.2f,%.2f",NHR.HR.DT.Year,NHR.HR.DT.Mon,NHR.HR.DT.Day,NHR.HR.DT.Hour,NHR.HR.DT.Min,NHR.HR.CH_Value[0],NHR.HR.CH_Value[1],0);}
	Tos_TaskDelay(10);
	}
	DeBug_Drop();
}
void W25Q16_EraseThis(uint32 Addr,uint32 Length)
{
	uint32 i,n;
	i=Length/SPI_FLASH_PageSize;
	if(Length%SPI_FLASH_PageSize)i++;
	for(n=0;n<i;n++)
	{
		SPI_FLASH_SectorErase(Addr+SPI_FLASH_PageSize*n);
		Tos_TaskDelay(1);
		if(n%36==0)DeBug("擦除页%d/%d",n/36,i/36,0);
	}
}
void Task_CleanHistory()
{
	DeBug_Get();
	DeBug("开始清空记录\r\n",0);
	W25Q16_EraseThis(0,sizeof(NandHistoryRecord)*0xffff);
	DeBug("清除完毕\r\n",0);
	DeBug_Drop();
}
