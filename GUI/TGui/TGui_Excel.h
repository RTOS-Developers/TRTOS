#ifndef _EXCEL_H_
#define _EXCEL_H_
#include <include.h>
typedef struct Excel{
	_TGui_2D *TB;
	uint16  	Row_High;//行高
	uint16  	Row_Count;//行数
	void    	*Columns_Array;//列宽度数组
	uint16  	Columns_Count;//列数
	void    	*Title;//标题
	uint8     Gird_Align;//靠齐方式
}_TGUI_Excel;

extern _TGUI_Excel *P_TGui_CurXls;
void Tos_SetExcleHandle(_TGUI_Excel *Xls);
void Tos_DrawTable(void);
void Tos_SetTableString(uint16 Columns,uint16 Rows,void *Tags);
void Tos_SetTableData(uint16 Columns,uint16 Rows,f32 Number,uint8 NumType,uint8 DotLenth);
void Tos_SetTableString(uint16 Columns,uint16 Rows,void *Tags);

#endif
