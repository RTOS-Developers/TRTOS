#ifndef _HX8347_DRIVER_H_
#define _HX8347_DRIVER_H_
#include <include.h>

#define HX8347_3_2_XSIZE  320
#define HX8347_3_2_YSIZE  240
#define HX8347_3_2_XMAX   319
#define HX8347_3_2_YMAX   239

#define LCD_Initializtion HX8347_Initializtion
#define LCD_Clear_Screen HX8347_Clear_Screen
#define LCD_XSIZE  HX8347_3_2_XSIZE
#define LCD_YSIZE  HX8347_3_2_YSIZE
#define LCD_XMAX HX8347_3_2_XMAX
#define LCD_YMAX HX8347_3_2_YMAX  
#define LCD_Set_Pixel SetPoint
#define LCD_Get_Pixel GetPoint
#define FAST_Draw_Full_List
#define FAST_Fill_Rectangle
extern uint8 DeviceId_LCD;
void HX8347_Initializtion(void);
void SetPoint(u16 x,u16 y,u16 point);
u16 GetPoint(u16 x,u16 y);
void HX8347_Clear_Screen(u16 dat);
void Fast_Draw_Fill_List(void *Data,uint16 Data_Length,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color);
void Fast_Fill_Rectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color);
#endif
