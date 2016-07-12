#include <Include.h>
#include <FM24C256_Driver.h>



void FM24C256_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE );
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//开漏输出
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
uint8 FM24C256_IOCONTROL(uint8 ConMode)
{
	uint8 R=0;
	switch(ConMode)
	{
		case I2C_HOST_SDA_SET:	FM24C256_SDA_SET  break;
		case I2C_HOST_SDA_CLR:	FM24C256_SDA_CLR	 break;
		case I2C_HOST_SCL_SET:	FM24C256_SCL_SET	 break;
		case I2C_HOST_SCL_CLR:	FM24C256_SCL_CLR  break;
		case I2C_HOST_SDA_DAT:	if(FM24C256_SDA_DAT)R=1;  break;		
	}
	return R;
}
extern BOOL PrintfVoid(char *fmt,...);
void FM24C256_Init()
{
	uint8 Buf[20];
	uint8 i;
	FM24C256_GPIO_Init();
	Tos_TaskDelay(100);
	for(i=0;i<20;i++)Buf[i]=i;
	FM24C256_Write(0x0000,&Buf[0],20);
	//Buf[0]=0;
	for(i=0;i<20;i++)
	{
	FM24C256_Read(i*0xff,&Buf[0],20);
	PrintfVoid("Buf[%d]=%d",i,Buf[i]);
	}
}
uint8 FM24C256_Write(uint16 Addr,uint8 *Buf,uint16 Length)
{
	uint8 Ack;
	//FM24C256_WRITE_EN;
	Ack=I2C_WriteDatas(FM24C256_IOCONTROL,0x50,(uint8 *)&Addr,2,Buf,Length);
	//FM24C256_WRITE_DIS;
	return Ack;
}
uint8 FM24C256_Read(uint16 Addr,uint8 *Buf,uint16 Length)
{
	uint8 Ack;
	Ack=I2C_ReadDatas(FM24C256_IOCONTROL,0x50,(uint8 *)&Addr,2,Buf,Length);
	return Ack;
}


