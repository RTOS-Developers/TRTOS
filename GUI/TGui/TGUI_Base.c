#include <Include.h>
#include <Gui_Include.h>
#include <Driver_Include.h>
#include <TGUI_Base.h>
#include <TGui_2D_Driver.h>


/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制对象的3D凸起效果
 Time: 2010-1-8
 Ver.: V1.0
 Note: (SX,SY)(EX,EY)/对象矩形位置  LightColor/亮边框色彩   DarkColor/暗边框色彩    FaceColor/表面填充色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_ObjectPopup(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 FaceColor)
{
  TGUI_Fill_Rectangle(SX,SY,EX,EY,FaceColor);	//表面背景色
	TGui_Draw_Line(SX,EY-1,EX,EY-1,0x8410);		//下边沿内侧暗线
	TGui_Draw_Line(EX-1,SY,EX-1,EY,0x8410);		//右边沿内侧暗线	
	TGui_Draw_Line(SX,SY,EX,SY,0xE73C);			//上边沿亮线
	TGui_Draw_Line(SX,SY,SX,EY,0xE73C);			//左边沿亮线	
	TGui_Draw_Line(SX,EY,EX,EY,0x4208);			//下边沿暗线
	TGui_Draw_Line(EX,SY,EX,EY,0x4208);			//右边沿暗线
}


/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制对象的3D凹下效果
 Time: 2010-1-8
 Ver.: V1.0
 Note: (SX,SY)(EX,EY)/对象矩形位置  LightColor/亮边框色彩   DarkColor/暗边框色彩    FaceColor/表面填充色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_ObjectDown(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 FaceColor)
{   
	TGUI_Fill_Rectangle(SX,SY,EX,EY,FaceColor);
	TGui_Draw_Line(SX,SY+1,EX,SY+1,0x8410);		//下边沿内侧暗线
	TGui_Draw_Line(SX+1,SY,SX+1,EY,0x8410);		//右边沿内侧暗线	
	TGui_Draw_Line(SX,SY,EX,SY,0x4208);			//上边沿亮线
	TGui_Draw_Line(SX,SY,SX,EY,0x4208);			//左边沿亮线	
	TGui_Draw_Line(SX,EY,EX,EY,0xE73C);			//下边沿暗线
	TGui_Draw_Line(EX,SY,EX,EY,0xE73C);			//右边沿暗线
}

/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D绘图驱动,绘制对象的3D凹下效果
 Time: 2010-1-8
 Ver.: V1.0
 Note: (SX,SY)(EX,EY)/对象矩形位置  LightColor/亮边框色彩   DarkColor/暗边框色彩    FaceColor/表面填充色彩
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_Draw_ObjectIn(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 FaceColor)
{   
	TGUI_Fill_Rectangle(SX,SY,EX,EY,FaceColor);
	TGui_Draw_Line(SX,SY+1,EX,SY+1,0x8410);		//下边沿内侧暗线
	TGui_Draw_Line(SX+1,SY,SX+1,EY,0x8410);		//右边沿内侧暗线	
	TGui_Draw_Line(SX,SY,EX,SY,0x4208);			//上边沿亮线
	TGui_Draw_Line(SX,SY,SX,EY,0x4208);			//左边沿亮线	
	TGui_Draw_Line(SX,EY,EX,EY,0xE73C);			//下边沿暗线
	TGui_Draw_Line(EX,SY,EX,EY,0xE73C);			//右边沿暗线
}
 




