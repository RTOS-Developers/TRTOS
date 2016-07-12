#include <Include.h>
#include <I2C1_Driver.h>
uint8 DeviceId_EEPROM=DeviceNull;

void I2C1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	I2C_InitTypeDef I2C_InitStructure;

	/* Configure I2C1 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//设置 I2C为 I2C模式
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//I2C快速模式 Tlow / Thigh = 2

	I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;//设置第一个设备地址

	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//使能应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//应答 7位地址
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;//设置时钟频率

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);//使能I2C外设
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStructure);
}

void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	I2C_GenerateSTART(I2C1, ENABLE);//产生起始位
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); //清除 EV5
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);//发送器件地址
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
	//ADDR=1，清除 EV6 
	I2C_SendData(I2C1, WriteAddr);    //EEPROM 的具体存储地址位置
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//移位寄存器非空，数据寄存器已经空，产生 EV8，发送数据到 DR 既可清除该事件
	while(NumByteToWrite--)    //利用 while 循环  发送数据
	{
	I2C_SendData(I2C1, *pBuffer); //发送数据
	pBuffer++; //数据指针移位
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//清除EV8
	}
	I2C_GenerateSTOP(I2C1, ENABLE);//产生停止信号
}

void I2C_EE_WaitEepromStandbyState(void)      
{
	vu16 SR1_Tmp = 0;
	do
	{
	I2C_GenerateSTART(I2C1, ENABLE);//产生起始信号
	SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);//读 SR1 寄存器
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);//发送器件地址清除事件
	}while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));//如果接收不到从机的应答（NACK）则说明 EEPROM 器件还在工作，直到完成操作跳出循环体！
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);//清除 AF 标志位
	I2C_GenerateSTOP(I2C1, ENABLE); //产生停止信号
}
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)//需要两个起始信号
{ 
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); //调用库函数检测 I2C 器件是否处于 BUSY 状态
	I2C_GenerateSTART(I2C1, ENABLE);//开启信号
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//清除 EV5
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);//写入器件地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//清除 EV6
	I2C_SendData(I2C1, ReadAddr);  //发送读的地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//清除 EV8
	I2C_GenerateSTART(I2C1, ENABLE);//开启信号
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//清除 EV5
	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);//将器件地址传出，主机为读
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));//清除EV6
	while(NumByteToRead) 
	{ 
	if(NumByteToRead == 1)//只剩下最后一个数据时进入 if 语句
	{
	I2C_AcknowledgeConfig(I2C1, DISABLE);//最后有一个数据时关闭应答位
	I2C_GenerateSTOP(I2C1, ENABLE);//最后一个数据时使能停止位
	}
	if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) //读取数据
	{ 
	*pBuffer = I2C_ReceiveData(I2C1);//调用库函数将数据取出到 pBuffer
	pBuffer++; //指针移位
	NumByteToRead--;//字节数减 1 
	} 
	}
	I2C_AcknowledgeConfig(I2C1, ENABLE);//将应答位使能回去，等待下次通信
}


void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	Addr = WriteAddr % I2C_PageSize;//查看输入的地址是不是 8 的整数倍
	count = I2C_PageSize -  Addr;//表示距离下一页页首地址的距离（步伐数）
	NumOfPage =   NumByteToWrite / I2C_PageSize;//算出一共有多少页
	NumOfSingle = NumByteToWrite % I2C_PageSize;//算出不够一页的数据的余数
	if(Addr == 0) //如果输入的地址是首页地址
	{
	if(NumOfPage == 0) //如果不足一页数据
	{
	I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//调用写函数，NumOfSingle 不够一页的余数作为实参
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	}
	/* If NumByteToWrite > I2C_PageSize */
	else  //如果数据有一页以上
	{
	while(NumOfPage--)//用一个 while 循环，执行页写循环操作，有多少页就写多少次
	{
	I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); //调用写函数，将I2C_PageSize 变量作为实参执行页写
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	WriteAddr +=   I2C_PageSize;//每执行完一次页写对应的地址也需要移 8 个位
	pBuffer += I2C_PageSize;//数据指针移 8 个位 
	}
	if(NumOfSingle!=0)//如果有不足一页的数据余数则执行
	{
	I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//调用写函数，NumOfSingle不够一页的余数作为实参
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	}
	}
	}
	else //输入的地址不是首页地址
	{
	if(NumOfPage== 0) //如果不足一页
	{
	I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//调用写函数，NumOfSingle 不够一页的余数作为实参
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	}
	else//如果有一页或一页以上
	{
	NumByteToWrite -= count;//将地址后续的缺省位置补上数据，数据的多少就是 count的值，NumByteToWrite 变量的值就是补上数据之后还剩下未发送的数量
	NumOfPage =   NumByteToWrite / I2C_PageSize;//剩余的页数
	NumOfSingle = NumByteToWrite % I2C_PageSize;//不足一页的数据数量
	if(count != 0)//将地址后续的缺省位置补上数据
	{  
	I2C_EE_PageWrite(pBuffer , WriteAddr, count);//调用写函数，以 count 为实参，将地址缺省下来的部分地址给填充上数据
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	WriteAddr += count;//加上 count 后，地址就移位到下一页的首地址
	pBuffer += count;//数据指针移 count 个位
	} 
	while(NumOfPage--)//将剩余的页数数据写入 EEPROM
	{
	I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);//调用写函数，将I2C_PageSize 变量作为实参执行页写
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	WriteAddr +=   I2C_PageSize;//将地址移 8 个位
	pBuffer += I2C_PageSize;   //将数据指针移 8 个位
	}
	if(NumOfSingle != 0)//将不足一页的数据写入 EEPROM
	{
	I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//调用写函数，NumOfSingle不够一页的余数作为实参
	I2C_EE_WaitEepromStandbyState();//等待 EEPROM 器件完成内部操作
	}
	}
	}  
}
void EEPROM_Write(uint32 Addr,void *Data,uint16 Length)
{
	I2C_EE_PageWrite(Data,Addr,Length);
}
void EEPROM_Read(uint32 Addr,void *Data,uint16 Length)
{
	I2C_EE_BufferRead(Data,Addr,Length);
}

void EEPROM_SaveConfig(void *Data,uint8 Sector)
{
	Save_VoidConfig(EEPROM_Write,0,Data,Sector);
}
BOOL EEPROM_LoadConfig(void *Data,uint8 Sector)
{
	return Load_VoidConfig(EEPROM_Read,0,Data,Sector);
}

void DeviceMount_EEPROM()
{
	DeviceId_EEPROM=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_EEPROM].DeviceName="24C08";
	Tos_Device_Tab[DeviceId_EEPROM].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_EEPROM].DeviceVirtue=DV_Task;
	Tos_Device_Tab[DeviceId_EEPROM].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_EEPROM].Init=I2C1_Configuration;
	Tos_Device_Tab[DeviceId_EEPROM].Note=Null;
	Tos_Device_Tab[DeviceId_EEPROM].Write=Null;
	Tos_Device_Tab[DeviceId_EEPROM].Read=Null;
	Tos_Device_Tab[DeviceId_EEPROM].Exit=Null;
}

/*END OF FUNCTION*/


