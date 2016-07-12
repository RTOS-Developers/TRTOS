#include <Include.h>
#include <AT24C08_Driver.h>

void AT24C08_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推完输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void AT24C08_GPIO_CFG(uint8 ENOUT)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	if(ENOUT)GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//开漏输出
	else GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
uint8 AT24C08_IOCONTROL(uint8 ConMode)
{
	uint8 R=0;
	switch(ConMode)
	{
		case I2C_HOST_SDA_SET:	AT24C08_SDA_SET;  break;
		case I2C_HOST_SDA_CLR:	AT24C08_SDA_CLR;	 break;
		case I2C_HOST_SCL_SET:	AT24C08_SCL_SET;	 break;
		case I2C_HOST_SCL_CLR:	AT24C08_SCL_CLR;  break;
		case I2C_HOST_SDA_DAT:if(AT24C08_SDA_DAT)R=1;break;		
	}
	return R;
}
void AT24C08_Init()
{
	AT24C08_GPIO_Init();
}
uint8 AT24C08_Write(uint16 Addr,uint8 *Buf,uint16 Length)
{
	uint8 Ack;
	uint8 CmdAddr[2],CmdLen,Offset,CurLen;
	AT24C08_WRITE_EN;
	while(Length>0)
	{
		Offset=Addr%AT24CXX_PAGESIZE;											//求取页内偏移地址
		if(Offset>0)
		{
			CurLen=AT24CXX_PAGESIZE-Offset;										//强制页对齐
			if(CurLen>Length)CurLen=Length;
		}else		CurLen=Length>AT24CXX_PAGESIZE?AT24CXX_PAGESIZE:Length;		//页面字节统计
		if(AT24CXX_MEMSIZE<2048){CmdAddr[0]=(uint8)Addr;CmdLen=1;}
		else {CmdAddr[0]=Addr>>8;CmdAddr[1]=(uint8)Addr;CmdLen=2;}
		Ack=I2C_WriteDatas(AT24C08_IOCONTROL,AT24CXX_ADDR,CmdAddr,CmdLen,Buf,CurLen);				//页写操作
		Addr+=CurLen;Length-=CurLen;Buf+=CurLen;								//更新地址		
		Tos_TaskDelay(20);	
	}	
	return Ack;		
}
uint8 AT24C08_Read(uint16 Addr,uint8 *Buf,uint16 Length)
{
	uint8 Ack;
	uint8 CmdAddr[2],CmdLen;
	if(AT24CXX_MEMSIZE<2048){CmdAddr[0]=(uint8)Addr;CmdLen=1;}
	else {CmdAddr[0]=Addr>>8;CmdAddr[1]=(uint8)Addr;CmdLen=2;}
	Ack=I2C_ReadDatas(AT24C08_IOCONTROL,AT24CXX_ADDR,CmdAddr,CmdLen,Buf,Length);
	return Ack;
}


