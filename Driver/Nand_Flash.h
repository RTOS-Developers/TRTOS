#ifndef _NAND_FLASH_H_
#define _NAND_FLASH_H_
#include <Include.h>


#define NAND_PAGE_SIZE 			2048		                //每页2048字节
#define NAND_SPARE_SIZE			64			                //每页64个备份字节
#define NAND_BLOCK_SIZE			64			                //每块有64个页
#define NAND_ZONE_SIZE			2048		                //一个设备2048块
#define NAND_ALLPAGE_SIZE   131072
#define NAND_ZONE_COUNT			1
#define NAND_BLOCK_TOTAL		(NAND_ZONE_COUNT*NAND_ZONE_SIZE)

typedef enum{
	Nand_PageOver=1,
	Nand_RefuseWrite=2,
	Nand_OtherError=3
}Nand_Error;

#define NAND_CMD 	(*((volatile uint8*) 0x70010000))	    //CMD->CLE A16
#define NAND_ADR 	(*((volatile uint8*) 0x70020000))	    //ADR->ALE A17							  
#define NAND_ADR16 	(*((volatile uint16*)0x70020000))	    //ADR->ALE A17							  
#define NAND_ADR32 	(*((volatile uint32*)0x70020000))	    //ADR->ALE A17							  
#define NAND_HDR 	(*((volatile uint8*) 0x78020000))	    //
#define NAND_DAT8 	(*((volatile uint8*) 0x70000000))	    //DAT8
#define NAND_DAT16	(*((volatile uint16*)0x70000000))
#define NAND_DAT32	(*((volatile uint32*)0x70000000))

extern uint8 DeviceId_NANDFLASH;

void  Nand_FSMC_Init(void);
uint8 	Nand_GetState(void);
uint32 	Nand_GetID(void);

void	Nand_WriteAddr(uint32 RowAddr,uint16 ColAddr);
void 	Nand_ReadPage (uint32 StartPage,uint8 *rBuffer,uint16 ByteOffset,uint16 Length);
void 	Nand_ReadPage_Random(uint16 ByteOffset,uint8 *rBuffer,uint16 Length);
void 	Nand_FillPage(uint32 StartPage,uint8 Data,uint16 ByteOffset,uint16 Length);
void 	Nand_WritePage(uint32 StartPage,uint8 *wBuffer,uint16 ByteOffset,uint16 Length);
void 	Nand_WritePage_WriteStart (uint32 StartPage);
void 	Nand_WritePage_WriteRandom(uint16 ByteOffset,uint8 *wBuffer,uint16 Length);
void 	Nand_WritePage_WriteStop  (void);
void 	Nand_CopyBack (uint32 StartPage,uint32 DestPage);
void 	Nand_CopyBack_WriteStart  (uint32 StartPage,uint32 DestPage);
void 	Nand_CopyBack_WriteRandom (uint16 ByteOffset,uint8 *wBuffer,uint16 Length);
void 	Nand_CopyBack_WriteStop   (void);
void 	Nand_EraseBlock(uint32 BlockAddr);	
uint16 Cheak_BadPage(uint32 Page);
uint16 Nand_FindPageKey(uint32 StartPage,uint8 Key);
uint16 Nand_PageKeyCount(uint32 StartPage,uint8 Key);


void DeviceMount_NandFlash(void);
#endif

