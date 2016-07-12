#include <Include.h>
#include <..\USER\Prj_TankCtrNew\IO_Driver.h>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.h>

#define Def_DelayValue 12 //如果发现信号传输延迟较大可以增加此值
DevRun_ValueStruct DevRun_Value;
TankRun_ValueStruct TankRun_Value;
RS485WriteStruct RS485Wite_Value;
RS485WriteStruct RS485WiteToSkyBus_Value;
RS485ReadStruct SkyBus_Value;
RS485ReadStruct CtrRoom_Value;
uint8 DelayBuf[SENSOR_CTR_COUNT];
uint8 TankSpeed=0;
void SetSpeed()
{

	switch(TankSpeed)
	{
		case 0:DelayBuf[6]=0;DelayBuf[8]=0;DelayBuf[7]=0;DelayBuf[4]=0;break;
		case 1:DelayBuf[6]=0xff;DelayBuf[8]=0xff;DelayBuf[7]=0;DelayBuf[4]=0xff;break;//前进一
		case 2:DelayBuf[6]=0xff;DelayBuf[8]=0;DelayBuf[7]=0;DelayBuf[4]=0xff;break;//前进二
		case 3:DelayBuf[7]=0xff;DelayBuf[8]=0xff;DelayBuf[6]=0;DelayBuf[4]=0xff;break;//后退一
		case 4:DelayBuf[7]=0xff;DelayBuf[8]=0;DelayBuf[6]=0;DelayBuf[4]=0xff;break;//后退二
		case 5:DelayBuf[6]=Def_DelayValue;DelayBuf[8]=Def_DelayValue;DelayBuf[7]=0;DelayBuf[4]=5;break;//前进一点动
		case 6:DelayBuf[7]=Def_DelayValue;DelayBuf[8]=Def_DelayValue;DelayBuf[6]=0;DelayBuf[4]=5;break;//后退一点动
		default:TankSpeed=0;break;
	}
}
void Stop()
{
	TankSpeed=0;
	SetSpeed();
}
void OC_TankForward()
{
	switch(TankSpeed)
	{
		case 0:TankSpeed=1;break;
		case 1:TankSpeed=2;break;
		case 2:TankSpeed=2;break;
		default:TankSpeed=0;break;
	}
	 if(TankSpeed!=0)
	 {
	 if(DevRun_Value.Host_Sig_Data&S2)TankSpeed=1;
	 if(DevRun_Value.Host_Sig_Data&S3)TankSpeed=1;
	 }
}
void OC_SubSpeed()
{
	switch(TankSpeed)
	{
		case 2:TankSpeed=1;break;
		case 4:TankSpeed=3;break;
	}
	SetSpeed();
}
void OC_AddSpeed()
{
	switch(TankSpeed)
	{
		case 1:TankSpeed=2;break;
		case 3:TankSpeed=4;break;
	}
	SetSpeed();
}
void OC_TankBack()
{
	 switch(TankSpeed)
	{
		case 0:TankSpeed=3;break;
		case 3:TankSpeed=4;break;
		case 4:TankSpeed=4;break;
		default:TankSpeed=0;break;
	}
	 if(TankSpeed!=0)
	 {
	 if(DevRun_Value.Host_Sig_Data&S2)TankSpeed=3;
	 if(DevRun_Value.Host_Sig_Data&S3)TankSpeed=3;
	 }
}
void App_SetButtonDelay(uint8 i)
{
	uint8 n;
	uint8 Data[8];
	switch(i)
	  {
		case OC_Start:Stop();DelayBuf[0]=Def_DelayValue;DelayBuf[1]=0;break;//遥控启动
		case OC_Stop:Stop();DelayBuf[1]=Def_DelayValue;DelayBuf[0]=0;DelayBuf[4]=0;DelayBuf[11]=0;DelayBuf[12]=0;DevRun_Value.AllStatus&=~WSA_TankPowerOn;DevRun_Value.AllStatus&=~WSA_Relieve;break;//遥控停止
	  case OC_TankPowerON:DelayBuf[11]=0xff;DelayBuf[12]=0xff;DevRun_Value.AllStatus|=WSA_TankPowerOn;break;//罐子上电
		case OC_TankPowerOFF:DelayBuf[11]=0;DelayBuf[12]=0;DevRun_Value.AllStatus&=~WSA_TankPowerOn;break;//罐子断电
		case OC_TankResetFrq:DelayBuf[10]=Def_DelayValue;break;//变频器复位
		case OC_TankLock:for(n=0;n<SENSOR_CTR_COUNT;n++)DelayBuf[n]=0;break;//急停
		case OC_Relieve:DelayBuf[4]=Def_DelayValue;DevRun_Value.AllStatus|=WSA_Relieve;break;//强制解除
		case OC_UnRelieve:DelayBuf[4]=0;DevRun_Value.AllStatus&=~WSA_Relieve;break;//恢复强制解除
		case OC_TankForward1:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;OC_TankForward();SetSpeed();break;
		case OC_TankForward2:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;OC_TankForward();SetSpeed();break;
		case OC_TankBack1:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;OC_TankBack();SetSpeed();break;
		case OC_TankBack2:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;OC_TankBack();SetSpeed();break; 
		case OC_TankForwardDot:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;TankSpeed=5;SetSpeed();break;
		case OC_TankBackDot:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;TankSpeed=6;SetSpeed();break;
		case OC_UpDoorOpen:if(DevRun_Value.AllStatus&WSA_BusRun)return;Data[0]=i;CAN_SIG_SendMsg(&Data[0],0x91);break;
		case OC_UpDoorClose:if(DevRun_Value.AllStatus&WSA_BusRun)return;Data[0]=i;CAN_SIG_SendMsg(&Data[0],0x91);break;
		case OC_DownDoorOpen:if(DevRun_Value.AllStatus&WSA_BusRun)return;Data[0]=i;CAN_SIG_SendMsg(&Data[0],0x91);break;
		case OC_DownDoorClose:if(DevRun_Value.AllStatus&WSA_BusRun)return;Data[0]=i;CAN_SIG_SendMsg(&Data[0],0x91);break;
		}
}
void App_BusButton(void *Tags)
{
	uint8 i;
	static uint32 Old_Sig;
	while(1)
	{
		Tos_TaskDelay(100);
		DevRun_Value.Host_Sig_Data=~SIG_GetUint32();
		if((DevRun_Value.Host_Sig_Data&S2)&&(DevRun_Value.Host_Sig_Data&S3))DevRun_Value.AllStatus&=~WSA_BusRun;
		else DevRun_Value.AllStatus|=WSA_BusRun;
		if(!(SkyBus_Value.AllStatus&S0))Stop();
		if(Old_Sig!=DevRun_Value.Host_Sig_Data)
			{
				if(DevRun_Value.Host_Sig_Data&S2)OC_SubSpeed();
				if(DevRun_Value.Host_Sig_Data&S3)Stop();
				if((!(DevRun_Value.Host_Sig_Data&S2))&&(!(DevRun_Value.Host_Sig_Data&S3)))OC_AddSpeed();
				Old_Sig=DevRun_Value.Host_Sig_Data;
			}
		for(i=0;i<SENSOR_CTR_COUNT;i++)
		{
			if(DelayBuf[i]<=Def_DelayValue)
			if(DelayBuf[i])DelayBuf[i]--;
			if(DelayBuf[i]==0)DevRun_Value.Host_Ctr_Data&=~BIT(i);else DevRun_Value.Host_Ctr_Data|=BIT(i);
		}
		CTR_SetUint32(DevRun_Value.Host_Ctr_Data);
	}
}

