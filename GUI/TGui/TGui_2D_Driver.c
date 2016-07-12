#include <Include.h>
#include <Lcd_Driver.h>
#include <Driver_Include.h>
#include <Math.h>
#include <TGui_2D_Driver.h>
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:水平打印一字节显示像素
Note:(Data) 一字节控制像素亮灭 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/
void Draw_X_8Bit(uint8 Data,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(BIT(i)&Data)LCD_Set_Pixel(X,Y,F_Color);
		else if(B_Color!=A_Color)LCD_Set_Pixel(X,Y,B_Color);
		X++;
	}
}
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:垂直打印一字节显示像素
Note:(Data) 一字节控制像素亮灭 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/
void Draw_Y_8Bit(uint8 Data,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color)
{
	char i;
	for(i=0;i<8;i++)
	{
		if(BIT(i)&Data)LCD_Set_Pixel(X,Y,F_Color);
		else if(B_Color!=A_Color)LCD_Set_Pixel(X,Y,B_Color);
		Y++;
	}
}
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:垂直平铺一字节宽度
Note:(Data) 平铺一系列参数 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/

void Draw_Full_List(void *Data,uint16 Data_Length,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color)
{
#ifdef FAST_Draw_Full_List
	Fast_Draw_Fill_List(Data,Data_Length,X,Y,F_Color,B_Color,A_Color);
#else 
	uint16 i;
	uint8 *p;
	p=(uint8 *)Data;
	for(i=0;i<Data_Length;i++)
	{
		Draw_X_8Bit(*p++,X,Y,F_Color,B_Color,A_Color);
		Y++;
	}
#endif
}
/****************************************************************************
Date:2013/8/14
Vision:V1.0
Func:水平平铺一字节宽度
Note:(Data) 平铺一系列参数 (X,Y) 引用全局地址但不改变全局地址
****************************************************************************/
void Draw_Row_List(void *Data,uint16 Data_Length,uint16 X,uint16 Y,uint16 F_Color,uint16 B_Color,uint16 A_Color)
{
	uint16 i;
	uint8 *p;
	p=(uint8 *)Data;
	for(i=0;i<Data_Length;i++)
	{
		Draw_Y_8Bit(*p++,X,Y,F_Color,B_Color,A_Color);
		X++;
	}
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: 绘制点线
 Time: 2010-1-5
 Ver.: V1.0
 Note: 本方法用于绘制任意直线,采用第一象限直线插补算法
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_DotLine(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Fcolor,uint16 Bcolor)
{	
	uint8 T;
	uint16 X,Y,C,B,K=0;
	int16 Fe;
	if(X0>X1){													//整理成从左至右绘线
	 	C=X0;X0=X1;X1=C;				
		C=Y0;Y0=Y1;Y1=C;
	}
	if(Y0>Y1){Y1=Y0-Y1;T=1;Y=0;}								//将所有类型直线转换到第一象限
	else {Y1=Y1-Y0;T=0;Y=Y0;}
	X1-=X0;
	if(X0==X1)LCD_Draw_DotLineY(X0,Y0,Y1,Fcolor,Bcolor);		//快速绘制垂直线
	else if(Y0==Y1)LCD_Draw_DotLineX(X0,Y0,X1,Fcolor,Bcolor);	//快速绘制水平线
	else{														//快速第一象限直线插补
		C=Y1+X1;Fe=0;X=X0;									     
		while(C--){
			if(Fe>0){Fe-=Y1;} else {Fe+=X1;}
			if(Fe>0){X++;}    else {Y++;}																				
			if(K&0x0001)B=Bcolor;								//绘制背景色点
			else		B=Fcolor;								//绘制前景色点
			if(T>0)LCD_Set_Pixel(X,Y0-Y,B);			
			else LCD_Set_Pixel(X,Y,B);	
			K++;												//点统计
		}	
	}
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: 绘制矩形
 Time: 2010-1-6
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_Rectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color)
{
	LCD_Draw_LineX(X0,Y0,X1-X0,Color);
	LCD_Draw_LineX(X0,Y1,X1-X0,Color);	
	LCD_Draw_LineY(X0,Y0,Y1-Y0,Color);	
	LCD_Draw_LineY(X1,Y0,Y1-Y0,Color); 	
}
/*--------------------------------------------------------------------------------------------------------
 修改矩形区域
--------------------------------------------------------------------------------------------------------*/
void TGUI_Set_Rectangle(Rectangle *Rect,uint16 SX,uint16 SY,uint16 EX,uint16 EY)
{
	Rect->SX=SX;
	Rect->SY=SY;
	Rect->EX=EX;
	Rect->EY=EY;
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图,绘制矩形
 Time: 2010-1-6
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_DotRectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Fcolor,uint16 Bcolor)
{
	LCD_Draw_DotLineX(X0,Y0,X1-X0,Fcolor,Bcolor);
	LCD_Draw_DotLineX(X0,Y1,X1-X0,Fcolor,Bcolor);
	LCD_Draw_DotLineY(X0,Y0,Y1-Y0,Fcolor,Bcolor);	
	LCD_Draw_DotLineY(X1,Y0,Y1-Y0,Fcolor,Bcolor);
}                        

/*---------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图,绘制圆角矩形
 Time: 2011-6-11
 Ver.: V1.0
 Note: ColorU/上边线色彩  ColorP/下边线色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_ArcRectangle(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 R,uint16 ColorU,uint16 ColorD)
{
	uint16 C,X,Y,XA,YA,XB,YB;
	int16 Fe;
	TGui_Draw_Line(SX+R,SY,EX-R,SY,ColorU);						//上边线
	TGui_Draw_Line(SX+R,EY,EX-R,EY,ColorD);						//下边线
	TGui_Draw_Line(SX,SY+R,SX,EY-R,ColorU);						//左边线
	TGui_Draw_Line(EX,SY+R,EX,EY-R,ColorD);						//右边线
	XA=EX-R;YA=SY+R;											//第一象限原点
	SX+=R;	SY+=R;												//第二象限原点
	XB=SX; 	YB=EY-R;											//第三象限原点
	EX-=R;	EY-=R;												//第四象限原点
	C=R<<1;Fe=0;X=R,Y=0;
	while(C--){
		if(Fe>0)Fe=Fe-2*X+1;
		else Fe=Fe+2*Y+1;
		if((Fe>0)&&(X>0))X--;
		else Y++;
		LCD_Set_Pixel(EX+X,EY+Y,ColorD);						//绘制第四象限
		LCD_Set_Pixel(XB-X,YB+Y,ColorU); 						//绘制第三象限
		LCD_Set_Pixel(SX-X,SY-Y,ColorU);						//绘制第二象限
		LCD_Set_Pixel(XA+X,YA-Y,ColorU);						//绘制第一象限
	}	
}


/*---------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制圆形
 Time: 2010-1-5
 Ver.: V1.0
 Note: (X0,Y0)圆心坐标  R/圆半径
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_Circle(uint16 X0,uint16 Y0,uint16 R,uint16 Color)
{
	uint16 C,X,Y;
	int16 Fe;
	C=R<<1;Fe=0;X=R,Y=0;
	while(C--){
		if(Fe>0)Fe=Fe-2*X+1;
		else Fe=Fe+2*Y+1;
		if(Fe>0)X--;
		else Y++;
		LCD_Set_Pixel(X0+X,Y0+Y,Color);							//绘制第一象限
		LCD_Set_Pixel(X0-X,Y0+Y,Color);	 						//绘制第二象限
		LCD_Set_Pixel(X0-X,Y0-Y,Color);							//绘制第三象限
		LCD_Set_Pixel(X0+X,Y0-Y,Color);							//绘制第四象限
	}	
}

void CirclePoint(int x,int y,int x0,int y0,int color)
{
	LCD_Set_Pixel(x+x0,y+y0,color);
	LCD_Set_Pixel(y+x0,x+y0,color);
	LCD_Set_Pixel(y+x0,-x+y0,color);
	LCD_Set_Pixel(x+x0,-y+y0,color);
	LCD_Set_Pixel(-x+x0,-y+y0,color);
	LCD_Set_Pixel(-y+x0,-x+y0,color);
	LCD_Set_Pixel(-y+x0,x+y0,color);
	LCD_Set_Pixel(-x+x0,y+y0,color);
}
void TGUI_DrawRound(int x0,int y0,int r,int color)
{
	int x,y,d;
	x=0;
	y=r;
	d=1-r;
	CirclePoint(x,y,x0,y0,color);
	while(x<=y)
	{
		if(d<0){d+=2*x+3;x++;}
		else{d+=2*(x-y)+5;x++;y--;}
		CirclePoint(x,y,x0,y0,color);
	}
}
void TGUI_DrawRound2(uint16 X0,uint16 Y0,uint16 R0,uint16 Xo,uint16 Color)
{
	uint16 X,Y,RR,XX;
	RR=R0*R0;
	for(X=0;X<R0;X++)
	{
		XX=X*X;
		for(Y=0;Y<=R0;Y++)
		{
			if(fabs((XX+Y*Y)<RR)&&fabs((XX+Y*Y)>(Xo)))CirclePoint(X,Y,X0,Y0,Color);
		}
	}
}
/*---------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制圆形
 Time: 2010-1-5
 Ver.: V1.0
 Note: (X0,Y0)圆心坐标  R/圆半径
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_DotCircle(uint16 X0,uint16 Y0,uint16 R,uint16 Fcolor,uint16 Bcolor)
{
	uint16 C,X,Y,B,K=0;
	int16 Fe;
	C=R<<1;Fe=0;X=R,Y=0;
	while(C--){
		if(Fe>0)Fe=Fe-2*X+1;
		else Fe=Fe+2*Y+1;
		if(Fe>0)X--;
		else Y++;
		if(K&0x0001)B=Bcolor;									//绘制背景点
		else B=Fcolor;											//绘制前景点
		LCD_Set_Pixel(X0+X,Y0+Y,B);								//绘制第一象限
		LCD_Set_Pixel(X0-X,Y0+Y,B);		 						//绘制第二象限
		LCD_Set_Pixel(X0-X,Y0-Y,B);								//绘制第三象限
		LCD_Set_Pixel(X0+X,Y0-Y,B);								//绘制第四象限
		K++;
	}	
}


/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图驱动,圆形内部填充
 Time: 2010-1-5
 Ver.: V1.0
 Note: (X,Y)圆心坐标  R/圆半径
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Fill_Circle(uint16 X0,uint16 Y0,uint16 R,uint16 Color)
{
	uint16 C;
	int16 Fe,X,Y;
	C=R<<1;Fe=0;X=R,Y=0;
	while(C--){
		if(Fe>0)Fe=Fe-2*X+1;
		else Fe=Fe+2*Y+1;
		if(Fe>0)X--;
		else {Y++;		
		    LCD_Draw_LineX(X0-X+1,Y0+Y,X*2-1,Color);
		    LCD_Draw_LineX(X0-X+1,Y0-Y,X*2-1,Color);
		}
	}
	LCD_Draw_LineX(X0-R+2,Y0,2*R-3,Color);	
}


/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图驱动,绘制实线环形
 Time: 2010-1-5
 Ver.: V1.0
 Note: (X0,Y0)圆心坐标  R0/内圆半径 R1/外圆半径	ColorR0/内圆色彩	ColorR1/外圆色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_Ring(uint16 X0,uint16 Y0,uint16 R0,uint16 R1,uint16 ColorR0,uint16 ColorR1)
{
	TGUI_Draw_Circle(X0,Y0,R0,ColorR0);										//绘制内圆	
	TGUI_Draw_Circle(X0,Y0,R1,ColorR1);										//绘制外圆
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图驱动,填充环形
 Time: 2010-11-9
 Ver.: V1.0
 Note: (X0,Y0)圆心坐标  R0/内圆半径 R1/外圆半径	ColorR0/内圆色彩	ColorR1/外圆色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Fill_Ring(uint16 X0,uint16 Y0,uint16 R0,uint16 R1,uint16 ColorR0,uint16 ColorR1)
{
	uint16 C,C1;
	int16 Fe,X,Y,Fe1,X1,Y1;	
	C=R0<<1; Fe=0; X=R0; Y=0;
	C1=R1<<1;Fe1=0;X1=R1;Y1=0;
	while(C1--){													//以外圆做为结束标志
		if(Fe1>0)Fe1=Fe1-(X1<<1)+1;									//外圆进给一步
		else 	 Fe1=Fe1+(Y1<<1)+1;												  						
		if(Fe1>0)X1--;
		else{
			Y1++;
			if(C){
				while((Y!=Y1)&&(C>0)){
					if(Fe>0) Fe=Fe-(X<<1)+1;							//内圆进给一步
					else	 Fe=Fe+(Y<<1)+1;
					if(Fe>0)X--;   
					else {Y++;}
					C--;
				}
				if(C>0){												//是否到达顶点
					LCD_Draw_LineX(X0-X1, Y0+Y, X1-X,  ColorR1);		//填充外圆上左半环	
					LCD_Draw_LineX(X0-X,  Y0+Y, X*2+1, ColorR0);		//填充内圆上半环
					LCD_Draw_LineX(X0+X+1,Y0+Y, X1-X,  ColorR1);		//填充外圆上右半环
					LCD_Draw_LineX(X0-X1, Y0-Y, X1-X,  ColorR1);		//填充外圆下左半环	
					LCD_Draw_LineX(X0-X,  Y0-Y, X*2+1, ColorR0);		//填充内圆下半环
					LCD_Draw_LineX(X0+X+1,Y0-Y, X1-X,  ColorR1);		//填充外圆下右半环		
				}else{
					LCD_Draw_LineX(X0-X1,Y0+Y1,X1*2+1,ColorR1);			//填充外圆上半环
		    		LCD_Draw_LineX(X0-X1,Y0-Y1,X1*2+1,ColorR1);			//填充外圆下半环
				}
			}else{
				LCD_Draw_LineX(X0-X1,Y0+Y1,X1*2+1,ColorR1);				//填充外圆上半环
		    	LCD_Draw_LineX(X0-X1,Y0-Y1,X1*2+1,ColorR1);				//填充外圆下半环
			}
		}		
	}	
	LCD_Draw_LineX(X0-R1,Y0,R1-R0,ColorR1);	
	LCD_Draw_LineX(X0-R0+1,Y0,R0*2,ColorR0);	
	LCD_Draw_LineX(X0+R0,Y0,R1-R0,ColorR1);	
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图,矩形填充
 Time: 2010-1-6
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/

void TGUI_Fill_Rectangle(uint16 X0,uint16 Y0,uint16 X1,uint16 Y1,uint16 Color)
{
#ifdef FAST_Fill_Rectangle
	Fast_Fill_Rectangle(X0,Y0,X1,Y1+1,Color);
#else 
	uint16 T;
	T=X1-X0+1;
	while(Y0<=Y1)LCD_Draw_LineX(X0,Y0++,T,Color);
#endif
}

void TGUI_Fill_Bar(uint16 x1, uint16 y1, uint16 x2, uint16 y2,uint16 Max,uint16 Rate)
{
   uint16 W,H;
   uint8 attr=1,i=0;
   W=x2-x1;
   H=y2-y1;
   if(W>H)
   while(1)
    {
       if((i++)>(W*Rate/Max))attr=0;
       TGui_Draw_Line(x1,y1,x1,y2,attr);
       if(x1++>=x2)break; 
    } 
}                                                                                  
/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图,填充圆角矩形
 Time: 2011-6-11
 Ver.: V1.0
 Note: R/圆角半径 ColorU/上部分色彩  ColorP/下部分色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Fill_ArcRectangle(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 R,uint16 ColorU,uint16 ColorD)
{
	uint16 C,X,Y;
	int16 Fe;
	C=(EY+SY)>>1;
	SY+=R;	EY-=R;
	LCD_Clear_Area(SX,SY,EX,C,ColorU);							//填充body上部分
	LCD_Clear_Area(SX,C,EX,EY,ColorD);							//填充body下部分
	SX+=R;	EX-=R;
	C=R<<1;Fe=0;X=R,Y=0;
	while(C--){
		if(Fe>0)Fe=Fe-2*X+1;
		else Fe=Fe+2*Y+1;		
		if((Fe>0)&&(X>0))X--;
		else {														
			Y++;
			TGui_Draw_Line(SX-X,SY-Y,EX+X,SY-Y,ColorU);		    //绘制body上部
			TGui_Draw_Line(SX-X,EY+Y,EX+X,EY+Y,ColorD);			//绘制body下部
		}																							
	}
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图,绘制正三角形
 Time: 2010-1-6
 Ver.: V1.0
 Note; (SX,SY)/起始顶点的位置   Direction/绘制方向 =0:上至下 1:下至上 2:左至右 3:右至左 Color/绘制色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_Triangle(uint16 SX,uint16 SY,uint16 Lenth,uint8 Direction,uint16 Color)
{                                                  
    uint16 i=2;
    LCD_Set_Pixel(SX,SY,Color);Lenth-=2;
    switch(Direction)
    {           
        case Dir_UpDown:    
            while(Lenth--){LCD_Set_Pixel(--SX,++SY,Color);LCD_Set_Pixel(SX+i,SY,Color);i+=2;} 
            LCD_Draw_LineX(--SX,++SY,i+1,Color); 
            break;
        case Dir_DownUp:    
            while(Lenth--){LCD_Set_Pixel(--SX,--SY,Color);LCD_Set_Pixel(SX+i,SY,Color);i+=2;} 
            LCD_Draw_LineX(--SX,--SY,i+1,Color);
            break;
        case Dir_LeftRight: 
            while(Lenth--){LCD_Set_Pixel(++SX,--SY,Color);LCD_Set_Pixel(SX,SY+i,Color);i+=2;} 
            LCD_Draw_LineY(++SX,--SY,i+1,Color);
            break;
        case Dir_RightLeft: 
            while(Lenth--){LCD_Set_Pixel(--SX,--SY,Color);LCD_Set_Pixel(SX,SY+i,Color);i+=2;} 
            LCD_Draw_LineY(--SX,--SY,i+1,Color);
            break;   
    }                                        
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图,填充正三角形
 Time: 2010-1-6
 Ver.: V1.0
 Note; (SX,SY)/起始顶点的位置   Direction/绘制方向 =0:上至下 1:下至上 2:左至右 3:右至左 Color/填充色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Fill_Triangle(uint16 SX,uint16 SY,uint16 Lenth,uint8 Direction,uint16 Color)
{                                                  
    uint16 i=1;                                              
    Lenth-=2;
    switch(Direction)
    {
        case Dir_UpDown:    while(Lenth--){LCD_Draw_LineX(SX--,++SY,i,Color);i+=2;} break;
        case Dir_DownUp:    while(Lenth--){LCD_Draw_LineX(SX--,--SY,i,Color);i+=2;} break;
        case Dir_LeftRight: while(Lenth--){LCD_Draw_LineY(++SX,SY--,i,Color);i+=2;} break;
        case Dir_RightLeft: while(Lenth--){LCD_Draw_LineY(--SX,SY--,i,Color);i+=2;} break;   
    }
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图驱动,绘制BMP图形
 Time: 2010-1-5
 Ver.: V1.0
 Note: (X,Y)图形起始位置 W/位图宽度 H/位图高度 *Bmp/位图点阵
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_Bitmap(uint16 X,uint16 Y,uint16 W,uint16 H,uint16 *Bmp)
{
	while(H--){LCD_Fill_LineX(X,Y++,W,Bmp);Bmp+=W;}
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图驱动,BMP图形快速水平填充
 Time: 2010-1-5
 Ver.: V1.0
 Note: 本方法可以将位图进行快速水平方向填充
 	   (X,Y)图形起始位置 W/位图宽度 H/位图高度 WL/填充区域宽度 *Bmp/位图点阵
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Fill_BitmapX(uint16 X,uint16 Y,uint16 W,uint16 H,uint16 WL,uint16 *Bmp)
{
	uint16 i,j;
	i=WL/W;j=WL%W;					
	while(i--){TGUI_Draw_Bitmap(X,Y,W,H,Bmp);X+=W;}
	while(H--){LCD_Fill_LineX(X,Y++,j,Bmp);Bmp+=W;}		
}

/*---------------------------------------------------------------------------------------------------------------------------
 Func: WGUI_2D绘图驱动,BMP图形快速垂直填充
 Time: 2010-1-5
 Ver.: V1.0
 Note: 本方法可以将位图进行快速垂直方向填充
 	   (X,Y)图形起始位置 W/位图宽度 H/位图高度 HL/填充区域高度 *Bmp/位图点阵
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Fill_BitmapY(uint16 X,uint16 Y,uint16 W,uint16 H,uint16 HL,uint16 *Bmp)
{
	uint16 i,j;
	i=HL/H;j=HL%H;					
	while(i--){TGUI_Draw_Bitmap(X,Y,W,H,Bmp);Y+=H;}
	while(j--){LCD_Fill_LineX(X,Y++,W,Bmp);Bmp+=W;}
}

/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制对象的3D凸起效果
 Time: 2010-1-8
 Ver.: V1.0
 Note: (SX,SY)(EX,EY)/对象矩形位置  LightColor/亮边框色彩   DarkColor/暗边框色彩    FaceColor/表面填充色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_ObjectPopUp(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 LightColor,uint16 DarkColor,uint16 FaceColor)
{
    LCD_Draw_LineX(SX,SY,EX-SX,LightColor);
    LCD_Draw_LineY(SX,SY,EY-SY,LightColor);
    LCD_Draw_LineX(SX,EY,EX-SX,DarkColor);
    LCD_Draw_LineY(EX,SY,EY-SY+1,DarkColor);
    TGUI_Fill_Rectangle(SX,SY,EX,EY,FaceColor);
}

/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制对象的3D凹下效果
 Time: 2010-1-8
 Ver.: V1.0
 Note: (SX,SY)(EX,EY)/对象矩形位置  LightColor/亮边框色彩   DarkColor/暗边框色彩    FaceColor/表面填充色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_ObjectPopDown(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 LightColor,uint16 DarkColor,uint16 FaceColor)
{
    LCD_Draw_LineX(SX,SY,EX-SX,DarkColor);
    LCD_Draw_LineY(SX,SY,EY-SY,DarkColor);
    LCD_Draw_LineX(SX,EY,EX-SX,LightColor);
    LCD_Draw_LineY(EX,SY,EY-SY+1,LightColor);
    TGUI_Fill_Rectangle(SX,SY,EX,EY,FaceColor);
}
/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制图片
 Time: 2014-6-10
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_Image(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 PX,uint16 PY,void *ImageAddr)
{
	uint32 T;
	T=(uint32)ImageAddr;	
	if(T<0xFFFF){														//编号图片
		
	}else if(T<0x8000000){												//非法地址
			
	}else if(T<0x20000000){												//ROM地址
		TGUI_Draw_Bitmap(SX,SY,EX-SX,EY-SY,ImageAddr);
	}else{																//RAM地址
	}
}
