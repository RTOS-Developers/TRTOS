#include <..\USER\Prj_TP502T\Ht1621_Driver.h>

uint8 DeviceId_HT1621=DeviceNull;


/*******************************************************************************
Func:LCD驱动芯片操作延迟
Date:2016-3-11
Note:
*******************************************************************************/
void Ht1621_Delay(uint16 D)
{
	D*=3;
	while(D--);
}

/*******************************************************************************
Func:LCD IO初始化
Date:2016-3-11
Note:
*******************************************************************************/
const GPIO_InitStruct IO_LCD_List[]=
{
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,HT1621_CS|HT1621_WR|HT1621_DAT},//LCD端口初始化
	{0},
};
/*******************************************************************************
Func:LCD写入数据
Date:2016-3-11
Note:
*******************************************************************************/
void Ht1621Wr_Data(u8 Data, u8 cnt)
{
	u8 i;
	for (i = 0; i < cnt; i++)
	{
	 GPIO_ResetBits(GPIOC,HT1621_WR);  
	 Ht1621_Delay(3);
	 if (Data & 0x80)
	 {
			GPIO_SetBits(GPIOC,HT1621_DAT); 
	 }
	 else
	 {
			GPIO_ResetBits(GPIOC,HT1621_DAT);
	 }
	 Ht1621_Delay(3);
	 GPIO_SetBits(GPIOC,HT1621_WR); 
	 Data <<= 1;
	 Ht1621_Delay(1);
	}
}


/*******************************************************************************
Func:LCD写入命令
Date:2016-3-11
Note:
*******************************************************************************/
void Ht1621Wr_Cmd(u8 Cmd)   //写命令也可以连续写，这里一次只写一次
{
	GPIO_ResetBits(GPIOC,HT1621_CS);  //CS = 0;  先拉底，准备传数据
	Ht1621_Delay(3);
  Ht1621Wr_Data(0x80, 4); //写入命令标志 "100+0",命令格式="100+0+8位的命令字节"
  Ht1621Wr_Data(Cmd, 8);  //写入命令数据
	GPIO_SetBits(GPIOC,HT1621_CS);   //不写数据就再次拉高
	Ht1621_Delay(4);    //写数据结束
}
/*******************************************************************************
Func:LCD写入寄存器
Date:2016-3-11
Note:
*******************************************************************************/
void Ht1621_WriteReg(u8 Addr, u8 Data)   //可以控制四个段
{
	GPIO_ResetBits(GPIOC,HT1621_CS);  //CS = 0;  先拉底，准备传数据
	Ht1621_Delay(3);
	Ht1621Wr_Data(0xa0, 3);         //写入数据标志101
	Ht1621Wr_Data(Addr << 2, 6);    //写入地址数据  因为数据是八位的，只用
	Ht1621Wr_Data(Data << 4, 4);    //写入数据   这个道理如上，只用后4位。
	GPIO_SetBits(GPIOC,HT1621_CS);     //拉高，终止传数据 
}
/*******************************************************************************
Func:LCD 连续写寄存器
Date:2016-3-11
Note:
*******************************************************************************/
void Ht1621_WriteRegs(u8 SPaddr, u8* SpData, u8 Len)
{
	u8 i;
	if ((SPaddr + Len) > 31)return; 
	GPIO_ResetBits(GPIOC,HT1621_CS);  //CS = 0;  先拉底，准备传数据
	Ht1621_WriteReg(0xa0, 3);         //写入数据标志101
	Ht1621_WriteReg(SPaddr << 2, 6);  //写入地址数据
	for (i = 0; i < Len; i++)
	{
	Ht1621_WriteReg(SpData[i], 8);//写入数据//循环一次写2个数据
	} 
	GPIO_SetBits(GPIOC,HT1621_CS);  // //CS拉高，片选结束
}
/*******************************************************************************
Func:LCD 初始化
Date:2016-3-11
Note:
*******************************************************************************/
void HT1621_Init(void)    //初始化LCD   参数
{
	uint8 i=0;
	GPIO_InitList(IO_LCD_List);
	Ht1621Wr_Cmd(0x56);      //1/3duty 4com
	Ht1621Wr_Cmd(0x30);     //使用内部振荡器
	Ht1621Wr_Cmd(0x00);    //关振系统荡器和LCD偏压发生器
	Ht1621Wr_Cmd(0x0a);    //禁止看门狗
	Ht1621Wr_Cmd(0x03);     //打开系统振荡器
	Ht1621Wr_Cmd(0x06);     //打开LCD偏压
	for(i=0;i<32;i++)Ht1621_WriteReg(i, 0xff);//全亮
}
/*******************************************************************************
Func:显示设备挂载
Date:2016-3-11
Note:
*******************************************************************************/
void DeviceMount_HT1621(void)
{
	DeviceId_HT1621=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_HT1621].DeviceId=DeviceId_HT1621;
	Tos_Device_Tab[DeviceId_HT1621].DeviceName="HT1621";
	Tos_Device_Tab[DeviceId_HT1621].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_HT1621].DeviceVirtue=DV_Task;//初始化属于任务
	Tos_Device_Tab[DeviceId_HT1621].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_HT1621].Init=HT1621_Init;
	Tos_Device_Tab[DeviceId_HT1621].Note=Null;
	Tos_Device_Tab[DeviceId_HT1621].Write=Null;
	Tos_Device_Tab[DeviceId_HT1621].Read=Null;
	Tos_Device_Tab[DeviceId_HT1621].Exit=Null;
}

