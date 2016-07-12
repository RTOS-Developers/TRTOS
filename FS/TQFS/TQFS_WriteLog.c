#include <include.h>
#include <FS_Include.h>
#include <Tools_include.h>
#include <TQFS_WriteLog.h>
TQFS_Open_File DeBug_OpenFile;


/****************************************************************************
Date:2014-5-12
Func:写日志文件初始化
Note:
****************************************************************************/
void WriteLog_Init()
{
	File_Infor FI;
	uint32 i;
	TQFS_FileRoomInfor(&FI,"DeBugFile.txt",0,ReadOnly|WriteOnly);//创建文件基本信息
	i=TQFS_FindKeyPageMark(FT_NotUsed,FT_FileHead);//寻找可用区域
 	TQFS_CreateFileRoom(&DeBug_OpenFile,i,&FI);	//写文件头
 	TQFS_SetCursor(&DeBug_OpenFile,i,0);//设置光标
}
/****************************************************************************
Date:2014-5-12
Func:写日志文件
Note:
****************************************************************************/
void DeBug_WriteLog(void *String)
{
	TQFS_WritePageContext(&DeBug_OpenFile,String,GetTextLength(String));//追加内容到日志尾部
	DeBug("WriteLog:%s",String,Infor_Infor);//并打印写入的日志信息到调试接口
}
