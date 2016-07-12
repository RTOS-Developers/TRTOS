#ifndef _SSD1963_H_
#define _SSD1963_H_
#include <Include.h>
#include <STM32LIB_Include.h>


#define SSD1963_4_3_XSIZE  480
#define SSD1963_4_3_YSIZE  272
#define SSD1963_4_3_XMAX   479
#define SSD1963_4_3_YMAX   271

#define SSD1963_HT 	   525 	
#define SSD1963_HPS	   2
#define SSD1963_HPW	   41
#define SSD1963_LPS	   0

#define SSD1963_VT	   286
#define SSD1963_VPS	   2
#define SSD1963_VPW	   2
#define SSD1963_FPS	   0


#define SSD1963_RESET_SET   	GPIOD->BSRR=S3;						//LCD复位高电平
#define SSD1963_RESET_CLR		GPIOD->BRR=S3;						//LCD复位低电平


#define SSD1963_CMD	  (*((volatile uint16*)0x60000000))				//0x60080000
#define SSD1963_DAT8  (*((volatile uint8* )0x60020000))
#define SSD1963_DAT	  (*((volatile uint16*)0x60020000))				//0x600A0000
#define SSD1963_DAT32 (*((volatile uint32*)0x60020000))				//0x600A0000


// #define Bank1_SSD1963_D    ((uint32_t)0x60020000)    //disp Data ADDR
// #define Bank1_SSD1963_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

#define SSD1963_INC_DIRH {SSD1963_CMD=0x36;SSD1963_DAT=0x00;} 		//水平左->右 垂直上->下
#define SSD1963_INC_DIRV {SSD1963_CMD=0x36;SSD1963_DAT=0x20;} 		//垂直上->下 水平左->右
#define SSD1963_DEC_DIRV {SSD1963_CMD=0x36;SSD1963_DAT=0x80;}		//水平左->右 垂直下->上


#define DISP_HOR_RESOLUTION				SSD1963_XSIZE
#define DISP_VER_RESOLUTION				SSD1963_YSIZE
#define DISP_HOR_PULSE_WIDTH		1
#define DISP_HOR_BACK_PORCH		210
#define DISP_HOR_FRONT_PORCH	 45
#define DISP_VER_PULSE_WIDTH		1
#define DISP_VER_BACK_PORCH		 34	
#define DISP_VER_FRONT_PORCH	 10

#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)






#define SSD1963_WR_REG(reg) 	(*(__IO uint16_t *) (SSD1963_CMD)= reg )
#define SSD1963_WR_DATA(data) (*(__IO uint16_t *) (SSD1963_DAT)= data )
#define SSD1963_RD_DATA() 		(*(__IO uint16_t *) (Bank1_SSD1963_D))

#define SRAM_RD_DATA(addr) 			(*(__IO uint16_t *) (Bank1_RAM_D + (addr<<1)))
#define SRAM_WR_DATA(data,addr) (*(__IO uint16_t *) (Bank1_RAM_D + (addr<<1))= data )


extern uint8 DeviceId_LCD;
void SSD1963_Clear_Screen(uint16 Color);
void SSD1963_Set_Cursor(uint16 X,uint16 Y,uint16 EX,uint16 EY);
void SSD1963_Initializtion(void);
void SSD1963_SET_Light(u8 Light);
void SSD1963_CLOSE_DISP(void);
void SSD1963_Clear(uint16_t Color);
void SSD1963_Draw_LineX(uint16 SX,uint16 SY,uint16 W,uint16 Color);
void SSD1963_Fill_LineX(uint16 SX,uint16 SY,uint16 W,uint16 *Cbuffer);
void SSD1963_Fill_LineY(uint16 SX,uint16 SY,uint16 H,uint16 *Cbuffer);
void SSD1963_Draw_LineY(uint16 SX,uint16 SY,uint16 H,uint16 Color);
void SSD1963_Draw_DotLineX(uint16 SX,uint16 SY,uint16 W,uint16 Fcolor,uint16 Bcolor);
void SSD1963_Draw_DotLineY(uint16 SX,uint16 SY,uint16 H,uint16 Fcolor,uint16 Bcolor);
void SSD1963_Clear_Area(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Color);
void SSD1963_Set_Pixel(uint16 X,uint16 Y,uint16 Color);
uint16 SSD1963_Get_Pixel(uint16 X,uint16 Y);
void SSD1963_Write(void *Addr,void *Buffer,void *Length);
void SSD1963_Read(void *Addr,void *Buffer,void *Length);
#endif

