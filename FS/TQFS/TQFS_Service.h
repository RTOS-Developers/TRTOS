#ifndef _TQFS_SERVICE_H_
#define _TQFS_SERVICE_H_
#include <Include.h>

uint32 TQFS_SearchFileByNameAndType(void *Name,uint8 Type);
uint32 TQFS_SearchFile(void *Name);
void TQFS_SearchEndPage(TQFS_Open_File *OpenFile);
void TQFS_SearchEndByte(TQFS_Open_File *OpenFile);
#endif
