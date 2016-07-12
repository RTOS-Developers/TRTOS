#include <Include.h>
#include <..\USER\Prj_Prafire\Game.h>
#include <..\USER\Prj_Prafire\Tetris_Lib.c>
#include<stdlib.h>
#include<time.h> 
typedef struct
{
	uint16 StartX;
	uint16 StartY;
	uint8 Row,Rank;//列
	uint8 GridSize;
	uint16 *Buf;
	uint16 *OldBuf;
	uint8 Select;
	uint8 OldSelect;
	uint8 Index;
	uint8 Mode;
	char X,Y;
	uint8 Value[3];
}TetrisRunValue;
void T_DrawWords(TetrisRunValue *RunValue,uint8 Mode);
/****************************************************************************
Date:2014-11-12
Func:显示图片不管是ROM中的还是RAM中的
Note:Mode 0画死1画活2画框3清画4清框
****************************************************************************/
void T_DrawCell(uint16 x,uint16 y,uint8 Mode)
{
	switch(Mode)
	{
		case 0:MGUI_DrawBitMap(x,y,(uintbus)TetrisCell,0);break;
		case 1:MGUI_DrawBitMap(x,y,(uintbus)TetrisCell,1);break;
	}
}
uint16 T_DrawFillNextForm(uint8 Mode)
{
	uint8 Mo,Mb;
	const uint16 Buf[7][4]={
											{0x3300,0x3300,0x3300,0x3300},//田
											{0x2222,0xf000,0x2222,0xf000},//1
											{0x4700,0x6440,0x7100,0x2260},//L
											{0x2700,0x2320,0x7200,0x2620},//T
											{0x3600,0x4620,0x3600,0x4620},//s
											{0x6300,0x2640,0x6300,0x2640},//z
											{0x7400,0x6220,0x1700,0x4460},//7
										};
	Mo=Mode/0x10;
	Mb=Mode%0x10;
	if(Mo>=7)return 0x0000;
	if(Mb>=4)return 0x0000;
	return Buf[Mo][Mb];
} 
/****************************************************************************
Date:2014-11-12
Func:显示图片不管是ROM中的还是RAM中的
Note:Mode 0清屏1选中2画点
****************************************************************************/
void T_DrawDotArray(TetrisRunValue *RunValue,uint8 Mode)
{
	uint16 x,y;
	switch(Mode)
	{
		case 0:
			for(y=0;y<RunValue->Row;y++)for(x=0;x<RunValue->Rank;x++)T_DrawCell(RunValue->StartX+x*RunValue->GridSize,RunValue->StartY+y*RunValue->GridSize,0);break;
		case 2:
			for(y=0;y<RunValue->Row;y++)
			if(RunValue->Buf[y]!=RunValue->OldBuf[y])
			{
				for(x=0;x<RunValue->Rank;x++)
				{
					if((RunValue->Buf[y]&BIT(x))!=(RunValue->OldBuf[y]&BIT(x)))
					{
						T_DrawCell(RunValue->StartX+x*RunValue->GridSize,RunValue->StartY+y*RunValue->GridSize,!!(RunValue->Buf[y]&BIT(x)));	
					}
				}
				RunValue->OldBuf[y]=RunValue->Buf[y];
			}
			break;
	}
}
/****************************************************************************
Date:2014-11-12
Func:绘制下帧预览
Note:Mode =图形库索引
****************************************************************************/
void T_DrawNextForm(TetrisRunValue *RunValue,uint8 Mode)
{
	uint8 Dp,i,j;
	uint16 x,y,Temp=T_DrawFillNextForm(Mode);
	x=RunValue->Rank*RunValue->GridSize+10;
	y=RunValue->StartY+1;
	for(i=0;i<4;i++)
	{
		Dp=Temp>>12;
		Temp<<=4;
		for(j=0;j<3;j++)
		{
			T_DrawCell(x+RunValue->GridSize*j,y+RunValue->GridSize*i,((Dp&BIT(2-j))>0));
		}
	}
}
/****************************************************************************
Date:2014-11-12
Func:设置缓冲区里的点死活
Note:
****************************************************************************/
uint8 T_SGetCell(TetrisRunValue *RunValue,uint8 x,uint8 y,uint8 Mode)
{
	if(x>=RunValue->Rank)return 0xff;
	if(y>=RunValue->Row)return 0xff;
	if(Mode==0)RunValue->Buf[y]&=~BIT(x);
	else if(Mode==1)RunValue->Buf[y]|=BIT(x);
	return ((RunValue->Buf[y]&BIT(x))>0);
}
/****************************************************************************
Date:2014-11-12
Func:绘制或者求是否可以绘制
Note:
****************************************************************************/
uint8 T_FormVoid(TetrisRunValue *RunValue,uint8 x,uint8 y,uint8 Data,uint8 Mode)
{
	uint8 i,j,Dp,T;
	uint16 Temp=T_DrawFillNextForm(Data);
	for(i=0;i<4;i++)
	{
		Dp=Temp>>12;
		Temp<<=4;
		for(j=0;j<4;j++)if(Dp&BIT(3-j))
			{
				switch(Mode)
				{
					case 0:T=T_SGetCell(RunValue,x+j,i+y,0xff);if(T)return T;break;
					case 1:T_SGetCell(RunValue,x+j,i+y,1);break;
					case 2:T_SGetCell(RunValue,x+j,i+y,0);break;
				}
			}
	}
	return False;
}
/****************************************************************************
Date:2014-11-12
Func:整行删除
Note:
****************************************************************************/
void T_DeleteLine(TetrisRunValue *RunValue)
{
	uint8 i,z=0,m=0;
	uint16 Temp[20];
	BufferFill(&Temp[0],0x00,sizeof(Temp[0]));
	for(i=0;i<RunValue->Row;i++)
	if(RunValue->Buf[i]!=0x3ff)Temp[z++]=RunValue->Buf[i];
	for(i=(RunValue->Row-z);i<RunValue->Row;i++)RunValue->Buf[i]=Temp[m++];
	RunValue->Value[2]+=(RunValue->Row-z);
	if(RunValue->Value[2]>100){RunValue->Value[0]++;RunValue->Value[2]=0;}
	if(RunValue->Row-z)T_DrawWords(RunValue,S1|S2);
}
/****************************************************************************
Date:2014-11-12
Func:核心出来按键图像移动等
Note:
****************************************************************************/
BOOL T_TetrisPlan(TetrisRunValue *RunValue,uint8 KeyMode)
{
	uint8 Temp;
	if(KeyMode==Key_Right)
	{
		T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,2);//清除旧
		if(!T_FormVoid(RunValue,RunValue->X+1,RunValue->Y,RunValue->OldSelect,0))//判断新位置是否容得下
		{
			T_FormVoid(RunValue,RunValue->X+1,RunValue->Y,RunValue->OldSelect,1);//绘画新的
			RunValue->X++;
		}else T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,1);//否则重绘旧的
	}
	if(KeyMode==Key_Left)
	{
		T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,2);//清除旧
		if(!T_FormVoid(RunValue,RunValue->X-1,RunValue->Y,RunValue->OldSelect,0))//判断新位置是否容得下
		{
			T_FormVoid(RunValue,RunValue->X-1,RunValue->Y,RunValue->OldSelect,1);//绘画新的
			RunValue->X--;
		}else T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,1);//否则重绘旧的
	}
	if(KeyMode==Key_Down)
	{
		T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,2);//清除旧
		if(!T_FormVoid(RunValue,RunValue->X,RunValue->Y+1,RunValue->OldSelect,0))//判断新位置是否容得下
		{
			T_FormVoid(RunValue,RunValue->X,RunValue->Y+1,RunValue->OldSelect,1);//绘画新的
			RunValue->Y++;
		}else {T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,1);T_DeleteLine(RunValue);return True;}//否则重绘旧的;新行
	}
	if(KeyMode==Key_Up)
	{
		Temp=RunValue->OldSelect%0x10;Temp++;if(Temp>=4)Temp=0;Temp|=(RunValue->OldSelect&0xf0);
		T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,2);//清除旧
		if(!T_FormVoid(RunValue,RunValue->X,RunValue->Y,Temp,0))//判断新位置是否容得下
		{
			T_FormVoid(RunValue,RunValue->X,RunValue->Y,Temp,1);//绘画新的
			RunValue->OldSelect=Temp;
		}else T_FormVoid(RunValue,RunValue->X,RunValue->Y,RunValue->OldSelect,1);//否则重绘旧的;
	}
	return False;
}
/****************************************************************************
Date:2014-11-12
Func:产生新的帧
Note:
****************************************************************************/
void T_JoinNew(TetrisRunValue *RunValue)
{
	srand(RunValue->Select);
	RunValue->OldSelect=RunValue->Select&0xf0;
	RunValue->Select=rand()%0x6f;
	T_DrawNextForm(RunValue,RunValue->Select&0xf0);//随机新样式
}
/****************************************************************************
Date:2014-11-12
Func:绘制界面非框架文字
Note:
****************************************************************************/
void T_DrawWords(TetrisRunValue *RunValue,uint8 Mode)
{
	char *Menu[7]={"Run  ","New","Load","Option","Help","Quit","Pause"};
	char *Menu1[7]={"Score:%d  ","Level:%d  ","DLine:%d  "};
	uint8 i,GridSize=(MenuValue->EndY-RunValue->StartY)/6;
	uint16 ColorTemp;
	if(Mode&S0)
	{
		TGUI_Set_Font(Run_Value.MenuCFG.MenuFont);
		for(i=0;i<6;i++)
		{
				if(RunValue->Index==i)Tos_GetStyleHandle()->SelectTrue=True;else Tos_GetStyleHandle()->SelectTrue=False;
				TGUI_Set_Region(MenuValue->EndX-100,MenuValue->StartY+i*GridSize,MenuValue->EndX,MenuValue->StartY+(i+1)*GridSize,Align_LeftCenter);
				if(RunValue->Mode==4&i==0)Printf(Menu[6]);
				else	 Printf(Menu[i]);
		}
	}
	if(Mode&S1)
	{
		GridSize=(MenuValue->EndY-RunValue->StartY)/4;
		TGUI_Set_Font(TGUI_FontType_16X16);
		for(i=0;i<3;i++)
		{
				Tos_GetStyleHandle()->SelectTrue=False;
				TGUI_Set_Region(RunValue->Rank*RunValue->GridSize+2,MenuValue->StartY+(i+1)*GridSize,RunValue->Rank*RunValue->GridSize+60,MenuValue->StartY+(i+2)*GridSize,Align_LeftTop);
				Printf(Menu1[i],RunValue->Value[i]);
		}
	}
	if(Mode&S2)
	{
		if(Tos_GetStyleHandle()->B_Color==Tos_GetStyleHandle()->F_Color)ColorTemp=Tos_GetStyleHandle()->B_Color;else ColorTemp=Run_Value.MenuCFG.K_Color;
		TGUI_Draw_Rectangle(RunValue->Rank*RunValue->GridSize+2,RunValue->StartY-2,RunValue->Rank*RunValue->GridSize+52,LCD_YSIZE,ColorTemp);
		LCD_Draw_LineX(RunValue->Rank*RunValue->GridSize+2,MenuValue->StartY+(RunValue->Row*RunValue->GridSize)/4*1+2,50,ColorTemp);
		LCD_Draw_LineX(RunValue->Rank*RunValue->GridSize+2,MenuValue->StartY+(RunValue->Row*RunValue->GridSize)/4*2+2,50,ColorTemp);
		LCD_Draw_LineX(RunValue->Rank*RunValue->GridSize+2,MenuValue->StartY+(RunValue->Row*RunValue->GridSize)/4*3+2,50,ColorTemp);
	}
}
void T_Save(TetrisRunValue *Value)
{
	BufferCoppy(&Value->Buf[0],&Run_Value.LifePlayBuf[0],Value->Row);
	Task_SaveConfigs(0);
}
void T_Load(TetrisRunValue *Value)
{
	Task_LoadConfigs(0);
	BufferCoppy(&Run_Value.LifePlayBuf[0],&Value->Buf[0],Value->Row);
}
void Task_Tetris()
{
	uint16 Buf[2][20];
	uint8 KeyData;
	TetrisRunValue RunValue;
	MUI_ReSetTitle("Tetris");
	RunValue.StartX=MenuValue->StartX+2;
	RunValue.StartY=MenuValue->StartY+2;
	RunValue.Row=17;
	RunValue.Rank=10;
	RunValue.Mode=1;
	RunValue.GridSize=11;
	RunValue.Buf=&Buf[0][0];
	RunValue.OldBuf=&Buf[1][0];
	RunValue.Select=0x00;
	RunValue.Index=0x00;
	RunValue.OldSelect=0x01;
	MUI_SetDrawClear(False);
	T_DrawWords(&RunValue,0xff);
	while(RunValue.Mode)
	{
		switch(RunValue.Mode)
		{
		  case 1:
						BufferFill(&Buf[0],0x00,sizeof(Buf[0]));
						BufferFill(&Buf[1],0xff,sizeof(Buf[0]));
						RunValue.Mode=3;break;
			case 2:	

						break;
			case 3://菜单
						T_DrawWords(&RunValue,0);
						KeyData=MUI_GetKey(0);
						if(KeyData==Key_Up)if(RunValue.Index==0)RunValue.Index=5;else RunValue.Index--;
						if(KeyData==Key_Down)RunValue.Index++;if(RunValue.Index>=6)RunValue.Index=0;
						if(KeyData==Key_Ctrl){RunValue.Mode=5;RunValue.Index=0;}
						if(KeyData==Key_Right)
						{
							if(RunValue.Index==5)RunValue.Mode=0;
							if(RunValue.Index==0)RunValue.Mode=4;
							if(RunValue.Index==1)RunValue.Mode=5;
							if(RunValue.Index==2)T_Load(&RunValue);
						}break;
			case 4://运行
						KeyData=MUI_GetKey(0);
					  if(T_TetrisPlan(&RunValue,KeyData)){T_JoinNew(&RunValue);RunValue.Y=0;RunValue.X=0;}//新块
						if(KeyData==Key_Ctrl)RunValue.Mode=6;break;
			case 5://游戏初始化
						BufferFill(&Buf[0],0x00,sizeof(Buf[0]));
						BufferFill(&Buf[1],0xff,sizeof(Buf[0]));
						T_JoinNew(&RunValue);
						T_JoinNew(&RunValue);
						RunValue.Y=0;RunValue.X=0;
						RunValue.Mode=4;
						RunValue.Index=0;
						BufferFill(&RunValue.Value[0],0x00,sizeof(RunValue.Value));
						T_DrawWords(&RunValue,S0|S1|S2);break;
			case 6://游戏暂停
						T_DrawWords(&RunValue,S0);
						KeyData=MUI_GetKey(0);
						if(KeyData==Key_Up)if(RunValue.Index==0)RunValue.Index=5;else RunValue.Index--;
						if(KeyData==Key_Down)RunValue.Index++;if(RunValue.Index>=6)RunValue.Index=0;
						if(KeyData==Key_Ctrl){RunValue.Mode=4;RunValue.Index=0;}
						if(KeyData==Key_Right)
						{
							if(RunValue.Index==5)RunValue.Mode=0;
							if(RunValue.Index==0){RunValue.Mode=4;RunValue.Index=0;}
							if(RunValue.Index==1)RunValue.Mode=5;
						}
						T_DrawWords(&RunValue,S0);break;
			default:break;
		}
		T_DrawDotArray(&RunValue,1);
		T_DrawDotArray(&RunValue,2);
	}
	T_DrawDotArray(&RunValue,0);
	T_DrawNextForm(&RunValue,0xff);
	MUI_SetDrawClear(True);
	T_DrawWords(&RunValue,0xff);
}
