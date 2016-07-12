#include <include.h>
#include <isl1208_driver.h>
//盖个戳
uint8 DeviceId_ISL1208=DeviceNull;
const GPIO_InitStruct *IO_ISL1208_P;

void ISL1208_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推完输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*-------------------------------------------------------------------
 Func: ISL1208_I2C硬件控制
 Time: 2013-4-16
 Ver.: V1.0
 Note:
-------------------------------------------------------------------*/
uint8 ISL1208_IOCONTROL(uint8 ConMode)
{
	uint8 R=0;
	switch(ConMode)
	{
		case I2C_HOST_SDA_SET:GPIO_SetBits((&IO_ISL1208_P[0])->GPIOx,(&IO_ISL1208_P[0])->GPIO_Pin);break;
		case I2C_HOST_SDA_CLR:GPIO_ResetBits((&IO_ISL1208_P[0])->GPIOx,(&IO_ISL1208_P[0])->GPIO_Pin);break;
		case I2C_HOST_SCL_SET:GPIO_SetBits((&IO_ISL1208_P[1])->GPIOx,(&IO_ISL1208_P[1])->GPIO_Pin);break;
		case I2C_HOST_SCL_CLR:GPIO_ResetBits((&IO_ISL1208_P[1])->GPIOx,(&IO_ISL1208_P[1])->GPIO_Pin);break;
		case I2C_HOST_SDA_DAT:if(GPIO_ReadInputDataBit((&IO_ISL1208_P[0])->GPIOx,(&IO_ISL1208_P[0])->GPIO_Pin))R=1;  break;		
	}
	return R;
}


/*-------------------------------------------------------------------
 Func: ISL1208写寄存器
 Time: 2012-3-28
 Ver.: V1.0
 Note:
-------------------------------------------------------------------*/
uint8 ISL1208_WriteReg(uint8 RegAddr,uint8 RegValue)
{
	return I2C_WriteDatas(ISL1208_IOCONTROL,ISL1208_ADDR,&RegAddr,1,&RegValue,1);
}

/*-------------------------------------------------------------------
 Func: ISL1208写寄存器
 Time: 2012-3-28
 Ver.: V1.0
 Note: return 0/Error  >0/OK
-------------------------------------------------------------------*/
uint8 ISL1208_ReadReg(uint8 RegAddr,uint8 *RegValues,uint8 Length)
{
	return I2C_ReadDatas(ISL1208_IOCONTROL,ISL1208_ADDR,&RegAddr,1,RegValues,Length);
}


/*-------------------------------------------------------------------
 Func: ISL1208电容设置
 Time: 2013-1-10
 Ver.: V1.0
 Note: 
-------------------------------------------------------------------*/
uint8 ISL1208_GetATR(f16 Cvalue)
{
	uint8  R;
	uint16 V;
	V=(uint16)(Cvalue*200);
	R=(uint8)((V-900)/50);
	R^=S5;	   //S5那一位取反
	return R;
}


/*-------------------------------------------------------------------
 Func: ISL1208获取时间
 Time: 2012-3-28
 Ver.: V1.0
 Note:
-------------------------------------------------------------------*/
uint8 ISL1208_GetDateTime(Date_Time *DateTime)
{
	uint8 R,Buffer[7];
	R=ISL1208_ReadReg(ISL1208_RTC_SECOND,Buffer,7);	
	if(R==0)return 0;
	else{
		DateTime->Sec =Bcd_Decimal(Buffer[0]&0x7F);
		DateTime->Min =Bcd_Decimal(Buffer[1]&0x7F);
		DateTime->Hour   =Bcd_Decimal(Buffer[2]&0x3F);
		DateTime->Day	 =Bcd_Decimal(Buffer[3]&0x3F);
		DateTime->Mon	 =Bcd_Decimal(Buffer[4]&0x1F);
		DateTime->Year =Bcd_Decimal(Buffer[5]);
		DateTime->Week	 =Bcd_Decimal(Buffer[6]);
		return 1;
	}
}

/*-------------------------------------------------------------------
 Func: ISL1208设置时间
 Time: 2012-3-28
 Ver.: V1.0
 Note:
-------------------------------------------------------------------*/
uint8 ISL1208_SetDateTime(Date_Time *DateTime)
{
	uint8 i,R,Buffer[7];
	Buffer[0]=Decimal_Bcd(DateTime->Sec);
	Buffer[1]=Decimal_Bcd(DateTime->Min);
	Buffer[2]=Decimal_Bcd(DateTime->Hour)|S7;		//转换到24小时制
	Buffer[3]=Decimal_Bcd(DateTime->Day);
	Buffer[4]=Decimal_Bcd(DateTime->Mon);
	Buffer[5]=Decimal_Bcd(DateTime->Year);
	//Buffer[6]=BIN_TO_BCD(DateTime->Week);
	for(i=0;i<7;i++) 
		R=R&ISL1208_WriteReg(ISL1208_RTC_SECOND+i,Buffer[i]);
	return R;	
}


/*-------------------------------------------------------------------
 Func: ISL1208设置闹钟
 Time: 2012-3-28
 Ver.: V1.0
 Note: AlarmFlag/ S0: 秒比较  S1: 分比较  S2: 时比较
-------------------------------------------------------------------*/
uint8 ISL1208_SetAlarm(uint8 Hour,uint8 Min,uint8 Sec,uint8 AlarmFlag)
{
	uint8 i,R,Buffer[3];
	R=1;
	Buffer[0]=Decimal_Bcd(Sec);
	Buffer[1]=Decimal_Bcd(Min);
	Buffer[2]=Decimal_Bcd(Hour);
	if(AlarmFlag&S0)Buffer[0]|=0x80;
	if(AlarmFlag&S1)Buffer[1]|=0x80;
	if(AlarmFlag&S2)Buffer[2]|=0x80;
	for(i=0;i<3;i++)
	{
	  R&=ISL1208_WriteReg(ISL1208_ALM_SECOND+i,Buffer[i]);
	}
	if(AlarmFlag>0)R=R&ISL1208_WriteReg(ISL1208_INT_REG,S4|S7|S6);
	else	R=R&ISL1208_WriteReg(ISL1208_INT_REG,S4|S7);
	return R;
}

/*-------------------------------------------------------------------
 Func: ISL1208初始化
 Time: 2012-3-28	2013-6-3
 Ver.: V1.0 		V2.0 修正初始值Day=0的问题
 Note: return 0/Error  >0/OK
-------------------------------------------------------------------*/
uint8 ISL1208_Init(void *GIS)
{
	uint8 R=0,Value;
	IO_ISL1208_P=(const GPIO_InitStruct *)GIS;
	GPIO_InitList(IO_ISL1208_P);
	R=ISL1208_ReadReg(ISL1208_USER_DAT0,&Value,1);
	if(Value!=0x5D)										 //标志位
	{
		R=R&ISL1208_WriteReg(ISL1208_SR_REG,S4);		//设置SR,使能RTC
		R=R&ISL1208_WriteReg(ISL1208_INT_REG,S4|S7);	//设置INT  INT方式|关报警|BAT供电关闭INT
		Value=ISL1208_GetATR(6);
		R=R&ISL1208_WriteReg(ISL1208_ATR_REG,Value);	//晶振12.5pF
		R=R&ISL1208_WriteReg(ISL1208_DTR_REG,0);		//数字微调DTR
		R=R&ISL1208_WriteReg(ISL1208_USER_DAT0,0x5D);	//写入标志
		R=R&ISL1208_SetAlarm(0,0,0,0);					//清除闹钟
		 /*------------初始化时间-----------------*/
		R=R&ISL1208_WriteReg(ISL1208_RTC_SECOND,0);		//初始化时间
		R=R&ISL1208_WriteReg(ISL1208_RTC_MINUTE,0);
		R=R&ISL1208_WriteReg(ISL1208_RTC_HOUR,0x80);	//24小时的
		R=R&ISL1208_WriteReg(ISL1208_RTC_DAY,1);
		R=R&ISL1208_WriteReg(ISL1208_RTC_MONTH,1);
		R=R&ISL1208_WriteReg(ISL1208_RTC_YEAR,0);
		R=R&ISL1208_WriteReg(ISL1208_RTC_WEEK,6);
	} 
	return R;
}
void DeviceMount_ISL1208(const GPIO_InitStruct *GIS)
{
	DeviceId_ISL1208=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_ISL1208].DeviceId=DeviceId_ISL1208;
	Tos_Device_Tab[DeviceId_ISL1208].DeviceName="ISL1208";
	Tos_Device_Tab[DeviceId_ISL1208].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_ISL1208].DeviceVirtue=DV_TaskValue;
	Tos_Device_Tab[DeviceId_ISL1208].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_ISL1208].Init=ISL1208_Init;
	Tos_Device_Tab[DeviceId_ISL1208].Note=(uintbus)GIS;
	Tos_Device_Tab[DeviceId_ISL1208].Write=Null;
	Tos_Device_Tab[DeviceId_ISL1208].Read=Null;
	Tos_Device_Tab[DeviceId_ISL1208].Exit=Null;
}


