#include <DHT11_Driver.c>
#include <DS18B20_Driver.c>

u8 Sensor_Init(void)
{
	uint8 Ds18B20CHOK=0x00;
	if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)
	{
		DTH11_SetPort(GPIOB,GPIO_Pin_9);
		if(!DHT11_Read(Null,Null))Ds18B20CHOK+=2;
	}else
	{
			DS_SetPort(GPIOB,GPIO_Pin_9);
		if(!DS18B20_Init())Ds18B20CHOK++;
			DS_SetPort(GPIOB,GPIO_Pin_8);
		if(!DS18B20_Init())Ds18B20CHOK++;

	}
	return Ds18B20CHOK;
} 
//其实可以实现单线混合读取，但是考虑到比较浪费CPU还是算了
void DS_Read_Value(float *Ch1,float *Ch2)
{
	uint8 u=0;
	static uint8 Buf[2];
	if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)
	{
		DTH11_SetPort(GPIOB,GPIO_Pin_9);
		if(DHT11_Read(Ch1,Ch2))
		{
			Buf[0]++;
			if(Buf[0]>10)UIShowValue.RunFlag&=~(WRF_SensorCH1Connected|WRF_SensorCH2Connected);
		}else {Buf[0]=0;UIShowValue.RunFlag|=(WRF_SensorCH1Connected|WRF_SensorCH2Connected);}
	}
	else
	{
		DS_SetPort(GPIOB,GPIO_Pin_9);
		u=DS_Get_Temp(Ch1);
		if(u)
		{
			Buf[0]++;
			if(Buf[0]>10)
			{
			DS18B20_Init();
			UIShowValue.RunFlag&=~WRF_SensorCH1Connected;
			}
		}else {Buf[0]=0;UIShowValue.RunFlag|=WRF_SensorCH1Connected;}
		DS_SetPort(GPIOB,GPIO_Pin_8);
		u=DS_Get_Temp(Ch2);
		if(u)
		{
			Buf[1]++;
			if(Buf[1]>10)
			{
			DS18B20_Init();
			UIShowValue.RunFlag&=~WRF_SensorCH2Connected;
			}
		}else {Buf[1]=0;UIShowValue.RunFlag|=WRF_SensorCH2Connected;}
	}
}
