#include <Include.h>
#include <..\USER\Prj_Prafire\Game.h>
//#include <..\USER\Prj_Prafire\LifePlay_Lib.c>
typedef struct
{
	uint16 StartX;
	uint16 StartY;
	uint8 Row;//行
	uint8	Rank;//列
	uint8 GridSize;
	uint8 Buf[8];
	uint8 OldBuf[8];
	uint8 SelectX;
	uint8 SelectY;
	uint8 OldSelectX;
	uint8 OldSelectY;
	uint8 Mode;
}PictureMosaicRunValue;
/****************************************************************************
Date:2014-11-12
Func:显示图片不管是ROM中的还是RAM中的
Note:Mode 0画死1画活2画框3清画4清框
****************************************************************************/
void PM_DrawCell(uint16 x,uint16 y,uint8 Mode)
{
	switch(Mode)
	{
//		case 0:MGUI_DrawBitMap(x,y,(uintbus)LifePlayCellDie,1);break;
//		case 1:MGUI_DrawBitMap(x,y,(uintbus)LifePlayCellLive,1);break;
//		case 2:TGUI_Draw_Rectangle(x-1,y-1,x+LifePlayCellDie[0]+1,y+LifePlayCellDie[1]+1,Tos_GetStyleHandle()->S_Color);break;
//		case 3:MGUI_DrawBitMap(x,y,(uintbus)LifePlayCellDie,0);break;
//		case 4:TGUI_Draw_Rectangle(x-1,y-1,x+LifePlayCellDie[0]+1,y+LifePlayCellDie[1]+1,Tos_GetStyleHandle()->B_Color);break;
	}
}

void PM_DrawWords(PictureMosaicRunValue *RunValue,uint8 Select)
{
	uint16 ColorTemp;
	char *Menu[7]={"Run ","Save","Load","Option","Help","Quit","Stop"};
	uint8 i,GridSize=(MenuValue->EndY-RunValue->StartY)/6;
	TGUI_Set_Font(Run_Value.MenuCFG.MenuFont);
	for(i=0;i<6;i++)
	{
			if(Select==i)Tos_GetStyleHandle()->SelectTrue=True;else Tos_GetStyleHandle()->SelectTrue=False;
			TGUI_Set_Region(MenuValue->EndX-100,MenuValue->StartY+i*GridSize,MenuValue->EndX,MenuValue->StartY+(i+1)*GridSize,Align_CenterCenter);
			if(RunValue->Mode==4&i==0)Printf(Menu[6]);
			else	 Printf(Menu[i]);
	}
	if(Tos_GetStyleHandle()->B_Color==Tos_GetStyleHandle()->F_Color)ColorTemp=Tos_GetStyleHandle()->B_Color;else ColorTemp=Run_Value.MenuCFG.K_Color;
	TGUI_Draw_Rectangle(RunValue->StartX-2,RunValue->StartY-3,RunValue->Rank*RunValue->GridSize+3,RunValue->StartY+RunValue->Rank*RunValue->GridSize,ColorTemp);
}
void Task_PictureMosaic()
{
	PictureMosaicRunValue *AppValue;
	AppValue=Mema_Malloc(sizeof(LifePlayRunValue));
	MUI_ReSetTitle("Pic Move");
	AppValue->StartX=MenuValue->StartX+3;
	AppValue->StartY=MenuValue->StartY+4;
	AppValue->Row=8;
	AppValue->Rank=8;
	AppValue->Mode=1;
	AppValue->GridSize=23;
	AppValue->SelectX=0x00;
	AppValue->SelectY=0x00;
	AppValue->OldSelectX=0x01;
	AppValue->OldSelectY=0x01;
	MUI_SetDrawClear(False);
	PM_DrawWords(AppValue,0xff);
	while(AppValue->Mode)
	{
		switch(AppValue->Mode)
		{
		  case 1:Tos_TaskDelay(10);break;
			default:break;
		}
	}
	MUI_SetDrawClear(True);
	PM_DrawWords(AppValue,0xff);
	Mema_Free((uint8 *)AppValue);
}
