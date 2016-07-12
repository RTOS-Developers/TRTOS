#include <Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <..\USER\Prj_TankCtr\TankCom.h>
#include <..\USER\Prj_TankCtr\App_TankCtr.h>
#include <..\Driver\SZZigBee_Driver.h>
uint8 LinkCount;
TankRun_ValueStruct TankRun_Value;

void Load_Sensor_Configs(uint8 Offset)
{
	FLASH_ReadDatas(Offset,(uint16 *)&DevRun_Value.CROSS_DATA[0][0],SENSOR_CROSS_SIZE*4*SENSOR_CH_SIZE);
}
void Save_Sensor_Configs(uint8 Offset)
{
	FLASH_WriteDatas(Offset,(uint16 *)&DevRun_Value.CROSS_DATA[0][0],SENSOR_CROSS_SIZE*4*SENSOR_CH_SIZE);
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
	if(Temp>100)Temp=-1;
	if(Temp<0)Temp=-1;
	return Temp;
}
void Sensor_Command(void *Tags)
{
	uint16 Length;
	uint8 SENSOR_CFG_OffSet=0;
	DevRun_Value.ADCChnenel=0;
	TankRun_Value.MasterAddr=0xffff;
	Load_Sensor_Configs(SENSOR_CFG_OffSet);
	while(1)
	{
		Tos_TaskDelay(1);
		ClearWithBuffer(&USART1_RX_Buffer[0],100);
		Length=USART1_ReadDatas(Null,100,0);
		DeBug_Get();
		TankRun_Value.MasterAddr=GetWordFromBytes(&USART1_RX_Buffer[0],1);
		if(ZigBee_CheckCrc(&USART1_RX_Buffer[0]))
		{
			Length=4;
			switch(USART1_RX_Buffer[Length-2])
			{
				case SCFF_SetSensorMax:Sensor_GetCross(DevRun_Value.ADCChnenel,1,DevRun_Value.ADC_Value[DevRun_Value.ADCChnenel],Float32FromByte(&USART1_RX_Buffer[Length]));break;
				case SCFF_SetSensorMin:Sensor_GetCross(DevRun_Value.ADCChnenel,0,DevRun_Value.ADC_Value[DevRun_Value.ADCChnenel],Float32FromByte(&USART1_RX_Buffer[Length]));break;
				case SCFF_SetSaveValue:Save_Sensor_Configs(SENSOR_CFG_OffSet);break;
				case SCFF_SetCTRBIT:CTR_SetBIT(USART1_RX_Buffer[Length]);break;
				case SCFF_ClrCTRBIT:CTR_ClrBIT(USART1_RX_Buffer[Length]);break;
				case SCFF_SetCTRUINT32:CTR_SetUint32(Uint32FromByte(&USART1_RX_Buffer[Length]));break;
				case SCFF_ButtonClick:App_SetButtonDelay(USART1_RX_Buffer[Length]);
					Length=ZigBee_SendMsg(&USART1_TX_Buffer[0],TankRun_Value.MasterAddr,SCFF_RefurBishSet,(uint8 *)&DevRun_Value,sizeof(DevRun_Value));
	        USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length);
				break;
				case SCFF_SetChnenel:if(USART1_RX_Buffer[Length]<SENSOR_CH_SIZE)DevRun_Value.ADCChnenel=USART1_RX_Buffer[Length];break;
				case SCFF_SetStructOffSet:Length=BufferCoppy(&USART1_RX_Buffer[Length],(&DevRun_Value)+USART1_RX_Buffer[Length],USART1_RX_Buffer[Length+1]);break;
				case SCFF_EraseFlase:if(USART1_RX_Buffer[8]==0x19)if(USART1_RX_Buffer[9]==0x89)if(USART1_RX_Buffer[10]==0x11)if(USART1_RX_Buffer[11]==0x06)MCU_Reset();break;
				default:break;
			}
			LED2_ON;
			LinkCount=2;
		}
	}
}



















