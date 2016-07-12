#include <Include.h>
#include <Driver_Include.h>
#include <LCD_Driver.h>

uint8 DeviceId_LCD=DeviceNull;

#if defined(LCD_SelectSSD1963)
#include <..\Driver\SSD1963_Driver.c>

#elif defined(LCD_SelectHX8347)
#include <..\Driver\HX8347_Driver.c>
#elif defined(LCD_SelectSSD1325)
#include <..\Driver\SSD1325_Driver.c>
#elif defined(LCD_SelectReally3_5)
#include <..\Driver\Really_Driver.c>

#endif




#ifndef Lcd_Write
void Lcd_Write(void *Addr,void *Buffer,void *Length){}
#endif
#ifndef Lcd_Read
void Lcd_Read(void *Addr,void *Buffer,void *Length){}
#endif
#ifndef LCD_SET_Light
void LCD_SET_Light(u8 Light){}
#endif
#ifndef LCD_Set_Pixel
void LCD_Set_Pixel(uint16 X,uint16 Y,uint16 Color){};
#endif
#ifndef LCD_Get_Pixel
uint16 LCD_Get_Pixel(uint16 X,uint16 Y){return Null;}

#endif
#ifndef LCD_Initializtion
void LCD_Initializtion(){};
#endif


/****************************************************************************
Func:Bresenham画斜线算法
Date:2013/8/14
Note:最优
****************************************************************************/
void TGui_Draw_Line(uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint16 color)
{
    int dx,dy,e;
    dx=x2-x1;
    dy=y2-y1;

    if(dx>=0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 1/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 2/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 8/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 7/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx=-dx;        //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    LCD_Set_Pixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}
                    y1-=1;
                    e+=dx;
                }
            }
        }
    }
}

#ifndef LCD_Draw_LineX
void LCD_Draw_LineX(uint16 SX,uint16 SY,uint16 W,uint16 Color)
{
	while(W--)LCD_Set_Pixel(SX++,SY,Color);
}
#endif
#ifndef LCD_Draw_LineY
void LCD_Draw_LineY(uint16 SX,uint16 SY,uint16 H,uint16 Color)
{
	while(H--)LCD_Set_Pixel(SX,SY++,Color);
}
#endif
#ifndef LCD_Clear_Area
void LCD_Clear_Area(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Color)
{
	uint16 H=EY-SX;
	while(H--)LCD_Draw_LineX(SX,SY++,EX-EY,Color);
}
#endif
#ifndef LCD_Draw_DotLineX
void LCD_Draw_DotLineX(uint16 SX,uint16 SY,uint16 W,uint16 Fcolor,uint16 Bcolor)
{
	W/=2;
	while(W--)
	{
		LCD_Set_Pixel(SX++,SY,Fcolor);
		LCD_Set_Pixel(SX++,SY,Bcolor);
	}
}
#endif
#ifndef LCD_Draw_DotLineY
void LCD_Draw_DotLineY(uint16 SX,uint16 SY,uint16 H,uint16 Fcolor,uint16 Bcolor)
{
	H/=2;
	while(H--)
	{
		LCD_Set_Pixel(SX++,SY,Fcolor);
		LCD_Set_Pixel(SX++,SY,Bcolor);
	}
}
#endif
#ifndef LCD_Fill_LineX
void LCD_Fill_LineX(uint16 SX,uint16 SY,uint16 W,uint16 *Cbuffer)
{
	while(W--)
	{
		LCD_Set_Pixel(SX++,SY,*Cbuffer++);
	}
}
#endif
#ifndef LCD_Fill_LineY
void LCD_Fill_LineY(uint16 SX,uint16 SY,uint16 H,uint16 *Cbuffer)
{
	while(H--)
	{
		LCD_Set_Pixel(SX,SY++,*Cbuffer++);
	}
}
#endif

void DeviceMount_Lcd()
{
	DeviceId_LCD=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_LCD].DeviceName="LCD";
	Tos_Device_Tab[DeviceId_LCD].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_LCD].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_LCD].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_LCD].Init=LCD_Initializtion;
	Tos_Device_Tab[DeviceId_LCD].Write=Lcd_Write;
	Tos_Device_Tab[DeviceId_LCD].Read=Lcd_Read;
	Tos_Device_Tab[DeviceId_LCD].Exit=Null;
}
