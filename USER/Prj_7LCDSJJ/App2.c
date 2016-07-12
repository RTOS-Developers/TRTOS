#include <Include.h>
#include <..\USER\Prj_7LCDSJJ\App1.h>
#include <..\USER\Prj_7LCDSJJ\App2.h>

void Load_Configs()
{
	FLASH_ReadDatas(Run_Value.SaveOffSet,(uint16 *)&Run_ValueRege,sizeof(Run_ValueRege)/2);
	 // AT24C08_Read(0x0000,(uint8 *)&Run_ValueRege,sizeof(Run_ValueRege));
}
void Save_Configs()
{
	if(Run_Value.OnlyRefresh==True)return;
	FLASH_WriteDatas(Run_Value.SaveOffSet,(uint16 *)&Run_ValueRege,sizeof(Run_ValueRege)/2);
	//AT24C08_Write(0x0000,(uint8 *)&Run_ValueRege,sizeof(Run_ValueRege));
}
void Task_SaveHome()
{
	uint8 Buf[50];
	Save_Configs();
	GetStringFromNumber(&Buf[0],Run_Value.LeftHeight,'D',2);
	DaCai_SetTextBox(0x00,0x06,&Buf[0],sizeof(Buf));
	GetStringFromNumber(&Buf[0],Run_Value.LeftWeight,'D',2);
	DaCai_SetTextBox(0x00,0x0a,&Buf[0],sizeof(Buf));
	DaCai_TextBox(0x00,23,"工作模式");
	DaCai_SetTextBox(0x00,0x0e,"上行",5);
	//DaCai_SetTextBox(0x00,0x11,"正常",5);
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_SaveRegDevice1()
{
	Save_Configs();
	DaCai_SetTextBox(0x0B,0x05,&Run_ValueRege.RecordNumber[0],sizeof(Run_ValueRege.RecordNumber));
	Tos_TaskDelay(10);
	DaCai_SetTextBox(0x0B,0x07,&Run_ValueRege.ProjectNumber[0],sizeof(Run_ValueRege.ProjectNumber));
	Tos_TaskDelay(10);
	DaCai_SetTextBox(0x0B,0x0b,&Run_ValueRege.LiftModel[0],sizeof(Run_ValueRege.LiftModel));
	Tos_TaskDelay(10);
	DaCai_SetTextBox(0x0B,0x0d,&Run_ValueRege.LiftNumber[0],sizeof(Run_ValueRege.LiftNumber));
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_SaveRegDevice2()
{
	Save_Configs();
	DaCai_SetTextBox(0x0C,0x05,&Run_ValueRege.BlockBoxNumber[0],sizeof(Run_ValueRege.BlockBoxNumber));
	Tos_TaskDelay(10);
	DaCai_SetTextBox(0x0C,0x07,&Run_ValueRege.HandVersion[0],sizeof(Run_ValueRege.HandVersion));
	Tos_TaskDelay(10);
	DaCai_SetTextBox(0x0C,0x0b,&Run_ValueRege.SoftVersion[0],sizeof(Run_ValueRege.SoftVersion));
	Tos_TaskDelay(10);
	DaCai_SetTextBox(0x0C,0x0d,&Run_ValueRege.SIMNumber[0],sizeof(Run_ValueRege.SIMNumber));
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_SaveCorrectHeight()
{ 
	uint8 Buf[50];
	Save_Configs();
	GetStringFromNumber(&Buf[0],Run_ValueRege.CorrectHeightV1,'D',2);
	DaCai_SetTextBox(0x05,0x0b,&Buf[0],sizeof(Buf));
	GetStringFromNumber(&Buf[0],Run_ValueRege.CorrectHeightV2,'D',2);
	DaCai_SetTextBox(0x05,0x0e,&Buf[0],sizeof(Buf));
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_SaveCorrectWeight()
{ 
	Save_Configs();
	DaCai_TextBox(0x06,0x0b,"%.2f",Run_ValueRege.CorrectWeightV1);
	DaCai_TextBox(0x06,0x0e,"%.2f",Run_ValueRege.CorrectWeightV2);
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_SetDateTime()
{ 
	ISL1208_GetDateTime(&Run_Value.SDT);
	DaCai_SetDateTime(&Run_Value.SDT);
  DaCai_TextBox(0x15,0x07,"%d",Run_Value.SDT.Year);
	DaCai_TextBox(0x15,0x0c,"%d",Run_Value.SDT.Mon);
	DaCai_TextBox(0x15,0x0e,"%d",Run_Value.SDT.Day);
	DaCai_TextBox(0x15,0x10,"%d",Run_Value.SDT.Hour);
	DaCai_TextBox(0x15,0x10,"%d",Run_Value.SDT.Min);
	DaCai_TextBox(0x15,0x12,"%d",Run_Value.SDT.Sec);
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_SaveDateTime()
{ 
	DaCai_SetDateTime(&Run_Value.SDT);
	ISL1208_SetDateTime(&Run_Value.SDT);
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_OutShowMaintenance()
{
	DaCai_ShowWidget(0x13,13,(uint8)((Run_Value.MaintenanceMode&S4)>0));
	DaCai_ShowWidget(0x13,15,(uint8)((Run_Value.MaintenanceMode&S3)>0));
	DaCai_ShowWidget(0x13,18,(uint8)((Run_Value.MaintenanceMode&S2)>0));
	DaCai_ShowWidget(0x13,20,(uint8)((Run_Value.MaintenanceMode&S1)>0));
	DaCai_ShowWidget(0x13,9,(uint8)((Run_Value.MaintenanceMode&S0)>0));
	DaCai_TextBox(0x13,23,"%d年%d月%d日",Run_ValueRege.AllCheckDate.Year,Run_ValueRege.AllCheckDate.Mon,Run_ValueRege.AllCheckDate.Day);
	DaCai_TextBox(0x13,26,"%d年%d月%d日",Run_ValueRege.AntiFailCheckDate.Year,Run_ValueRege.AntiFailCheckDate.Mon,Run_ValueRege.AntiFailCheckDate.Day);
}
void Task_OutSig()
{
	DaCai_ShowWidget(0x00,34,!(uint8)((Run_Value.Sig&S0)>0));
	DaCai_ShowWidget(0x00,35,!(uint8)((Run_Value.Sig&S1)>0));
	DaCai_ShowWidget(0x00,36,(uint8)((Run_Value.Sig&S2)>0));
	DaCai_ShowWidget(0x00,37,(uint8)((Run_Value.Sig&S3)>0));
	DaCai_SetTextBox(0x00,28,&Run_ValueRege.RecordNumber[0],sizeof(Run_ValueRege.RecordNumber));
if(Run_Value.WorkMode==0x01)
		{
			if(Run_Value.RunMode&S0)
			{
				DaCai_TextBox(0x00,23,"工作模式");
			}else DaCai_TextBox(0x00,23,"请进行面部识别");
		}else {DaCai_TextBox(0x00,23,"维保模式");}
		if(!(Run_Value.Sig&S0))DaCai_TextBox(0x00,0x0e,"上行");
		else if(!(Run_Value.Sig&S1))DaCai_TextBox(0x00,0x0e,"下行");
		else DaCai_TextBox(0x00,0x0e,"停止");
}
void Task_UpdataSig()
{ 
	char *Buf[2]={"开","关"};
  DaCai_TextBox(0x0f,5,Buf[(uint8)((Run_Value.Sig&S0)>0)]);
	DaCai_TextBox(0x0f,7,Buf[(uint8)((Run_Value.Sig&S1)>0)]);
	DaCai_TextBox(0x0f,11,Buf[(uint8)((Run_Value.Sig&S2)>0)]);
	DaCai_TextBox(0x0f,13,Buf[(uint8)((Run_Value.Sig&S3)>0)]);
	DaCai_TextBox(0x0f,20,Buf[(uint8)((Run_Value.Sig&S4)>0)]);
	SPK_OFF;
	Tos_TaskDelay(30);
	SPK_ON;
}
void Task_AttestationMode()
{
	if(CompareWithBuffer((uint8 *)&UserInfor.TempBuf[0],(uint8 *)"1888",5))
	{
		DaCai_SetPage(0x13);
		BufferFill((uint8 *)&UserInfor.TempBuf,0x00,sizeof(UserInfor.TempBuf));
		DaCai_TextBox(0x16,8,"");
	}
}
void Task_UpdataUserInfor()
{
	//while(1);
}
