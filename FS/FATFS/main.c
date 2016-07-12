/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.1.1
* Date               : 04/07/2010
* Description        : Mass Storage demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"	

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "sdcard.h"
//#include "lcd.h"
#include "ff.h"	
//#include "pic.h"
//#include "book.h"
#include <stdio.h>
#include <string.h>

static vu32 TimingDelay = 0;

extern SD_CardInfo SDCardInfo;
extern SD_Error Status ;
extern u8 ASCII_Table[95][16];

FATFS fs;
FIL fsrc, fdst;      // file objects
BYTE buffer[1024]; // file copy buffer
FRESULT res;         // FatFs function common result code 
UINT br, bw;         // File R/W count
BYTE buff_filename[15][20];	//最多只能读9个文件，保存9个文件名,文件名采用短文件名，最多13个字符
extern uint16_t MAL_Init (uint8_t lun);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//#define Lcd_On  GPIO_SetBits(GPIOB,GPIO_Pin_5);
//#define Lcd_Off GPIO_ResetBits(GPIOB,GPIO_Pin_5);

//#define Pow_On  GPIO_SetBits(GPIOD,GPIO_Pin_3);
//#define Pow_Off GPIO_ResetBits(GPIOD,GPIO_Pin_3);

//#define Set_Rst  GPIO_SetBits(GPIOF,GPIO_Pin_11);
//#define Clr_Rst  GPIO_ResetBits(GPIOF,GPIO_Pin_11);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u8 stringcopy(BYTE *buff_to,BYTE *buff_from)
{
  u8 i=0;
  for(i=0;i<13;i++)
  buff_to[i] = buff_from[i];
  return 1;
}

void ReadSDFile(void)
{
//  FIL file;
  FILINFO finfo;
  DIR dirs;
  int i_name=0;
//  char *fn;
  XCHAR path[50]={"picture"};
  disk_initialize(0);
  f_mount(0, &fs);
  res =  f_opendir(&dirs, path);
  if (res == FR_OK) 
  {
    while (f_readdir(&dirs, &finfo) == FR_OK)
	{
	  if (finfo.fattrib & AM_ARC) 
      {
        if(!finfo.fname[0])	  //文件名不为空，如果为空，则表明该目录下面的文件已经读完了
          break;         
        //res = f_open(&fsrc, finfo.lfname, FA_OPEN_EXISTING | FA_READ);
		stringcopy(buff_filename[i_name], (BYTE*)finfo.fname);
		i_name++;
//        res = f_read(&fsrc, &buffer, 50, &br);
        f_close(&fsrc);
	  }
	}
  }
 f_mount(0,NULL);
}
void ReadSDFiles(void)
{
    disk_initialize(0);
	f_mount(0, &fs);
	res = f_open(&fsrc,"picture/test.txt",FA_OPEN_EXISTING | FA_READ);       
	res = f_read(&fsrc, &buffer,9, &br);
	f_close(&fsrc);
	f_mount(0,NULL);
}

void WriteSDFile(void)
{
  FIL file;
 // FILINFO finfo;
  //DIR dirs;
 // int i_name=0;
//char *fn;
 //XCHAR path[50]={"picture"};
  disk_initialize(0);
  res = f_mount(0, &fs);
   res = f_open(&file, "33/10551.txt", FA_CREATE_ALWAYS | FA_WRITE);  //创建一个新的文件
	res = f_lseek(&file,file.fsize);
	 f_printf (&file,"20100508sdsd看看看%s\r\n" );
	   f_close(&file); 
   f_mount(0, NULL);
}
/**************************************************************************
*sd init and config
**************************************************************************/
SD_Error SD_InitAndConfig(void)
{
 /////////////////////////////////////////////////////////////////////
  //////// SDCARD Initialisation //////////////////////////////////////
  /////////////////Section adapted from ST example/////////////////////
  
  /*-------------------------- SD Init ----------------------------- */
  Status = SD_Init();
  
  if (Status == SD_OK)
  {	
    /*----------------- Read CSD/CID MSD registers ------------------*/
    Status = SD_GetCardInfo(&SDCardInfo);
  }
  if (Status == SD_OK)
  {
	/*----------------- Select Card --------------------------------*/
    Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
  }
  if (Status == SD_OK)
  {
    Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
  }
  /* Set Device Transfer Mode to DMA */
  if (Status == SD_OK)
  { 
    Status = SD_SetDeviceMode(SD_DMA_MODE);//oet
 //   Status = SD_SetDeviceMode(SD_POLLING_MODE);
//    Status = SD_SetDeviceMode(SD_INTERRUPT_MODE);
  }	 
return Status;
}
/*******************************************************
*Usb Connect
*******************************************************/
void UsbConnect(void)
{
	USB_Cable_Config(ENABLE);
	USB_Init();
	while (bDeviceState != CONFIGURED);
} 
/*******************************************************
*					 Delay ms us
*****************************************************/
void delay_us(unsigned long i)
 {     
    while( i--)
       {	
        
        }
 }		  

//    毫秒级延时函数	 
void delay_ms(unsigned long time)
	 {
	  while(time--)
	  	  {	
		   delay_us(1000);
		   
		  }
	 }
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
  delay_ms(nCount*10);
}
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  //Pow_On; 
  SD_InitAndConfig();
  USB_Init();
  //Set_Rst;
 // Delay(20);
 //Clr_Rst;
 // Delay(20);
 // Set_Rst;
 // Delay(20);
  
 // STM3210E_LCD_Init();
 // LCD_Clear(Black);
  
 // Lcd_On;
 // LCD_SetBackColor(Black);
  //LCD_SetTextColor(Yellow);   

 // ReadSDFile();			
   WriteSDFile();
  //delay_ms(500);
  
 // LCD_DisplayStringLine(Line0,"File read success");
 // LCD_DisplayStringLine(Line1,buff_filename[0]);
 // LCD_DisplayStringLine(Line2,buff_filename[1]);
 // 
 // LCD_DisplayStringLine(Line3,"                  ");
 // LCD_DisplayStringLine(Line4,"                  ");
  
 // LCD_DisplayStringLine(Line3,buff_filename[2]);
 // LCD_DisplayStringLine(Line4,buff_filename[3]);  

 // LCD_DisplayStringLine(Line5,buff_filename[4]);
//  LCD_DisplayStringLine(Line6,buff_filename[5]); 

 // LCD_DisplayStringLine(Line7,buff_filename[6]);
 // LCD_DisplayStringLine(Line8,buff_filename[7]); 

 // delay_ms(10000);
 // delay_ms(10000);
  while (1)
  {
 /* delay_ms(10000);
delay_ms(10000);
LCD_Clear(Black);
View_Bmp("picture/face0.bmp");
delay_ms(10000);
delay_ms(10000);
LCD_Clear(Black);
View_Bmp("picture/face.bmp");
delay_ms(10000);
delay_ms(10000);
LCD_Clear(Black);
View_Bmp("picture/loong.bmp");
delay_ms(10000);
delay_ms(10000);
LCD_Clear(Black);
View_Bmp("picture/loongs.bmp");
 */ 
  }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
