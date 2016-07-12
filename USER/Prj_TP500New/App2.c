#include <..\USER\Prj_TP500New\App2.h>
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
void Task_LoadConfigs()
{
	Load_AppConfig(&UIShowValue.SaveValue,0);
}
void Task_ReBackConfigs()
{
	Load_AppConfig(&UIShowValue.SaveValue,1);
}
void Task_Record()
{
	static uint8 OldMin=0xff;
	if((UIShowValue.DateTime.Min%5==0)|(OldMin==0xff))
	{	
	if(OldMin!=UIShowValue.DateTime.Min)if(UIShowValue.RecordCount<0xff)
	{
	HistoryRecord HR;
	BufferCoppy(&UIShowValue.DateTime,&HR.DT,sizeof(UIShowValue.DateTime));
	BufferCoppy(&UIShowValue.CH_Value,&HR.CH_Value,sizeof(UIShowValue.CH_Value));
	SPI_FLASH_BufferWrite((uint8*)&HR, (uint32)UIShowValue.RecordCount*sizeof(HR)+RECORDADDR, sizeof(HR));
	UIShowValue.RecordCount++;
	}
	OldMin=UIShowValue.DateTime.Min;
	}
}
void Task_PrintfLog()
{
	uint8 i;
	HistoryRecord HR;
	DeBug("用户:%s",&UIShowValue.SaveValue.PrintfName[0],Infor_Error);
	DeBug("标题:%s",&UIShowValue.SaveValue.PrintfTitle[0],Infor_Error);
	DeBug("********************************",Infor_Error);
	DeBug("记录时间           温度   湿度  ",Infor_Error);
	for(i=0;i<UIShowValue.RecordCount;i++)
	{
		W25Q16_Read((uint32)i*sizeof(HR)+RECORDADDR,&HR,sizeof(HR));
		DeBug("%02d-%02d-%02d %02d:%02d     %.2f   %.2f",HR.DT.Year,HR.DT.Mon,HR.DT.Day,HR.DT.Hour,HR.DT.Min,HR.CH_Value[0],HR.CH_Value[1],Infor_Error);
		if(i%50==49)Tos_TaskDelay(4000);
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
