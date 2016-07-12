#include <Include.h>
#include <..\USER\Prj_7LCDSJJ\App1.h>
Run_ValueRegeStruct Run_ValueRege;
const LCD_MenuStruct MenuConst[]=
{
	{0xEEB1110013000210,(void *)&GPRS_MaintenanceStop,VT_Task,Null},//维保完成
	{0xEEB1110013001910,(void *)&Task_OutShowMaintenance,VT_Task,Null},//维保完成
	{0xEEB1110013000110,(void *)&GPRS_MaintenanceStart,VT_Task,Null},//开始维保
	{0xEEB1110013000d10,(void *)&Run_Value.MaintenanceMode,VT_CheckBit,MT_AllCheck},//整机复检
  {0xEEB1110013000f10,(void *)&Run_Value.MaintenanceMode,VT_CheckBit,MT_AntiFailCheck},//防坠器维保
  {0xEEB1110013001210,(void *)&Run_Value.MaintenanceMode,VT_CheckBit,MT_YearCheck},//年度维保
  {0xEEB1110013001410,(void *)&Run_Value.MaintenanceMode,VT_CheckBit,MT_DayCheck},//日常维保
  {0xEEB1110013000910,(void *)&Run_Value.MaintenanceMode,VT_CheckBit,MT_AddCheck},//加节维保
	{0xEEB111000B001310,(void *)&Task_SaveRegDevice1,VT_Task,Null},
	{0xEEB111000C001610,(void *)&Task_SaveRegDevice2,VT_Task,Null},
	{0xEEB1110005001010,(void *)&Task_SaveCorrectHeight,VT_Task,Null},
	{0xEEB1110006001010,(void *)&Task_SaveCorrectWeight,VT_Task,Null},
	{0xEEB1110015000910,(void *)&Task_SaveDateTime,VT_Task,Null},
  {0xEEB1110001000e10,(void *)&Task_SetDateTime,VT_Task,Null},
  {0xEEB111000f001710,(void *)&Task_UpdataSig,VT_Task,Null},
  {0xEEB1110017000410,(void *)&Task_UpdataUserInfor,VT_Task,Null},
  {0xEEB1110016000410,(void *)&Task_AttestationMode,VT_Task,Null},
	{0xEEB111000B000511,(void *)&Run_ValueRege.RecordNumber,VT_Str,sizeof(Run_ValueRege.RecordNumber)},
	{0xEEB111000B000711,(void *)&Run_ValueRege.ProjectNumber,VT_Str,sizeof(Run_ValueRege.ProjectNumber)},
  {0xEEB111000B000B11,(void *)&Run_ValueRege.LiftModel,VT_Str,sizeof(Run_ValueRege.LiftModel)},
  {0xEEB111000B000D11,(void *)&Run_ValueRege.LiftNumber,VT_Str,sizeof(Run_ValueRege.LiftNumber)},
  {0xEEB111000C000511,(void *)&Run_ValueRege.BlockBoxNumber,VT_Str,sizeof(Run_ValueRege.BlockBoxNumber)},
  {0xEEB111000C000711,(void *)&Run_ValueRege.HandVersion,VT_Str,sizeof(Run_ValueRege.HandVersion)},
	{0xEEB111000C000b11,(void *)&Run_ValueRege.SoftVersion,VT_Str,sizeof(Run_ValueRege.SoftVersion)},
  {0xEEB111000C000d11,(void *)&Run_ValueRege.SIMNumber,VT_Str,sizeof(Run_ValueRege.SIMNumber)},
	{0xEEB1110005000b11,(void *)&Run_ValueRege.CorrectHeightV1,VT_Float,sizeof(Run_ValueRege.CorrectHeightV1)},
	{0xEEB1110005000e11,(void *)&Run_ValueRege.CorrectHeightV2,VT_Float,sizeof(Run_ValueRege.CorrectHeightV2)},
	{0xEEB1110006000b11,(void *)&Run_ValueRege.CorrectWeightV1,VT_Float,sizeof(Run_ValueRege.CorrectWeightV1)},
	{0xEEB1110006000e11,(void *)&Run_ValueRege.CorrectWeightV2,VT_Float,sizeof(Run_ValueRege.CorrectWeightV2)},
  {0xEEB1110015000711,(void *)&Run_Value.SDT.Year,VT_Uint8,sizeof(Run_Value.SDT.Year)},
  {0xEEB1110015000c11,(void *)&Run_Value.SDT.Mon,VT_Uint8,sizeof(Run_Value.SDT.Mon)},
  {0xEEB1110015000e11,(void *)&Run_Value.SDT.Day,VT_Uint8,sizeof(Run_Value.SDT.Day)},
  {0xEEB1110015000a11,(void *)&Run_Value.SDT.Hour,VT_Uint8,sizeof(Run_Value.SDT.Hour)},
	{0xEEB1110015001011,(void *)&Run_Value.SDT.Min,VT_Uint8,sizeof(Run_Value.SDT.Min)},
	{0xEEB1110015001211,(void *)&Run_Value.SDT.Sec,VT_Uint8,sizeof(Run_Value.SDT.Sec)},
	{0xEEB1110016000811,(void *)&UserInfor.TempBuf,VT_Str,sizeof(UserInfor.TempBuf)},
	{0x00},
};

void EnterTask(const LCD_MenuStruct *Menu,uint8 *Addr)
{
typedef   void (*TaskFunc)(uint8 d);
	switch(Menu->Type)
	{
			case VT_Task:if(Menu->Value)((Func)Menu->Value)();break;  
		  case VT_Str:ClearWithBuffer(Menu->Value,Menu->Note);BufferCoppy(Addr,Menu->Value,Menu->Note);break;  
			case VT_Float:*(float *)(Menu->Value)=GetStringToNumber(Addr);break;
		  case VT_Uint8:*(uint8 *)(Menu->Value)=GetStringToNumber(Addr);break;
			case VT_TaskUint8:if(Menu->Value)((TaskFunc)Menu->Value)(Menu->Note);break; 
			case VT_CheckBit:if(Addr[1])(*(uint8 *)(Menu->Value))|=BIT(Menu->Note);else (*(uint8 *)(Menu->Value))&=~BIT(Menu->Note);break;
	}
}
uint64_t BufToUint64(uint8 *D)
{
	uint8 n=8;
	uint64_t Data=0;
	while(n--){Data<<=8;Data|=*D++;}
	return Data;
}

void WhileWrite()
{
	uint8 i=0;
	while(1)
	{
	Tos_TaskDelay(100);
	Task_SaveHome();
	Task_SaveRegDevice1();
	Task_SaveRegDevice2();
	Task_SaveCorrectHeight();
	Task_SaveCorrectWeight();
	Task_SetDateTime();
		if(i++>10)return;
	}
}
void LCD_Command(void *Tags)
{
	uint8 i=0;
	uint64_t Data;
	Run_Value.SaveOffSet=FlashConfigNull;
	AT24C08_Init();
	FlashGetConfigRoom(&Run_Value.SaveOffSet,sizeof(Run_ValueRege)/2);
	Load_Configs();
	Tos_TaskDelay(1000);
	Run_Value.OnlyRefresh=True;
	WhileWrite();
	Run_Value.OnlyRefresh=False;
	while(1)
	{
		Tos_TaskDelay(1);
		DeBug_Get();
		ClearWithBuffer(&USART1_RX_Buffer[0],100);
  	USART1_ReadDatas(Null,100,0);
		Data=BufToUint64(&USART1_RX_Buffer[0]);
		i=0;
		while(MenuConst[i].Key)
		{
			if(MenuConst[i].Key==Data)EnterTask(&MenuConst[i],&USART1_RX_Buffer[8]);i++;
		}
	}
}
