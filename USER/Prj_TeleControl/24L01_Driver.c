#include <Include.h>
#include <SPI_Driver.h>
#include <..\USER\Prj_TeleControl\24L01_Driver.h>


 uint8 RX_BUF[RX_PLOAD_WIDTH];		//接收数据缓存
uint8 TX_BUF[TX_PLOAD_WIDTH];		//发射数据缓存
uint8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
uint8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};
uint8 DeviceId_ZIGBEE=DeviceNull;

const GPIO_InitStruct Nrf_GPIO_List[]=
{
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5},
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOA,GPIO_Pin_15},
	{GPIO_Speed_2MHz,GPIO_Mode_IPU,GPIOB,GPIO_Pin_6|GPIO_Pin_7},
	{0},
};

void Nrf_MOSI(BOOL Level){if(Level)GPIO_SetBits(GPIOB,GPIO_Pin_4);else GPIO_ResetBits(GPIOB,GPIO_Pin_4);}
void Nrf_SCLK(BOOL Level){if(Level)GPIO_SetBits(GPIOB,GPIO_Pin_5);else GPIO_ResetBits(GPIOB,GPIO_Pin_5);}
void Nrf_CE(BOOL Level){if(Level)GPIO_SetBits(GPIOB,GPIO_Pin_3);else GPIO_ResetBits(GPIOB,GPIO_Pin_3);}
void Nrf_CSN(BOOL Level){if(Level)GPIO_SetBits(GPIOA,GPIO_Pin_15);else GPIO_ResetBits(GPIOA,GPIO_Pin_15);}
BOOL Nrf_IRQ(){return (BOOL)GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);}
BOOL Nrf_MISO(){return (BOOL)GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);}


const SPI_PublicStruct SPI_Nrf={
	Nrf_MOSI,
	Nrf_SCLK,
	Nrf_CE,
	Nrf_CSN,
	Nrf_IRQ,
	Nrf_MISO,
	1,
};
void Nrf_RX_Mode(void)
{
	Nrf_CE(False);	
	SPI_WriteBuf(&SPI_Nrf,NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址    
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+RF_CH,CHANAL);      //设置RF通信频率    
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度      
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	Nrf_CSN(True);/*CE拉高，进入接收模式*/	
}   
void Nrf_TX_Mode()
{
	SPI_WriteBuf(&SPI_Nrf,NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 
	SPI_WriteBuf(&SPI_Nrf,NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK   
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+RF_CH,CHANAL);       //设置RF通道为CHANAL
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	Nrf_CSN(True);/*CE拉高，进入发送模式*/	
	SPI_Delay(0xffff); //CE要拉高一段时间才进入发送模式
}
uint8 Nrf_Check(void)
{
	uint8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8 buf1[5];
	uint8 i; 
	SPI_WriteBuf(&SPI_Nrf,NRF_WRITE_REG+TX_ADDR,buf,5);	/*写入5个字节的地址.  */  
	SPI_ReadBuf(&SPI_Nrf,TX_ADDR,buf1,5); /*读出写入的地址 */      
	for(i=0;i<5;i++)/*比较*/ 
	{
		if(buf1[i]!=0xC2)
		break;
	}        
	if(i==5)return SUCCESS ;        //MCU与NRF成功连接 
	else return ERROR ;        //MCU与NRF不正常连接
}

uint8 Nrf_Tx_Dat(uint8 *txbuf)
{
	uint8 state;  
	Nrf_CE(False);					
	SPI_WriteBuf(&SPI_Nrf,WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
	Nrf_CE(True);                           
	while(Nrf_IRQ()!=0); 	                            
	state = SPI_WR(&SPI_Nrf,STATUS);                 
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+STATUS,state); 	
	SPI_WriteReg(&SPI_Nrf,FLUSH_TX,NOP);    //清除TX FIFO寄存器 
	if(state&MAX_RT) return MAX_RT; 
	else if(state&TX_DS)                  //发送完成
	return TX_DS;
	else return ERROR;                 //其他原因发送失败
} 
uint8 Nrf_Rx_Dat(uint8 *rxbuf)
{
	uint8 state; 
	Nrf_CE(True);
	while(Nrf_IRQ()!=0){Tos_TaskDelay(1);} 
	Nrf_CE(False);  	             
	state=SPI_WR(&SPI_Nrf,STATUS);   
	SPI_WriteReg(&SPI_Nrf,NRF_WRITE_REG+STATUS,state);
	if(state&RX_DR)                                 //接收到数据
	{
	SPI_ReadBuf(&SPI_Nrf,RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
	SPI_WriteReg(&SPI_Nrf,FLUSH_RX,NOP);          //清除RX FIFO寄存器
	return RX_DR; 
	}
	else return ERROR;                    //没收到任何数据
}
void DeviceMount_ZigBee()
{
	DeviceId_ZIGBEE=DeviceId_Index++;
	Tos_Device_Tab[DeviceId_ZIGBEE].DeviceName="ZigBee";
	Tos_Device_Tab[DeviceId_ZIGBEE].DeviceOwnerId=Null;
	Tos_Device_Tab[DeviceId_ZIGBEE].DeviceVirtue=DV_IOList;
	Tos_Device_Tab[DeviceId_ZIGBEE].DeviceState=NotBusy;
	Tos_Device_Tab[DeviceId_ZIGBEE].Init=GPIO_InitList;
	Tos_Device_Tab[DeviceId_ZIGBEE].Note=(uintbus)&Nrf_GPIO_List[0];
	Tos_Device_Tab[DeviceId_ZIGBEE].Write=Null;
	Tos_Device_Tab[DeviceId_ZIGBEE].Read=Null;
	Tos_Device_Tab[DeviceId_ZIGBEE].Exit=Null;
}
void Nrf_Task(void *Tags)
{
	uint8 Flag;
	Nrf_RX_Mode();
	while(1)
	{
		Flag=Nrf_Rx_Dat(TX_BUF);
		if(Flag!=ERROR){if(TX_BUF[0])LED3_ON;else LED3_OFF;}
		Tos_TaskDelay(100);
	}
}

