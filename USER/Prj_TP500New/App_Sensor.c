#include <Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <App_Sensor.h>
#include <..\USER\Prj_TP500New\Sensor_Driver.h>
#include <..\USER\Prj_TP500New\App2.h>
#include <isl1208_driver.h>
char *String_Ok="OK";
uint8 tem;
extern void BEEP_Double(void);
const char *BoolString[2]={"False","True "};//不能放在子程序里，会出现未知错误
const COM_MenuStruct MenuList[]={
	{"保存配置",(void *)&Task_SaveConfigs,VT_Task,Null},
	{"读取配置",(void *)&Task_LoadConfigs,VT_Task,Null},
	{"重启设备",(void *)&MCU_Reset,VT_Task,Null},
	{"蜂鸣器测试",(void *)&Task_BeepDouble,VT_Task,Null},
	{"电话号码:%s",(void *)&UIShowValue.SaveValue.CallNumber[0][0],VT_Str,sizeof(UIShowValue.SaveValue.CallNumber[0])},
	{"主机[1]:%s",(void *)&UIShowValue.SaveValue.DomainName[0][0],VT_Str,sizeof(UIShowValue.SaveValue.DomainName[0])},
	{"服务端口:%d",(void *)&UIShowValue.SaveValue.ServerPort[0],VT_Uint16,sizeof(UIShowValue.SaveValue.ServerPort[0])},
	{"状态位:%d",(void *)&UIShowValue.SaveValue.WorkBit,VT_ROMUint32,sizeof(UIShowValue.SaveValue.WorkBit)},
	{"登陆包:%s",(void *)&UIShowValue.SaveValue.LoginHead[0],VT_Str,sizeof(UIShowValue.SaveValue.LoginHead)},
	{"打印名:%s",(void *)&UIShowValue.SaveValue.PrintfName[0],VT_Str,sizeof(UIShowValue.SaveValue.PrintfName)},
	{"打印标题:%s",(void *)&UIShowValue.SaveValue.PrintfTitle[0],VT_Str,sizeof(UIShowValue.SaveValue.PrintfTitle)},
	{"心跳包:%s",(void *)&UIShowValue.SaveValue.Heartbeat[0],VT_Str,sizeof(UIShowValue.SaveValue.Heartbeat)},
	{"温度上限:%.2f℃",(void *)&UIShowValue.SaveValue.CH_Limit[0].Max,VT_Float,Null},
	{"温度下限:%.2f℃",(void *)&UIShowValue.SaveValue.CH_Limit[0].Min,VT_Float,Null},
	{"湿度上限:%.2f℃",(void *)&UIShowValue.SaveValue.CH_Limit[1].Max,VT_Float,Null},
	{"湿度下限:%.2f℃",(void *)&UIShowValue.SaveValue.CH_Limit[1].Min,VT_Float,Null},
	{"菜单",Null,VT_End,Null},
};
uint8 Menu_OptionLength(const COM_MenuStruct *CurMenu)
{
	uint8 Length=0;
	const COM_MenuStruct *Menu=CurMenu;
	while(Menu->Type!=VT_End)
	{
		Menu++;
		Length++;
	}
	return Length;
}
/*******************************************************************************
Func:菜单项事件
Date:2016-3-25
Note:
*******************************************************************************/

uint8 COM_Menu_Enter(uint8 Index,uint8 *Buf)
{
	typedef   void 	(*FuncUint8)(uint8);
	const COM_MenuStruct *CurMenu;
	if(Index>=Menu_OptionLength(MenuList))return 0;
	CurMenu=&MenuList[Index];
	USART_WriteDatas(USART2,&Buf[0],20);
	switch(CurMenu->Type)
	{
  		case VT_Task:if(CurMenu->Value)((Func)CurMenu->Value)();break;
//		case VT_TaskUint8:if(CurMenu->Value)((FuncUint8)CurMenu->Value)(CurMenu->Note);break;
		  case VT_Uint8:*(uint8 *)(CurMenu->Value)=GetStringToNumber(Buf);break;
  		case VT_Int8:*(int8 *)(CurMenu->Value)=GetStringToNumber(Buf);break;
  		case VT_Uint16:*(uint16 *)(CurMenu->Value)=GetStringToNumber(Buf);break;
			case VT_ROMUint32:*(uint32 *)(CurMenu->Value)=GetStringToNumber(Buf);break;
			case VT_Float:*(float *)(CurMenu->Value)=GetStringToNumber(Buf);break;
			case VT_BOOL:*(uint8 *)(CurMenu->Value)=!(BOOL)*(uint8 *)(CurMenu->Value);break;
//		case VT_IP:*(uint32 *)(CurMenu->Value)=MUI_GetIP(*(uint32 *)(CurMenu->Value),IT_Num|IT_Dot);break;
//		case VT_Color:*(uint16 *)(CurMenu->Value)=MUI_GetColor(*(uint16 *)(CurMenu->Value),IT_Num|IT_Dot);break;
		case VT_Str:BufferCoppy(&Buf[0],(uint8 *)(CurMenu->Value),CurMenu->Note);break;
	}
	return 1;
}
void MUI_GetOptionString(char *Buf,const COM_MenuStruct *Menu)
{
	uint8 BufT[100];
	typedef   uint8 	(*Uint8Func)(void);
	if(Menu->Note){BufferCoppy((uint8 *)(Menu->Value),&BufT[0],Menu->Note);BufT[Menu->Note]=0x00;}
	else StringCoppy((uint8 *)(Menu->Value),&BufT[0]);
	switch(Menu->Type)
		{
			case VT_Menu:FmtToBuf(Buf,(char *)Menu->Option);break;
			case VT_Uint8:FmtToBuf(Buf,(char *)Menu->Option,*(uint8 *)(Menu->Value));break;
			case VT_Uint16:FmtToBuf(Buf,(char *)Menu->Option,*(uint16 *)(Menu->Value));break;
			case VT_Float:FmtToBuf(Buf,(char *)Menu->Option,*(float *)(Menu->Value));break;
			case VT_ROMUint8:FmtToBuf(Buf,(char *)Menu->Option,*(uint8 *)(Menu->Value));break;
			case VT_ROMStr:FmtToBuf(Buf,(char *)Menu->Option,(uint8 *)(Menu->Value));break;
			case VT_Str:FmtToBuf(Buf,(char *)Menu->Option,(uint8 *)(BufT));break;
			case VT_ROMUint32:FmtToBuf(Buf,(char *)Menu->Option,*(uint32 *)(Menu->Value));break;
		  case VT_Uint8Task:FmtToBuf(Buf,(char *)Menu->Option,((Uint8Func)Menu->Value)());break;
			case VT_BOOL:FmtToBuf(Buf,(char *)Menu->Option,BoolString[*(BOOL*)(Menu->Value)]);break;
			case VT_Int8:FmtToBuf(Buf,(char *)Menu->Option,*(int8 *)(Menu->Value));break;
			case VT_IP:FmtToBuf(Buf,(char *)Menu->Option,*(uint8 *)((uint32)Menu->Value+3),*(uint8 *)((uint32)Menu->Value+2),*(uint8 *)((uint32)Menu->Value+1),*(uint8 *)((uint32)Menu->Value));break;
			case VT_Color:FmtToBuf(Buf,(char *)Menu->Option,TFT_RGB(*(uint32 *)(Menu->Value)).R,TFT_RGB(*(uint32 *)(Menu->Value)).G,TFT_RGB(*(uint32 *)(Menu->Value)).B);break;
			default:FmtToBuf(Buf,(char *)Menu->Option);break;
		}
	
}

/*******************************************************************************
Func:菜单项回传
Date:2016-3-25
Note:
*******************************************************************************/
uint8 COM_Menu_Load(uint8 Index,uint8 *Menu)
{
	uint8 Length;
	if(Index>=Menu_OptionLength(MenuList))return 0;
	MUI_GetOptionString((char *)Menu,&MenuList[Index]);
	Length=GetTextLength(Menu);
	return Length;
}


void Sensor_Command(void *Tags)
{
	uint16 Length;
	uint8 Buf[200],DataStart;
	ModBus_Message ModBusMsg;
	USART_TypeDef *USARTx=(USART_TypeDef *)Tags;
	uint8 UsartCH=USART_GetIndex(USARTx);
	ModBus_Create(&ModBusMsg,0x01,0x05,WorkMode_Slave,MBCM_Crc);
	while(1)
	{
	//	DeBug_Drop();
		ClearWithBuffer(USART_RX_Buffer[0],50);
		Length=USART_ReadDatas(USARTx,Null,50,0);
	//	DeBug_Get();
		Length=ModBus_Expend(USART_RX_Buffer[0],Length,&ModBusMsg);
		DataStart=Length;
		if(ModBusMsg.ErrorFlag==ModBus_Ok)
		{
			switch(ModBusMsg.MsgFlag)
			{
				case SCFF_GetHandVersion:
						 Length=StringCoppy(SensorHandVersion,&Buf[0]);
						 break;
				case SCFF_GetSoftVersion:
						 Length=StringCoppy(SensorSoftVersion,&Buf[0]);
						 break;
				case SCFF_GetSystemInfor:
						 Length=StringCoppy(SystemInfor,&Buf[0]);
						 break;
				case SCFF_GetProgTime:
						 Length=StringCoppy(__DATE__" - "__TIME__,&Buf[0]);
						 break;
				case SCFF_GetLibVersion:
						 FmtToBuf((char *)&Buf[0],"V %d.%d.%d n",__STM32F10X_STDPERIPH_VERSION_MAIN,__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
						 Length=10;
						 break;
				case SCFF_GetFlashSize:
						 FmtToBuf((char *)&Buf[0],"%dKb",Get_FlashSize());
						 Length=5;
						 break;
				case SCFF_GetSensorModel:
						 Length=StringCoppy(SensorModel,&Buf[0]);
						 break;
				case SCFF_GetSensorName:
						 Length=StringCoppy(SensorName,&Buf[0]);
						 break;
				case SCFF_GetSensorNote:
						 Length=StringCoppy(SensorDefNote,&Buf[0]);
						 break;
				case SCFF_GetSensorTest:
					   Length=StringCoppy("Hello",&Buf[0]);
						 break;
				case SCFF_GetSensorNumber:
					   Length=StringCoppy(SensorDefNumber,&Buf[0]);
						 break;
				case SCFF_GetCPUModel:
						 Length=StringCoppy(CPU_Model,&Buf[0]);
					   break;
				case SCFF_GetSensorValue:
					   Length=Float32ToByte(&Buf[0],UIShowValue.CH_Value[0]);
						 Length+=Float32ToByte(&Buf[Length],UIShowValue.CH_Value[1]);
						 break;
				case SCFF_SetDeBugMode:
						 DeBug_SetType(USART_RX_Buffer[0][DataStart]);
						 Length=StringCoppy(String_Ok,&Buf[0]);
						 break;
				case SCFF_SetReg:
						 Ht1621_ShowASCII(USART_RX_Buffer[0][DataStart],USART_RX_Buffer[0][DataStart+1]);
				     Length=StringCoppy(String_Ok,&Buf[0]);
						 break;
				case SCFF_GetAlarmList:			
						 Length=Float32ToByte(&Buf[0],UIShowValue.SaveValue.CH_Limit[USART_RX_Buffer[0][DataStart]].Max);
						 Length+=Float32ToByte(&Buf[Length],UIShowValue.SaveValue.CH_Limit[USART_RX_Buffer[0][DataStart]].Min);				
						 break;
				case SCFF_SetAlarmList:			
						 UIShowValue.SaveValue.CH_Limit[USART_RX_Buffer[0][DataStart]].Max=Float32FromByte(&USART_RX_Buffer[0][DataStart+1]);
						 UIShowValue.SaveValue.CH_Limit[USART_RX_Buffer[0][DataStart]].Min=Float32FromByte(&USART_RX_Buffer[0][DataStart+5]);
						 Length=StringCoppy(String_Ok,&Buf[0]);
						 Task_SaveConfigs();				
						 break;
				case SCFF_GetIpPortList:	
						 Length=IpPortToString(&UIShowValue.SaveValue.IPPort[USART_RX_Buffer[0][DataStart]],&Buf[0]);					
						 break;
				case SCFF_SetIpPortList:	
						 IpPortFromString(&UIShowValue.SaveValue.IPPort[USART_RX_Buffer[0][DataStart]],&USART_RX_Buffer[0][DataStart+1]);	
						 Length=StringCoppy(String_Ok,&Buf[0]);		
						 Task_SaveConfigs();
						 break;
				case SCFF_GetCallList:
						 Length=BufferCoppy(&UIShowValue.SaveValue.CallNumber[USART_RX_Buffer[0][DataStart]][0],&Buf[0],sizeof(UIShowValue.SaveValue.CallNumber[USART_RX_Buffer[0][DataStart]]));
						 Buf[Length++]=0;
						 break;				
				case SCFF_SetCallList:
						 Length=BufferCoppy(&USART_RX_Buffer[0][DataStart+1],&UIShowValue.SaveValue.CallNumber[USART_RX_Buffer[0][DataStart]][0],sizeof(UIShowValue.SaveValue.CallNumber[USART_RX_Buffer[0][DataStart]]));
						 Length=StringCoppy(String_Ok,&Buf[0]);		
						 Task_SaveConfigs();
						 break;		
				case SCFF_GetLoginHead:
						 Length=BufferCoppy(&UIShowValue.SaveValue.LoginHead[0],&Buf[0],sizeof(UIShowValue.SaveValue.LoginHead));
						 Buf[Length++]=0;
						 break;
				case SCFF_SetLoginHead:
						 Length=BufferCoppy(&USART_RX_Buffer[0][DataStart],&UIShowValue.SaveValue.LoginHead[0],sizeof(UIShowValue.SaveValue.LoginHead));
						 Length=StringCoppy(String_Ok,&Buf[0]);		
						 break;
				case SCFF_GetHeartBeat:
						 Length=BufferCoppy(&UIShowValue.SaveValue.Heartbeat[0],&Buf[0],sizeof(UIShowValue.SaveValue.Heartbeat));
						 Buf[Length++]=0;
						 break;
				case SCFF_SetHeartBeat:
						 Length=BufferCoppy(&USART_RX_Buffer[0][DataStart],&UIShowValue.SaveValue.Heartbeat[0],sizeof(UIShowValue.SaveValue.Heartbeat));
						 Length=StringCoppy(String_Ok,&Buf[0]);		
						 break;
				case SCFF_SetDateTime:
						UIShowValue.DateTime.Year=USART_RX_Buffer[0][DataStart];
						UIShowValue.DateTime.Mon=USART_RX_Buffer[0][DataStart+1];
						UIShowValue.DateTime.Day=USART_RX_Buffer[0][DataStart+2];
						UIShowValue.DateTime.Hour=USART_RX_Buffer[0][DataStart+3];
						UIShowValue.DateTime.Min=USART_RX_Buffer[0][DataStart+4];
						UIShowValue.DateTime.Sec=USART_RX_Buffer[0][DataStart+5];
						UIShowValue.DateTime.Week=USART_RX_Buffer[0][DataStart+6];
						Length=StringCoppy(String_Ok,&Buf[0]);		
				 	  ISL1208_SetDateTime(&UIShowValue.DateTime);	
						 break;
				case SCFF_COMMenuList:
						 switch(USART_RX_Buffer[0][DataStart])
										{
											case 0x01:COM_Menu_Enter(USART_RX_Buffer[0][DataStart+1],&USART_RX_Buffer[0][DataStart+2]);
													 //Length=Length=StringCoppy(String_Ok,&Buf[0]);  
													 Length=COM_Menu_Load(USART_RX_Buffer[0][DataStart+1],&Buf[0]);											
														break;
											case 0x02:Length=COM_Menu_Load(USART_RX_Buffer[0][DataStart+1],&Buf[0]);break;
											default:break;
										}
						 break;
				default: Length=StringCoppy("Null",&Buf[0]);
						 break;
			}
			ModBus_Create(&ModBusMsg,USART_RX_Buffer[0][2],0x05,WorkMode_Slave,MBCM_Crc);
			Length=ModBus_CreateMsg(USART_TX_Buffer[0],&ModBusMsg,0x01,0x01,0x01,&Buf[0],Length);
			USART_WriteDatas(USART1,USART_TX_Buffer[0],Length);
		}else if(ModBusMsg.ErrorFlag==ModBus_Broad)
		{
			
		}
		else if(ModBusMsg.ErrorFlag==ModBus_FlagError)
		{
			DeBug("%s",USART_RX_Buffer[0],Infor_Error);
		}else DeBug("%d",ModBusMsg.ErrorFlag,Infor_Infor);
	}
}
