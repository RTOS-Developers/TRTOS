#include <Include.h>
#include <..\USER\Prj_TankCtr\IO_Driver.h>
#include <..\USER\Prj_TankCtr\App_TankCtr.h>
/*
0遥控启动
1遥控停止
2位置识别1
3位置识别2
4位置识别3
5位置识别4
6前进
7后退
8速度2
9速度3
10变频器复位
11料罐一通电
12料罐二通电
*/
#define Def_DelayValue 12 //如果发现信号传输延迟较大可以增加此值
DevRun_ValueStruct DevRun_Value;
TankRun_ValueStruct TankRun_Value;
uint8 DelayBuf[SENSOR_CTR_COUNT];
void App_SetButtonDelay(uint8 i)
{
	uint8 n;
	switch(i)
	  {
		case OC_Start:DelayBuf[0]=Def_DelayValue;DelayBuf[1]=0;break;//遥控启动
		case OC_Stop:DelayBuf[1]=Def_DelayValue;DelayBuf[0]=0;DelayBuf[4]=0;DevRun_Value.AllStatus&=~WSA_Relieve;break;//遥控停止
		case OC_TankPowerON:if(DevRun_Value.AllStatus&WSA_Tank1Place)DelayBuf[11]=Def_DelayValue;if(DevRun_Value.AllStatus&WSA_Tank2Place)DelayBuf[12]=Def_DelayValue;DevRun_Value.AllStatus|=WSA_TankPowerOn;break;//罐子上电
		case OC_TankPowerOFF:DelayBuf[11]=0;DelayBuf[12]=0;DevRun_Value.AllStatus&=~WSA_TankPowerOn;break;//罐子断电
		case OC_TankForward1:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[6]=Def_DelayValue;DelayBuf[8]=Def_DelayValue;break;//前进1速
		case OC_TankForward2:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[6]=Def_DelayValue;break;//前进2速
		case OC_TankBack1:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[7]=Def_DelayValue;DelayBuf[8]=Def_DelayValue;break;//后退1速
		case OC_TankBack2:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[7]=Def_DelayValue;break;//后退2速
		case OC_TankResetFrq:DelayBuf[10]=Def_DelayValue;break;//变频器复位
		case OC_TankLock:for(n=0;n<SENSOR_CTR_COUNT;n++)DelayBuf[n]=0;break;//急停
		case OC_HandReset:for(n=6;n<10;n++)DelayBuf[n]=0;break;//手柄归位
		case OC_Relieve:DelayBuf[4]=Def_DelayValue;DevRun_Value.AllStatus|=WSA_Relieve;break;//强制解除
		case OC_UnRelieve:DelayBuf[4]=0;DevRun_Value.AllStatus&=~WSA_Relieve;break;//恢复强制解除
		case OC_TankForward1R:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[4]=Def_DelayValue;DelayBuf[6]=Def_DelayValue;DelayBuf[8]=Def_DelayValue;break;//前进1速
		case OC_TankForward2R:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[4]=Def_DelayValue;DelayBuf[6]=Def_DelayValue;break;//前进2速
		case OC_TankBack1R:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[4]=Def_DelayValue;DelayBuf[7]=Def_DelayValue;DelayBuf[8]=Def_DelayValue;break;//后退1速
		case OC_TankBack2R:if(DevRun_Value.AllStatus&WSA_TankPowerOn)return;DelayBuf[4]=Def_DelayValue;DelayBuf[7]=Def_DelayValue;break;//后退2速
		}
}
void App_BusButton(void *Tags)
{
	uint8 i;
	while(1)
	{
		Tos_TaskDelay(100);
		for(i=0;i<SENSOR_CTR_COUNT;i++)
		{
			if((i!=11)&(i!=12))
			if(DelayBuf[i])DelayBuf[i]--;
			if(DelayBuf[i]==0)DevRun_Value.CTR_DATABIT&=~BIT(i);else DevRun_Value.CTR_DATABIT|=BIT(i);
			if(DelayBuf[4]==0)DevRun_Value.AllStatus&=~WSA_Relieve;else DevRun_Value.AllStatus|=WSA_Relieve;
		}
		CTR_SetUint32(DevRun_Value.CTR_DATABIT);
	}
}

void App_TankButton(void *Tags)
{
	
	uint8 i;
	while(1)
	{
		Tos_TaskDelay(100);
		for(i=0;i<SENSOR_CTR_COUNT;i++)
		{
			//if((i!=1)&(i!=0))
			if(DelayBuf[i])DelayBuf[i]--;
			if(DelayBuf[i]==0)DevRun_Value.CTR_DATABIT&=~BIT(i);else DevRun_Value.CTR_DATABIT|=BIT(i);
		}
		CTR_SetUint32(DevRun_Value.CTR_DATABIT);
	}
}
