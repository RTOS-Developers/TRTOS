#include <Include.h>
#include <..\USER\Prj_RemoteControl\24L01_Driver.h>

 uint8 RX_BUF[RX_PLOAD_WIDTH];		//接收数据缓存
 uint8 TX_BUF[TX_PLOAD_WIDTH];		//发射数据缓存
 uint8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
 uint8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

#define NRF_CE_SET      GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define NRF_CE_CLR      GPIO_ResetBits(GPIOB,GPIO_Pin_3)

#define NRF_CSN_SET     GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define NRF_CSN_CLR     GPIO_ResetBits(GPIOA,GPIO_Pin_15)

#define NRF_SCK_SET     GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define NRF_SCK_CLR     GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define NRF_MOSI_SET    GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define NRF_MOSI_CLR    GPIO_ResetBits(GPIOB,GPIO_Pin_4)

#define NRF_MISO_DAT   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define NRF_IRQ_DAT    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
void Nrf_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	NRF_CE_CLR;
	NRF_CSN_SET;
	NRF_SCK_CLR;
	Remap_SWJ_JTAGDisable();
}
void Nrf_Delay(uint16 i)
{
	while(i--);
}
uint8 Nrf_WR(uint8 Data)
{
	uint8 i,Temp=0x00;
	for(i=0;i<8;i++)
	{
		Temp<<=1;
		if(Data&0x80)NRF_MOSI_SET;else NRF_MOSI_CLR;
		if(NRF_MISO_DAT)Temp|=0x01;
		Data<<=1;
		Nrf_Delay(1);
		NRF_SCK_SET;
		Nrf_Delay(1);
		NRF_SCK_CLR;
	  Nrf_Delay(1);
	}
	return Temp;
}
uint8 Nrf_WriteReg(uint8 reg,uint8 dat)
{
	uint8 status;
	NRF_CE_CLR;
	NRF_CSN_CLR;
	status = Nrf_WR(reg);
	Nrf_WR(dat);        
	NRF_CSN_SET;	
	return(status);
}


/*
 * 函数名：Nrf_ReadReg
 * 描述  ：用于从NRF特定的寄存器读出数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 * 输出  ：寄存器中的数据
 * 调用  ：内部调用
 */
uint8 Nrf_ReadReg(uint8 reg)
{
 	uint8 reg_val;
	NRF_CE_CLR;
 	NRF_CSN_CLR;
	Nrf_WR(reg); 
	reg_val = Nrf_WR(NOP);
	NRF_CSN_SET;		
	return reg_val;
}	
uint8 Nrf_ReadBuf(uint8 reg,uint8 *pBuf,uint8 Length)
{
 	uint8 status, byte_cnt;
  NRF_CE_CLR;
	NRF_CSN_CLR;
	status = Nrf_WR(reg); 
	for(byte_cnt=0;byte_cnt<Length;byte_cnt++)		  
	pBuf[byte_cnt] = Nrf_WR(NOP); //从NRF24L01读取数据  
	NRF_CSN_SET;		
 	return status;		//返回寄存器状态值
}



/*
 * 函数名：Nrf_WriteBuf
 * 描述  ：用于向NRF的寄存器中写入一串数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 		   pBuf：存储了将要写入写寄存器数据的数组，外部定义
		   bytes: pBuf的数据长度	
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：外部调用
 */
uint8 Nrf_WriteBuf(uint8 reg ,uint8 *pBuf,uint8 Length)
{
	uint8 status,byte_cnt;
	NRF_CE_CLR;
	NRF_CSN_CLR;			
	status = Nrf_WR(reg); 
	for(byte_cnt=0;byte_cnt<Length;byte_cnt++)
	Nrf_WR(*pBuf++);	//写数据到缓冲区 	 
	NRF_CSN_SET;			
	return (status);	//返回NRF24L01的状态 		
}
void Nrf_RX_Mode(void)
{
	NRF_CE_CLR;	
	Nrf_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	Nrf_WriteReg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    
	Nrf_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址    
	Nrf_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //设置RF通信频率    
	Nrf_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度      
	Nrf_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	Nrf_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
	NRF_CSN_SET;/*CE拉高，进入接收模式*/	
}   
void Nrf_TX_Mode(void)
{  
	NRF_CE_CLR;		
	Nrf_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 
	Nrf_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK   
	Nrf_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
	Nrf_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
	Nrf_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	Nrf_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //设置RF通道为CHANAL
	Nrf_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	Nrf_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	NRF_CSN_SET;/*CE拉高，进入发送模式*/	
	Nrf_Delay(0xffff); //CE要拉高一段时间才进入发送模式
}
uint8 Nrf_Check(void)
{
	uint8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8 buf1[5];
	uint8 i; 
	Nrf_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);	/*写入5个字节的地址.  */  
	Nrf_ReadBuf(TX_ADDR,buf1,5); /*读出写入的地址 */      
	for(i=0;i<5;i++)/*比较*/ 
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU与NRF成功连接 
	else
		return ERROR ;        //MCU与NRF不正常连接
}
uint8 Nrf_Tx_Dat(uint8 *txbuf)
{
	uint8 state;  
	NRF_CE_CLR;					
	Nrf_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
	NRF_CE_SET;

	/*等待发送完成中断 */                            
	while(NRF_IRQ_DAT!=0); 	

	/*读取状态寄存器的值 */                              
	state = Nrf_ReadReg(STATUS);

	/*清除TX_DS或MAX_RT中断标志*/                  
	Nrf_WriteReg(NRF_WRITE_REG+STATUS,state); 	
	Nrf_WriteReg(FLUSH_TX,NOP);    //清除TX FIFO寄存器 

	/*判断中断类型*/    
	if(state&MAX_RT)                     //达到最大重发次数
	return MAX_RT; 

	else if(state&TX_DS)                  //发送完成
	return TX_DS;
	else						  
	return ERROR;                 //其他原因发送失败
} 
u8 Nrf_Rx_Dat(u8 *rxbuf)
{
	u8 state; 
	NRF_CE_SET;	 //进入接收状态
	/*等待接收中断*/
	while(NRF_IRQ_DAT!=0)Tos_TaskDelay(1); 

	NRF_CE_CLR;  	 //进入待机状态
	/*读取status寄存器的值  */               
	state=Nrf_ReadReg(STATUS);

	/* 清除中断标志*/      
	Nrf_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*判断是否接收到数据*/
	if(state&RX_DR)                                 //接收到数据
	{
	Nrf_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
	Nrf_WriteReg(FLUSH_RX,NOP);          //清除RX FIFO寄存器
	return RX_DR; 
	}
	else    
	return ERROR;                    //没收到任何数据
}

void Nrf_Task(void *Tags)
{
	uint8 Flag;
	BOOL EN=False,OldEN;
	Nrf_GPIO_Init();
	Nrf_TX_Mode();
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))EN=True;else EN=False;
		TX_BUF[0]=EN;
		TX_BUF[2]=ADCBuffer[2]/0xff;
		TX_BUF[3]=ADCBuffer[2]%0xff;
		if(EN)LED3_OFF;
		if(EN!=OldEN)
			{
			Flag=Nrf_Tx_Dat(TX_BUF);
			if(Flag==0x20){LED3_ON;OldEN=EN;}else LED3_OFF;
		}
		Tos_TaskDelay(100);
	}
}

