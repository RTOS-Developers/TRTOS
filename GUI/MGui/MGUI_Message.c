#include <include.h>
#include <MGUI_Message.h>
#include <Gui_Include.h>
#include <Tools_Include.h>
#include <LCD_Driver.h>

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
		case 0:TGUI_Fill_Rectangle(X,Y,X+W,Y+H,Tos_GetStyleHandle()->B_Color);break;
		case 1:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(XX+X,YY+Y,P[XX+YY*W+2]);break;
		case 2:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(XX+X,YY+Y,~P[XX+YY*W+2]);break;
		case 3:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(X+W-XX,YY+Y,P[XX+YY*W+2]);break;
		case 4:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(XX+X,Y+H-YY,P[XX+YY*W+2]);break;
		case 5:for(YY=0;YY<H;YY++)for(XX=0;XX<W;XX++)if(XX<LCD_XSIZE)if(Y<LCD_YSIZE)LCD_Set_Pixel(X+W-XX,Y+H-YY,P[XX+YY*W+2]);break;
		default:break;
	}
}
/****************************************************************************
Date:2014-11-12
Func:消息
Note:
****************************************************************************/
void * MessageBox(uint8 *String)
{
	void *P;
	//uint16 *PBMP;
	TGUI_DefaultStyle();
	TGUI_Set_Font(TGUI_FontType_48X48);
	TGUI_Set_Color(Color_White,Magenta,Black,Null);
	TGUI_Set_Region(0,0,LCD_XSIZE,LCD_YSIZE,Align_CenterCenter);
	MGUI_DrawBitMap((LCD_XSIZE-TOSLogoLib[0])/2,(LCD_YSIZE-TOSLogoLib[1])/2,(uintbus)TOSLogoLib,1);
	Tos_TaskDelay(1000);
	TGUI_Set_Color(Black,Black,Black,Null);
	MGUI_DrawBitMap((LCD_XSIZE-TOSLogoLib[0])/2,(LCD_YSIZE-TOSLogoLib[1])/2,(uintbus)TOSLogoLib,0);
	return P;
}
