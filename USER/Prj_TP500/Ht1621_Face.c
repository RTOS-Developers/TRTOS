#include <..\USER\Prj_TP500\Ht1621_Face.h>
#include <rtc_driver.h>
UIShowStruct UIShowValue;
uint8 *Ht1621CharList="0123456789.- ";
uint8 LCD_BUF[16];
/*


					 ----BIT(3)----
					|              |
					|BIT(7)        |BIT(2)
					 ----BIT(6)----
					|              |
					|BIT(5)        |BIT(1)
  	DP	   ----BIT(0)----	
		BIT(4)
*/
const uint8 Ht1621CodeUpList[]={
	X_X_XXXX,//0
	X_X_____,//1
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
};
const uint8 Ht1621CodeDownList[]={
	XXXXX_X_,//0
	____X_X_,//1
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
	if(H==1)Buf[3]=0x06;else Buf[3]=0x0c;
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
	for(i=0;i<15;i++)
	{
	Ht1621_WriteReg(i*2, LCD_BUF[i]&0x0f);
	Ht1621_WriteReg(i*2+1, LCD_BUF[i]>>4);
	}
}

void Ht1621_DrawHome()
{
	uint8 Buf[10];
	UIShowValue.UnitFlag=0x0000;
	UIShowValue.UnitFlag|=HSF_HCelsiusC|HSF_LPercentage;
	Ht1621_DrawTime(Timer.hour,Timer.min);
	FmtToBuf((char*)&Buf[0],"%.2f\n",UIShowValue.CH_Value[0]);
	Ht1621_SHowUpString(&Buf[0]);
	FmtToBuf((char*)&Buf[0],"%.2f\n",UIShowValue.CH_Value[1]);
	Ht1621_SHowDownString(&Buf[0]);
	Ht1621_ShowASCII(30,UIShowValue.UnitFlag&0xff);
	LCD_BUF[4]|=S1;//通道一铃显示
	LCD_BUF[6]|=S0;//通道二铃显示
	if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH1Min))LCD_BUF[4]|=S0;else LCD_BUF[4]&=~S0;//通道一报警
	if(UIShowValue.RunFlag&(WRF_CH2Max|WRF_CH2Min))LCD_BUF[5]|=S0;else LCD_BUF[5]&=~S0;//通道二报警
	if(UIShowValue.RunFlag&(WRF_CH1Max|WRF_CH2Max))LCD_BUF[14]|=S7;else LCD_BUF[14]&=~S7;//上限报警
	if(UIShowValue.RunFlag&(WRF_CH1Min|WRF_CH2Min))LCD_BUF[14]|=S5;else LCD_BUF[14]&=~S5;//上限报警
	//LCD_BUF[14]=S2;MEM
	//LCD_BUF[14]=S0;存储
	//LCD_BUF[14]=S4;USB
//	LCD_BUF[14]=S5;//MIN
//	LCD_BUF[14]=S6;//NET
//	LCD_BUF[14]=S7;//MAX
	if(UIShowValue.SaveValue.WorkBit&WCB_RecordEN)LCD_BUF[14]|=S3|S1;else LCD_BUF[14]&=~(S3|S1);
	Ht1621_DrawBase();
}
