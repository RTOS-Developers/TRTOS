#include <FS_Include.h>
#include <Tools_Include.h>
#include <TQFS_Service.h>

/*******************************************************************************
Func:寻找文件通过文件名并且文件类型
Date:2014-7-27
Note:
*******************************************************************************/
uint32 TQFS_SearchFileByNameAndType(void *Name,uint8 Type)
{
	uint32 Buf[20];
	uint8 i,Length;
	File_Infor Temp_File;
	Length=TQFS_FindAllKey(Type,&Buf[0],sizeof(Buf)/4,0);///获取符合类型条件的集合
	for(i=0;i<Length;i++)
	{
		
		Read_FileInfor(Buf[i],&Temp_File);
		if(CompareWithString(Temp_File.FileName,Name))return Buf[i];
	}
	return 0xffffffff;
}
uint32 TQFS_SearchFile(void *Name)
{
	return TQFS_SearchFileByNameAndType(Name,FT_FileHead);
}

void TQFS_SearchEndPage(TQFS_Open_File *OpenFile)
{
	File_Infor FI;
	while(1)
	{
		Read_FileInfor(OpenFile->PageCursor,&FI);
		if(FI.NextPageAddr!=0xffffffff)OpenFile->PageCursor=FI.NextPageAddr;
		else break;
	}
}
void TQFS_SearchEndByte(TQFS_Open_File *OpenFile)
{
	OpenFile->OffsetCursor=Nand_FindPageKey(OpenFile->PageCursor+Nand_Infor.FileStartPage,0xff);
}
/*******************************************************************************
Func:删除文件标准文件路径   .eg    /example/新建文件.txt
Date:2014-7-27
Note:FileSize=0时为不确定文件长度
*******************************************************************************/
uint8 TQFS_FileDelete(uint8 *Path)
{
	TQFS_Open_File Open_File;
	StringCoppy(Path,&Open_File.Path[0]);
	return 0;
}
void TQFS_RootDirCreate(TQFS_Open_File *Open_File)
{
	uint32 i;
	File_Infor TempFile;
	i=TQFS_FindKeyPageMark(FT_NotUsed,FT_FolderHead);
	TempFile.FileType=FT_RootDirHead;
	TQFS_FileRoomInfor(&TempFile,"RootDir",0,ReadOnly);
	TQFS_CreateFileRoom(Open_File,i,&TempFile);
}
uint8 TQFS_FileCreate(uint8 *Path,uint8 State)
{
	
//	TQFS_SearchFile(
	return 0;
}

