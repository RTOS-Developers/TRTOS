#include <Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <..\USER\Prj_TankCtrNew\TankCom.h>
#include <..\USER\Prj_TankCtrNew\App_TankCtr.h>
#include <..\Driver\SZZigBee_Driver.h>
#include <AppConfig.h>
uint8 LinkCount;
TankRun_ValueStruct TankRun_Value;


void Load_Sensor_Configs(uint8 Offset)
{
	
	Flash_ReadData(CONFIGS_ADDR_BASE,&DevRun_Value.FullWeight,sizeof(DevRun_Value.CROSS_DATA)+4);
}
void Save_Sensor_Configs(uint8 Offset)
{
	Flash_WriteData(CONFIGS_ADDR_BASE,&DevRun_Value.FullWeight,sizeof(DevRun_Value.CROSS_DATA)+4);
}
void Sensor_GetCross(uint8 SC,uint8 CH,uint16 ADC,float A)
{
	DevRun_Value.CROSS_DATA[SC][CH].ADC=ADC;
	DevRun_Value.CROSS_DATA[SC][CH].Sensor=A;
}
float Sensor_GetSensor(uint8 SC,uint16 ADC)
{
	float Temp;
	Temp=Opera_WhithADCFloat(ADC,DevRun_Value.CROSS_DATA[SC][0].ADC,DevRun_Value.CROSS_DATA[SC][0].Sensor,DevRun_Value.CROSS_DATA[SC][1].ADC,DevRun_Value.CROSS_DATA[SC][1].Sensor,1000);
	if(Temp>100)Temp=999;
	if(Temp<0)Temp=0;
	return Temp;
}
void Sensor_Command(void *Tags)
{
	uint16 Length;
	TankRun_Value.MasterAddr=0xffff;
	Load_Sensor_Configs(0);
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(USART_RX_Buffer[0],100);
		Length=USART_ReadDatas(USART1,Null,100,0);
		DeBug_Get();
		TankRun_Value.MasterAddr=GetWordFromBytes(&USART_RX_Buffer[0][0],1);
		if(ZigBee_CheckCrc(&USART_RX_Buffer[0][0]))
		{
			Length=4;
			switch(USART_RX_Buffer[0][Length-2])
			{
				case SCFF_CorrectRange:
					Sensor_GetCross(1,1,DevRun_Value.ADC_Value[1],Float32FromByte(&USART_RX_Buffer[0][Length]));
				break;
				case SCFF_ClearRange:
					Sensor_GetCross(1,0,DevRun_Value.ADC_Value[1],Float32FromByte(&USART_RX_Buffer[0][Length]));
				break;
				case SCFF_CorrectWeight:
					Sensor_GetCross(0,1,DevRun_Value.ADC_Value[0],Float32FromByte(&USART_RX_Buffer[0][Length]));
				break;
				case SCFF_ClearWeight:
					Sensor_GetCross(0,0,DevRun_Value.ADC_Value[0],Float32FromByte(&USART_RX_Buffer[0][Length]));
				break;
				case SCFF_SetSaveValue:Save_Sensor_Configs(0);break;
				case SCFF_SetReadValue:Load_Sensor_Configs(0);break;
				case SCFF_SetFullWeight:DevRun_Value.FullWeight=USART_RX_Buffer[0][Length];break;
				case SCFF_SetCTRBIT:CTR_SetBIT(USART_RX_Buffer[0][Length]);break;
				case SCFF_ClrCTRBIT:CTR_ClrBIT(USART_RX_Buffer[0][Length]);break;
				case SCFF_SetCTRUINT32:CTR_SetUint32(Uint32FromByte(&USART_RX_Buffer[0][Length]));break;
				case SCFF_ButtonClick:App_SetButtonDelay(USART_RX_Buffer[0][Length]);break;
				default:break;
			}
			LED2_ON;
			LinkCount=2;
		}
	}
}



















