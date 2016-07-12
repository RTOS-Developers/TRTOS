#include <HX8347_Driver.h>

#define Bank1_LCD_D    ((uint32_t)0x60020000)     //Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	    //Reg ADDR
#define DEBUG_DELAY()   Delay(20)
#define LCD_WR_REG(Value) *(__IO uint16_t *) (Bank1_LCD_C)= Value
#define LCD_WR_CMD(Index,Value) (*(__IO uint16_t *) (Bank1_LCD_C)= index;*(__IO uint16_t *) (Bank1_LCD_D)= Value)
#define LCD_WR_Data(val) *(__IO uint16_t *) (Bank1_LCD_D)= val
void Delay(__IO uint32_t nCount);

extern void Delay(__IO uint32_t nCount);

/*读GRAM数据-ili9341*/
unsigned int LCD_RD_data(void)	
{
	u16 R=0, G=0, B=0 ;
	R =(*(__IO u16 *) (Bank1_LCD_D));  /*FIRST READ OUT DUMMY DATA*/
	R =(*(__IO u16 *) (Bank1_LCD_D));  /*READ OUT RED DATA  */
	B =(*(__IO u16 *) (Bank1_LCD_D));  /*READ OUT BLACK DATA*/
	G =(*(__IO u16 *) (Bank1_LCD_D));  /*READ OUT GREEN DATA*/
	
    return (((R>>11)<<11) | ((G>>10)<<5) | (B>>11)) ;
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/**********************************
 * 函数名：Set_direction
 * 描述  ：设置ILI9341GRAM指针扫描方向
 * 输入  : 0： 横向扫描
 		   1： 纵向扫描
 * 输出  ：无
 * 举例  ：无
 * 注意  ：无
*************************************/
void Set_direction(u8 option)
{

	switch(option)
	{
	case 0:
	{	    /*横屏*/
	DEBUG_DELAY();
	LCD_WR_REG(0x36);
	DEBUG_DELAY(); 
	LCD_WR_Data(0x68);    //横屏
	DEBUG_DELAY();
	LCD_WR_REG(0X2A); 
	DEBUG_DELAY();
	LCD_WR_Data(0x00);	//start 
	DEBUG_DELAY();
	LCD_WR_Data(0x00);
	DEBUG_DELAY();
	LCD_WR_Data(0x01);	//end
	DEBUG_DELAY();
	LCD_WR_Data(0x3F);
	DEBUG_DELAY();		
	LCD_WR_REG(0X2B); 
	DEBUG_DELAY();
	LCD_WR_Data(0x00);   //start
	DEBUG_DELAY();
	LCD_WR_Data(0x00);
	DEBUG_DELAY();
	LCD_WR_Data(0x00);   //end
	DEBUG_DELAY();
	LCD_WR_Data(0xEF);
	DEBUG_DELAY();		
	}break;
	case 1:
	{		/*竖屏*/
	LCD_WR_REG(0x36); 
	LCD_WR_Data(0x48);	//竖屏 
	LCD_WR_REG(0X2A); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0xEF);	

	LCD_WR_REG(0X2B); 
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x01);
	LCD_WR_Data(0x3F);
	}break;

	}

}
void  HX8347_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE); 	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				     //LED1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_6|GPIO_Pin_3;		 //LED2, LED3
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 //LCD 背光控制
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 //LCD-RST
  GPIO_Init(GPIOE, &GPIO_InitStructure);  	
  
  	
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 


  /* NE1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  /* RS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  
  GPIO_SetBits(GPIOD, GPIO_Pin_7);			//CS=1 
  GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
  GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   
  GPIO_ResetBits(GPIOE, GPIO_Pin_0);
  GPIO_ResetBits(GPIOE, GPIO_Pin_1);			//RESET=0
  GPIO_SetBits(GPIOD, GPIO_Pin_4);		    //RD=1
  GPIO_SetBits(GPIOD, GPIO_Pin_5);			//WR=1	  



//  GPIO_SetBits(GPIOD, GPIO_Pin_13);			//LIGHT
  GPIO_ResetBits(GPIOD, GPIO_Pin_13);			//LIGHT
}
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  p.FSMC_AddressSetupTime = 0x00;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x01;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_A;
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;	  
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

void HX8347_Initializtion(void)
{
	unsigned long i;
	HX8347_GPIO_Init();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	FSMC_LCD_Init();
	/*RESET TFT*/	 
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	Delay(0xAFFf<<2);					   
	GPIO_SetBits(GPIOE, GPIO_Pin_1 );	
	Delay(0xAFFf<<2);

	DEBUG_DELAY();
	LCD_WR_REG(0xCF);
	DEBUG_DELAY();
	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x81);
	DEBUG_DELAY();	LCD_WR_Data(0x30);

	DEBUG_DELAY();
	LCD_WR_REG(0xED);
	DEBUG_DELAY();
	LCD_WR_Data(0x64);
	DEBUG_DELAY();	LCD_WR_Data(0x03);
	DEBUG_DELAY();	LCD_WR_Data(0x12);
	DEBUG_DELAY();	LCD_WR_Data(0x81);

	DEBUG_DELAY();
	LCD_WR_REG(0xE8);
	DEBUG_DELAY();
	LCD_WR_Data(0x85);
	DEBUG_DELAY();	LCD_WR_Data(0x10);
	DEBUG_DELAY();	LCD_WR_Data(0x78);

	DEBUG_DELAY();
	LCD_WR_REG(0xCB);
	DEBUG_DELAY();
	LCD_WR_Data(0x39);
	DEBUG_DELAY();	LCD_WR_Data(0x2C);
	DEBUG_DELAY();	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x34);
	DEBUG_DELAY();	LCD_WR_Data(0x02);

	DEBUG_DELAY();
	LCD_WR_REG(0xF7);
	DEBUG_DELAY();
	LCD_WR_Data(0x20);

	DEBUG_DELAY();
	LCD_WR_REG(0xEA);
	DEBUG_DELAY();
	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x00);

	DEBUG_DELAY();
	LCD_WR_REG(0xB1);
	DEBUG_DELAY();
	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x1B);

	DEBUG_DELAY();
	LCD_WR_REG(0xB6);
	DEBUG_DELAY();
	DEBUG_DELAY();	LCD_WR_Data(0x0A);
	DEBUG_DELAY();	LCD_WR_Data(0xA2);

	DEBUG_DELAY();
	DEBUG_DELAY();	LCD_WR_REG(0xC0);
	DEBUG_DELAY();	LCD_WR_Data(0x35);

	DEBUG_DELAY();
	LCD_WR_REG(0xC1);
	DEBUG_DELAY();	LCD_WR_Data(0x11);

	LCD_WR_REG(0xC5);
	LCD_WR_Data(0x45);
	DEBUG_DELAY();	LCD_WR_Data(0x45);

	LCD_WR_REG(0xC7);
	DEBUG_DELAY();	LCD_WR_Data(0xA2);

	LCD_WR_REG(0xF2);
	DEBUG_DELAY();	LCD_WR_Data(0x00);

	DEBUG_DELAY();	LCD_WR_REG(0x26);
	DEBUG_DELAY();	LCD_WR_Data(0x01);
	DEBUG_DELAY();
	LCD_WR_REG(0xE0); //Set Gamma
	DEBUG_DELAY();	LCD_WR_Data(0x0F);
	LCD_WR_Data(0x26);
	LCD_WR_Data(0x24);
	LCD_WR_Data(0x0B);
	LCD_WR_Data(0x0E);
	LCD_WR_Data(0x09);
	LCD_WR_Data(0x54);
	LCD_WR_Data(0xA8);
	LCD_WR_Data(0x46);
	LCD_WR_Data(0x0C);
	LCD_WR_Data(0x17);
	LCD_WR_Data(0x09);
	LCD_WR_Data(0x0F);
	LCD_WR_Data(0x07);
	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_Data(0x00);
	LCD_WR_Data(0x19);
	LCD_WR_Data(0x1B);
	LCD_WR_Data(0x04);
	LCD_WR_Data(0x10);
	LCD_WR_Data(0x07);
	LCD_WR_Data(0x2A);
	LCD_WR_Data(0x47);
	LCD_WR_Data(0x39);
	LCD_WR_Data(0x03);
	LCD_WR_Data(0x06);
	LCD_WR_Data(0x06);
	LCD_WR_Data(0x30);
	LCD_WR_Data(0x38);
	LCD_WR_Data(0x0F);
	DEBUG_DELAY();
	LCD_WR_REG(0x36); 
	DEBUG_DELAY();	LCD_WR_Data(0x68);    /*横屏  左上角到(起点)到右下角(终点)扫描方式*/
	DEBUG_DELAY();
	LCD_WR_REG(0X2A); 
	DEBUG_DELAY();	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x01);
	DEBUG_DELAY();	LCD_WR_Data(0x3F);

	DEBUG_DELAY();
	DEBUG_DELAY();	LCD_WR_REG(0X2B); 
	DEBUG_DELAY();	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0x00);
	DEBUG_DELAY();	LCD_WR_Data(0xEF);

	DEBUG_DELAY();
	LCD_WR_REG(0x3a); // Memory Access Control
	DEBUG_DELAY();	LCD_WR_Data(0x55);
	DEBUG_DELAY();	LCD_WR_REG(0x11); //Exit Sleep
	DEBUG_DELAY();
	DEBUG_DELAY();
	LCD_WR_REG(0x29); //display on
	DEBUG_DELAY();
	DEBUG_DELAY();	DEBUG_DELAY();
	DEBUG_DELAY();	DEBUG_DELAY();
	DEBUG_DELAY();	DEBUG_DELAY();
	DEBUG_DELAY();	DEBUG_DELAY();
	DEBUG_DELAY();	DEBUG_DELAY();
	DEBUG_DELAY();   
	Set_direction(0);	  //传入0-->初始化默认为横屏方式
	Set_direction(0);	  //传入0-->初始化默认为横屏方式
	Set_direction(0);	  //传入0-->初始化默认为横屏方式
	Set_direction(0);	  //传入0-->初始化默认为横屏方式

	LCD_WR_REG(0x2c); 			
	for(i=0; i<(320*240); i++)		 		
	{
	LCD_WR_REG(0); 			    
	}
	
}

void SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY) 
{
	LCD_WR_REG(0X2A); 
	LCD_WR_Data(StartX>>8);	
	LCD_WR_Data(StartX&0xff);
	LCD_WR_Data(EndX>>8);
	LCD_WR_Data(EndX&0xff);
	
	LCD_WR_REG(0X2B); 
	LCD_WR_Data(StartY>>8);  
	LCD_WR_Data(StartY&0xff);
	LCD_WR_Data(EndY>>8);   
	LCD_WR_Data(EndY&0xff); 
	LCD_WR_REG(0x2c); 
}
void HX8347_Clear_Screen(u16 dat)	
{
	u32 n;
	SetWindows(0,0,HX8347_3_2_XMAX,HX8347_3_2_YMAX);
	for(n=0; n<320*240; n++)
	LCD_WR_Data(dat); 
}
u16 GetPoint(u16 x,u16 y)
{ 
	u16 temp;
	LCD_WR_REG(0X2A); 
	LCD_WR_Data(x>>8);	
	LCD_WR_Data(x&0xff);
	LCD_WR_Data(HX8347_3_2_XMAX>>8);
	LCD_WR_Data(HX8347_3_2_XMAX&0xff);
	
	LCD_WR_REG(0X2B); 
	LCD_WR_Data(y>>8);  
	LCD_WR_Data(y&0xff);
	LCD_WR_Data(HX8347_3_2_YMAX>>8);   
	LCD_WR_Data(HX8347_3_2_YMAX&0xff); 
	LCD_WR_REG(0x2e);
	temp=LCD_RD_data();
	return (temp);
}
void SetPoint(u16 x,u16 y,u16 point)	
{
	LCD_WR_REG(0X2A); 
	LCD_WR_Data(x>>8);	
	LCD_WR_Data(x&0xff);
	LCD_WR_Data(HX8347_3_2_XMAX>>8);
	LCD_WR_Data(HX8347_3_2_XMAX&0xff);
	
	LCD_WR_REG(0X2B); 
	LCD_WR_Data(y>>8);  
	LCD_WR_Data(y&0xff);
	LCD_WR_Data(HX8347_3_2_YMAX>>8);   
	LCD_WR_Data(HX8347_3_2_YMAX&0xff); 
	LCD_WR_REG(0x2c); 
	LCD_WR_Data(point); 	
}
void SSD1963_Fill_LineX(uint16 SX,uint16 SY,uint16 W,uint16 *Cbuffer)
{
	SetWindows(SX,SY,SX+W,SY);
	while(W--)LCD_WR_Data(*Cbuffer++); 
}
void SSD1963_Fill_LineY(uint16 SX,uint16 SY,uint16 H,uint16 *Cbuffer)
{
	SetWindows(SX,SY,SX,SY+H);
	while(H--)LCD_WR_Data(*Cbuffer++); 
}
void Fast_Draw_Fill_List(void *Data,uint16 Data_Length,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color)
{
	char i,n;
	uint8 *P=(uint8 *)Data;
	SetWindows(X,Y,X+7,Y+Data_Length);
	for(n=0;n<Data_Length;n++)
	{
		for(i=0;i<8;i++)
		{
			if(BIT(i)&*P)LCD_WR_Data(F_Color);
			else LCD_WR_Data(B_Color);//if(B_Color!=A_Color)
		}
		P++;
	}
}
void Fast_Fill_Rectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color)
{
	uint32 i;
	SetWindows(X0,Y0,X1,Y1);
	i=(X1-X0)*(Y1-Y0);
	while(i--)LCD_WR_Data(Color);
}
