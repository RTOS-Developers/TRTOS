#ifndef _TQFS_H_
#define _TQFS_H_
#include <Define.h>
#include <Nand_Flash.h>
#include <Tos_Timer.h>

enum Page_Status
{
	PS_NotUsed=0xff,//页未被使用
	PS_CanUse=0xfe, //页可以使用
	PS_BeUse=0xfc,  //页即将被使用
	PS_NotFully=0xf8,//页未使用完全
	PS_UsedFully=0xf0,//页被使用完
	PS_Delete=0xe0, //页删除
	PS_Dis=0xc0,    //页屏蔽区
	PS_Bad=0x00     //页损坏区
};
enum FileType{
	FT_NotUsed=0xff,//页未被使用
	FT_ReadyRoom=0x7f,//未定义长度的泛型文件存储区
	FT_File=0x1f,//文件类型的未定义长度存储区
	FT_FileHead=0x1e,//文件首
	FT_FileCenter=0x1c,//文件中
	FT_FileEnd=0x18,//文件尾部
	FT_Folde=0x2f,//文件夹类型的未定义长度存储区
	FT_FolderHead=0x2e,//文件夹头标志
	FT_FolderCenter=0x2c,//文件夹中间标志
	FT_FolderEnd=0x28,//文件夹尾标志
	FT_Link=0x3f,//链接类型的未定义长度存储区
	FT_LinkHead=0x3e,//链接文件头标志
	FT_LinkCenter=0x3c,//链接文件中间标志
	FT_LinkEnd=0x38,//链接文件尾标志
	FT_RootDir=0x4f,//路径文件标志
	FT_RootDirHead=0x4e,//根文件头标志
	FT_RootDirCenter=0x4c,//根文件中间标志
	FT_RootDirEnd=0x48,//根文件尾标志
	FT_Delete=0x08,//文件被删除标志
	FT_Tab=0x6f,//文件页被用来存放Tab表标志
	FT_Bad=0x00     //页损坏标志
};

typedef struct
{
	uint32 FT_NotUsed;
	uint32 FT_CanUse;
	uint16 FT_Bad;
	uint16 FT_FolderHead;
	uint16 FT_FileHead;
	uint16 FT_LinkHead;
	uint16 FT_RootDirHead;
	uint16 FT_Other; 
	uint16 TabInBlock;
	uint32 FileStartPage;
}Nand_UserInfor; 


typedef struct
{
	uint8  FileType;//该页内容类型
	uint8  Page_Status;//该区域为页最重要的位置
	uint8  PS_ST;//页状态暂时未使用
	uint8  FileName[14];//文件名长度为14个字节
	uint8  FileFormat[6];//文件格式
	uint8  FileVirtue;//文件属性
	Date_Time CreateDate;//文件创建时间
	Date_Time ReBuildDate;//文件修改时间
	uint32 PreviousPageAddr;//文件上一页的地址
	uint32 NextPageAddr;//文件下一页的地址
	uint32 FileSize;//该文件长度
	uint16 FileIndex;//文件索引该页为第几页
	uint16 DataLegnth;//当前页有效内容长度
	uint32 FileID;//文件ID等于文件头所在页的相对位置
	uint16 SpareCrc;//Spare区域校验值
	uint16 PageCrc;//Page内容区域校验值
}File_Infor;//页头结构放在Spare区

extern Nand_UserInfor Nand_Infor;
BOOL Read_FileInfor(uint32 Page,File_Infor *FI);
BOOL Write_FileInfor(uint32 Page,File_Infor *FI);
uint16 TQFS_FormatNand(void);
uint16 TQFS_FindTabAddr(void);
uint32 TQFS_FindFirstUsingPage(uint32 StartPage);
void Nand_InforCollect(Nand_UserInfor *NUI);
void TQFS_Init(void);
uint32 TQFS_FindKeyPageMark(uint8 Key,uint8 Mark);
uint8 TQFS_FindAllKey(uint8 Key,uint32 *AddrBuf,uint8 BufLength,uint16 Page);
uint32 TQFS_FindFristTypeKey(uint8 Key);
uint32 TQFS_FindFristTypeName(uint8 Key,uint8 *Name);
#endif
