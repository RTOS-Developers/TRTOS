#include <..\Driver\SSD1306_Driver.h>


uint8 DeviceId_SSD1306=DeviceNull;
const GPIO_InitStruct *IO_SSD1306_P;
uint8 *OLED_GRAM;	 

void OLED_WR_Byte(uint8 dat,uint8 cmd)
{	
	uint8 i;			  
	if(cmd)GPIO_SetBits((&IO_SSD1306_P[3])->GPIOx,(&IO_SSD1306_P[3])->GPIO_Pin);
	else GPIO_ResetBits((&IO_SSD1306_P[3])->GPIOx,(&IO_SSD1306_P[3])->GPIO_Pin); 
	GPIO_ResetBits((&IO_SSD1306_P[4])->GPIOx,(&IO_SSD1306_P[4])->GPIO_Pin);
	for(i=0;i<8;i++)
	{			  
		GPIO_ResetBits((&IO_SSD1306_P[0])->GPIOx,(&IO_SSD1306_P[0])->GPIO_Pin); 
		if(dat&0x80)GPIO_SetBits((&IO_SSD1306_P[1])->GPIOx,(&IO_SSD1306_P[1])->GPIO_Pin);
		else GPIO_ResetBits((&IO_SSD1306_P[1])->GPIOx,(&IO_SSD1306_P[1])->GPIO_Pin);
			GPIO_SetBits((&IO_SSD1306_P[0])->GPIOx,(&IO_SSD1306_P[0])->GPIO_Pin); 
			dat<<=1;   
	}				
	GPIO_SetBits((&IO_SSD1306_P[4])->GPIOx,(&IO_SSD1306_P[4])->GPIO_Pin); 	
	GPIO_SetBits((&IO_SSD1306_P[3])->GPIOx,(&IO_SSD1306_P[3])->GPIO_Pin); 	  
} 

	   


void OLED_Refresh_Gram(void)
{
	uint8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n+128*i],OLED_DATA); 
	}   
}
  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

void LCD_Clear_Screen(uint8 Fill)  
{  
	uint8 i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n+128*i]=Fill;  
	OLED_Refresh_Gram();
}

		   
void LCD_Set_Pixel(uint16 x,uint16 y,uint16 t)
{
	uint8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;						//如18 是在第二页的但是原子的扫描是从上到下，得到7-2=5
	bx=y%8;								
	temp=1<<(7-bx);
	if(t)
		OLED_GRAM[x+128*pos]|=temp;
	else 
		OLED_GRAM[x+128*pos]&=~temp;	    
}
				    
void OLED_Init(void *GIS)
{ 	
	IO_SSD1306_P=(const GPIO_InitStruct *)GIS;
	OLED_GRAM=Mema_Malloc(128*8);
	GPIO_InitList(IO_SSD1306_P);				  
	GPIO_SetBits((&IO_SSD1306_P[4])->GPIOx,(&IO_SSD1306_P[4])->GPIO_Pin); 	
	GPIO_SetBits((&IO_SSD1306_P[3])->GPIOx,(&IO_SSD1306_P[3])->GPIO_Pin); 	
	GPIO_ResetBits((&IO_SSD1306_P[2])->GPIOx,(&IO_SSD1306_P[2])->GPIO_Pin); 
	BareDelay(1);
	GPIO_SetBits((&IO_SSD1306_P[2])->GPIOx,(&IO_SSD1306_P[2])->GPIO_Pin); 
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,	OLED_CMD); //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0
	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.												    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置	 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
}  
void OLED_Exit()
{
	Mema_Free(OLED_GRAM);
}


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
void LCD_Draw_LineX(uint16 SX,uint16 SY,uint16 W,uint16 Color)
{
	while(W--)LCD_Set_Pixel(SX++,SY,Color);
}
void LCD_Draw_LineY(uint16 SX,uint16 SY,uint16 H,uint16 Color)
{
	while(H--)LCD_Set_Pixel(SX,SY++,Color);
}

void LCD_Clear_Area(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Color)
{
	uint16 H=EY-SX;
	while(H--)LCD_Draw_LineX(SX,SY++,EX-EY,Color);
}

void LCD_Draw_DotLineX(uint16 SX,uint16 SY,uint16 W,uint16 Fcolor,uint16 Bcolor)
{
	W/=2;
	while(W--)
	{
		LCD_Set_Pixel(SX++,SY,Fcolor);
		LCD_Set_Pixel(SX++,SY,Bcolor);
	}
}
void LCD_Draw_DotLineY(uint16 SX,uint16 SY,uint16 H,uint16 Fcolor,uint16 Bcolor)
{
	H/=2;
	while(H--)
	{
		LCD_Set_Pixel(SX++,SY,Fcolor);
		LCD_Set_Pixel(SX++,SY,Bcolor);
	}
}

void LCD_Fill_LineX(uint16 SX,uint16 SY,uint16 W,uint16 *Cbuffer)
{
	while(W--)
	{
		LCD_Set_Pixel(SX++,SY,*Cbuffer++);
	}
}

void LCD_Fill_LineY(uint16 SX,uint16 SY,uint16 H,uint16 *Cbuffer)
{
	while(H--)
	{
		LCD_Set_Pixel(SX,SY++,*Cbuffer++);
	}
}
void DeviceMount_SS1306(const GPIO_InitStruct *GIS)
{
	DeviceId_SSD1306=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_SSD1306].DeviceId=DeviceId_SSD1306;
	Tos_Device_Tab[DeviceId_SSD1306].DeviceName="SSD1306";
	Tos_Device_Tab[DeviceId_SSD1306].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_SSD1306].DeviceVirtue=DV_TaskValue;
	Tos_Device_Tab[DeviceId_SSD1306].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_SSD1306].Init=OLED_Init;
	Tos_Device_Tab[DeviceId_SSD1306].Note=(uintbus)GIS;
	Tos_Device_Tab[DeviceId_SSD1306].Write=Null;
	Tos_Device_Tab[DeviceId_SSD1306].Read=Null;
	Tos_Device_Tab[DeviceId_SSD1306].Exit=OLED_Exit;
}
void Task_RBuffer(void *Tags)
{
	uint16 OlDCrc,Crc;
	while(1)
	{
		
		Crc=Tools_GetCRC16(OLED_GRAM,128*8);
		if(Crc!=OlDCrc)
		{
			OLED_Refresh_Gram();
			OlDCrc=Crc;
		}
		Tos_TaskDelay(10);
	}
}


























