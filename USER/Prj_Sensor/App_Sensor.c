#include <Include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <App_Sensor.h>

SENSOR_CROSS_TYPE SENSOR_CROSS_DATA[SENSOR_CROSS_SIZE];
void Load_Sensor_Configs(uint8 Offset,SENSOR_CROSS_TYPE SENSOR_CROSS_DATA[])
{
	FLASH_ReadDatas(Offset,(uint16 *)&SENSOR_CROSS_DATA[0],SENSOR_CROSS_SIZE*4);
}
void Save_Sensor_Configs(uint8 Offset,SENSOR_CROSS_TYPE SENSOR_CROSS_DATA[])
{
	FLASH_WriteDatas(Offset,(uint16 *)&SENSOR_CROSS_DATA[0],SENSOR_CROSS_SIZE*4);
}
void Sensor_GetCross(SENSOR_CROSS_TYPE *SENSOR_CROSS_DATA,uint16 ADC,float A,uint8 CH)
{
	SENSOR_CROSS_DATA[CH].ADC=ADC;
	SENSOR_CROSS_DATA[CH].Sensor=A;
}
float Sensor_GetSensor(SENSOR_CROSS_TYPE *SC_DATA,uint16 ADC)
{
	return Opera_WhithADCFloat(ADC,SC_DATA[0].ADC,SC_DATA[0].Sensor,SC_DATA[1].ADC,SC_DATA[1].Sensor,360);
}
void Sensor_Command(void *Tags)
{
	uint16 Length;
	uint8 Buf[20];
	int16 *ADC_Value;
	ModBus_Message ModBusMsg;
	BOOL Mode_DeBug=False;
	ModBus_Create(&ModBusMsg,0x01,0x05,WorkMode_Slave,MBCM_Crc);
	while(1)
	{
		if(!Mode_DeBug)DeBug_Drop();
		ClearWithBuffer(&USART1_RX_Buffer[0],50);
		Length=USART1_ReadDatas(Null,50,0);
		DeBug_Get();
		Length=ModBus_Expend(&USART1_RX_Buffer[0],Length,&ModBusMsg);
		if(ModBusMsg.ErrorFlag==ModBus_Ok)
		{
			switch(ModBusMsg.MsgFlag)
			{
				case SCFF_GetSensorTest:
					   Length=StringCoppy(String_Hello,&Buf[0]);
						 break;
				case SCFF_GetSensorModel:
						 Length=StringCoppy(SensorModel,&Buf[0]);
						 break;
				case SCFF_GetSensorNumber:
					   Length=StringCoppy(SensorDefNumber,&Buf[0]);
						 break;
				case SCFF_GetSensorName:
					   Length=StringCoppy(SensorName,&Buf[0]);
						 break;
				case SCFF_GetSensorNote:
					   Length=StringCoppy(SensorDefNote,&Buf[0]);
						 break;
				case SCFF_GetBSD:
						 Length=StringCoppy(Board_Name,&Buf[0]);
					   break;
				case SCFF_GetCPUModel:
						 Length=StringCoppy(CPU_Model,&Buf[0]);
					   break;
//				case SCFF_GetCPUID:
//						// Length=Get_CPUID(&Buf[0],0);
//				   //  Length+=Get_CPUID(&Buf[Length],1);
//					   break;
				case SCFF_GetCANSpeed:
						 Length=0;
						 break;	
				case SCFF_InDeBug:
					   Mode_DeBug=True;
				     Length=StringCoppy(String_Ok,&Buf[0]);
						 break;
				case SCFF_OutDeBug:
						 Mode_DeBug=False;
				     Length=StringCoppy(String_Ok,&Buf[0]);
						 break;
				case SCFF_GetSensorADC:
					   Length=SetWordToBytes(&Buf[0],*ADC_Value,0);
						 break;
				case SCFF_GetSensorValue:
					   Length=Float32ToByte(&Buf[0],Sensor_GetSensor(&SENSOR_CROSS_DATA[0],*ADC_Value));
						 break;
				case SCFF_SetSensorMax:
					   Sensor_GetCross(&SENSOR_CROSS_DATA[0],*ADC_Value,Float32FromByte(&USART1_RX_Buffer[Length]),1);
				     Save_Sensor_Configs(SENSOR_CFG_OffSet,&SENSOR_CROSS_DATA[0]);
				     Length=StringCoppy(String_Ok_ADC,&Buf[0]);
				     Length+=GetStringFromNumber(&Buf[Length],*ADC_Value,'D',0);
						 break;
				case SCFF_SetSensorMin:
					   Sensor_GetCross(&SENSOR_CROSS_DATA[0],*ADC_Value,Float32FromByte(&USART1_RX_Buffer[Length]),0);
				     Save_Sensor_Configs(SENSOR_CFG_OffSet,&SENSOR_CROSS_DATA[0]);
				     Length=StringCoppy(String_Ok_ADC,&Buf[0]);
				     Length+=GetStringFromNumber(&Buf[Length],*ADC_Value,'D',0);
						 break;
				case SCFF_GetCANGroup:
						 Length=SetWordToBytes(&Buf[0],CAN_GROUP,0);
						 break;
				case SCFF_GetCANStdId:
						 Length=SetWordToBytes(&Buf[0],CBDevI.Addr,0);
						 break;
				case SCFF_SetCANStdId:
						 Length=SetWordToBytes(&Buf[0],CBDevI.Addr,0);
						 break;
				case SCFF_SetCANGroup:
						 Length=StringCoppy(String_Hello,&Buf[0]);
						 break;
				default:Length=0;
						 break;
			}
			ModBus_Create(&ModBusMsg,USART1_RX_Buffer[2],0x05,WorkMode_Slave,MBCM_Crc);
			Length=ModBus_CreateMsg(&USART1_TX_Buffer[0],&ModBusMsg,0x01,0x01,0x01,&Buf[0],Length);
			USART1_DMAWriteDatas(&USART1_TX_Buffer[0],Length);
		}else if(ModBusMsg.ErrorFlag==ModBus_Broad)
		{
			
		}
		else if(ModBusMsg.ErrorFlag==ModBus_FlagError)
		{
			DeBug("%s",&USART1_RX_Buffer[0],Infor_Infor);
		}else DeBug("%d",ModBusMsg.ErrorFlag,Infor_Infor);
	}
}

