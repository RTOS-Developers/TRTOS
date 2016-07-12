#include <..\USER\Prj_TP502T\Ht1621_Face.h>
#include <Rtc_Driver.h>
UIShowStruct UIShowValue;
uint8 *Ht1621CharList="0123456789.- UPFTBOHCX";
uint8 LCD_BUF[16];
/*U


					 ----BIT(3)----
					|              |
					|BIT(7)        |BIT(2)
					 ----BIT(6)----
					|              |
					|BIT(5)        |BIT(1)
  	DP	   ----BIT(0)----	
		BIT(4)
*/
/*D


					 ----BIT(7)----
					|              |
					|BIT(3)        |BIT(6)
					 ----BIT(2)----
					|              |
					|BIT(1)        |BIT(5)
  	DP	   ----BIT(4)----	
		BIT(0)
*/
const uint8 Ht1621CodeUpList[]={
	X_X_XXXX,//0
  _____XX_,//1
	_XX_XX_X,//2
	_X__XXXX,//3
	XX___XX_,//4
	XX__X_XX,//5
	XXX_X_XX,//6
	____XXX_,//7
	XXX_XXXX,//8
	XX__XXXX,//9
	___X____,//.
	_X______,//-
	________,// 
	X_X__XXX,//U
	XXX_XX__,//P
	XXX_X___,//F
	XXX____X,//T
	XXX___XX,//B
	_XX_X_XX,//O
	XXX__XX_,//H
	X_X_X__X,//C
};
const uint8 Ht1621CodeDownList[]={
	XXXXX_X_,//0
	_XX_____,//1
	XX_X_XX_,//2
	XXXX_X__,//3
	_XX_XX__,//4
	X_XXXX__,//5
	X_XXXXX_,//6
	XXX_____,//7
	XXXXXXX_,//8
	XXXXXX__,//9
	_______X,//.
	_____X__,//-
	________,// 
	_XXXX_X_,//U
	XX__XXX_,//P
	X___XXX_,//F
	___XXXX_,//T
	__XXXXX_,//B
	X_XX_XX_,//O
	_XX_XXX_,//H
	X__XX_X_,//C
};
const uint8 Ht1621CodeTime[]=
{
	X_X_XXXX,/*0*/
	_____XX_,/*1*/
	_XX_XX_X,/*2*/
	_X__XXXX,/*3*/
	XX___XX_,/*4*/
	XX__X_XX,/*5*/
	XXX_X_XX,/*6*/
	____XXX_,/*7*/
	XXX_XXXX,/*8*/
	XX__XXXX,/*9*/
};
uint8 SerachChear(uint8 Char,const uint8 *Code)
{
	uint8 i=0;
	while(*Code)
	{
		if(Char==*Code)return i;
		i++;
		Code++;
	}
	return i;
}
uint8 Ht1621_GetCode(uint8 Char,const uint8 *P)
{
	uint8 Data;
	Data=P[SerachChear(Char,Ht1621CharList)];
	return Data;
}
	
void Ht1621_ShowASCII(uint8 Index,uint8 Data)
{
	Ht1621_WriteReg(Index, Data&0x0f);
	Ht1621_WriteReg(Index+1, Data>>4);
	LCD_BUF[Index/2]=Data;
}
void Ht1621_SHowUpString(uint8 *Str)
{
	uint8 i=0;
	uint8 Buf[8];
	uint8 *P=Str;
	BufferFill(&Buf[0],0x00,sizeof(Buf));
	while(*P)
	{
		if(*P!='.')
		{
			Buf[i]|=Ht1621_GetCode(*P,Ht1621CodeUpList);
			i++;
			P++;
		}
		else 
		{
			P++;
			Buf[i]|=Ht1621_GetCode('.',Ht1621CodeUpList);
		}
		if(i>3)break;
	}
	P=&Buf[0];i=6;
	while(1)
	{
		if(Buf[1]&Ht1621_GetCode('.',Ht1621CodeUpList))
		{
			for(i=3;i>0;i--){Buf[i]=Buf[i-1];Buf[i-1]=0x00;}
		}else break;
	}
	for(i=0;i<4;i++){Ht1621_ShowASCII(6-(i*2),*P++);}
}
void Ht1621_SHowDownString(uint8 *Str)
{
	uint8 i=0;
	uint8 Buf[8];
	uint8 *P=Str;
	BufferFill(&Buf[0],0x00,sizeof(Buf));
	while(*P)
	{
		if(*P!='.')
		{
			Buf[i]|=Ht1621_GetCode(*P,Ht1621CodeDownList);
			i++;
			P++;
		}
		else 
		{
			P++;
			Buf[i]|=Ht1621_GetCode('.',Ht1621CodeDownList);
		}
		if(i>3)break;
	}
	P=&Buf[0];
	while(1)
	{
		if(Buf[1]&Ht1621_GetCode('.',Ht1621CodeDownList))
		{
			for(i=3;i>0;i--){Buf[i]=Buf[i-1];Buf[i-1]=0x00;}
		}else break;
	}
	for(i=0;i<4;i++){Ht1621_ShowASCII(10+(i*2),*P++);}
	
}
void Ht1621_DrawTime(uint8 Hour,uint8 Min)
{
	uint8 H;
	static BOOL ShowDot;
	uint8 Buf[4];
	Buf[0]=Ht1621CodeTime[Min%10];
	Buf[1]=Ht1621CodeTime[Min/10];
	Buf[2]=Ht1621CodeTime[Hour%10];
	H=Hour/10;
	if(H==0)Buf[3]=0x00;else if(H==1)Buf[3]=0x06;else Buf[3]=0x0c;
	if(ShowDot)Buf[0]|=0x10;
	ShowDot=(BOOL)!ShowDot;
	Buf[3]|=0x80;//钟图标
	Buf[3]|=0x10;//横线图标
	Ht1621_ShowASCII(20,Buf[0]);
	Ht1621_ShowASCII(22,Buf[1]);
	Ht1621_ShowASCII(24,Buf[2]);
	Ht1621_ShowASCII(26,Buf[3]);
}
void Ht1621_DrawBase()
{
	uint8 i=0;
	for(i=0;i<16;i++)
	{
	Ht1621_WriteReg(i*2, LCD_BUF[i]&0x0f);
	Ht1621_WriteReg(i*2+1, LCD_BUF[i]>>4);
	}
	//for(i=0;i<32;i++)Ht1621_WriteReg(i, 0xff);//全亮
}
void Ht1621_DrawUpgrade()
{
	uint8 Buf[10];
	UIShowValue.UnitFlag=HSF_LPercentage;
	Ht1621_DrawTime(UIShowValue.DateTime.Hour,UIShowValue.DateTime.Min);
	Ht1621_SHowUpString("-UP-");
	FmtToBuf((char*)&Buf[0],"%.2f  ",(float)UIShowValue.UpgradePogress);
	Ht1621_SHowDownString(&Buf[0]);
	LCD_BUF[4]|=S1;//通道一铃显示
	LCD_BUF[6]|=S0;//通道二铃显示
	LCD_BUF[15]=UIShowValue.UnitFlag;
	LCD_BUF[9]=S0;
	if(UIShowValue.CH_Value[2]>3.8)LCD_BUF[9]|=S2;
	if(UIShowValue.CH_Value[2]>3.9)LCD_BUF[9]|=S1;
	if(UIShowValue.CH_Value[2]>4.0)LCD_BUF[9]|=S3;
	if(UIShowValue.CH_Value[2]>4.1)LCD_BUF[9]|=S4;
	if(UIShowValue.RSSI<99)LCD_BUF[4]|=S4;else LCD_BUF[4]&=~S4;
	if(UIShowValue.RSSI<50)LCD_BUF[4]|=S5;else LCD_BUF[4]&=~S5;
	if(UIShowValue.RSSI<20)LCD_BUF[4]|=S6;else LCD_BUF[4]&=~S6;
	if(UIShowValue.RunFlag&WRF_GPRS_Connected)LCD_BUF[14]|=S6;else LCD_BUF[14]&=~S6;
	if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH1Min))LCD_BUF[4]|=S0;else LCD_BUF[4]&=~S0;//通道一报警
	if(UIShowValue.RunFlag&(WRF_CH2Max|WRF_CH2Min))LCD_BUF[5]|=S0;else LCD_BUF[5]&=~S0;//通道二报警
	if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH2Max))LCD_BUF[14]|=S7;else LCD_BUF[14]&=~S7;//上限报警
	if(UIShowValue.RunFlag&(WRF_CH1Min|WRF_CH2Min))LCD_BUF[14]|=S5;else LCD_BUF[14]&=~S5;//上限报警
	if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)LCD_BUF[14]|=S3|S1;else LCD_BUF[14]&=~(S3|S1);
	Ht1621_DrawBase();
}
void Ht1621_DrawMenu(uint8 i)
{
	uint8 n;
	for(n=0;n<16;n++)
	{
		LCD_BUF[n]=0x00;
	}
	Ht1621_SHowUpString("BOOT");
	switch(i)
	{
		case 0:Ht1621_SHowDownString("TP-0");break;
		case 1:Ht1621_SHowDownString("0C-1");break;
		case 2:Ht1621_SHowDownString("UC-2");break;
		case 3:Ht1621_SHowDownString("UP-3");break;
		case 4:Ht1621_SHowDownString("0P-3");break;
		default:Ht1621_SHowDownString("TP-0");break;
	}
	
	Ht1621_DrawBase();
}
void Ht1621_DrawHome()
{
	uint8 Buf[10];
	static uint8 i=0;
	UIShowValue.UnitFlag=0;
	Ht1621_DrawTime(UIShowValue.DateTime.Hour,UIShowValue.DateTime.Min);
	FmtToBuf((char*)&Buf[0],"%.2f  ",UIShowValue.CH_Value[0]);
	if(UIShowValue.RunFlag&WRF_SensorCH1Connected)
	{
		Ht1621_SHowUpString(&Buf[0]);
		UIShowValue.UnitFlag|=HSF_HCelsiusC;
	}else
	{
		Ht1621_SHowUpString("----");
		UIShowValue.UnitFlag|=HSF_HCelsiusC;
	}
	FmtToBuf((char*)&Buf[0],"%.2f  ",UIShowValue.CH_Value[1]);
	if(UIShowValue.RunFlag&WRF_SensorCH2Connected)
	{
		if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)UIShowValue.UnitFlag|=HSF_LPercentage;
		else UIShowValue.UnitFlag|=HSF_LCelsiusC;
		Ht1621_SHowDownString(&Buf[0]);
	}else
	{
		if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)UIShowValue.UnitFlag|=HSF_LPercentage;
		else UIShowValue.UnitFlag|=HSF_LCelsiusC;
		Ht1621_SHowDownString("----");
	}
	LCD_BUF[4]|=S1;//通道一铃显示
	LCD_BUF[6]|=S0;//通道二铃显示
	LCD_BUF[15]=UIShowValue.UnitFlag;
	LCD_BUF[9]=S0;
	if(UIShowValue.RunFlag&WRF_EXP_Connected)
	{
		if(i>1)LCD_BUF[9]|=S2;
		if(i>2)LCD_BUF[9]|=S2|S1;
		if(i>3)LCD_BUF[9]|=S1|S2|S3;
		if(i++>3)i=1;
	}
	else
	{
		if(UIShowValue.CH_Value[2]>3.5)LCD_BUF[9]|=S2;
		if(UIShowValue.CH_Value[2]>3.7)LCD_BUF[9]|=S1;
		if(UIShowValue.CH_Value[2]>4.0)LCD_BUF[9]|=S3;
	}
	if(UIShowValue.RunFlag&WRF_EXP_Connected)LCD_BUF[9]|=S4;
	if(UIShowValue.RunFlag&WRF_USB_Connected)LCD_BUF[14]=S4;else LCD_BUF[14]&=~S4;
	//if(UIShowValue.CH_Value[2]>4.1)LCD_BUF[9]|=S4;
	if(UIShowValue.RSSI<99&UIShowValue.RSSI>0)LCD_BUF[4]|=S4;else LCD_BUF[4]&=~S4;
	if(UIShowValue.RSSI<99&UIShowValue.RSSI>10)LCD_BUF[4]|=S5;else LCD_BUF[4]&=~S5;
	if(UIShowValue.RSSI<99&UIShowValue.RSSI>20)LCD_BUF[4]|=S6;else LCD_BUF[4]&=~S6;
	if(UIShowValue.RunFlag&WRF_GPRS_Connected)LCD_BUF[14]|=S6;else LCD_BUF[14]&=~S6;
	if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH1Min))LCD_BUF[4]|=S0;else LCD_BUF[4]&=~S0;//通道一报警
	if(UIShowValue.RunFlag&(WRF_CH2Max|WRF_CH2Min))LCD_BUF[5]|=S0;else LCD_BUF[5]&=~S0;//通道二报警
	if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH2Max))LCD_BUF[14]|=S7;else LCD_BUF[14]&=~S7;//上限报警
	if(UIShowValue.RunFlag&(WRF_CH1Min|WRF_CH2Min))LCD_BUF[14]|=S5;else LCD_BUF[14]&=~S5;//上限报警
	if(UIShowValue.RunFlag&WRF_GPS_Connected)LCD_BUF[4]|=S2;else LCD_BUF[4]&=~S2;
	//LCD_BUF[14]=S2;MEM
	//LCD_BUF[14]=S0;存储
	//LCD_BUF[14]=S4;USB
//	LCD_BUF[14]=S5;//MIN
//	LCD_BUF[14]=S7;//MAX
	if(UIShowValue.RecordCount==0xff)LCD_BUF[13]|=S6;else LCD_BUF[13]&=~S6;
	if(UIShowValue.RunFlag&WRF_BeepDisable)LCD_BUF[13]&=~S5;else LCD_BUF[13]|=S5;
	if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)LCD_BUF[14]|=S3|S1;else LCD_BUF[14]&=~(S3|S1);
	Ht1621_DrawBase();
}
void Ht1621_DrawUpDown(uint8 i)
{
	uint8 BufH[10],BufL[10];
	uint8 n;
	for(n=0;n<16;n++)
	{
		LCD_BUF[n]=0x00;
	}
	switch(i)
	{
		case 0:
			FmtToBuf((char*)&BufH[0],"%.2f  ",UIShowValue.CH_Hold[0].Max);
			FmtToBuf((char*)&BufL[0],"%.2f  ",UIShowValue.CH_Hold[0].Min);
			Ht1621_SHowUpString(&BufH[0]);
			Ht1621_SHowDownString(&BufL[0]);
			LCD_BUF[4]|=S1;//通道一铃显示
			LCD_BUF[14]=S5|S7;//MAXMIN
			break;
		case 1:
			FmtToBuf((char*)&BufH[0],"%.2f  ",UIShowValue.CH_Hold[1].Max);
			FmtToBuf((char*)&BufL[0],"%.2f  ",UIShowValue.CH_Hold[1].Min);
			Ht1621_SHowUpString(&BufH[0]);
			Ht1621_SHowDownString(&BufL[0]);
			LCD_BUF[6]|=S0;//通道二铃显示
			LCD_BUF[14]=S5|S7;//MAXMIN
			break;
		case 2:
			FmtToBuf((char*)&BufH[0],"%.2f  ",UIShowValue.SaveValue.CH_Limit[0].Max);
			FmtToBuf((char*)&BufL[0],"%.2f  ",UIShowValue.SaveValue.CH_Limit[0].Min);
			Ht1621_SHowUpString(&BufH[0]);
			Ht1621_SHowDownString(&BufL[0]);
			LCD_BUF[4]|=S1;//通道一铃显示
			LCD_BUF[4]|=S0;
			break;
		case 3:
			//UIShowValue.UnitFlag=HSF_LPercentage;
			FmtToBuf((char*)&BufH[0],"%.2f ",UIShowValue.SaveValue.CH_Limit[1].Max);
			FmtToBuf((char*)&BufL[0],"%.2f ",UIShowValue.SaveValue.CH_Limit[1].Min);
			Ht1621_SHowUpString(&BufH[0]);
			Ht1621_SHowDownString(&BufL[0]);
			LCD_BUF[6]|=S0;//通道二铃显示
			LCD_BUF[5]|=S0;
			break;
		case 4:
			FmtToBuf((char*)&BufH[0],"   ");
			FmtToBuf((char*)&BufL[0],"%03d   ",UIShowValue.RecordCount);
			Ht1621_SHowUpString(&BufH[0]);
			Ht1621_SHowDownString(&BufL[0]);
			UIShowValue.UnitFlag=0;
			LCD_BUF[14]|=S3|S1;
			break;
	}
	
	LCD_BUF[15]=0;
	Ht1621_DrawTime(UIShowValue.DateTime.Hour,UIShowValue.DateTime.Min);
	Ht1621_DrawBase();
}
