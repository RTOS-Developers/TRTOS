#include <Tools.h>
/*******************************************************************************
Func:获取唯一标示
Date:2014-5-22
Note:
*******************************************************************************/
uint16 GetLockCode()//获取唯一编号
{
	uint16 CrcValue;
	CrcValue=Tools_GetCRC16((uint8 *)(0x1ffff7e8),12);  
	return CrcValue;
}
uint16 Get_CPUID(uint8 *Buf,uint8 Index)
{
	uint32 Temp=*(vu32*)(0x1ffff7e8+Index*4);
	return GetStringFromNumber(Buf,Temp,'H',0);
}
uint32 GetLockCode8(void)
{
	static u32 CpuID[3];
	static u32 Lock_Code;
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[1]=*(vu32*)(0x1ffff7ec);
	CpuID[2]=*(vu32*)(0x1ffff7f0);
	Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);
	return Lock_Code;
}
//uint8 Get_OnlyOneID(uint8 *Buf)
//{  
//	uint8 temp[12];
//	u32 temp0,temp1,temp2;
//	temp0=*(__IO u32*)(0x1FFFF7E8);    //产品唯一身份标识寄存器（96位）
//	temp1=*(__IO u32*)(0x1FFFF7EC);
//	temp2=*(__IO u32*)(0x1FFFF7F0);
//	temp[0] = (u8)(temp0 & 0x000000FF);
//	temp[1] = (u8)((temp0 & 0x0000FF00)>>8);
//	temp[2] = (u8)((temp0 & 0x00FF0000)>>16);
//	temp[3] = (u8)((temp0 & 0xFF000000)>>24);
//	temp[4] = (u8)(temp1 & 0x000000FF);
//	temp[5] = (u8)((temp1 & 0x0000FF00)>>8);
//	temp[6] = (u8)((temp1 & 0x00FF0000)>>16);
//	temp[7] = (u8)((temp1 & 0xFF000000)>>24);
//	temp[8] = (u8)(temp2 & 0x000000FF);
//	temp[9] = (u8)((temp2 & 0x0000FF00)>>8);
//	temp[10] = (u8)((temp2 & 0x00FF0000)>>16);
//	temp[11] = (u8)((temp2 & 0xFF000000)>>24);
//	FmtToBuf((char *)&Buf[0],"%2x",temp[1]);
//	return 12;
//}
uint8 GetCodeStr64(uint8 *Buf)
{
	uint32 CpuID[3];
	uint8 i,c=0;
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[2]=*(vu32*)(0x1ffff7ec);
	CpuID[1]=*(vu32*)(0x1ffff7f0);
	for(i=0;i<3;i++)
	{
		while(CpuID[i])
		{
			Buf[c++]=ToHex64(CpuID[i]%62);
			CpuID[i]/=62;
		}
	}
	Buf[c++]=0;
	return c;
}
uint16 Get_FlashSize()
{
	uint16 STM32_FLASH_SIZE;
	STM32_FLASH_SIZE= *(u16*)(0x1FFFF7E0); 
	return STM32_FLASH_SIZE;
}
/*******************************************************************************
Func:获取任务个数
Date:2014-5-22
Note:
*******************************************************************************/
uint8 GetTosTask_Count()
{
	uint8 i;
	uint8 Count=0;
	for(i=0;i<Tos_TaskTotal;i++)
	{
		if(Tos_TaskTcb[i].Task_ST!=_TTS_Deleted)Count++;
	}
	return Count;
}
/*******************************************************************************
Func:打印唯一ID
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_LockCode()
{
	uint8 Buf[20];
	GetCodeStr64(Buf);
	DeBug("OnlyID:%s",Buf,Infor_Infor);
}
/*******************************************************************************
Func:打印时间
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_DateTime(char *Str,Date_Time *DT)
{
	DeBug("%s%d/%d/%d %d:%d:%d",Str,DT->Year,DT->Mon,DT->Day,DT->Hour,DT->Min,DT->Sec,Infor_Infor);
}
/*******************************************************************************
Func:打印数组
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_Bytes(uint8 *Bytes,uint8 Length)
{
	uint8 i;
	for(i=0;i<Length;i++)DeBug("Buf[%d]=0x%x",i,Bytes[i],Infor_Infor);
}
/*******************************************************************************
Func:打印任务信息
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_TaskInfor()
{

	DeBug("TaskPoolSize:%d but has %d task create.",Tos_TaskTotal,GetTosTask_Count(),Infor_Infor);
}
/*******************************************************************************
Func:打印lcd屏幕信息
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_LcdType()
{
#ifdef LCD_XSIZE
	if(Device_Ok(DeviceId_LCD))
	{
	DeBug("LcdType:%dx%d",LCD_XSIZE,LCD_YSIZE,Infor_Infor);
	}else DeBug("Not Find LCD",Infor_Infor);
#endif
}
/*******************************************************************************
Func:打印系统信息
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_TosInfor()
{
	uint8 i;
	for(i=0;i<Tos_TaskTotal;i++)
	{
		if(Tos_TaskTcb[i].Task_Name!=Null)DeBug("Task[%d]Name:%s(%d)",i,Tos_TaskTcb[i].Task_Name,Tos_TaskTcb[i].TaskStackSize,Infor_Infor);
	}
	i--;
	DeBug("MemorySize:%d but user:%d",MEMA_BUFFER_SIZE,Mema_Count(),Infor_Infor);
}
/*******************************************************************************
Func:当挂在nandflash 打印信息
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_NandInfor()
{
	#if defined(Nand_Infor)
	if(Device_Ok(DeviceId_NANDFLASH))
	{
	DeBug("Find NandFlash:",Infor_Infor);
	DeBug("ID:%x\n",Nand_GetID(),Infor_Infor);
	DeBug("TabIn[%d]Block",Nand_Infor.TabInBlock,Infor_Infor);
	DeBug("FileStartIn[%d]Page",Nand_Infor.FileStartPage,Infor_Infor);
	DeBug("TQFS_CanUse:%dPages",Nand_Infor.FT_CanUse,Infor_Infor);
	DeBug("FT_Bad:%dPages",Nand_Infor.FT_Bad,Infor_Infor);
	DeBug("FileCount:%d",Nand_Infor.FT_FileHead,Infor_Infor);
	DeBug("FolderCount:%dPages",Nand_Infor.FT_FolderHead,Infor_Infor);
	DeBug("LinkCount:%dPages",Nand_Infor.FT_LinkHead,Infor_Infor);
	DeBug("FT_Other%dPages",Nand_Infor.FT_Other,Infor_Infor);
	}else DeBug("Not Find NandFlash",Infor_Infor);
	#endif
}
/*******************************************************************************
Func:打印板子信息
Date:2014-5-22
Note:
*******************************************************************************/
void Printf_BoardInfor()
{
	DeBug("Board_Name:%s",Board_Name,Infor_Infor);
	DeBug("CPU_Model:%s",CPU_Model,Infor_Infor);
	DeBug("Cpu_OSC_FREQ: %dMhz",OSC_FREQ,Infor_Infor);
	DeBug("Cpu_Freq: %dMhz",Tos_Cpu_Freq,Infor_Infor);
}
/******************************************************************************
 Func:打印系统信息 
 Time: 2014-7-27
 Ver.: V1.0
 Note:
******************************************************************************/
void Printf_SysInfor()
{
	DeBug("\nTos System Start..........",Infor_Infor);
	Printf_BoardInfor();
	Printf_LcdType();
	Printf_LockCode();
	DeviceList_Printf();
	Printf_TaskInfor();
	Printf_NandInfor();
	Printf_TosInfor();
}

