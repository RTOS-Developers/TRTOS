#include <FS_Include.h>
#include <Tools_Include.h>
#include <TQFS_Infor.h>

/*******************************************************************************
Func:打印文件属性
Date:2014-7-27
Note:通常只在调试的时候需要
*******************************************************************************/
void Printf_FileInfor(File_Infor *FI)
{
	CreateTrueString(&FI->FileName[0]);
	CreateTrueString(&FI->FileFormat[0]);
	DeBug("FileName:%s",&FI->FileName[0],Infor_Infor);
	DeBug("FileFormat:%s",&FI->FileFormat[0],Infor_Infor);
	DeBug("Page_Status:%x",FI->Page_Status,Infor_Infor);
	Printf_DateTime("Create_DateTime:",&FI->CreateDate);
	Printf_DateTime("ReBuild_DateTime:",&FI->ReBuildDate);
	DeBug("FileSize%dBytes",FI->FileSize,Infor_Infor);
	DeBug("FileType:0x%x",FI->FileType,Infor_Infor);
	DeBug("PreviousPageAddr:%d",FI->PreviousPageAddr,Infor_Infor);
	DeBug("NextPageAddr:%d",FI->NextPageAddr,Infor_Infor);
	DeBug("FileVirtue:0x%x",FI->FileVirtue,Infor_Infor);
	DeBug("PS_ST:0x%x",FI->PS_ST,Infor_Infor);
	DeBug("FileID:%d",FI->FileID,Infor_Infor);
	DeBug("SpareCrc:0x%x",FI->SpareCrc,Infor_Infor);
	DeBug("PageCrc:0x%x",FI->PageCrc,Infor_Infor);	
}
/*******************************************************************************
Func:枚举打印泛型文件
Date:2014-7-27
Note:通常只在调试的时候需要
*******************************************************************************/
void Printf_ObjectFileList(uint8 Key,uint16 Page)
{
	uint32 Buf[21];
	uint8 i,Length;
	File_Infor Temp_File;
	DeBug("SearchFile....",Length,Infor_Infor);
	Length=TQFS_FindAllKey(FT_FileHead,&Buf[0],20,Page);
	DeBug("Search Show Between %d And %d",Length*Page,Length*(Page+1),Infor_Infor);
	for(i=0;i<Length;i++)
	{
		
		Read_FileInfor(Buf[i],&Temp_File);
		if(Temp_File.FileID==Buf[i])DeBug("%s.%s  Page[%d]",Temp_File.FileName,Temp_File.FileFormat,Buf[i],Infor_Infor);
	}
}
/*******************************************************************************
Func:指定页头获取页内容长度
Date:2014-7-27
Note:通常只在调试的时候需要
*******************************************************************************/
void Printf_FilePageCount(uint32 StartPage)
{
	File_Infor Temp_File;
	uint16 Count=0;
	while(1)
	{
		Read_FileInfor(StartPage,&Temp_File);
		Count++;
		StartPage=Temp_File.NextPageAddr;
		if(StartPage>0xffff){DeBug("ThisHas %dPage",Count,Infor_Infor);return;}
	}
}
