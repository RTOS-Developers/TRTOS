#include <Include.h>
#include <..\Driver\ADCForTankHand_Driver.h>
#include <..\USER\Prj_TankHand\IO_Driver.h>
#include <..\USER\Prj_TankHand\KeyBoard_Driver.h>
#include <..\USER\Prj_TankHand\Oled_GUI.h>
#include <..\USER\Prj_TankHand\App1_TankHand.h>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.h>
#include <..\Driver\SZZigBee_Driver.h> 
#include<stdlib.h>
#include<time.h> 
#define LicRegCount 1000
DevRun_ValueStruct DevRun_Value;
//DevRun_ValueStruct DevRun_Value;
char *String_Wating="Wating";
/**************************************************************************************
 Func: 获取计算电池电压
 Time: 2015-1-10
 Ver.: V1.0
 Note; 
**************************************************************************************/
float Get_BatteryVoltage()
{
	uint16 Temp;
	float TempF;
	static uint8 i;
	static uint32 Pool;
	Temp=FlowPoolFilter(&Pool,ADCForTankHand_ReadFitel(2),&i,20);;
	TempF=Temp;
	TempF*=18.8;
	TempF/=4096;
	TempF+=0.96;
	return TempF;
}
void Beep_WithKey()
{
			SPK_ON;
			LED2_ON;
			Tos_TaskDelay(2);
			SPK_OFF;
			LED2_OFF;
}
void LampOutShow()
{
	if(DevRun_Value.AllStatus&WSA_TankPowerOn)LAMP1_OFF;else LAMP1_ON;
	if(DevRun_Value.AllStatus&WSA_SysPowerOn)LAMP2_OFF;else LAMP2_ON;
	if((DevRun_Value.AllStatus&WSA_TankPowerOn)&&(DevRun_Value.AllStatus&WSA_TankPowerCheckRealy))LAMP9_OFF;else LAMP9_ON;
	if(DevRun_Value.AllStatus&WSA_Relieve)LAMP4_OFF;else LAMP4_ON;
	if(DevRun_Value.AllStatus&WSA_TapOpenRealy)LAMP10_OFF;else LAMP10_ON;
	if(DevRun_Value.AllStatus&WSA_TapCloseRealy)LAMP11_OFF;else LAMP11_ON;
	if(DevRun_Value.AllStatus&WSA_SealedInPlace)LAMP12_OFF;else LAMP12_ON;
}
void StopSys_Doing()
{
	DevRun_Value.AllStatus&=~WSA_TankPowerCheckRealy;
	DevRun_Value.AllStatus&=~WSA_TapOpenRealy;
	DevRun_Value.AllStatus&=~WSA_TapCloseRealy;
	DevRun_Value.AllStatus&=~WSA_SealedInPlace;
}
/**************************************************************************************
 Func: 加载本地配置
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Load_Configs()
{
	Flash_ReadData(CONFIGS_ADDR_BASE,&Run_Value.SValue,sizeof(Run_Value.SValue));
	if(Run_Value.SValue.Brightness<50)Run_Value.SValue.Brightness=50;
}
void TankPowerClick()
{
	uint8 Count=5,Com=OC_TankPowerON;
	BOOL ON=True;
	if(DevRun_Value.AllStatus&WSA_TankPowerOn){ON=False;Com=OC_TankPowerOFF;}
	while(Count--)
	{
	Send_ButtonCommand(Com);
	Tos_TaskDelay(Count*100);
	if((DevRun_Value.AllStatus&WSA_TankPowerOn)==ON)
	{
		if(Com==OC_TankPowerOFF)StopSys_Doing();
		if(Run_Value.SValue.LicCount>0){Run_Value.SValue.LicCount--;Save_Configs();}
		return;
	}
	}
}
BOOL StartCheck()
{
	if(Run_Value.SValue.LicCount==-2)return True;
	if(Run_Value.SValue.LicCount>0){Run_Value.SValue.LicCount--;Save_Configs();return True;}
	else {Printf_Message("设备需要激活",1000);return False;}
}
char * NotLic="未注册";
void Cheack_Lic()
{
	if(Run_Value.SValue.LicCount>LicRegCount)Run_Value.SValue.LicCount=LicRegCount;
	switch(Run_Value.SValue.LicCount)
	{
		case 0:Run_Value.LicStatus="已过期";break;
		case -1:Run_Value.LicStatus="未注册";break;
		case -2:Run_Value.LicStatus="已注册";break;
		default:Run_Value.LicStatus="已激活";break;
	}
}
void Click_RegLic()
{
	uint8 i,key[5]={0};
  uint16 Temp,Temp1;
	BOOL IfOk;
	char *str="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	srand(Run_Value.SValue.PassKey);
	for(i=0;i<5;i++)key[i] = str[rand()%58];
	Printf_Message((char *)&key[0],2000);
	Temp1=Tools_GetCRC16(key,sizeof(key))%9999;
  Temp=Page_IfGetData(0,&IfOk);
  if(!IfOk)return;
  if(Temp==Temp1)
	{
		Run_Value.SValue.LicCount=LicRegCount;
		Run_Value.SValue.PassKey++;
		Save_Configs();
	}
	Temp1=Tools_GetCRC16(key,sizeof(key)-1)%9999;
	if(Temp1==Temp)
	{
		Run_Value.SValue.LicCount=-2;
		Run_Value.SValue.PassKey++;
		Save_Configs();
	}
}
void TankRelieveClick()
{
	uint8 Count=10,Com=OC_Relieve;
	BOOL Relieve=True;
	if(DevRun_Value.AllStatus&WSA_Relieve){Relieve=False;Com=OC_UnRelieve;}
	while(Count--)
	{
	Send_ButtonCommand(Com);
	Tos_TaskDelay(100);
	if((DevRun_Value.AllStatus&WSA_Relieve)==Relieve)return;
	}
	
}
/**************************************************************************************
 Func: 保存本地配置
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Save_Configs()
{
	Printf_Waiting(String_Wating,10);
	Flash_WriteData(CONFIGS_ADDR_BASE,&Run_Value.SValue,sizeof(Run_Value.SValue));
	Set_Brightness();
}
void Save_Consig()
{
	Flash_WriteData(CONFIGS_ADDR_BASE,&Run_Value.SValue,sizeof(Run_Value.SValue));
}
/**************************************************************************************
 Func: 设置亮度
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Set_Brightness()
{
	SSD1325_SET_Light(Run_Value.SValue.Brightness);
}
void Send_Msg(uint16 Addr,uint8 TaskFlag,uint8 *Buf,uint8 Length)
{
	DeBug_Get();
	Length=ZigBee_SendMsg(USART_TX_Buffer[0],Addr,TaskFlag,Buf,Length);
	USART_WriteDatas(USART1,&USART_TX_Buffer[0][2],Length-2);//新模块
}
/**************************************************************************************
 Func: 发送命令通过Zigbee
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Send_Message(uint8 TaskFlag,uint8 *Buf,uint8 Length)
{
	Send_Msg(Run_Value.SValue.BusAddr,TaskFlag,Buf,Length);
}
uint8 SleepCount=0;

BOOL CheckPowerLow(float Vol)
{
	if((Vol<9.5)&(Vol>8))
	{
	SleepCount++;
	SPK_ON;
	Tos_TaskDelay(5);
	SPK_OFF;
		if(SleepCount>=Run_Value.SValue.TimeSleep)return True;
		if(SleepCount%2==0)LAMP14_OFF;
		else LAMP14_ON;
	}else LAMP14_OFF;
	return False;
}
/**************************************************************************************
 Func: 远程保
存设置
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Click_SaveValue()
{
	if(Admin_GetRoot()){Send_Message(SCFF_SetSaveValue,Null,Null);Printf_Waiting(String_Wating,10);}
	else Printf_InforFail();
}
void Click_ReadValue()
{
	Send_Message(SCFF_SetReadValue,Null,Null);
	Printf_Waiting(String_Wating,10);
}

/**************************************************************************************
 Func: 标定重量
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Click_CorrectWeight()
{
	float Temp=0;
	BOOL IfOk;
	uint8 Length=3,Buf[10];
	Temp=Page_IfGetData(DevRun_Value.CROSS_DATA[0][1].Sensor,&IfOk);
	if(!IfOk)return;
	Length=Float32ToByte(&Buf[0],Temp);
	Send_Message(SCFF_CorrectWeight,&Buf[0],Length);
	Printf_Waiting(String_Wating,10);
	PrintfVoid("ADC %d",DevRun_Value.ADC_Value[0]);
}
/**************************************************************************************
 Func: 清零重量
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Click_ClearWeight()
{
	uint8 Length,Buf[10];
	Length=Float32ToByte(&Buf[0],0);
	Send_Message(SCFF_ClearWeight,&Buf[0],Length);
	Printf_Waiting(String_Wating,10);
	PrintfVoid("ADC=%d",DevRun_Value.ADC_Value[0]);
}
void Click_ResetFrq()
{
	if(Send_ButtonCommand(15))Printf_Waiting(String_Wating,10);
}
/**************************************************************************************
 Func: 标定距离
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Click_CorrectRange()
{
	float Temp=0;
	uint8 Length,Buf[10];
	BOOL IfOk;
	Buf[0]=1;
	Temp=Page_IfGetData(DevRun_Value.CROSS_DATA[1][1].Sensor,&IfOk);
	if(!IfOk)return;
	Length=Float32ToByte(&Buf[0],Temp);
	Send_Message(SCFF_CorrectRange,&Buf[0],Length);
	Printf_Waiting(String_Wating,10);
	PrintfVoid("ADC=%d",DevRun_Value.ADC_Value[1]);
}
/**************************************************************************************
 Func: 设定额定重量
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Click_CorrectFullWeight()
{
	uint8 Temp=0;
	uint8 Buf[10];
	BOOL IfOk;
	Buf[0]=1;
	Temp=Page_IfGetData(DevRun_Value.FullWeight,&IfOk);
	if(!IfOk)return;
	Buf[0]=Temp;
	Send_Message(SCFF_SetFullWeight,&Buf[0],1);
	Printf_Waiting(String_Wating,10);
}
/**************************************************************************************
 Func: 清零距离
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Click_ClearRange()
{
	float Temp=0;
	BOOL IfOk;
	uint8 Length,Buf[10];
	Temp=Page_IfGetData(DevRun_Value.CROSS_DATA[1][0].Sensor,&IfOk);
	if(!IfOk)return;
	Length=Float32ToByte(&Buf[0],Temp);
	Send_Message(SCFF_ClearRange,&Buf[0],Length);
	Printf_Waiting(String_Wating,10);;
	PrintfVoid("ADC=%d",DevRun_Value.ADC_Value[1]);
}
BOOL Admin_GetRoot()
{
	float Temp;
	BOOL IfOk;
	Printf_Message("输入密码",1000);
	Temp=Page_IfGetData(0,&IfOk);
	if(!IfOk)return False;
	if(Temp==Run_Value.SValue.PassWord)return True;
	return False;
}
void Click_ChangePassword()
{
	float Temp,Temp2;
	BOOL IfOk;
	//if(!Printf_YesOrNo("输入原密码"))return;
	Printf_Message("输入原密码",1000);
	Temp=Page_IfGetData(0,&IfOk);
	if(!IfOk)return;
	if(!(Temp==Run_Value.SValue.PassWord))
	{
		Printf_InforFail();
		return;
	}
//	if(!Printf_YesOrNo("输入新密码"))return;
	Printf_Message("输入新密码",1000);
	Temp=Page_IfGetData(Run_Value.SValue.PassWord,&IfOk);
	if(!IfOk)return;
	//if(!Printf_YesOrNo("再输入新密码"))return;
	Printf_Message("再输入新密码",1000);
	Temp2=Page_IfGetData(Run_Value.SValue.PassWord,&IfOk);
	if(!IfOk)return;
	if(Temp==Temp2){Run_Value.SValue.PassWord=Temp;Save_Configs();}
	else Printf_InforFail();
}
void Click_SetPassword()
{
	float Temp,Temp2;
	BOOL IfOk;
	//if(!Printf_YesOrNo("输入管理员密码"))return;
	Printf_Message("输入管理员密码",1000);
	Temp=Page_IfGetData(0,&IfOk);
	if(!IfOk)return;
	if(!(Temp==1888))
	{
		Printf_InforFail();
		return;
	}
	//if(!Printf_YesOrNo("输入新密码"))return;
	Printf_Message("输入新密码",1000);
	Temp=Page_IfGetData(Run_Value.SValue.PassWord,&IfOk);
	if(!IfOk)return;
	//if(!Printf_YesOrNo("再输入新密码"))return;
	Printf_Message("再输入新密码",1000);
	Temp2=Page_IfGetData(Run_Value.SValue.PassWord,&IfOk);
	if(!IfOk)return;
	if(Temp==Temp2){Run_Value.SValue.PassWord=Temp;Save_Configs();}
	else Printf_InforFail();
}
void Draw_Battery(uint16 x,uint16 y,uint8 Vol)
{
	uint8 i;
	TGUI_Draw_Rectangle(x,y,x+12,y+7,1);
	TGui_Draw_Line(x+13,y+2,x+13,y+5,1);
	for(i=0;i<Vol;i++)
	TGui_Draw_Line(x+2+i*2,y+2,x+2+i*2,y+5,1);
}
/**************************************************************************************
 Func: 绘制页眉和页脚
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Draw_Home()
{
	
	Draw_Menu();
	TGui_Draw_Line(0,50,127,50,1);
	SetDraw_Mode(WDM_Normal);
	Cheack_Lic();
	Draw_Battery(58,55,Opera_WhithADC(Run_Value.Voltage,9,0,12,5,5));
	TGUI_Set_Region(0,0,LCD_XSIZE-1,LCD_YSIZE-12,Align_LeftBottom);
	Printf("启动");
	TGUI_Set_Region(0,0,LCD_XSIZE-1,LCD_YSIZE-12,Align_RightBottom);
	Printf("设置");
}
/**************************************************************************************
 Func: 发送指令通过zigbee
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
BOOL Send_ButtonCommand(uint8 Com)
{
	uint8 Buf[2];
	Buf[0]=Com;
	Send_Message(SCFF_ButtonClick,&Buf[0],1);
	return True;
}
void Send_OnlyCommand(uint8 Cmd)
{
	uint8 Buf[2];
	Send_Message(Cmd,&Buf[0],0);
}
void Draw_Tank(uint16 x,uint16 y,uint32 TankRealy,uint32 PowerRealy)
{
	TGui_Draw_Line(x-4,y,x+4,y,1);//车底板
	TGui_Draw_Line(x-4,y,x-4,y-2,1);//车左挡板
	TGui_Draw_Line(x+4,y,x+4,y-2,1);//车右挡板
	TGui_Draw_Line(x+3,y,x+3,y+1,1);//车右轮
	TGui_Draw_Line(x-3,y,x-3,y+1,1);//车左轮
	if(TankRealy)
	{
		TGui_Draw_Line(x-5,y-3,x+5,y-3,1);//罐底板
	//	TGui_Draw_Line(x-2,y-2,x+2,y-2,1);//罐三相电接触器
		TGui_Draw_Line(x-5,y-3,x-5,y-6,1);//罐左壁
		TGui_Draw_Line(x+5,y-3,x+5,y-6,1);//罐右壁
		
		TGui_Draw_Line(x+2,y-3,x+2,y-2,1);//罐三相电接触器
	  TGui_Draw_Line(x-2,y-3,x-2,y-2,1);//罐三相电接触器
		if(PowerRealy)
	{
		TGui_Draw_Line(x+2,y-1,x+2,y,1);//罐三相电接触器
	  TGui_Draw_Line(x-2,y-1,x-2,y,1);//罐三相电接触器
	}
	}
	
}

/**************************************************************************************
 Func: 绘制页眉和页脚
 Time: 2015-1-10
 Ver.: V1.0
 Note;
**************************************************************************************/
void Draw_Work()
{
	
	CurStyle->RowCount=3;
	CurStyle->ColumnCount=2;
	CurStyle->CursorEnable=False;
	Draw_Menu();
	Cheack_Lic();
	TGui_Draw_Line(0,50,127,50,1);
	Draw_Battery(58,55,Opera_WhithADC(Run_Value.Voltage,9,0,12,5,5));
	Draw_Tank(40,62,(DevRun_Value.AllStatus&WSA_Tank1Place),(DevRun_Value.AllStatus&WSA_TankPowerOn));
	Draw_Tank(88,62,(DevRun_Value.AllStatus&WSA_Tank2Place),(DevRun_Value.AllStatus&WSA_TankPowerOn));
	SetDraw_Mode(WDM_Normal);
	TGUI_Set_Region(0,0,LCD_XSIZE-1,LCD_YSIZE-12,Align_LeftBottom);
	Printf("停止");
	TGUI_Set_Region(0,0,LCD_XSIZE-1,LCD_YSIZE-12,Align_RightBottom);
	if(WSA_SysError&DevRun_Value.AllStatus)Printf("故障");
	else Printf("工作");
}
