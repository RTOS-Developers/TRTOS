#include <..\USER\Prj_7LCDSJJ\GPRS_Driver.h>
#include <..\USER\Prj_7LCDSJJ\App1.h>
#include <..\USER\Prj_7LCDSJJ\App2.h>
#include <Include.h>
Tos_Timer GPRS_Timer[10];

const GPRS_KeyStruct GPRSConst[]={
	{0x0802,(void *)&GPRS_GetLimitSensorConfigs,VT_Task,Null},//读取称重传感器参数帧
  {0x0804,(void *)&GPRS_GetCurrentLimitSensorADC,VT_Task,Null},//读取当前称重传感器ADC
  {0x0902,(void *)&GPRS_GetSpeedSensor,VT_Task,Null},//读取速度传感器参数帧
  {0x0902,(void *)&GPRS_GetCurrentFrequencyFrame,VT_Task,Null},//读取当卡频率帧
	{0x00},
};
uint8 StringToVersion(uint8 *String,uint8 *Bcd)
{
	f32 Temp;
	Temp=GetStringToNumber(String);
	Bcd[0]=Temp;
	Temp-=Bcd[0];
	Temp*=100;
	Bcd[1]=Temp;
	return 2;
}
uint8 StringToSimNumber(uint8 *Buf,uint8 *Bcd)
{
	uint8 i;
	Bcd[0]=0;
	for(i=1;i<12;i++)
	{
		Bcd[i]=(Buf[i-1]-'0');
	}
	return 12;
}
uint8 GPRS_SendHandle(uint8 *Buf,uint8 Length)
{
	uint8 i,z=0;
	uint8 BufTemp[255];
	for(i=0;i<2;i++)BufTemp[z++]=Buf[i];
	for(i=2;i<(Length-2);i++)
		switch (Buf[i])
		{
				case 0x5A: BufTemp[z++]=0x99;BufTemp[z++]=~Buf[i];break;
				case 0x99: BufTemp[z++]=0x99;BufTemp[z++]=~Buf[i];break;
				case 0x6A: BufTemp[z++]=0x99;BufTemp[z++]=~Buf[i];break;
				default:   BufTemp[z++]=Buf[i];break;
		}
		for(i=(Length-2);i<Length;i++)BufTemp[z++]=Buf[i];
		for(i=0;i<z;i++)Buf[i]=BufTemp[i];
		return z;
}
uint8 ReviceHandle(uint8 *Buf,uint8 Length)
{
	uint8 i,z=0,Temp=0;
	uint8 BufTemp[255];
	if(Length==0)return 0;
	for(i=0;i<2;i++)BufTemp[z++]=Buf[i];
	for(i=2;i<(Length-3);i++)
	{
		if(Buf[i]==0x99)
		{
			i++;
			switch(Buf[i])
			{
					case 0xa5:BufTemp[z++]=~Buf[i];break;
					case 0x66:BufTemp[z++]=~Buf[i];break;
					case 0x95:BufTemp[z++]=~Buf[i];break;
					default:return 0xff;
			}
		}
		BufTemp[z++]=Buf[i];
	}
	for(i=(Length-3);i<Length;i++)BufTemp[z++]=Buf[i];
	for(i=0;i<z;i++)Buf[i]=BufTemp[i];
	for(i=0;i<(z-3);i++)Temp+=Buf[i];
	if(Buf[z-3]!=Temp)return 0xfe;
	return z;
}
void GPRS_Export(uint8 *Buf,uint8 Length,GPRS_RxMessage *RxMsg)
{
	uint8 i;
	RxMsg->Cmd=Buf[6];
	i=(Buf[2]-6);
	if(i<sizeof(RxMsg->Buf))
		{
			BufferCoppy(&Buf[7],&RxMsg->Buf[0],i);
			RxMsg->Length=i;
		}else RxMsg->Length=0;
}
uint8 __GPRS_SendHandle(uint8 *Buf,uint8 Length)
{
	uint8 i,z=0;
	uint8 BufTemp[255];
	for(i=0;i<2;i++)BufTemp[z++]=Buf[i];
	for(i=2;i<Length;i++)
		switch (Buf[i])
		{
				case 0x5A: BufTemp[z++]=0x99;BufTemp[z++]=0xa5;break;
				case 0x99: BufTemp[z++]=0x99;BufTemp[z++]=0x66;break;
				case 0x6A: BufTemp[z++]=0x99;BufTemp[z++]=0x95;break;
				default:   BufTemp[z++]=Buf[i];break;
		}
		for(i=0;i<z;i++)Buf[i]=BufTemp[i];
		return z;
}
uint16 GPRS_CreateMsg(uint8 *Buf,uint8 Cmd, uint8 *Data,uint16 Length)
{
	uint16 i =0,n;
	uint8 Temp=0;
	Buf[i++] =0x5a;
	Buf[i++] =0x55;
	Buf[i++] =Length + 6;
	Buf[i++] =(Run_Value.RunningWaterID >> 8);
	Buf[i++] =(Run_Value.RunningWaterID & 0xff);
	Run_Value.RunningWaterID++;
	Buf[i++] =0X0D;
	Buf[i++] =Cmd;
	for(n=0;n<Length;n++)Buf[i++]=Data[n];//复制数据到缓冲区
	i=__GPRS_SendHandle(&Buf[0],i);
	for(n=0;n<(i);n++)Temp+=Buf[n];//计算校验和
	Buf[i++] = Temp;
	Buf[i++] = 0x6a;
	Buf[i++] = 0x69;
	return i;
}
uint8 GPRS_CreateCalibrationTime(uint8 *Buf)
{
	uint8 i=0,n,Data[100];
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	n=GPRS_CreateMsg(Buf,0x03,&Data[0],i);
	return n;
}
uint8 GPRS_CreateGeartBeat(uint8 *Buf)
{	
	uint8 i=0,n,Data[15];
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	n=GPRS_CreateMsg(&Buf[0],0x02,&Data[0],i);
	return n;
}
BOOL GPRS_CheckOk(BOOL DelayEN)
{
	uint8 n;
	ClearWithBuffer(&USART3_RX_Buffer[0],100);
	n=USART3_ReadDatas(Null,100,4000);
	n=ReviceHandle(&USART3_RX_Buffer[0],n);
	if(n<0xf0&n>0)
	{	
	if(DelayEN){DaCai_Printf("发送成功");Tos_TaskDelay(200);}
	return True;
	}else {if(DelayEN){DaCai_Printf("等待超时");Tos_TaskDelay(200);}return False;}
}
void GPRS_GeartBeat()
{
	uint16 Length;
	uint8 Buf[200];
	DaCai_Printf("心跳包");
	Length=GPRS_CreateGeartBeat(&Buf[0]);
	USART3_WriteDatas(&Buf[0],Length);
	ClearWithBuffer(&USART3_RX_Buffer[0],100);
	Length=USART3_ReadDatas(Null,100,10000);
  GPRS_CheckOk(True);
}
uint8 Coppy_DateTimeToBcd(Date_Time *DT,uint8 *Buf)
{
	uint8 i=0;
	Buf[i++]=Decimal_Bcd(DT->Year);
	Buf[i++]=Decimal_Bcd(DT->Mon);
	Buf[i++]=Decimal_Bcd(DT->Day);
	Buf[i++]=Decimal_Bcd(DT->Hour);
	Buf[i++]=Decimal_Bcd(DT->Min);
	Buf[i++]=Decimal_Bcd(DT->Sec);
	return i;
}
uint8 Coppy_DateToBcd(Date *DT,uint8 *Buf)
{
	uint8 i=0;
	Buf[i++]=Decimal_Bcd(DT->Year);
	Buf[i++]=Decimal_Bcd(DT->Mon);
	Buf[i++]=Decimal_Bcd(DT->Day);
	return i;
}
uint8 Coppy_DateTimeFromBcd(Date_Time *DT,uint8 *Buf)
{
	uint8 i=0;
	DT->Year =Bcd_Decimal(Buf[i++]);
	DT->Mon	 =Bcd_Decimal(Buf[i++]&0x1F);
	DT->Day	 =Bcd_Decimal(Buf[i++]&0x3F);
	DT->Hour =Bcd_Decimal(Buf[i++]&0x3F);
	DT->Min =Bcd_Decimal(Buf[i++]&0x7F);
	DT->Sec =Bcd_Decimal(Buf[i++]&0x7F);	
	return i;
}
uint8 Coppy_DateFromBcd(Date *DT,uint8 *Buf)
{
	uint8 i=0;
	DT->Year =Bcd_Decimal(Buf[i++]);
	DT->Mon	 =Bcd_Decimal(Buf[i++]&0x1F);
	DT->Day	 =Bcd_Decimal(Buf[i++]&0x3F);	
	return i;
}
uint8 GPRS_CreateMaintenance(uint8 *Buf,uint8 Mode)
{
	uint8 i=0,n,Data[100];
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	for(n=0;n<sizeof(UserInfor.DriverID);n++)Data[i++]=UserInfor.DriverID[n];
	Data[i++]=Mode;
	i+=Coppy_DateTimeToBcd(&Run_Value.DT,&Data[i]);
	n=GPRS_CreateMsg(Buf,0x0a,&Data[0],i);
	return n;
}
void GPRS_FristMaintenance()
{
	uint8 i=0,n,Data[100];
	Data[i++]=0x01;
	DaCai_Printf("发送维保配置");
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	i+=Coppy_DateToBcd(&Run_ValueRege.AllCheckDate,&Data[i]);
	i+=Coppy_DateToBcd(&Run_ValueRege.AntiFailCheckDate,&Data[i]);
	n=GPRS_CreateMsg(&USART3_TX_Buffer[0],0x70,&Data[0],i);
	USART3_WriteDatas(&USART3_TX_Buffer[0],n);
	ClearWithBuffer(&USART3_RX_Buffer[0],100);
	n=USART3_ReadDatas(Null,100,10000);
	n=ReviceHandle(&USART3_RX_Buffer[0],n);
	if(n<0xf0&n>0)
	{	
		DaCai_Printf("发送成功");
		Tos_TaskDelay(1000);
	}else 
	{
		DaCai_Printf("发送失败");
		Tos_TaskDelay(1000);
	}
}

uint8 GPRS_CreateDriverName(uint8 *Buf)
{
	uint8 i=0,n,Data[100];
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	Data[i++]=Run_Value.WorkMode;
	for(n=0;n<sizeof(UserInfor.DriverID);n++)Data[i++]=UserInfor.DriverID[n];
	for(n=0;n<sizeof(UserInfor.UserName);n++)Data[i++]=UserInfor.UserName[n];
	StringCoppy("浙M052014001200",&Data[i]);
	i+=48;
	n=GPRS_CreateMsg(Buf,0x06,&Data[0],i);
	return n;
}
uint8 GPRS_CreateUserTime(uint8 *Buf)
{
	uint8 i=0,n,Data[100];
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	i+=Coppy_DateTimeToBcd(&Run_Value.DT,&Data[i]);
	i+=D32ToArray(&Data[i],Run_Value.SysRunSec,0);
	n=GPRS_CreateMsg(Buf,0x04,&Data[0],i);
	return n;
}
void GPRS_UserTime()
{
	uint16 Length;
	uint8 Buf[200];
	DaCai_Printf("发送使用时长");
	Length=GPRS_CreateUserTime(&Buf[0]);
	USART3_WriteDatas(&Buf[0],Length);
	ClearWithBuffer(&USART3_RX_Buffer[0],100);
	Length=USART3_ReadDatas(Null,100,10000);
	Length=ReviceHandle(&USART3_RX_Buffer[0],Length);
	if(Length<0xf0&Length>0)
	{	
		DaCai_Printf("发送成功");
		Tos_TaskDelay(1000);
	}
	
}
void GPRS_DriverName()
{
	uint16 Length;
	uint8 Buf[200];
	DaCai_Printf("发送身份信息");
	Length=GPRS_CreateDriverName(&Buf[0]);
	USART3_WriteDatas(&Buf[0],Length);
	ClearWithBuffer(&USART3_RX_Buffer[0],100);
	Length=USART3_ReadDatas(Null,100,2000);
	Length=ReviceHandle(&USART3_RX_Buffer[0],Length);
	if(Length<0xf0&Length>0)
	{	
		if(USART3_RX_Buffer[20]==1)DaCai_Printf("注册地不符");
		else if(USART3_RX_Buffer[20]==0)DaCai_Printf("注册成功");
		else if(USART3_RX_Buffer[20]==2)DaCai_Printf("非法身份");
		else DaCai_Printf("未知码%d",USART3_RX_Buffer[20]);
		Tos_TaskDelay(1000);
		return;
	}else {DaCai_Printf("发送失败");Tos_TaskDelay(1000);}
}

void GPRS_Maintenance()
{
	uint16 Length;
	uint8 Buf[200];
	DaCai_Printf("发送维保帧");
	Length=GPRS_CreateMaintenance(&Buf[0],Run_Value.MaintenanceMode);
	USART3_WriteDatas(&Buf[0],Length);
	ClearWithBuffer(&USART3_RX_Buffer[0],100);
	Length=USART3_ReadDatas(Null,100,10000);
	Length=ReviceHandle(&USART3_RX_Buffer[0],Length);
  GPRS_CheckOk(True);
}
void GPRS_MaintenanceStart()
{
	Run_Value.WorkMode=0x02;
	Run_Value.Ctr|=S2;
	if(Run_Value.MaintenanceMode&BIT(MT_AllCheck))BufferCoppy(&Run_Value.DT,&Run_ValueRege.AllCheckDate,3);
	if(Run_Value.MaintenanceMode&BIT(MT_AntiFailCheck))BufferCoppy(&Run_Value.DT,&Run_ValueRege.AntiFailCheckDate,3);	
	if(Run_Value.MaintenanceMode&BIT(MT_AllCheck))Save_Configs();
	if(Run_Value.MaintenanceMode&BIT(MT_AntiFailCheck))Save_Configs();
	GPRS_SendOne(5,1,1,&GPRS_Maintenance);
}
void GPRS_MaintenanceStop()
{
	Run_Value.WorkMode=0x01;
	Run_Value.Ctr&=~S2;
}
void GPRS_RealTimeData()
{
	uint8 i=0,n,Data[100];
	DaCai_Printf("发送实时数据");
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	Run_Value.LeftWeight=65;//重量65kg
	i+=SetWordToBytes(&Data[i],Run_Value.LeftWeight,0);//重量
	Run_Value.PeopleNum=2;//两个人
	Data[i++]=Run_Value.PeopleNum;//人数
	Run_Value.LeftSpeed=0;//速度
	i+=SetWordToBytes(&Data[i],Run_Value.LeftSpeed,0);//速度
	Run_Value.LeftHeight=2;//高度
	i+=SetWordToBytes(&Data[i],Run_Value.LeftHeight,0);//高度
	i+=SetWordToBytes(&Data[i],Run_Value.LeftActive,0);//动作
	i+=SetWordToBytes(&Data[i],Run_Value.LeftSafe,0);//安全
	i+=SetWordToBytes(&Data[i],Run_Value.LeftAlarm,0);//报警
	i+=SetWordToBytes(&Data[i],Run_Value.LeftError,0);//错误
	i+=Coppy_DateTimeToBcd(&Run_Value.DT,&Data[i]);
	n=GPRS_CreateMsg(&USART3_TX_Buffer[0],0x05,&Data[0],i);
	USART3_WriteDatas(&USART3_TX_Buffer[0],n);
	GPRS_CheckOk(True);
}
void GPRS_Register()
{
	uint16 Length,n,c=5;
	uint8 i=0,Data[100];
	while(c--)
	{
	DaCai_Printf("注册到网络!");
	if(Run_ValueRege.RecordNumber[4]!='-')
	{
		Run_ValueRege.RecordNumber[4]='-';
		Save_Configs();
	}
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	for(n=0;n<sizeof(Run_ValueRege.ProjectNumber);n++)Data[i++]=Run_ValueRege.ProjectNumber[n];
	for(n=0;n<sizeof(Run_ValueRege.LiftModel);n++)Data[i++]=Run_ValueRege.LiftModel[n];
	for(n=0;n<sizeof(Run_ValueRege.LiftNumber);n++)Data[i++]=Run_ValueRege.LiftNumber[n];
	for(n=0;n<sizeof(Run_ValueRege.BlockBoxNumber);n++)Data[i++]=Run_ValueRege.BlockBoxNumber[n];	
  i+=StringToVersion(Run_ValueRege.HandVersion,&Data[i]);
	i+=StringToVersion(Run_ValueRege.SoftVersion,&Data[i]);
	i+=StringToSimNumber(&Run_ValueRege.SIMNumber[0],&Data[i]);
	Length=GPRS_CreateMsg(&USART3_TX_Buffer[0],0x01,&Data[0],i);
	USART3_WriteDatas(&USART3_TX_Buffer[0],Length);
	ClearWithBuffer(&USART3_RX_Buffer[0],sizeof(USART3_RX_Buffer));
	Length=USART3_ReadDatas(Null,100,5000);
	Length=ReviceHandle(&USART3_RX_Buffer[0],Length);
	if(Length<0xf0&Length>0){	
		if(USART3_RX_Buffer[20]==0){DaCai_Printf("注册成功!");return;}
		else {DaCai_Printf("注册失败![%d]",USART3_RX_Buffer[20]);Tos_TaskDelay(5000);return;}
		}else {DaCai_Printf("无数据返回!");Tos_TaskDelay(1000);}
	}
	DaCai_Printf("检测网络或备案信息!");
	Tos_TaskDelay(1000);
}
uint8 _GPRS_CreateRegister(uint8 *Buf)
{
	uint8 i=0,n,Data[100];
	Data[i++]=0x01;
	
	if(Run_ValueRege.RecordNumber[4]!='-')
	{
		Run_ValueRege.RecordNumber[4]='-';
		Save_Configs();
		//Run_ValueRege.RecordNumber[11]='L';
	}
	
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	for(n=0;n<sizeof(Run_ValueRege.ProjectNumber);n++)Data[i++]=Run_ValueRege.ProjectNumber[n];
	for(n=0;n<sizeof(Run_ValueRege.LiftModel);n++)Data[i++]=Run_ValueRege.LiftModel[n];
	for(n=0;n<sizeof(Run_ValueRege.LiftNumber);n++)Data[i++]=Run_ValueRege.LiftNumber[n];
	for(n=0;n<sizeof(Run_ValueRege.BlockBoxNumber);n++)Data[i++]=Run_ValueRege.BlockBoxNumber[n];	
  i+=StringToVersion(Run_ValueRege.HandVersion,&Data[i]);
	i+=StringToVersion(Run_ValueRege.SoftVersion,&Data[i]);
	i+=StringToSimNumber(&Run_ValueRege.SIMNumber[0],&Data[i]);
	n=GPRS_CreateMsg(Buf,0x01,&Data[0],i);
	return n;
}
void _GPRS_Register()
{
	uint16 Length,i=5;
	uint8 Buf[200];
	while(i--)
	{
		DaCai_Printf("注册到网络!");
	  Length=_GPRS_CreateRegister(&Buf[0]);
		USART3_WriteDatas(&Buf[0],Length);
		ClearWithBuffer(&USART3_RX_Buffer[0],sizeof(USART3_RX_Buffer));
		Length=USART3_ReadDatas(Null,100,20000);
		Length=ReviceHandle(&USART3_RX_Buffer[0],Length);
		if(Length<0xf0&Length>0){	
			SPK_OFF;
			Tos_TaskDelay(20);
			SPK_ON;
			if(USART3_RX_Buffer[20]==0){DaCai_Printf("注册成功!");return;}
			else {DaCai_Printf("注册失败!");Tos_TaskDelay(5000);return;}
			}else DaCai_Printf("无数据返回!");
	}
	DaCai_Printf("检测网络或备案信息!");
	Tos_TaskDelay(10000);
}
void GPRS_CalibrationTime()
{	
	uint8 i=6,Length,n=13;
	GPRS_RxMessage RxMsg;
	DaCai_Printf("更新时间!");
	while(i--)
	{
		ClearWithBuffer(&USART3_RX_Buffer[0],sizeof(USART3_RX_Buffer));
		Length=USART3_ReadDatas(Null,100,5000);
		Length=ReviceHandle(&USART3_RX_Buffer[0],Length);
		if(Length<0xf0&Length>0){	
		GPRS_Export(&USART3_RX_Buffer[0],Length,&RxMsg);
		if(RxMsg.Cmd==0x03)
		if(RxMsg.Buf[0]==0x02)
		{
			Run_Value.DT.Year  =Bcd_Decimal(RxMsg.Buf[n++]);
			Run_Value.DT.Mon   =Bcd_Decimal(RxMsg.Buf[n++]&0x1F);
			Run_Value.DT.Day   =Bcd_Decimal(RxMsg.Buf[n++]&0x3F);
			Run_Value.DT.Hour	 =Bcd_Decimal(RxMsg.Buf[n++]&0x7F);
			Run_Value.DT.Min	 =Bcd_Decimal(RxMsg.Buf[n++]&0x7F);
			Run_Value.DT.Sec   =Bcd_Decimal(RxMsg.Buf[n++]&0X7F);
			ISL1208_SetDateTime(&Run_Value.DT);
			DaCai_SetDateTime(&Run_Value.DT);
			DaCai_Printf("完成校时!");
			Tos_TaskDelay(2000);
			return;
		}
		Tos_TaskDelay(200);
	}
	}
}
uint8 GPRS_CreatePicture(uint8 *Buf,uint8 *Src,uint8 PageSize,uint16 Count,uint16 Index)
{
	uint16 i=0,n;
	uint8 Data[300];
	Data[i++]=0x02;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
	Data[i++]=0x01;//文件类型
	for(n=0;n<sizeof(UserInfor.DriverID);n++)Data[i++]=UserInfor.DriverID[n];
	i+=D16ToArray(&Data[i],Count,0);
	i+=D16ToArray(&Data[i],Index,0);
	for(n=0;n<PageSize;n++)Data[i++]=Src[n];
	n=GPRS_CreateMsg(&Buf[0],0x0c,&Data[0],i);
	ClearWithBuffer(&USART3_RX_Buffer[0],sizeof(USART3_RX_Buffer));
	USART3_WriteDatas(&Buf[0],n);
	return  GPRS_CheckOk(False);
}
void GPRS_SendAlarm()
{
	uint8 i=0,n,Data[100];
	DaCai_Printf("发送报警参数");
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
  for(n=0;n<13;n++)Data[i++]=0;//备填内容
	n=GPRS_CreateMsg(&USART3_TX_Buffer[0],0x07,&Data[0],i);
	ClearWithBuffer(&USART3_RX_Buffer[0],sizeof(USART3_RX_Buffer));
	USART3_WriteDatas(&USART3_TX_Buffer[0],n);
  GPRS_CheckOk(True);
}
void GPRS_GetLimitSensorConfigs()
{
	uint8 i=0,n,Data[100];
	DaCai_Printf("发送传感器配置");
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
  for(n=0;n<14;n++)Data[i++]=0;//备填内容
	n=GPRS_CreateMsg(&USART3_TX_Buffer[0],0x08,&Data[0],i);
	ClearWithBuffer(&USART_RX_Buffer[0],sizeof(USART3_RX_Buffer));
	USART_WriteDatas(USART3,USART_TX_Buffer[2],n);
  GPRS_CheckOk(True);
}
void GPRS_GetSpeedSensor()
{
	uint8 i=0,n,Data[100];
	DaCai_Printf("发送速度传感器参数");
	Data[i++]=0x01;
	for(n=0;n<sizeof(Run_ValueRege.RecordNumber);n++)Data[i++]=Run_ValueRege.RecordNumber[n];
  for(n=0;n<2;n++)Data[i++]=0;//备填内容
	n=GPRS_CreateMsg(USART_TX_Buffer[2],0x09,&Data[0],i);
	ClearWithBuffer(USART_RX_Buffer[2],USART_DEF_CH_SIZE);
	USART_WriteDatas(USART_TX_Buffer[2],n);
  GPRS_CheckOk(True);
}
void GPRS_GetCurrentLimitSensorADC()
{
	
}
void GPRS_GetCurrentFrequencyFrame()
{
	
}
void TaskSPK()
{
	SPK_OFF;
	Tos_TaskDelay(20);
	SPK_ON;
}

void GPRS_SendOne(uint8 Offset,uint16 Delay,uint16 LiftCry,Func Task)
{
	Tos_TimerCreate(&GPRS_Timer[Offset],Delay,LiftCry,Task);
	Tos_TimerStart(GPRS_Timer[Offset].ID);
}
void GPRSEnterTask(const GPRS_KeyStruct *Menu,uint8 *Addr)
{
typedef   void (*TaskFunc)(uint8 d);
	switch(Menu->Type)
	{
			case VT_Task:if(Menu->Value)((Func)Menu->Value)();break;  
	}
}
void GPRS_ReadCmd()
{
	uint8 n,i;
	uint32 Data;
	ClearWithBuffer(USART_RX_Buffer[2],100);
	n=USART_ReadDatas(USART3,Null,100,100);
	n=ReviceHandle(USART_RX_Buffer[2],n);
	if(n<0xf0&n>0)
	{	
	  Data=D16FromArray(&USART_RX_Buffer[0][6],0);
		i=0;
		while(GPRSConst[i].Key)
		{
			if(GPRSConst[i].Key==Data)GPRSEnterTask(&GPRSConst[i],&USART_RX_Buffer[0][GPRSConst[i].Note]);i++;
		}
	}
}
void GPRS_Task2(void *Tags)
{
	ISL1208_Init();
	Tos_TaskDelay(2000);
	DaCai_Printf("初始化网络网络!");
	Tos_TaskDelay(1000);
	_GPRS_Register();
	GPRS_CalibrationTime();
	GPRS_SendAlarm();
	GPRS_FristMaintenance();
	GPRS_SendOne(0,60,0xffff,&GPRS_GeartBeat);
	GPRS_SendOne(1,10,0xffff,&GPRS_RealTimeData);
	GPRS_SendOne(2,300,0xffff,&GPRS_UserTime);
	while(1)
	{
		Tos_TimerExecute();
		LED2_G;
		//GPRS_ReadCmd();//服务器发送命令设备解析
		Tos_TaskDelay(100);
		LED2_OFF;
		Tos_TaskDelay(100);
	}
}
