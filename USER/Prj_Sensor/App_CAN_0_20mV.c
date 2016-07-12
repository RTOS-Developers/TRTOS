#include <Include.h>
#include <App_CAN_0_20mV.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#ifdef   SENSOR_TYPE_0_20MV_TO_CAN
uint8 WEIGHT_CFG_OffSet=FlashConfigNull;


WEIGHT_CROSS_TYPE WEIGHT_CROSS_DATA[WEIGHT_CROSS_SIZE];

void Load_Weight_Configs()
{
	FLASH_ReadDatas(WEIGHT_CFG_OffSet,(uint16 *)&WEIGHT_CROSS_DATA[0],8);
}
void Save_Weight_Configs()
{
	FLASH_WriteDatas(WEIGHT_CFG_OffSet,(uint16 *)&WEIGHT_CROSS_DATA[0],8);
}
void Weight_GetCross(uint16 ADC,float A,uint8 CH)
{
	WEIGHT_CROSS_DATA[CH].ADC=ADC;
	WEIGHT_CROSS_DATA[CH].Weight=A;
}
float Weight_GetWeight(uint16 ADC)
{
	return Opera_WhithADCFloat(ADC,WEIGHT_CROSS_DATA[0].ADC,WEIGHT_CROSS_DATA[0].Weight,WEIGHT_CROSS_DATA[1].ADC,WEIGHT_CROSS_DATA[1].Weight,360);
}
void CAN_0_20_Command(void *Tags)
{
	uint16 Length;
	uint8 Buf[20];
	MB ModBusMsg;
	FlashGetConfigRoom(&WEIGHT_CFG_OffSet,8);
	Load_Weight_Configs();
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
					   Length=SetWordToBytes(&Buf[0],CS1168_ADC,0);
						 break;
				case ACFF_GetAngle:
					   Length=Float32ToByte(&Buf[0],Weight_GetWeight(CS1168_ADC));
						 break;
				case ACFF_SetAngleMax:
					   Weight_GetCross(CS1168_ADC,Float32FromByte(&USART1_RX_Buffer[Length]),1);
				     Save_Weight_Configs();
				     Length=StringCoppy(String_Ok_ADC,&Buf[0]);
				     Length+=GetStringFromNumber(&Buf[Length],CS1168_ADC,'D',0);
						 break;
				case ACFF_SetAngleMin:
					   Weight_GetCross(CS1168_ADC,Float32FromByte(&USART1_RX_Buffer[Length]),0);
				     Save_Weight_Configs();
				     Length=StringCoppy(String_Ok_ADC,&Buf[0]);
				     Length+=GetStringFromNumber(&Buf[Length],CS1168_ADC,'D',0);
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
