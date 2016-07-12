#include <include.h>

u8 RegServer_Connect()
{
	char Buf[100];
	FmtToBuf(&Buf[0],"AT+QIOPEN=\"TCP\",\"%s\",%d\r\n","link.tlink.io",1500);
	DeBug("连接到:%s",&Buf[0],Infor_Warning);
	return GPRS_Connect(&Buf[0]);
}
u8 Reg_SendHead()
{
	uint8 Buf[100];
	uint16 i=0;
	i+=GetStringByFmt((char *)&Buf[i],"12345678912345AB");
	DeBug("发送头:%s",&Buf[0],Infor_Warning);
	USART_WriteLine(USART2,"AT+QISEND=%d\r\n",i);
	if(!GPRS_AckDelay(">",2000))return 1;
	USART_WriteDatas(USART2,&Buf[0],i);
	if(!GPRS_AckDelay("SEND OK",1000))return 2;//检查发送成功
	GPRS_Printf(2000);
	return 0;
}
uint8 Get_Mon(char *Str)
{
	if(Strstr(Str,"Jan"))return 1;
	if(Strstr(Str,"Feb"))return 2;
	if(Strstr(Str,"Mar"))return 3;
	if(Strstr(Str,"Apr"))return 4;
	if(Strstr(Str,"May"))return 5;
	if(Strstr(Str,"Jun"))return 6;
	if(Strstr(Str,"Jul"))return 7;
	if(Strstr(Str,"Aug"))return 8;
	if(Strstr(Str,"Sep"))return 9;
	if(Strstr(Str,"Oct"))return 10;
	if(Strstr(Str,"Nov"))return 11;
	if(Strstr(Str,"Dec"))return 12;
	return 1;
}
uint8 Get_PragmDateTime(uint8 *Buf)
{
	uint8 i=0;
	i+=FindNumFromString(__DATE__,&Buf[i],1);
	i+=GetStringByFmt((char *)&Buf[i],"-%02d-",Get_Mon(__DATE__));
	i+=FindNumFromString(__DATE__,&Buf[i],0);
	Buf[i]=0;
	return i;
}

uint8 GetCodeStr16(uint8 *Buf,uint8 Index)
{
	uint32 CpuID[3];
	uint8 i,c=0;
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[2]=*(vu32*)(0x1ffff7ec);
	CpuID[1]=*(vu32*)(0x1ffff7f0);
	for(i=0;i<3;i++)
	{
		while(CpuID[i])
		{
			Buf[c++]=ToHex64(CpuID[i]%62);
			CpuID[i]/=62;
		}
	}
	c=15;
	Buf[c++]=ToHex64(Index);
	Buf[c++]=0x00;
	return c;
}
u8 Reg_SendInfor()
{
	uint8 Buf[300],Date[20],Num[20];
	uint16 i,z=0;
R:
	i=0;
	i+=GetStringByFmt((char *)&Buf[i],"{");
	i+=GetStringByFmt((char *)&Buf[i],"\"method\":\"1\",");
	i+=GetStringByFmt((char *)&Buf[i],"\"ent_key\":\""__ENT_KEY__"\",");
	i+=GetStringByFmt((char *)&Buf[i],"\"device_id\":\"\",");
	GetCodeStr16(Num,z),
	i+=GetStringByFmt((char *)&Buf[i],"\"device_no\":\"%s\",",Num);
	i+=GetStringByFmt((char *)&Buf[i],"\"device_name\":\"温湿度\",");
	i+=GetStringByFmt((char *)&Buf[i],"\"hw_version\":\"%d.%d.%d\",",__STM32F10X_STDPERIPH_VERSION_MAIN,__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	i+=GetStringByFmt((char *)&Buf[i],"\"dev_mode\":\""Board_Name"\",");
	Get_PragmDateTime(Date);i+=GetStringByFmt((char *)&Buf[i],"\"create_date\":\"%s "__TIME__"\",",Date);
	i+=GetStringByFmt((char *)&Buf[i],"\"mcu_mode\":\"%s\"}",CPU_Model);
	DeBug("发送内容%s",&Buf[0],Infor_Warning);
	USART_WriteLine(USART2,"AT+QISEND=%d\r\n",i);
	if(!GPRS_AckDelay(">",2000))return 1;
	USART_WriteDatas(USART2,&Buf[0],i);
	if(!GPRS_AckDelay("SEND OK",1000))return 2;
	GPRS_Printf(2000);
	if(USART_RX_Buffer[1][0]=='0')
	if(USART_RX_Buffer[1][1]=='1')
	{
		DeBug("设备注册失败[%d]",z,Infor_Warning);
		z++;
		DeBug("再次尝试注册[%d]",z,Infor_Warning);
		if(z>60)
		{
			DeBug("服务器数据库错误[%d]",z,Infor_Warning);
			return z;
		}
		goto R;
	}
	if(USART_RX_Buffer[1][0]=='0')
	if(USART_RX_Buffer[1][1]=='0')
	{
		DeBug("设备注册成功[%d]",z,Infor_Warning);
		UIShowValue.SaveValue.DeBugCFG=1;
		UIShowValue.SaveValue.ServerPort[0]=1883;
		UIShowValue.SaveValue.CH_Limit[0].Max=66;
		UIShowValue.SaveValue.CH_Limit[0].Min=-1;
		UIShowValue.SaveValue.CH_Limit[1].Max=66;
		UIShowValue.SaveValue.CH_Limit[1].Min=-1;
		UIShowValue.SaveValue.SendCycle=60;
		UIShowValue.SaveValue.RecordCycle=5;
		UIShowValue.SaveValue.WorkBit|=WCB_MQTTEN;
		StringCoppy("tlink.io",&UIShowValue.SaveValue.DomainName[0][0]);
		StringCoppy("拓普瑞",&UIShowValue.SaveValue.PrintfName[0]);
		BufferCoppy("13267216247",&UIShowValue.SaveValue.CallNumber[0][0],sizeof(UIShowValue.SaveValue.CallNumber[0]));
		StringCoppy(Printf_Title,&UIShowValue.SaveValue.PrintfTitle[0]);
		UIShowValue.SaveValue.CorrectValue[0]=0;
		UIShowValue.SaveValue.CorrectValue[1]=0;
		BufferCoppy(Num,&UIShowValue.SaveValue.LoginHead[0],sizeof(UIShowValue.SaveValue.LoginHead));
		Task_SaveConfigs();
		TaskBigShow();//此操作格外重要，必然声光共鸣
		return 0;
	}
	goto R;
}

void Task_AutoReg(void *Tags)
{
	uint8 sta;
	if(BOOL_LoadConfigs()==False)
	{
	DeBug("似乎是个新设备",Infor_Warning);
	sta=RegServer_Connect();
	if(sta)DeBug("连接服务器[%d]",sta,Infor_Warning);
	DeBug("设备注册",Infor_Warning);
	sta=Reg_SendHead();
	if(sta)DeBug("进入注册错误[%d]",sta,Infor_Warning);
	sta=Reg_SendInfor();
	if(sta)DeBug("未成功初始化设备[%d]",sta,Infor_Warning);
	}
}
