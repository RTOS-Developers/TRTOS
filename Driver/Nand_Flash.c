#include <include.h>
#include <STM32LIB_Include.h>
#include <Tools_Include.h>
#include <Nand_Flash.h>

uint8 DeviceId_NANDFLASH=DeviceNull;
/**
  * @brief  Nandfalsh挂载在FSMC_Bank2_NAND上
  * @param  None
  * @retval None
  */
void Nand_FSMC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  FSMC_NAND_PCCARDTimingInitTypeDef  p;
  FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
  
  /*FSMC总线使用的GPIO组时钟使能*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | 
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
  
/*FSMC CLE, ALE, D0->D3, NOE, NWE and NCE2初始化，推挽复用输出*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 |  
                                 GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                 GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  /*FSMC数据线FSMC_D[4:7]初始化，推挽复用输出*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
  /*FSMC NWAIT初始化，输入上拉*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  /*FSMC INT2初始化，输入上拉*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  /*--------------FSMC 总线 存储器参数配置------------------------------*/
  p.FSMC_SetupTime = 0x1;         //建立时间
  p.FSMC_WaitSetupTime = 0x3;     //等待时间
  p.FSMC_HoldSetupTime = 0x2;     //保持时间
  p.FSMC_HiZSetupTime = 0x1;      //高阻建立时间
  FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND; //使用FSMC BANK2
  FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable; //使能FSMC的等待功能
  FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b; //NAND Flash的数据宽度为8位
  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;                  //使能ECC特性
  FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes; //ECC页大小2048
  FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;             
  FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;
  FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;
  FSMC_NANDInit(&FSMC_NANDInitStructure);
  /*!使能FSMC BANK2 */
  FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}
void Nand_Extremely(Nand_Error NE)
{
	if(NE==Nand_PageOver)
	{
		DeBug("NandPageOverFlow",Infor_Error);
	}else
	if(NE==Nand_RefuseWrite)
	{
		DeBug("Nand_RefuseWrite",Infor_Error);
	}
	else DeBug("Nand_DefaultError",Infor_Error);
}
/******************************************************************************
 Func: NAND_FLASH读取状态
 Time: 2014-7-10
 Note:
******************************************************************************/
uint8 Nand_GetState(void)
{
	NAND_CMD=0x70;
	return NAND_DAT8;	
}

void Nand_LongWaitBusy()
{
	NAND_CMD=0x70;
	while(NAND_DAT8==0x81)Tos_TaskDelay(1);	//不忙0xc0
}
/******************************************************************************
 Func: 等待设备不忙
 Time: 2014-7-10
 Note:
******************************************************************************/
void Nand_WaitBusy()
{
	while(!GPIO_ReadInputDataBit(GPIOG,6));
}
/******************************************************************************
 Func: NAND_FLASH写地址
 Time: 2011-4-20
 Ver.: V1.0
 Note: 本方法将从LBA地址映射到页/块地址
 	   由Area指定映射到Data区还是Spare区
******************************************************************************/

void Nand_WriteAddr(uint32 RowAddr,uint16 ColAddr)
{
	if(RowAddr<NAND_ALLPAGE_SIZE)
	{
	NAND_ADR16=ColAddr;
	NAND_ADR=(uint8)RowAddr;
	NAND_ADR16=(RowAddr>>8);
	}else Nand_Extremely(Nand_PageOver);	
}


/******************************************************************************
 Func: NAND_FLASH读取ID
 Time: 2011-4-20
 Ver.: V1.0
 Note: 
******************************************************************************/
uint32 Nand_GetID(void)
{		
	NAND_CMD=0x90;
	NAND_ADR=0x00;	
	return NAND_DAT32;
}


/******************************************************************************
 Func: NAND_FLASH读物理页数据
 Time: 2011-4-20
 Ver.: V1.0
 Note: 一页包含数据部分和Spare部分
 入口参数:  StartPage --- 开始读取的页地址
            *rBuffer ---- 数据写入缓冲区指针
            ByteOffset -- 页内偏移字节
            Length ------ 要读取的字节数量
******************************************************************************/
void Nand_ReadPage(uint32 StartPage,uint8 *rBuffer,uint16 ByteOffset,uint16 Length)
{
	if(StartPage<NAND_ALLPAGE_SIZE)
	{
	//uint16 *DP=(uint16 *)rBuffer;
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x00;
	Nand_WriteAddr(StartPage,ByteOffset);
	NAND_CMD=0x30;
	Tos_TaskDelay(1);
	while(Length--)*rBuffer++=NAND_DAT8;
//  	Length>>=1;                 
// 	do{
// 		*DP++=NAND_DAT16;
// 	}while(--Length);	
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	}else Nand_Extremely(Nand_PageOver);
}
/******************************************************************************
 Func: 在页内的内容区域搜索关键字返回相对位置
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回数据在该页内的偏移位置
******************************************************************************/
uint16 Nand_FindPageKey(uint32 StartPage,uint8 Key)
{
	uint16 i;
	if(StartPage<NAND_ALLPAGE_SIZE)
	{
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x00;
	Nand_WriteAddr(StartPage,NAND_SPARE_SIZE);
	NAND_CMD=0x30;
	Tos_TaskDelay(1);
 	for(i=0;i<NAND_PAGE_SIZE;i++)
	{
		if(NAND_DAT8==Key){Tos_TaskDropDev(DeviceId_NANDFLASH);return i;}
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	}else Nand_Extremely(Nand_PageOver);
	return 0xffff;
}
/******************************************************************************
 Func: 页关键字统计
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回数据在该页内的偏移位置
******************************************************************************/
uint16 Nand_PageKeyCount(uint32 StartPage,uint8 Key)
{
	uint16 i;
	uint16 Count=0;
	if(StartPage<NAND_ALLPAGE_SIZE)
	{
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x00;
	Nand_WriteAddr(StartPage,NAND_SPARE_SIZE);
	NAND_CMD=0x30;
	Tos_TaskDelay(1);
 	for(i=0;i<NAND_PAGE_SIZE;i++)
	{
		if(NAND_DAT8==Key)Count++;
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	}else Nand_Extremely(Nand_PageOver);
	return Count;
}

/******************************************************************************
 Func: 检查坏页
 Time: 2014-7-23
 Ver.: V1.0
 Note:返回数据在该页内的偏移位置
******************************************************************************/
uint16 Cheak_BadPage(uint32 Page)
{
	uint16 i;
	uint32 Temp;
	uint32 Length=NAND_PAGE_SIZE+NAND_SPARE_SIZE;
	if(Page<NAND_ALLPAGE_SIZE)
	{
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x00;
	Nand_WriteAddr(Page,0);
	NAND_CMD=0x30;
	Tos_TaskDelay(1);
 	Length>>=2;   
  for(i=0;i<Length;i++)
	{
		Temp=NAND_DAT32;
		if(Temp!=0xffffffff){Tos_TaskDropDev(DeviceId_NANDFLASH);return i;}
	}
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	}else Nand_Extremely(Nand_PageOver);
  return 0xffff;
}



/*---------------------------------------------------------------------------------------------------------
 Func: NAND_FLASH填充物理页数据
 Time: 2011-6-10
 Ver.: V1.0
 Note: 仅能将1写为0,不能将0写为1
 
 入口参数:  StartPage --- 开始读取的页地址
            Data---- ---- 待填充的数据
            ByteOffset -- 页内偏移字节
            Length ------ 要填充的字节数量 (4---n*4)
---------------------------------------------------------------------------------------------------------*/
void Nand_FillPage(uint32 StartPage,uint8 Data,uint16 ByteOffset,uint16 Length)
{
	uint32 T;
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	T=Data;T<<=8;T|=Data;T<<=8;T|=Data;T<<=8;T|=Data;   //  合并成32Bit数据
	NAND_CMD=0x80;
	Nand_WriteAddr(StartPage,ByteOffset);
	Length>>=2;
	do{NAND_DAT32=T;}while(--Length);
	NAND_CMD=0x10;
	Tos_TaskDropDev(DeviceId_NANDFLASH);
}


/*---------------------------------------------------------------------------------------------------------
 Func: NAND_FLASH写物理页数据
 Time: 2011-4-20
 Ver.: V1.0
 Note: 仅能将1写为0,不能将0写为1
  入口参数:  StartPage -- 开始读取的页地址
            *wBuffer----- 待填充的数据缓冲区指针
            ByteOffset -- 页内偏移字节
            Length ------ 要填充的字节数量 (1---n)            
---------------------------------------------------------------------------------------------------------*/
void Nand_WritePage(uint32 StartPage,uint8 *wBuffer,uint16 ByteOffset,uint16 Length)
{
	if(StartPage<NAND_ALLPAGE_SIZE)
	{
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x80;
	Nand_WriteAddr(StartPage,ByteOffset);
	while(Length--)NAND_DAT8=*wBuffer++;
	NAND_CMD=0x10;
	Nand_WaitBusy();
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	}else Nand_Extremely(Nand_PageOver);
}



/*---------------------------------------------------------------------------------------------------------
 Func: NAND_FLASH内部页拷贝
 Time: 2011-4-20
 Ver.: V1.0
 Note: 
    把StartPage页中的数据复制到DestPage页中
---------------------------------------------------------------------------------------------------------*/
void Nand_CopyBack(uint32 StartPage,uint32 DestPage)
{
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x00;
	Nand_WriteAddr(StartPage,0);
	NAND_CMD=0x35;
	NAND_CMD=0x85;
	Nand_WriteAddr(DestPage,0);
	NAND_CMD=0x10;
	Tos_TaskDropDev(DeviceId_NANDFLASH);
}


/*---------------------------------------------------------------------------------------------------------
 Func: NAND_FLASH内部页拷贝随机写
 Time: 2011-4-20
 Ver.: V1.0
 Note: 
        
---------------------------------------------------------------------------------------------------------*/
void Nand_CopyBack_WriteStart(uint32 StartPage,uint32 DestPage)
{		
	NAND_CMD=0x00;
	Nand_WriteAddr(StartPage,0);
	NAND_CMD=0x35;
	NAND_CMD=0x85;
	Nand_WriteAddr(DestPage,0);
}



/*---------------------------------------------------------------------------------------------------------
 Func: NAND_FLASH块擦除
 Time: 2011-4-20
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------*/
void Nand_EraseBlock(uint32 BlockAddr)
{
	BlockAddr*=NAND_BLOCK_SIZE;
	if(BlockAddr<NAND_ALLPAGE_SIZE)
	{
	Tos_TaskGetDev(DeviceId_NANDFLASH,0);
	NAND_CMD=0x60;
	NAND_ADR=(uint8)BlockAddr;
	NAND_ADR=(uint8)(BlockAddr>>8);
	NAND_ADR=(uint8)(BlockAddr>>16);	
	NAND_CMD=0xD0;	
	Nand_LongWaitBusy();
	Tos_TaskDropDev(DeviceId_NANDFLASH);
	}else Nand_Extremely(Nand_PageOver);
}

void Nand_Erase_Chip(void)
 {
	 uint32  i;
	 for(i=0;i<NAND_ZONE_SIZE;i++)
	 {
			 Nand_EraseBlock(i);
	 }
 }
/******************************************************************************
 Func: TQFS挂载
 Time: 2014七夕
 Ver.: V1.0
 Note:
******************************************************************************/
void DeviceMount_NandFlash()
{
	DeviceId_NANDFLASH=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_NANDFLASH].DeviceId=DeviceId_NANDFLASH;
	Tos_Device_Tab[DeviceId_NANDFLASH].DeviceName="TQFS";
	Tos_Device_Tab[DeviceId_NANDFLASH].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_NANDFLASH].DeviceVirtue=ReadOnly|WriteOnly|CharDerice;
	Tos_Device_Tab[DeviceId_NANDFLASH].DeviceState=Ready;
	Tos_Device_Tab[DeviceId_NANDFLASH].Init=Nand_FSMC_Init;
	Tos_Device_Tab[DeviceId_NANDFLASH].Write=Null;
	Tos_Device_Tab[DeviceId_NANDFLASH].Read=Null;
	Tos_Device_Tab[DeviceId_NANDFLASH].Exit=Null;
}
