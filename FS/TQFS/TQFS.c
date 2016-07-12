#include <Include.h>
#include <Tools_Include.h>
#include <TQFS.h>

/*
注意：TQFS除了文件信息表外，普通数据存储区域以页为单位，跟据文件信息
		文件信息块每字节对应一个页描述该页的状态信息
		TQFS里的文件结构没有规定目录结构，可以使用在没有目录结构的情况下，也可以
		自行构建目录结构，没有目录结构的情况下，目录及目录文件，超链接等都是以文件的形式存在，
		需要构建目录里成员，需要在目录文件的内容区域里创建文件或目录。


*/
Nand_UserInfor Nand_Infor;

/******************************************************************************
 Func: 读取文件信息从页的Spare区域
 Time: 2014-7-23
 Ver.: V1.0
 Note:Page为相对位置
******************************************************************************/
BOOL Read_FileInfor(uint32 Page,File_Infor *FI)
{
	Page+=Nand_Infor.FileStartPage;
	Nand_ReadPage(Page,(uint8 *)FI,0,sizeof(File_Infor));
  return True;
}

/******************************************************************************
 Func: 写入文件信息到页的Spare区域
 Time: 2014-7-23
 Ver.: V1.0
 Note:Page为相对位置
******************************************************************************/
BOOL Write_FileInfor(uint32 Page,File_Infor *FI)
{
	Page+=Nand_Infor.FileStartPage;
	Nand_WritePage(Page,(uint8 *)FI,0,sizeof(File_Infor));
	return True;
}

/******************************************************************************
 Func: 写页的映射标记到TAB
 Time: 2014-7-23
 Ver.: V1.0
 Note:
******************************************************************************/
void TQFS_MarkPageInTab(uint32 Block,uint8 *Mark)
{
	uint32 TabPage,TabOffset;
	if(NAND_ZONE_SIZE>Nand_Infor.TabInBlock)
	{
		Block-=(Nand_Infor.TabInBlock+1);//偏移内容块到相对位置0
		Block*=NAND_BLOCK_SIZE;
		TabPage=Block/NAND_PAGE_SIZE;
		TabOffset=Block%NAND_PAGE_SIZE;
		Nand_WritePage(TabPage,&Mark[0],TabOffset+NAND_SPARE_SIZE,NAND_BLOCK_SIZE);
	}
}
/******************************************************************************
 Func: 寻找第一个空白内容页并标记Tab标记位
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回坏页个数并标记在页Spare的第一个字节
******************************************************************************/
uint32 TQFS_FindKeyPageMark(uint8 Key,uint8 Mark)
{
	uint32 m,n,j;
	uint8 Buf[NAND_BLOCK_SIZE];
	for(n=0;n<NAND_BLOCK_SIZE;n++)//页数
	{
		for(m=0;m<(NAND_PAGE_SIZE/NAND_BLOCK_SIZE);m++)//页中有多少个64字节
		{
			Nand_ReadPage(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+n,&Buf[0],NAND_SPARE_SIZE+m*NAND_BLOCK_SIZE,NAND_BLOCK_SIZE);
			j=SerchKeyFromBytes(&Buf[0],Key,NAND_BLOCK_SIZE);
			if(j!=0xffff)
			{
				Buf[j]=Mark;
				Nand_WritePage(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+n,&Buf[0],NAND_SPARE_SIZE+m*NAND_BLOCK_SIZE,NAND_BLOCK_SIZE);
				return (n*NAND_PAGE_SIZE+m*NAND_BLOCK_SIZE+j);
			}
		}
	}	
	return 0xffffffff;
}
/******************************************************************************
 Func: 在TAB位置选着关键字并且记录关键字对应内容页的页位置
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回符合记录条数
******************************************************************************/
uint8 _TQFS_FindAllKey(uint8 Key,uint32 *AddrBuf,uint8 BufLength)
{
	uint32 m,n,j;
	uint8 i=0;
	uint8 Buf[NAND_BLOCK_SIZE];
	for(n=0;n<NAND_BLOCK_SIZE;n++)//页数
	{
		for(m=0;m<(NAND_PAGE_SIZE/NAND_BLOCK_SIZE);m++)//页中有多少个64字节
		{
			Nand_ReadPage(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+n,&Buf[0],NAND_SPARE_SIZE+m*NAND_BLOCK_SIZE,NAND_BLOCK_SIZE);
			for(j=0;j<NAND_BLOCK_SIZE;j++)if(Buf[j]==Key)AddrBuf[i++]=(n*NAND_PAGE_SIZE+m*NAND_BLOCK_SIZE+j);
			if(i>=BufLength)return i;
		}
	}	
	return i;
}
/******************************************************************************
 Func: 在TAB位置选着关键字并且记录关键字存到AddrBuf里，并以分页形式
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回符合记录条数
******************************************************************************/
uint8 TQFS_FindAllKey(uint8 Key,uint32 *AddrBuf,uint8 BufLength,uint16 Page)
{

	uint32 m,n;
	uint8 i=0;
	uint8 Count=0;
	uint16 Start,End;
	Start=Page*BufLength;
	End=BufLength*(Page+1);
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	for(m=0;m<NAND_BLOCK_SIZE;m++)
	{
		NAND_CMD=0x00;
		Nand_WriteAddr(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+m,NAND_SPARE_SIZE);
		NAND_CMD=0x30;
		Tos_TaskDelay(1);
		for(n=0;n<NAND_PAGE_SIZE;n++)
		{
			if(NAND_DAT8==Key)
			{
				if(i>=Start&&i<End)if(AddrBuf)AddrBuf[Count++]=(m*NAND_PAGE_SIZE+n);
				if(i>=End)break;
				if(Count>=BufLength)break;
				i++;
			}
		}
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	return Count;
}
/******************************************************************************
 Func: 寻找TAB中首个符合类型的页的偏移位置
 Time: 2014-7-23
 Ver.: V1.0
 Note:
******************************************************************************/
uint32 TQFS_FindFristTypeKey(uint8 Key)
{

	uint32 m,n;
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	for(m=0;m<NAND_BLOCK_SIZE;m++)
	{
		NAND_CMD=0x00;
		Nand_WriteAddr(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+m,NAND_SPARE_SIZE);
		NAND_CMD=0x30;
		Tos_TaskDelay(1);
		for(n=0;n<NAND_PAGE_SIZE;n++)
		{
			if(NAND_DAT8==Key)
			{
				Tos_TaskDropDev(DeviceId_NANDFLASH);
				return (m*NAND_PAGE_SIZE+n);
			}
		}
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	return 0xffffffff;
}
uint32 TQFS_FindFristTypeName(uint8 Key,uint8 *Name)
{

	uint32 m,n;
//	File_Infor FI;
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	for(m=0;m<NAND_BLOCK_SIZE;m++)
	{
		NAND_CMD=0x00;
		Nand_WriteAddr(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+m,NAND_SPARE_SIZE);
		NAND_CMD=0x30;
		Tos_TaskDelay(1);
		for(n=0;n<NAND_PAGE_SIZE;n++)
		{
			if(NAND_DAT8==Key)
			{
// 				Read_FileInfor(m*NAND_PAGE_SIZE+n,&FI);
// 				if(CompareWithString(FI.FileName,Name))
// 				{
// 					DeBug("FileName=%s",FI.FileName,Infor_Infor);
// 				Tos_TaskDropDev(DeviceId_TQFS);
				return (m*NAND_PAGE_SIZE+n);
// 				}
// 				NAND_CMD=0x00;
// 				Nand_WriteAddr(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+m,NAND_SPARE_SIZE+n);
// 				NAND_CMD=0x30;
// 				Tos_TaskDelay(1);
			}
		}
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	return 0xffffffff;
}
/******************************************************************************
 Func: 格式化NandFlash 
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回坏页个数并标记在页Spare的第一个字节
******************************************************************************/
uint16 TQFS_FormatNand()
{
	uint32 m,n;
	uint8  Buf[NAND_BLOCK_SIZE];
	uint16 Bad_Count=0;//总坏的页
	uint16 BlockBad_Count;//块坏的页
	
	if(!Tos_Device_Tab[DeviceId_NANDFLASH].Init)
	{
		DeBug("You Must Be Run NandInit",Infor_Error);
		return 0xffff;
	}
	TQFS_Init();
	Nand_Infor.TabInBlock=0xffff;
	for(m=0;m<NAND_ZONE_SIZE;m++)
	{
		BlockBad_Count=0;
		Nand_EraseBlock(m);
		for(n=0;n<NAND_BLOCK_SIZE;n++)
		{
			if(Cheak_BadPage(NAND_BLOCK_SIZE*m+n)==0xffff)Buf[n]=FT_NotUsed;
			else {Buf[n]=FT_Bad;BlockBad_Count++;Bad_Count++;}
		}
		TQFS_MarkPageInTab(m,&Buf[0]);
		if(Nand_Infor.TabInBlock==0xffff&&!BlockBad_Count)Nand_Infor.TabInBlock=m;//记录首个能用的块当作系统页映射表
		DeBug("Block[%d]Find[%d]BadPage",m,BlockBad_Count,Infor_Infor);
	}
	DeBug("DeviceFind[%d]BadPage",m,Bad_Count,Infor_Infor);
	return Bad_Count;
}
/******************************************************************************
 Func: 在块中寻找Tab的位置
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回Tab所在块的偏移位置
******************************************************************************/
uint16 TQFS_FindTabAddr()
{
	uint8 Buf[2];
	uint32 i;
	for(i=0;i<NAND_ZONE_SIZE;i++)
	{
		Nand_ReadPage(i*NAND_BLOCK_SIZE,&Buf[0],0,2);
		if(Buf[0]==0xff)return i;
	}
	return 0xffff;
}
/******************************************************************************
 Func: NandFlash信息收集
 Time: 2014-7-23
 Ver.: V1.0
 Note:Nand的使用情况
******************************************************************************/
void Nand_InforCollect(Nand_UserInfor *NUI)
{

	uint32 m,n;
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	for(m=0;m<NAND_BLOCK_SIZE;m++)
	{
		NAND_CMD=0x00;
		Nand_WriteAddr(Nand_Infor.TabInBlock*NAND_BLOCK_SIZE+m,NAND_SPARE_SIZE);
		NAND_CMD=0x30;
		Tos_TaskDelay(1);
		for(n=0;n<NAND_PAGE_SIZE;n++)
		{
			switch(NAND_DAT8)
			{
				case FT_NotUsed:NUI->FT_NotUsed++;break;
				case FT_Bad:NUI->FT_Bad++;break;
				case FT_FolderHead:NUI->FT_FolderHead++;break;
				case FT_FileHead:NUI->FT_FileHead++;break;
				case FT_LinkHead:NUI->FT_LinkHead++;break;
				case FT_RootDirHead:NUI->FT_RootDirHead++;break;
				default :NUI->FT_Other++;break;
			}
		}
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
}
/******************************************************************************
 Func: TQFS初始化
 Time: 2014七夕
 Ver.: V1.0
 Note:
******************************************************************************/
void TQFS_Init()
{
 	Nand_Infor.TabInBlock=TQFS_FindTabAddr();
 	Nand_Infor.FileStartPage=Nand_Infor.TabInBlock+1;//加上Tab表占用的一个块
 	Nand_Infor.FileStartPage*=NAND_BLOCK_SIZE;//文件起始页
 	Nand_InforCollect(&Nand_Infor);
}
