#ifndef _SSD1325_H_
#define _SSD1325_H_
#include <Include.h>


//#define OLED_MODE 0
//#define SIZE 16
//#define XLevelL		0x02
//#define XLevelH		0x10
//#define XLevel		((XLevelH&0x0F)*16+XLevelL)

//#define Max_Column	0x3F  // 128/2-1 (Total Columns Devided by 2)
//#define Max_Row		0x3F  // 64-1
//#define	Brightness	0xFF 
#define SSD1325_2_7_XSIZE 	128
#define SSD1325_2_7_YSIZE 	64	    

#define OLED_RST_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_11)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOE,GPIO_Pin_11)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_2)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

 		     
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý

void OLED_WR_Byte(u8 dat,u8 cmd);
void SSD1325_Init(void);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Row_Address(unsigned char a, unsigned char b);
void SSD1325_Write(void *Addr,void *Buffer,void *Length);
void SSD1325_Read(void *Addr,void *Buffer,void *Length);
void SSD1325_Clear_Screen(uint16 Color);
void SSD1325_Set_Pixel(uint16 X,uint16 Y,uint16 Color);
uint16 SSD1325_Get_Pixel(uint16 X,uint16 Y);
void SSD1325_OutBuffer(void);
void SSD1325_SET_Light(uint8 L);

#endif
