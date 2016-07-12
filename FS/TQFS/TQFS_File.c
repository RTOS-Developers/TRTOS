#include <FS_Include.h>
#include <TQFS_File.h>
#include <Tools_Include.h>
#include <stdlib.h>



/*******************************************************************************
Func:填充文件容器基本信息
Date:2014-7-27
Note:FileSize=0时为不确定文件长度
*******************************************************************************/
void TQFS_FileRoomInfor(File_Infor *FI,void *File,uint32 FileSize,uint8 FileVirtue)
{
	uint8 Buf[14];
	BufferFill(FI,0xff,sizeof(File_Infor));//配置不设置项为不修改状态
	GetFileNameFormat(File,&Buf[0],Null);
	StringCoppy(&Buf[0],&FI->FileName[0]);
	GetFileNameFormat(File,Null,&Buf[0]);
	StringCoppy(&Buf[0],&FI->FileFormat[0]);
	FI->FileType=FT_ReadyRoom;//待分配的文件类型
	BufferCoppy(&NowTime,&FI->CreateDate,sizeof(Date_Time));
	FI->Page_Status=FT_ReadyRoom;//页即将被使用
	if(FileSize)FI->FileSize=FileSize;
	FI->FileID=rand();//文件ID其实就是随机数，但必须保证同一个文件的ID号相同
	FI->FileVirtue=FileVirtue;//可读可写隐藏等属性	
}
/*******************************************************************************
Func:在某某页创建文件空间
Date:2014-7-27
Note:Page为相对位置即认为文件存储区首页为第一页
*******************************************************************************/
uint32 TQFS_CreateFileRoom(TQFS_Open_File *Open_File,uint32 Page,File_Infor *FI)
{
	FI->FileID=Page;
	FI->FileIndex=0;
	DeBug("Create Object File In Page[%d]",Page,Infor_Infor);
	Write_FileInfor(Page,FI);
	TQFS_SetCursor(Open_File,Page,0);
	return 0;
}
/*******************************************************************************
Func:设置读写操作游标
Date:2014-7-27
Note:Page为相对位置即认为文件存储区首页为第一页
*******************************************************************************/
void TQFS_SetCursor(TQFS_Open_File *Open_File,uint32 Page,uint16 Offset)
{
	Open_File->PageCursor=Page;
	Open_File->OffsetCursor=Offset;
}
/*******************************************************************************
Func:读取页面内容
Date:2014-7-27
Note:返回状态
*******************************************************************************/
TQFS_Error TQFS_ReadPageContext(TQFS_Open_File *Open_File,uint8 *rBuffer,uint16 Length)
{
	File_Infor TempFile;
	int OviLenth;
	uint16 Offset=0;
  OviLenth=(Open_File->OffsetCursor+Length);
	OviLenth-=NAND_PAGE_SIZE;//写不下的长度
	if(OviLenth>=0)//当前页不够
	{
		Nand_ReadPage(Nand_Infor.FileStartPage+Open_File->PageCursor,&rBuffer[Offset],Open_File->OffsetCursor+NAND_SPARE_SIZE,Length-OviLenth);
		Offset=Length-OviLenth;
		Open_File->OffsetCursor=0;
		Read_FileInfor(Open_File->PageCursor,&TempFile);
		if(TempFile.NextPageAddr==0xffffffff)return TE_NoNextPage;
		else{
			Open_File->PageCursor=TempFile.NextPageAddr;
			Length=OviLenth;
		}			
	}
		Nand_ReadPage(Nand_Infor.FileStartPage+Open_File->PageCursor,&rBuffer[Offset],Open_File->OffsetCursor+NAND_SPARE_SIZE,Length);
		Open_File->OffsetCursor+=Length;
	if(OviLenth>=0)return TE_NewPage;
	else return TE_CurrentPage;
}
/*******************************************************************************
Func:写内容到指定页
Date:2014-7-27
Note:返回该页写完的剩余量
*******************************************************************************/
TQFS_Error TQFS_WritePageContext(TQFS_Open_File *Open_File,uint8 *Buffer,uint16 Length)
{
	File_Infor TempFile;
	int OviLenth;
	uint16 Offset=0;
  OviLenth=(Open_File->OffsetCursor+Length);
	OviLenth-=NAND_PAGE_SIZE;//写不下的长度
	if(OviLenth>=0)//当前页不够
	{
		Nand_WritePage(Nand_Infor.FileStartPage+Open_File->PageCursor,&Buffer[Offset],Open_File->OffsetCursor+NAND_SPARE_SIZE,Length-OviLenth);
		Offset=Length-OviLenth;
		Open_File->OffsetCursor=0;
		Read_FileInfor(Open_File->PageCursor,&TempFile);
		TempFile.NextPageAddr=TQFS_FindKeyPageMark(FT_NotUsed,FT_FileCenter);
		DeBug("TempFile.NextPageAddrTempFile.NextPageAddr%x",TempFile.NextPageAddr,Infor_Infor);
		if(TempFile.NextPageAddr==0xffffffff)
		{
			DeBug("Page after use All",Infor_Infor);
			return TE_NoNextPage;
		}
		TempFile.Page_Status=PS_UsedFully;//标记页被使用完
		Write_FileInfor(Open_File->PageCursor,&TempFile);//在当前页写入下页地址
		Open_File->PageCursor=TempFile.NextPageAddr;
	  TempFile.Page_Status=PS_NotFully;
		TempFile.PreviousPageAddr=Open_File->PageCursor;
		TempFile.NextPageAddr=0xffffffff;
		TempFile.FileIndex++;
		Write_FileInfor(Open_File->PageCursor,&TempFile);//在当前页写入下页地址
		Length=OviLenth;		
	}
		Nand_WritePage(Nand_Infor.FileStartPage+Open_File->PageCursor,&Buffer[Offset],Open_File->OffsetCursor+NAND_SPARE_SIZE,Length);
		Open_File->OffsetCursor+=Length;
	if(OviLenth>=0)return TE_NewPage;
	else return TE_CurrentPage;
}

