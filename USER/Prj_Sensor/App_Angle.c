#include <Tools_Include.h>
#include <Driver_Include.h>
#include <App_Angle.h>
#ifdef   SENSOR_TYPE_ANGLE
uint8 ANGLE_CFG_OffSet=FlashConfigNull;

ANGLE_CROSS_TYPE ANGLE_CROSS_DATA[ANGLE_CROSS_SIZE];
void Load_Angle_Configs()
{
	FLASH_ReadDatas(ANGLE_CFG_OffSet,(uint16 *)&ANGLE_CROSS_DATA[0],8);
}
void Save_Angle_Configs()
{
	FLASH_WriteDatas(ANGLE_CFG_OffSet,(uint16 *)&ANGLE_CROSS_DATA[0],8);
}
void Angle_GetCross(uint16 ADC,float A,uint8 CH)
{
	ANGLE_CROSS_DATA[CH].ADC=ADC;
	ANGLE_CROSS_DATA[CH].Angle=A;
}
float Angle_GetAngle(uint16 ADC)
{
	return Opera_WhithADCFloat(ADC,ANGLE_CROSS_DATA[0].ADC,ANGLE_CROSS_DATA[0].Angle,ANGLE_CROSS_DATA[1].ADC,ANGLE_CROSS_DATA[1].Angle,360);
}
void Angle_Command(void *Tags)
{
	uint16 Length;
	uint8 Buf[20];
	MB ModBusMsg;
	FlashGetConfigRoom(&ANGLE_CFG_OffSet,8);
	Load_Angle_Configs();
	ModBus_Create(&ModBusMsg,0x01,0xfe,WorkMode_Slave,CheakMode_Crc);
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
				case ACFF_GetSensorTest:
					   Length=StringCoppy(String_Hello,&Buf[0]);
						 break;
				case ACFF_GetSensorModel:
						 Length=StringCoppy(SensorModel,&Buf[0]);
						 break;
				case ACFF_GetSensorNumber:
					   Length=StringCoppy(SensorDefNumber,&Buf[0]);
						 break;
				case ACFF_GetSensorName:
					   Length=StringCoppy(SensorName,&Buf[0]);
						 break;
				case ACFF_GetSensorNote:
					   Length=StringCoppy(SensorDefNote,&Buf[0]);
						 break;
				case ACFF_GetBSD:
						 Length=StringCoppy(Board_Name,&Buf[0]);
					   break;
				case ACFF_GetCPUModel:
						 Length=StringCoppy(CPU_Model,&Buf[0]);
					   break;
				case ACFF_GetCPUID:
						 Length=Get_CPUID(&Buf[0],0);
				     Length+=Get_CPUID(&Buf[Length],1);
					   break;
				case ACFF_GetCANSpeed:
						 Length=0;
						 break;	
				case ACFF_InDeBug:
					   Mode_DeBug=True;
				     Length=StringCoppy(String_Ok,&Buf[0]);
						 break;
				case ACFF_OutDeBug:
						 Mode_DeBug=False;
				     Length=StringCoppy(String_Ok,&Buf[0]);
						 break;
				case ACFF_GetAngleADC:
					   Length=SetWordToBytes(&Buf[0],CS1180_ADC1,0);
						 break;
				case ACFF_GetAngle:
					   Length=Float32ToByte(&Buf[0],Angle_GetAngle(CS1180_ADC1));
						 break;
				case ACFF_SetAngleMax:
					   Angle_GetCross(CS1180_ADC1,Float32FromByte(&USART1_RX_Buffer[Length]),1);
				     Save_Angle_Configs();
				     Length=StringCoppy(String_Ok_ADC,&Buf[0]);
				     Length+=GetStringFromNumber(&Buf[Length],CS1180_ADC1,'D',0);
						 break;
				case ACFF_SetAngleMin:
					   Angle_GetCross(CS1180_ADC1,Float32FromByte(&USART1_RX_Buffer[Length]),0);
				     Save_Angle_Configs();
				     Length=StringCoppy(String_Ok_ADC,&Buf[0]);
				     Length+=GetStringFromNumber(&Buf[Length],CS1180_ADC1,'D',0);
						 break;
				default:Length=0;
						 break;
			}
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


#endif
