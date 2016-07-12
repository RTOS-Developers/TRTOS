#ifndef _UPGRADEBYGPRS_H_
#define _UPGRADEBYGPRS_H_
#include <include.h>
enum
{
	UGC_SetBinFile=0x01,
	UGC_SetTextFile=0x02,
	UGC_GetBinFileInfor=0x03,
	UGC_GetTextFileInfor=0x04,
	UGC_ReadBinFile=0x05,
	UGC_ReadTextFile=0x06,
	UCG_GetDateTime=0x07,
	UCG_CreateBinCFG=0x08,
	UCG_SendBinCFG=0x09,
	UCG_SetBinCFGFile=0x0a,
	UCG_SetMenuList=0x0b,
	UCG_GetMenuList=0x0c,
	UCG_CheckNewMenu=0x0d,
}UpGradeCmd;
#endif
