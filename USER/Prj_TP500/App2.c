#include <..\USER\Prj_TP500\App2.h>
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
void Task_SaveConfigs()
{
	UIShowValue.SaveValue.CH.Size=sizeof(UIShowValue.SaveValue);
	Save_AppConfig(&UIShowValue.SaveValue,0);
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
void Task_PrintfLog()
{
	DeBug_OutSet(DeBug_USART1);//调试信息通过串口打印
	DeBug_SetType(Infor_Infor);//
	DeBug_Get();
	RTC_Get();
	DeBug("用户:%s",&UIShowValue.SaveValue.PrintfName[0],Infor_Infor);
	DeBug("标题:%s",&UIShowValue.SaveValue.PrintfTitle[0],Infor_Infor);
	DeBug("********************************",Infor_Infor);
	DeBug("温度【1】:%.2f℃",UIShowValue.CH_Value[0],Infor_Infor);
	DeBug("湿度【1】:%.2f℃",UIShowValue.CH_Value[1],Infor_Infor);
	DeBug("********************************",Infor_Infor);
	DeBug("记录时间:%d-%d-%d %d:%d:%d",Timer.w_year,Timer.w_month,Timer.w_date,Timer.hour,Timer.min,Timer.sec,Infor_Infor);
	DeBug("                                ",Infor_Infor);
	DeBug("                                ",Infor_Infor);
	DeBug("                                ",Infor_Infor);
	DeBug_SetType(Infor_Error);
}
