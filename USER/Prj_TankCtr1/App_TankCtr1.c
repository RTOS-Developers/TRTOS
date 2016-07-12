#include <Include.h>
#include <..\USER\Prj_TankCtr\IO_Driver.h>
#include <..\USER\Prj_TankCtr\App_TankCtr1.h>
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
		switch(i)
	  {
		case OC_UpDoorOpen:DelayBuf[1]=0;DelayBuf[0]=Def_DelayValue;break;
		case OC_UpDoorClose:DelayBuf[0]=0;DelayBuf[1]=Def_DelayValue;break;
		case OC_DownDoorOpen:DelayBuf[3]=0;DelayBuf[2]=Def_DelayValue;break;
		case OC_DownDoorClose:DelayBuf[2]=0;DelayBuf[3]=Def_DelayValue;break;
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
			if(DelayBuf[i])DelayBuf[i]--;
			if(DelayBuf[i]==0)DevRun_Value.CTR_DATABIT&=~BIT(i);else DevRun_Value.CTR_DATABIT|=BIT(i);
		}
		CTR_SetUint32(DevRun_Value.CTR_DATABIT);
	}
}
