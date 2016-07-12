#include <Include.h>
#include <SSD1325_Driver.h>

uint8 SSD1325Buffer[16*64];
uint8 SSD1325LightBuffer[4]={0x00,0x0a,0xa0,0xaa};
void SSD1325_Delay(uint16 i)
{ 
	while(i--)
	for(i=5000;i>0;i--);
}

//向SSD1325写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	//OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	//OLED_CS_Set();
	OLED_DC_Set();   	  
} 
  
  
void SSD1325_SET_Light(uint8 L)
{
	L>>=4;
	SSD1325LightBuffer[0]=0x00;
	SSD1325LightBuffer[2]=L;
	SSD1325LightBuffer[1]=L<<4;
	SSD1325LightBuffer[3]=L<<4|L;
}


//初始化SSD1325				    
void SSD1325_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能E端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能E端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	 //PE4,PE6    CS DC
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  //初始化PE4,PE6
 	GPIO_SetBits(GPIOE,GPIO_Pin_11);	//PE4,PE6输出高

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //PE2,PE3,PE5推挽输出	 RES CLK DIN
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);						 //PE2,PE3,PE5 输出高
  OLED_RST_Set();
	SSD1325_Delay(100);
	OLED_RST_Clr();
	SSD1325_Delay(100);
	OLED_RST_Set(); 
	OLED_WR_Byte(0xAE,OLED_CMD); //	Display Off
	OLED_WR_Byte(0xB3,OLED_CMD); // Display Clock Divider/Osciallator Frequency
	OLED_WR_Byte(0x90,OLED_CMD); // Set Clock as 135 Frames/Sec
	OLED_WR_Byte(0xA8,OLED_CMD); // Set Multiplex Ratio
	OLED_WR_Byte(0x3F,OLED_CMD); // 1/64 Duty (0x0F~0x5F)
	OLED_WR_Byte(0xA2,OLED_CMD); // Shift Mapping RAM Counter (0x00~0x5F)
	OLED_WR_Byte(0x4C,OLED_CMD); //	
	OLED_WR_Byte(0xA1,OLED_CMD); // Set Mapping RAM Display Start Line (0x00~0x5F)
	OLED_WR_Byte(0x00,OLED_CMD); //	
	OLED_WR_Byte(0xAE,OLED_CMD); // Disable Embedded DC/DC Converter 
	OLED_WR_Byte(0x02,OLED_CMD); //	
	OLED_WR_Byte(0xA0,OLED_CMD); //Set Column Address 0 Mapped to SEG0 
	OLED_WR_Byte(0x50,OLED_CMD);      	//     Disable Nibble Remap
						                //     Horizontal Address Increment
					                 	//     Scan from COM[N-1] to COM0
					                	//     Enable COM Split Odd Even	

	OLED_WR_Byte(0x86,OLED_CMD); // Full Current Range
	OLED_WR_Byte(0xB8,OLED_CMD); //	 Set Gray Scale Table
	OLED_WR_Byte(0x01,OLED_CMD);
	OLED_WR_Byte(0x11,OLED_CMD);
	OLED_WR_Byte(0x22,OLED_CMD);
	OLED_WR_Byte(0x32,OLED_CMD);
	OLED_WR_Byte(0x43,OLED_CMD);
	OLED_WR_Byte(0x54,OLED_CMD);
	OLED_WR_Byte(0x65,OLED_CMD);
	OLED_WR_Byte(0x76,OLED_CMD);
  OLED_WR_Byte(0x81,OLED_CMD); // Set Contrast Value
  OLED_WR_Byte(0xFF,OLED_CMD);
	OLED_WR_Byte(0xB2,OLED_CMD); // Set Frame Frequency (Row Period)
	OLED_WR_Byte(0x51,OLED_CMD); //	
	OLED_WR_Byte(0xB1,OLED_CMD); // Set Frame Frequency (Row Period)
	OLED_WR_Byte(0x55,OLED_CMD); //	 
	OLED_WR_Byte(0xBC,OLED_CMD); // Set Pre-Charge Voltage Level
  OLED_WR_Byte(0x10,OLED_CMD);
	OLED_WR_Byte(0xB4,OLED_CMD); //Set Pre-Charge Compensation
  OLED_WR_Byte(0x02,OLED_CMD);
	OLED_WR_Byte(0xB0,OLED_CMD);
	OLED_WR_Byte(0x28,OLED_CMD);
 	OLED_WR_Byte(0xBE,OLED_CMD);  // Set Output Level High Voltage for COM Signal
	OLED_WR_Byte(0x1C,OLED_CMD);
	OLED_WR_Byte(0xBF,OLED_CMD);  // Set Low Voltage Level of SEG Pin
	OLED_WR_Byte(0x0F,OLED_CMD);
	SSD1325_Clear_Screen(0x00);
	OLED_WR_Byte(0xA4,OLED_CMD); // Normal Display Mode 
	OLED_WR_Byte(0xAF,OLED_CMD); //	Display On
	
}  

void SSD_Convert4Byte(uint8 DATA)
{
   unsigned char i;
   unsigned char k;
  for(i=0;i<4;i++)   
   {
    k=DATA&0x03;   
    DATA=DATA>>2;                                //左移两位
	  OLED_WR_Byte(SSD1325LightBuffer[k],OLED_DATA);
   }
}
void SSD1325_OutBuffer()
{
	unsigned int i,j,j_Temp,ij_Temp,n,k;
	OLED_WR_Byte(0xA0,OLED_CMD); //Set Column Address 0 Mapped to SEG0 
	OLED_WR_Byte(0x52,OLED_CMD);      	//     Disable Nibble Remap
	Set_Column_Address(0,63); //设置列地址
	Set_Row_Address(0,64); //设置行地址
	for(j=0;j<64;j++)
	 {
		 j_Temp=j*16;
	 	for (i=0;i<16;i++)
	  {
				  ij_Temp=i+j_Temp;
					for(n=0;n<4;n++)   
					 {
						k=SSD1325Buffer[ij_Temp]&0x03;   
						SSD1325Buffer[ij_Temp]>>=2;    
						OLED_WR_Byte(SSD1325LightBuffer[k],OLED_DATA);
					 }
	   }
	 }
	 	OLED_WR_Byte(0xA0,OLED_CMD); //Set Column Address 0 Mapped to SEG0 
	  OLED_WR_Byte(0x50,OLED_CMD);      	//     Disable Nibble Remap
	 i=sizeof(SSD1325Buffer);
	 while(i--)SSD1325Buffer[i]=0;
}
void SSD1325_Clear_Screen(uint16 Color)
{
	uint16 i=16*64;
	while(i--)SSD1325Buffer[i]=Color;
	SSD1325_OutBuffer();
}
void SSD1325_Set_Pixel(uint16 X,uint16 Y,uint16 Color)
{
	if(Color)SSD1325Buffer[X/8+Y*16]|=BIT(X%8);
	else SSD1325Buffer[X/8+Y*16]&=~BIT(X%8);
}
uint16 SSD1325_Get_Pixel(uint16 X,uint16 Y)
{
	if(SSD1325Buffer[X/8+Y*16]&BIT(X%8))return 1;
	else return 0;
}

void Set_Column_Address(unsigned char a, unsigned char b)
{
	OLED_WR_Byte(0x15,OLED_CMD);			// Set Column Address
	OLED_WR_Byte(a,OLED_CMD);			//   Default => 0x00
	OLED_WR_Byte(b,OLED_CMD);			//   Default => 0x3F (Total Columns Devided by 2)
}


void Set_Row_Address(unsigned char a, unsigned char b)
{
	OLED_WR_Byte(0x75,OLED_CMD);			// Set Row Address
	OLED_WR_Byte(a,OLED_CMD);			//   Default => 0x00
	OLED_WR_Byte(b,OLED_CMD);			//   Default => 0x4F
}


void SSD1325_Write(void *Addr,void *Buffer,void *Length)
{

}
void SSD1325_Read(void *Addr,void *Buffer,void *Length)
{
	
}



