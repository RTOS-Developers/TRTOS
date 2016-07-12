#include <..\USER\Prj_TP500New\GPS_Driver.h>							   								   
#include <string.h>
#include <math.h>
void Task_Gps(void *Tags)
{
	uint16 Length;
	uint8 *P;
	GPS_ON;
	while(1)
	{
		Length=USART_ReadDatas(USART3,Null,USART_RX_BUFFER_SIZE,1000);
		if(Length)
		{
			USART_RX_Buffer[2][50]=0;
			
			if(strstr((const char *)USART_RX_Buffer[2],"$GPRMC"))
			{
				if(strstr((const char *)USART_RX_Buffer[2],"A"))
				{
					P=(uint8 *)strstr((const char *)USART_RX_Buffer[2],"A");
					UIShowValue.GPS_Latitude=GetStringToNumber(P+2);
					//UIShowValue.GPS_Latitude=(uint32)(UIShowValue.GPS_Latitude/100)+(UIShowValue.GPS_Latitude-(uint32)(UIShowValue.GPS_Latitude/100)*100)/60;//服务器上做了修正就不需要在这里做
					P=(uint8 *)strstr((const char *)USART_RX_Buffer[2],"N");
					UIShowValue.GPS_Longitude=GetStringToNumber(P+2);
					//UIShowValue.GPS_Longitude=(uint32)(UIShowValue.GPS_Longitude/100)+(UIShowValue.GPS_Longitude-(uint32)(UIShowValue.GPS_Longitude/100)*100)/60;
					
					if(strstr((const char *)USART_RX_Buffer[2],"W"))UIShowValue.GPS_Latitude=0-UIShowValue.GPS_Latitude;
					if(strstr((const char *)USART_RX_Buffer[2],"S"))UIShowValue.GPS_Latitude=0-UIShowValue.GPS_Latitude;
					UIShowValue.RunFlag|=WRF_GPS_Connected;
					DeBug("经纬度:%.8f,%.7f",UIShowValue.GPS_Latitude,UIShowValue.GPS_Longitude,Infor_Warning);
					if(Abs(UIShowValue.GPS_Latitude)>100)
					if(Abs(UIShowValue.GPS_Longitude)>100)
					{
						GPS_OFF;
						Tos_TaskDelay(60000);
						GPS_ON;
					}
				}else UIShowValue.RunFlag&=~WRF_GPS_Connected;
			}
		}
		Tos_TaskDelay(3000);
	}
}

