#include <Include.h>
#include <SPI_Driver.h>

	
void SPI_Delay(uint16 i)
{
	while(i--);
}
uint8 SPI_WR(const SPI_PublicStruct *SPI,uint8 Data)
{
	uint8 i,Temp=0x00;
	for(i=0;i<8;i++)
	{
		Temp<<=1;
		if(Data&0x80)SPI->MOSI(True);else SPI->MOSI(False);
		if(SPI->MISO())Temp|=0x01;
		Data<<=1;
		SPI_Delay(SPI->Delay);
		SPI->SCLK(True);
		SPI_Delay(SPI->Delay);
		SPI->SCLK(False);
	  SPI_Delay(SPI->Delay);
	}
	return Temp;
}
uint8 SPI_WriteReg(const SPI_PublicStruct *SPI,uint8 reg,uint8 dat)
{
	uint8 status;
	SPI->CE(False);
	SPI->CSN(False);
	status = SPI_WR(SPI,reg);
	SPI_WR(SPI,dat);        
	SPI->CSN(True);	
	return(status);
}
uint8 SPI_ReadReg(const SPI_PublicStruct *SPI,uint8 reg,uint8 dat)
{
	uint8 reg_val;
	SPI->CE(False);
	SPI->CSN(False);
	SPI_WR(SPI,reg); 
	reg_val =SPI_WR(SPI,0xff);
	SPI->CSN(True);		
	return reg_val;
}
uint8 SPI_ReadBuf(const SPI_PublicStruct *SPI,uint8 reg,uint8 *pBuf,uint8 LCEgth)
{
 	uint8 status, byte_cnt;
	SPI->CE(False);
	SPI->CSN(False);
	status =SPI_WR(SPI,reg); 
	for(byte_cnt=0;byte_cnt<LCEgth;byte_cnt++)		  
	pBuf[byte_cnt] = SPI_WR(SPI,0xff); //从NRF24L01读取数据  
	SPI->CSN(True);			
 	return status;		//返回寄存器状态值
}
uint8 SPI_WriteBuf(const SPI_PublicStruct *SPI,uint8 reg ,uint8 *pBuf,uint8 LCEgth)
{
	uint8 status,byte_cnt;
	SPI->CE(False);
	SPI->CSN(False);
	status =SPI_WR(SPI,reg); 
	for(byte_cnt=0;byte_cnt<LCEgth;byte_cnt++)
	SPI_WR(SPI,*pBuf++);	//写数据到缓冲区 	 
	SPI->CSN(True);				
	return (status);	//返回NRF24L01的状态 		
}
