#include <Include.h>
#include <..\USER\Prj_Prafire\Game.h>
#include <..\USER\Prj_Prafire\LifePlay_Lib.c>
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
}LifePlayRunValue;
/****************************************************************************
Date:2014-11-12
Func:显示图片不管是ROM中的还是RAM中的
Note:Mode 0画死1画活2画框3清画4清框
****************************************************************************/
void LP_DrawCell(uint16 x,uint16 y,uint8 Mode)
{
	switch(Mode)
	{
		case 0:MGUI_DrawBitMap(x,y,(uintbus)LifePlayCellDie,1);break;
		case 1:MGUI_DrawBitMap(x,y,(uintbus)LifePlayCellLive,1);break;
		case 2:TGUI_Draw_Rectangle(x-1,y-1,x+LifePlayCellDie[0]+1,y+LifePlayCellDie[1]+1,Tos_GetStyleHandle()->S_Color);break;
		case 3:MGUI_DrawBitMap(x,y,(uintbus)LifePlayCellDie,0);break;
		case 4:TGUI_Draw_Rectangle(x-1,y-1,x+LifePlayCellDie[0]+1,y+LifePlayCellDie[1]+1,Tos_GetStyleHandle()->B_Color);break;
	}
}

/****************************************************************************
Date:2014-11-12
Func:显示图片不管是ROM中的还是RAM中的
Note:Mode 0清屏1选中2画点
****************************************************************************/
void LP_DrawDotArray(LifePlayRunValue *RunValue,uint8 Mode)
{
	uint16 x,y;
	switch(Mode)
	{
		case 0:
			for(y=0;y<RunValue->Row;y++)for(x=0;x<RunValue->Rank;x++)LP_DrawCell(RunValue->StartX+x*RunValue->GridSize,RunValue->StartY+y*RunValue->GridSize,3);break;
		case 1:
			if(RunValue->Mode==2)
			{
				if((RunValue->SelectX!=RunValue->OldSelectX)|(RunValue->SelectY!=RunValue->OldSelectY))
				{
				LP_DrawCell(RunValue->StartX+RunValue->OldSelectX*RunValue->GridSize,RunValue->StartY+RunValue->OldSelectY*RunValue->GridSize,4);
				LP_DrawCell(RunValue->StartX+RunValue->SelectX*RunValue->GridSize,RunValue->StartY+RunValue->SelectY*RunValue->GridSize,2);
				RunValue->OldSelectX=RunValue->SelectX;RunValue->OldSelectY=RunValue->SelectY;
				}
			}else LP_DrawCell(RunValue->StartX+RunValue->OldSelectX*RunValue->GridSize,RunValue->StartY+RunValue->OldSelectY*RunValue->GridSize,4);break;
		case 2:
			for(y=0;y<RunValue->Row;y++)
			if(RunValue->Buf[y]!=RunValue->OldBuf[y])
			{
				for(x=0;x<RunValue->Rank;x++)
				{
					if((RunValue->Buf[y]&BIT(x))!=(RunValue->OldBuf[y]&BIT(x)))
					{
						LP_DrawCell(RunValue->StartX+x*RunValue->GridSize,RunValue->StartY+y*RunValue->GridSize,!!(RunValue->Buf[y]&BIT(x)));	
					}
				}
				RunValue->OldBuf[y]=RunValue->Buf[y];
			}break;
	}
}

void LP_DrawWords(LifePlayRunValue *RunValue,uint8 Select)
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
void LP_Save(LifePlayRunValue *Value)
{
	BufferCoppy(&Value->Buf[0],&Run_Value.LifePlayBuf[0],Value->Row);
	Task_SaveConfigs(0);
}
void LP_Load(LifePlayRunValue *Value)
{
	Task_LoadConfigs(0);
	BufferCoppy(&Run_Value.LifePlayBuf[0],&Value->Buf[0],Value->Row);
}
void Task_LifePlay()
{

	uint8 KeyData;
	LifePlayRunValue *AppValue;
	AppValue=Mema_Malloc(sizeof(LifePlayRunValue));
	MUI_ReSetTitle("Life Play");
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
	LP_DrawWords(AppValue,0xff);
	AppValue=Mema_Extend(AppValue,sizeof(LifePlayRunValue)+20);//测试内存变更
	AppValue=Mema_Extend(AppValue,sizeof(LifePlayRunValue)+100);//测试内存变更
	while(AppValue->Mode)
	{
		switch(AppValue->Mode)
		{
		  case 1:
						BufferFill(&AppValue->Buf[0],0x00,sizeof(AppValue->Buf));
						BufferFill(&AppValue->OldBuf[0],0xff,sizeof(AppValue->OldBuf));
						AppValue->Mode=2;break;
			case 2:	
						KeyData=MUI_GetKey(0);
						if(KeyData==Key_Right)if(++AppValue->SelectX>=AppValue->Rank){AppValue->Mode=3;AppValue->SelectY=0x00;}//{AppValue->SelectX=0;if(++AppValue->SelectY>=AppValue->Row)AppValue->SelectY=0;}
						if(KeyData==Key_Left)if(AppValue->SelectX>0)AppValue->SelectX--;else {AppValue->SelectX=AppValue->Rank-1;if(AppValue->SelectY>0)AppValue->SelectY--;else AppValue->SelectY=AppValue->Row-1;}
						if(KeyData==Key_Down)if(++AppValue->SelectY>=AppValue->Row){AppValue->SelectY=0;if(++AppValue->SelectX>=AppValue->Rank)AppValue->SelectX=0;}
						if(KeyData==Key_Up)if(AppValue->SelectY>0)AppValue->SelectY--;else {AppValue->SelectY=AppValue->Row-1;if(AppValue->SelectX>0)AppValue->SelectX--;else AppValue->SelectX=AppValue->Rank-1;}
						if(KeyData==Key_Ctrl)if(AppValue->Buf[AppValue->SelectY]&BIT(AppValue->SelectX))AppValue->Buf[AppValue->SelectY]&=~BIT(AppValue->SelectX);else AppValue->Buf[AppValue->SelectY]|=BIT(AppValue->SelectX);
						break;
			case 3:
						LP_DrawWords(AppValue,AppValue->SelectY);
						KeyData=MUI_GetKey(0);
						if(KeyData==Key_Up)if(AppValue->SelectY==0)AppValue->SelectY=5;else AppValue->SelectY--;
						if(KeyData==Key_Down)AppValue->SelectY++;if(AppValue->SelectY>=6)AppValue->SelectY=0;
						if(KeyData==Key_Left){AppValue->Mode=2;AppValue->SelectX=0x00;AppValue->SelectY=0x00;LP_DrawWords(AppValue,0xff);}
						if(KeyData==Key_Right)
						{
							if(AppValue->SelectY==5)AppValue->Mode=0;
							if(AppValue->SelectY==0)AppValue->Mode=4;
							if(AppValue->SelectY==1)LP_Save(AppValue);
							if(AppValue->SelectY==2)LP_Load(AppValue);
							if(AppValue->SelectY==4)AppValue->Mode=1;
						}break;
			case 4:
						if(KeyData)LP_DrawWords(AppValue,AppValue->SelectY);
						if(!LP_LifePlan(&AppValue->Buf[0],&AppValue->OldBuf[0],AppValue->Row,AppValue->Rank))AppValue->Mode=3;
						Tos_TaskDelay(120);
						KeyData=MUI_GetKey(1);
						if(KeyData==Key_Right)AppValue->Mode=3;break;
			default:break;
		}
		LP_DrawDotArray(AppValue,1);
		LP_DrawDotArray(AppValue,2);
	}
	LP_DrawDotArray(AppValue,0);
	MUI_SetDrawClear(True);
	LP_DrawWords(AppValue,0xff);
	Mema_Free((uint8 *)AppValue);
}
