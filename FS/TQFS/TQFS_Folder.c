#include <Include.h>
#include <Tools_Include.h>
#include <FS_Include.h>

/*******************************************************************************
Func:创建根目录
Date:2014-8-4
Note:
*******************************************************************************/
void TQFS_CreateRootFolder()
{
	uint32 i;
	File_Infor TempFile;
	TQFS_Open_File Open_File;
  if(TQFS_SearchFileByNameAndType("Root",FT_RootDirHead)==0xffffffff)
	{
	DeBug("File Create Succeed",Infor_Infor);
	i=TQFS_FindKeyPageMark(FT_NotUsed,FT_RootDirHead);
	TQFS_SetCursor(&Open_File,i,0);
	TQFS_FileRoomInfor(&TempFile,"Root.Dir",0,ReadOnly|WriteOnly);
	TQFS_CreateFileRoom(&Open_File,i,&TempFile);
	}else DeBug("File Already Exists",Infor_Infor);
}
/******************************************************************************
 Func: 读取文件夹信息从页的内容区域
 Time: 2014-7-23
 Ver.: V1.0
 Note:Page为相对位置
******************************************************************************/
void Read_FolderInfor(TQFS_Open_File *Open_File,Folder_FileInfor *FFI)
{
	;
	Nand_ReadPage(Open_File->PageCursor+Nand_Infor.FileStartPage,(uint8 *)FFI,Open_File->OffsetCursor,sizeof(Folder_FileInfor));
}
/******************************************************************************
 Func: 读取文件夹信息从页的内容区域
 Time: 2014-7-23
 Ver.: V1.0
 Note:Page为相对位置
******************************************************************************/
void Write_FolderInfor(TQFS_Open_File *Open_File,Folder_FileInfor *FFI)
{
	Nand_WritePage(Open_File->PageCursor+Nand_Infor.FileStartPage,(uint8 *)FFI,Open_File->OffsetCursor,sizeof(Folder_FileInfor));
}

void TQFS_FolderWriteFile(TQFS_Open_File *Open_File,Folder_FileInfor *Folder_File)
{
	
}
