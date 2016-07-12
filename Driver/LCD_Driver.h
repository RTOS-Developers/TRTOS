#ifndef _LCD_DRIVER_H_
#define _LCD_DRIVER_H_
#include <Include.h>



#if defined(LCD_SelectSSD1963)
	#include <SSD1963_Driver.h>
	#define LCD_Initializtion SSD1963_Initializtion
	#define Lcd_Write  SSD1963_Write 
	#define Lcd_Read   SSD1963_Read
	#define LCD_Clear_Screen SSD1963_Clear_Screen
	#define LCD_XSIZE  SSD1963_4_3_XSIZE
	#define LCD_YSIZE  SSD1963_4_3_YSIZE
	#define LCD_XMAX SSD1963_4_3_XMAX
	#define LCD_YMAX SSD1963_4_3_YMAX
	#define LCD_Set_Pixel SSD1963_Set_Pixel
	#define LCD_Get_Pixel SSD1963_Get_Pixel
	#define LCD_SET_Light SSD1963_SET_Light
	#define LCD_Draw_LineX SSD1963_Draw_LineX
	#define LCD_Draw_LineY SSD1963_Draw_LineY
	#define LCD_Clear_Area SSD1963_Clear_Area
	#define LCD_Draw_DotLineX SSD1963_Draw_DotLineX
	#define LCD_Draw_DotLineY SSD1963_Draw_DotLineY
	#define LCD_Fill_LineX SSD1963_Fill_LineX
	#define LCD_Fill_LineY SSD1963_Fill_LineY

#elif defined(LCD_SelectHX8347)
	#include <HX8347_Driver.h>

	
#elif defined(LCD_SelectSSD1325)
	#include <SSD1325_Driver.h>
	#define LCD_Initializtion SSD1325_Init
	#define Lcd_Write  SSD1325_Write 
	#define Lcd_Read   SSD1325_Read
	#define LCD_Clear_Screen SSD1325_Clear_Screen
	#define LCD_XSIZE  SSD1325_2_7_XSIZE
	#define LCD_YSIZE  SSD1325_2_7_YSIZE
	#define LCD_Set_Pixel SSD1325_Set_Pixel
	#define LCD_Get_Pixel SSD1325_Get_Pixel
	#define LCD_SET_Light SSD1325_SET_Light
#else
	
 

#endif

	void LCD_Draw_LineY(uint16 SX,uint16 SY,uint16 H,uint16 Color); 
	void LCD_Draw_LineX(uint16 SX,uint16 SY,uint16 W,uint16 Color);
	void LCD_Fill_LineX(uint16 SX,uint16 SY,uint16 H,uint16 *Cbuffer);
	void LCD_Fill_LineY(uint16 SX,uint16 SY,uint16 H,uint16 *Cbuffer);
	void LCD_Draw_DotLineY(uint16 SX,uint16 SY,uint16 H,uint16 Fcolor,uint16 Bcolor);
	void LCD_Draw_DotLineX(uint16 SX,uint16 SY,uint16 W,uint16 Fcolor,uint16 Bcolor);
	void LCD_Clear_Area(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Color);
	extern uint16 LCD_Get_Pixel(uint16 X,uint16 Y);
	extern void LCD_Set_Pixel(uint16 X,uint16 Y,uint16 Color);
//	#ifndef LCD_XSIZE
//	#define LCD_XSIZE  240
//#endif
//#ifndef LCD_YSIZE
//	#define LCD_YSIZE  320
// #endif
/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define WHITE          0xFFFF
#define BLACK          0x0000
#define GRAY           0xF7DE
#define BLUE           0x001F
#define BLUE2          0x051F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define DGRAY          Grey
#define DCYAN          CYAN
#define PURPLE         MAGENTA
#define DGREEN          0x07E0

extern uint8 DeviceId_LCD;
void TGui_Draw_Line(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color);
void DeviceMount_Lcd(void);

#endif

