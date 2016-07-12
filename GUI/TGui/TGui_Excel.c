#include <Include.h>
#include <Tgui_include.h>
#include <Tools_include.h>
#include <TGui_Excel.h>

_TGUI_Excel *P_TGui_Excel;

/*---------------------------------------------------------------------------------------------------------
 Func:设置操作对象
 Time: 2014-1-31
 Ver.: V1.0
 Note; 
---------------------------------------------------------------------------------------------------------*/
void Tos_SetExcleHandle(_TGUI_Excel *Xls)
{
	P_TGui_Excel=Xls; 
}
/*---------------------------------------------------------------------------------------------------------
 Func: 绘制表格
 Time: 2014-1-31
 Ver.: V1.0
 Note; 本方法用于打印表格
---------------------------------------------------------------------------------------------------------*/
void Tos_DrawTable()
{
	uint8 i,Columns_Count;
	uint16 TableWith;
	uint16 ColumsSum=0;
	uint16 SX,SY,EX,EY;
	_TGui_2D *TB;
	TB=(P_TGui_Excel->TB);
	Columns_Count=GetTextLength(P_TGui_Excel->Columns_Array);
	TableWith=GetArraySum(P_TGui_Excel->Columns_Array,0xffff)*8;
	SX=TB->SX;
	SY=TB->SY+Font_Max*8;
	EX=TB->SX+TableWith;
	EY=SY+P_TGui_Excel->Row_High*(P_TGui_Excel->Row_Count);
	TGUI_Set_Font(Font_Max);
	TGUI_Draw_String(TB->SX,TB->SY,EX,SY,P_TGui_Excel->Title,Align_CenterCenter);
	TGUI_Draw_Rectangle(SX,SY,EX,EY,TB->S_Color);
	for(i=0;i<P_TGui_Excel->Row_Count-1;i++)
	{
		TGUI_Draw_DotLine(SX+1,SY+(i+1)*P_TGui_Excel->Row_High,EX-1,SY+(i+1)*P_TGui_Excel->Row_High,TB->S_Color,TB->B_Color);
	}
  for(i=0;i<(Columns_Count-1);i++)
	{
		ColumsSum+=((uint8 *)P_TGui_Excel->Columns_Array)[i]*8;
		TGUI_Draw_DotLine(SX+ColumsSum,SY+1,SX+ColumsSum,EY-1,TB->S_Color,TB->B_Color);
	}
}
/*---------------------------------------------------------------------------------------------------------
 Func: 填充表格
 Time: 2014-1-31
 Ver.: V1.0
 Note; 本方法用于填充表格内容
---------------------------------------------------------------------------------------------------------*/
void Tos_SetTableString(uint16 Columns,uint16 Rows,void *Tags)
{
	_TGui_2D *TB;
	uint16 SX,SY;
	TB=(P_TGui_Excel->TB);
	SX=TB->SX+GetArraySum(P_TGui_Excel->Columns_Array,Columns)*8;
	SY=TB->SY+Font_Max*8;
	TGUI_Set_Font(Font_Min);
  TGUI_Draw_String(SX,TB->SY+SY+Rows*P_TGui_Excel->Row_High,SX+((uint8 *)P_TGui_Excel->Columns_Array)[Columns]*8,SY+(Rows+1)*P_TGui_Excel->Row_High,Tags,P_TGui_Excel->Gird_Align);
	
}
/*---------------------------------------------------------------------------------------------------------
 Func: 填充数据到表格
 Time: 2014-1-31
 Ver.: V1.0
 Note; 本方法用于填充数据到表格
---------------------------------------------------------------------------------------------------------*/
void Tos_SetTableData(uint16 Columns,uint16 Rows,f32 Number,uint8 NumType,uint8 DotLenth)
{
	uint8 Buf[20];
	GetStringFromNumber(&Buf[0],Number,NumType,DotLenth);
	Tos_SetTableString(Columns,Rows,&Buf[0]);
}
/*---------------------------------------------------------------------------------------------------------
 Func: 测试表格
 Time: 2014-1-31
 Ver.: V1.0
 Note; 这是一个测试表格的案例
---------------------------------------------------------------------------------------------------------*/
void Tos_TaskTabInfor(void)
{
	uint8 i;
	Tos_SetTableString(0,0,"Id");
	Tos_SetTableString(1,0,"Task_ST");
	Tos_SetTableString(2,0,"Task_SP");
	Tos_SetTableString(3,0,"Task_FT");
	Tos_SetTableString(4,0,"Faddr");
	Tos_SetTableString(5,0,"Task_TM");
	Tos_SetTableString(6,0,"Task_QS");
	Tos_SetTableString(7,0,"Run");
	for(i=0;i<Tos_TaskTotal;i++)
	{
		Tos_SetTableData(0,i+1,i,'D',0);
		Tos_SetTableData(1,i+1,Tos_TaskTcb[i].Task_ST,'D',0);
		Tos_SetTableData(2,i+1,Tos_TaskTcb[i].Task_SP,'H',0);
		Tos_SetTableData(3,i+1,Tos_TaskTcb[i].Task_SA,'H',0);
		Tos_SetTableData(4,i+1,(uint32)Tos_TaskTcb[i].vfFunc,'H',0);
		Tos_SetTableData(5,i+1,Tos_TaskTcb[i].Task_TD,'D',0);
		Tos_SetTableData(6,i+1,Tos_TaskTcb[i].Task_QS,'D',0);
	}
}
