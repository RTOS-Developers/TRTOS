#ifndef _TGUI_2D_DRIVER_H_
#define _TGUI_2D_DRIVER_H_
#include <Include.h>


#define Color_Red       0xF800
#define Color_Green     0x07E0
#define Color_Blue      0x001F
#define Color_White     (0xFFFF)
#define Color_Black     (0x0000)	

#define Align_LeftTop          0x00
#define Align_CenterTop        0x01
#define Align_RightTop         0x02
#define Align_LeftCenter       0x03
#define Align_CenterCenter     0x04
#define Align_RightCenter      0x05
#define Align_LeftBottom       0x06
#define Align_CenterBottom     0x07
#define Align_RightBottom      0x08

#define Dir_UpDown      0x00
#define Dir_DownUp      0x01
#define Dir_LeftRight   0x02
#define Dir_RightLeft   0x03
#define Dir_Last        0x04    //上一个
#define Dir_Next        0x05    //下一个

/*定义矩形结构*/
typedef struct
{
    uint16 SX;
    uint16 SY;
    uint16 EX;
    uint16 EY;
}Rectangle;   


typedef struct TB{
	uint16 B_Color;
	uint16 F_Color;
	uint16 S_Color;
	uint16 Null_Color;
	uint16 SX;
	uint16 SY;
	uint16 EX;
	uint16 EY;
}_TGui_2D;

void Draw_X_8Bit(uint8 Data,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color);
void Draw_Y_8Bit(uint8 Data,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color);
void Draw_Full_List(void *Data,uint16 Data_Length,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color);
void Draw_Row_List(void *Data,uint16 Data_Length,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color);
void TGUI_Draw_DotLine(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Fcolor,uint16 Bcolor);
void TGUI_Draw_Rectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color);
void TGUI_Fill_Rectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color);
void TGUI_Draw_Circle(uint16 X0,uint16 Y0,uint16 R,uint16 Color);
void TGUI_DrawRound(int x0,int y0,int r,int color);
void TGUI_Draw_Image(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 PX,uint16 PY,void *ImageAddr);
void TGUI_Set_Rectangle(Rectangle *Rect,uint16 SX,uint16 SY,uint16 EX,uint16 EY);
void TGUI_Draw_DotRectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Fcolor,uint16 Bcolor);
void TGUI_Fill_ArcRectangle(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 R,uint16 ColorU,uint16 ColorD);
void TGUI_Draw_ArcRectangle(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 R,uint16 ColorU,uint16 ColorD);
void TGUI_Draw_Bitmap(uint16 X,uint16 Y,uint16 W,uint16 H,uint16 *Bmp);
void TGUI_Fill_Ring(uint16 X0,uint16 Y0,uint16 R0,uint16 R1,uint16 ColorR0,uint16 ColorR1);
void TGUI_Fill_Bar(uint16 x1, uint16 y1, uint16 x2, uint16 y2,uint16 Max,uint16 Rate);
#endif
