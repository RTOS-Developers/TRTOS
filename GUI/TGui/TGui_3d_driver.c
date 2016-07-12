#include <Include.h>
#include <Gui_include.h>
#include <TGui_3d_driver.h>
/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_3D»æÍ¼Çý¶¯,»æÖÆ3D¶ÔÏñÒõÓ°
 Time: 2011-6-10
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_3D_Draw_ObjectShadow(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Params)
{		
	//TGUI_Draw_ArcRectangle(SX,SY,  EX,EY,TGUI_ArcCorner_Radius+3,C,TGUI_3D_Shadow_Class1);
	//TGUI_Draw_ArcRectangle(SX,SY+1,EX,EY,TGUI_ArcCorner_Radius+4,C,TGUI_3D_Shadow_Class2);
	//TGUI_Draw_ArcRectangle(SX,SY+2,EX,EY,TGUI_ArcCorner_Radius+5,C,TGUI_3D_Shadow_Class3);
}


/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_3D»æÍ¼Çý¶¯,»æÖÆ3D¶ÔÏñ±ß¿ò
 Time: 2011-6-10
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_3D_Draw_ObjectBorder(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Params)
{
	uint16 C;
	switch(Params){
		case TGUI_State_OnFocus:		C=TGUI_3D_OnFocus_ColorB;	break;
		case TGUI_State_UnFocus:		C=TGUI_3D_UnFocus_ColorB;	break;
		case TGUI_State_EntryEdit:		C=TGUI_3D_OnEdit_ColorB;	break;
		default: C=Color_White;	break;
	}
	TGUI_Draw_ArcRectangle(SX-1,SY-1,EX+1,EY+1,TGUI_ArcCorner_Radius+1,C,C);					  
	TGUI_Draw_ArcRectangle(SX-2,SY-2,EX+2,EY+2,TGUI_ArcCorner_Radius+2,C,C);
}



/*--------------------------------------------------------------------------------------------------------------------------
 Func: TGUI_2D»æÍ¼Çý¶¯,»æÖÆ3D¶ÔÏñµ¯Æð
 Time: 2011-6-10
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------------------------*/
void TGUI_3D_Draw_ObjectBody(uint16 SX,uint16 SY,uint16 EX,uint16 EY,uint16 Params)
{			
	uint16 ColorU,ColorD;
	switch(Params){
		case TGUI_State_OnFocus:									   
			ColorU=TGUI_3D_OnFocus_ColorU;	ColorD=TGUI_3D_OnFocus_ColorD;
			break;
		case TGUI_State_UnFocus:
			ColorU=TGUI_3D_UnFocus_ColorU;  ColorD=TGUI_3D_UnFocus_ColorD;
			break;
		case TGUI_State_EntryEdit:
			ColorU=TGUI_3D_OnEdit_ColorU;	ColorD=TGUI_3D_OnEdit_ColorD;
			break;	
	}
	TGUI_Fill_ArcRectangle(SX,SY,EX,EY,TGUI_ArcCorner_Radius,ColorU,ColorD);
}

