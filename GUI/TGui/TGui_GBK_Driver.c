#include <Include.h>
#include <Gui_include.h>
#include <Tools_include.h>
#include <Driver_Include.h>
#include <TGui_GBk_Driver.h>
#ifndef _GBK_LIB_H_
#include <GBK_Lib.c>
#endif
_TGui_Style *P_TGui_Style;
_TGUI_Font *P_TGUI_Font;
_TGui_Style TGui_Style;
_TGUI_Font TGUI_Font;
uint16 Cursor_X,Cursor_Y;

/****************************************************************************
Date:2014-5-12
Func:获得参数配置实体
Note:
****************************************************************************/
void Tos_SetStyleHandle(_TGui_Style *PTG)
{
	P_TGui_Style=PTG;
}
/****************************************************************************
Date:获取实体指针
Func:获得参数配置实体
Note:
****************************************************************************/
_TGui_Style *Tos_GetStyleHandle()
{
	return P_TGui_Style;
}
/****************************************************************************
Date:2014-5-12
Func:设置字体描述实体
Note:
****************************************************************************/
void Tos_SetFontHandle(_TGUI_Font *TF)
{
	P_TGUI_Font=TF;
}
/****************************************************************************
Date:2014-5-12
Func:获取字体描述指针
Note:
****************************************************************************/
_TGUI_Font *Tos_GetFontHandle()
{
	return P_TGUI_Font;
}
/****************************************************************************
Date:2014-5-12
Func:设置为默认风格
Note:
****************************************************************************/
void TGUI_DefaultStyle()
{
	P_TGui_Style=&TGui_Style;
	P_TGUI_Font=&TGUI_Font;
}
/****************************************************************************
Date:2014-5-12
Func:设置字体类型
Note:
****************************************************************************/
void TGUI_Set_LocalFont(const FontTypeStruct *Font)
{
	P_TGUI_Font->FontLib=Font;
	P_TGUI_Font->SizeEH=P_TGUI_Font->FontLib->High;
	P_TGUI_Font->SizeEW=P_TGUI_Font->FontLib->Wide;
	P_TGUI_Font->SizeCH=P_TGUI_Font->FontLib->High;
	P_TGUI_Font->SizeCW=P_TGUI_Font->FontLib->Wide*2;
	P_TGUI_Font->LibSizeE=P_TGUI_Font->FontLib->High*(P_TGUI_Font->SizeEW/8+!(!(P_TGUI_Font->SizeEW%8)));
	P_TGUI_Font->LibSizeC=P_TGUI_Font->FontLib->High*(P_TGUI_Font->SizeCW/8+!(!(P_TGUI_Font->SizeCW%8)));
	P_TGUI_Font->P_ELib=&P_TGUI_Font->FontLib->ELib[0];
	P_TGUI_Font->P_CLib=&P_TGUI_Font->FontLib->CLib[0];
	P_TGUI_Font->P_CIndex=&P_TGUI_Font->FontLib->CIndex[0];
	P_TGUI_Font->LibCCount=GetCountSize(P_TGUI_Font->P_CIndex);
	if(P_TGUI_Font->FontLib->EIndex)P_TGUI_Font->LibECount=GetTextLength((void *)P_TGUI_Font->FontLib->EIndex);//英文字符无序时添加索引
}
void TGUI_Set_Font(TGUI_FontType FontType)
{
	P_TGui_Style->FontType=FontType;
	switch((uint8)FontType)
	{
		case TGUI_FontType_12X12:TGUI_Set_LocalFont(&GBKLib_FontType1212);break;
		case TGUI_FontType_16X16:TGUI_Set_LocalFont(&GBKLib_FontType1616);break;
		case TGUI_FontType_16X24:TGUI_Set_LocalFont(&GBKLib_FontType1624);break;
		case TGUI_FontType_24X24:TGUI_Set_LocalFont(&GBKLib_FontType2424);break;
		case TGUI_FontType_32X32:TGUI_Set_LocalFont(&GBKLib_FontType3232);break;
		case TGUI_FontType_48X48:TGUI_Set_LocalFont(&GBKLib_FontType4848);break;
	}
	
}
/****************************************************************************
Date:2014-5-12
Func:设置全局光标
Note:光标在哪里，紧着着打印哪里
****************************************************************************/

void TGUI_Set_LineN(uint8 Line)
{
	P_TGui_Style->Line=Line;
}
/****************************************************************************
Date:2014-5-12
Func:设置颜色
Note:
****************************************************************************/
void TGUI_Set_Color(uint16 FColor,uint16 SColor,uint16 BColor,uint16 AColor)
{
	P_TGui_Style->F_Color=FColor;
	P_TGui_Style->B_Color=BColor;
	P_TGui_Style->S_Color=SColor;
	P_TGui_Style->A_Color=AColor;
}
/****************************************************************************
Date:2014-5-12
Func:设置打印区域
Note:
****************************************************************************/
void TGUI_Set_Region(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint8 AlignMode)
{
	P_TGui_Style->SX=SX;
	P_TGui_Style->SY=SY;
	P_TGui_Style->EX=EX;
	P_TGui_Style->EY=EY;
	P_TGui_Style->AlignMode=AlignMode;
}
/****************************************************************************
Date:2014-5-12
Func:绘制单个英文字符
Note:
****************************************************************************/
void TGUI_Draw_ASCII(uint16 X,uint16 Y,uint16 Char)
{              
  uint32 Addr;
	uint16 Color;
	uint16 BColor;
	uint8 i,w;
	Addr=Char-' '; 
	Addr*=P_TGUI_Font->LibSizeE;
	Addr+=(uint32)P_TGUI_Font->P_ELib;
	w=(P_TGUI_Font->SizeEW+4)/8;//+7是为了照顾宽度为6/12的字符
	for(i=0;i<w;i++)
	{
		if(P_TGui_Style->SelectTrue==True){Color=P_TGui_Style->S_Color;BColor=P_TGui_Style->F_Color;}
		else {Color=P_TGui_Style->F_Color;BColor=P_TGui_Style->B_Color;}
		Draw_Full_List((uint8 *)(Addr+i*P_TGUI_Font->SizeEH),P_TGUI_Font->SizeEH,X,Y,Color,BColor,P_TGui_Style->A_Color);
		X+=8;
	}
}
/****************************************************************************
Date:2014-5-12
Func:从字库索引中搜索中文字符的偏移位置
Note:
****************************************************************************/
uint16 TGUI_Index_Word(uint16 Word)
{
 	 uint16 i=0,temp;
	 for(i=0;i<P_TGUI_Font->LibCCount;i++)
	 {
	 temp=P_TGUI_Font->P_CIndex[i];
	 temp<<=8;
	 temp+=P_TGUI_Font->P_CIndex[i+1];
	 if(Word==temp)return i/2;
	 i++;
	 }
	 return 0;
}
/****************************************************************************
Date:2014-5-12
Func:绘制单个中文字符
Note:
****************************************************************************/
void TGUI_Draw_Word(uint16 X,uint16 Y,uint16 Word)
{
	uint32 Addr;
	uint16 Color;
	uint16 i,w;
	Addr=TGUI_Index_Word(Word);
	Addr*=P_TGUI_Font->LibSizeC;
	Addr+=(uint32)P_TGUI_Font->P_CLib;
	w=P_TGUI_Font->SizeCW/8+!(!(P_TGUI_Font->SizeCW%8));//(P_TGUI_Font->SizeCW)/8;//弥补1212大小的中文字符
	for(i=0;i<w;i++)
	{
		if(P_TGui_Style->SelectTrue==True)Color=P_TGui_Style->S_Color;
		else Color=P_TGui_Style->F_Color;
		Draw_Full_List((uint8 *)(Addr+i*P_TGUI_Font->SizeCH),P_TGUI_Font->SizeCH,X,Y,Color,P_TGui_Style->B_Color,P_TGui_Style->A_Color);
		X+=8;
	}
}
/*---------------------------------------------------------------------------------------------------------------------------
 Func: 获得字符串的像素长度
 Time: 2014-1-5
 Ver.: V1.0            
 Note: Bit15: 1/含中文字符  0/仅西文字符   
       Bit[14:0]: 字符串显示占用的像素数
       本方法最高统计长度1024字符
---------------------------------------------------------------------------------------------------------------------------*/
uint16 TGUI_Get_StringPixel(uint8 *Pstr)
{      
    uint16 L=0; 
    while(*Pstr>0){
        if(*Pstr++ > 0x80){*Pstr++;L+=P_TGUI_Font->SizeCW;L|=0x8000;}
        else L+=P_TGUI_Font->SizeEW;
    }          
    return L;
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D带格式绘制字符串
 Time: 2014-5-5
 Ver.: V1.0		V1.1
 Note: 本方法用于绘制RAM中的字符串,带分行格式
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_FormatString(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint8 *Pstr,uint8 AlignMode,uint8 Line)
{                                                 
    uint16 L,X,Y,T,K;
	  uint8 LineIndex=1;
    SX+=2;EX-=2;X=0;Y=0;										                        //保留显示间距
    L=TGUI_Get_StringPixel(Pstr);        												//取得当前字符串显示像素数
    if(L&0x8000){L&=0x7FFF;T=P_TGUI_Font->SizeCH;}
    else {T=P_TGUI_Font->SizeEH;}
    switch(AlignMode){
        case Align_LeftTop:
            X=SX+1;
            Y=SY+1;
            break;
        case Align_CenterTop:
            X=(EX-SX-L)/2+SX;
            Y=SY+1;
            break;
        case Align_RightTop:
            X=EX-L-1;
            Y=SY+1;
            break;
        case Align_LeftCenter:
            X=SX+1;
            Y=(EY-SY-T)/2+SY;
            break;
        case Align_CenterCenter:
            X=(EX-SX-L)/2+SX;
            Y=(EY-SY-T)/2+SY;
            break;
        case Align_RightCenter:
            X=EX-L-1;
            Y=(EY-SY-T)/2+SY;
            break;
        case Align_LeftBottom:
            X=SX+1;          
            Y=EY-Y-1;
            break;
        case Align_CenterBottom:
            X=(EX-SX-L)/2+SX;
            Y=EY-Y-1;
            break;
        case Align_RightBottom:
            X=EX-L-1;
            Y=EY-Y-1;
            break;
    }                 
    while((*Pstr)>0)
    {    
        L=*Pstr++;                   
        if(L>0x80){
			L<<=8;	L|=*Pstr;	Pstr++;
            //L=(L-0xA1)*94+(*Pstr++)-0xA1; 	 //GB2312			
			//L=(L-0x81)*192+(*Pstr++)-0x40;		 //GBK
            K=P_TGUI_Font->SizeCW; 
            TGUI_Draw_Word(X,Y,L);
            X+=K;
        }else{ 
            if(L=='\n'){X=SX;Y+=T;SX=P_TGui_Style->SX;P_TGui_Style->SY+=T;}
						else if(L=='\r');
            else {
                K=P_TGUI_Font->SizeEW;
                TGUI_Draw_ASCII(X,Y,L);
                X+=K;
            }
        }                                                           
        L=*Pstr;
        if(L>0x80) K=X+P_TGUI_Font->SizeCW;
        else       K=X+P_TGUI_Font->SizeEW;        
        if(K>EX){X=SX;Y+=T;if(Y>EY)return;LineIndex++;if(LineIndex>Line);}
    }
}
       
void TGUI_Clear_Show()
{
		LCD_Clear_Screen(P_TGui_Style->B_Color);
}
void Printf(char *fmt,...)
{
    va_list ap;
    char String[256];
    va_start(ap,fmt);
    vsprintf(String,fmt,ap);
	  TGUI_Draw_FormatString(P_TGui_Style->SX,P_TGui_Style->SY,P_TGui_Style->EX,P_TGui_Style->EY,(uint8 *)String,P_TGui_Style->AlignMode,P_TGui_Style->Line);
    va_end(ap);
}
/****************************************************************************
Date:2014-11-12
Func:显示图片不管是ROM中的还是RAM中的
Note:Mode 0清除1绘画2反绘3左右镜像4上下镜像5上下左右镜像
****************************************************************************/
void MGUI_DrawBitMap(uint16 X,uint16 Y,uintbus BMP,uint8 Mode)
{
	uint16 XX,YY,W,H;
	uint16 *P;
	P=(uint16 *)BMP;
	W=P[0];H=P[1];
	switch(Mode)
	{
		case 0:TGUI_Fill_Rectangle(X,Y,X+W,Y+H+1,Tos_GetStyleHandle()->B_Color);break;
		case 1:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)if(Tos_GetStyleHandle()->A_Color!=P[XX+YY*W+2])LCD_Set_Pixel(XX+X,YY+Y,P[XX+YY*W+2]);break;
		case 2:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)if(Tos_GetStyleHandle()->A_Color!=~P[XX+YY*W+2])LCD_Set_Pixel(XX+X,YY+Y,~P[XX+YY*W+2]);break;
		case 3:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)if(Tos_GetStyleHandle()->A_Color!=P[XX+YY*W+2])LCD_Set_Pixel(X+W-1-XX,YY+Y,P[XX+YY*W+2]);break;
		case 4:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(XX+X,Y+H-1-YY,P[XX+YY*W+2]);break;
		case 5:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(X+W-1-XX,Y+H-1-YY,P[XX+YY*W+2]);break;
		case 6:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)if(Tos_GetStyleHandle()->A_Color!=P[XX+YY*W+2])LCD_Set_Pixel(XX+X,YY+Y,P[XX+YY*W+2]);break;
		default:break;
	}
}
