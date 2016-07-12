#ifndef _TQFS_FILE_H_
#define _TQFS_FILE_H_
#include <Include.h>
typedef struct{
	uint8  Path[16];
	uint32 FileID;//操作文件ID
	uint32 PageCursor;//操作页游标
	uint16 OffsetCursor;//操作页内游标
}TQFS_Open_File;

typedef enum {
	TE_NoNextPage=0x01,
	TE_NewPage=0x02,
	TE_CurrentPage=0x03,
	TE_NotFind=0x04,
	TE_Find=0x05,
}TQFS_Error;
typedef struct{
	uint8 FileType;
	uint32 File_Addr;
}TQFS_File;

uint8 TQFS_FileDelete(uint8 *Path);
void TQFS_FileRoomInfor(File_Infor *FI,void *File,uint32 FileSize,uint8 FileVirtue);
uint32 TQFS_CreateFileRoom(TQFS_Open_File *Open_File,uint32 Page,File_Infor *FI);
uint32 TQFS_WriteFile(uint32 PageOffSet,uint8 *Buffer,uint16 Length);
uint8 TQFS_FileCreate(uint8 *Path,uint8 State);
void TQFS_SetCursor(TQFS_Open_File *Open_File,uint32 Page,uint16 Offset);
TQFS_Error TQFS_ReadPageContext(TQFS_Open_File *Open_File,uint8 *rBuffer,uint16 Length);
TQFS_Error TQFS_WritePageContext(TQFS_Open_File *Open_File,uint8 *rBuffer,uint16 Length);

#endif

