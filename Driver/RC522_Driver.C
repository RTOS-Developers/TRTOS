#include <Include.h>
#include <RC522_Driver.H>


#define MAXRLEN 18
                 

/*******************************************************************************
Func:GPIO初始化
Date:2014-10-9
Note:
*******************************************************************************/
void RC522_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
           
/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;  
 


	}
   
   return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];

         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//          addr[IN]：钱包地址
//          pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                 
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
       // memcpy(ucComMF522Buf, pValue, 4);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：备份钱包
//参数说明: sourceaddr[IN]：源地址
//          goaladdr[IN]：目标地址
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}


/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
		status=status;
    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
		RC522_GPIO_Init();
	Tos_TaskDelay(100);
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
	//unsigned char i;
    MF522_RST_SET;

		Tos_TaskDelay(1);                 

    MF522_RST_CLR;

		Tos_TaskDelay(1);                   

    MF522_RST_SET;

		Tos_TaskDelay(1);                  
	
    WriteRawRC(CommandReg,PCD_RESETPHASE);

		Tos_TaskDelay(1);                   
	
    
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
   WriteRawRC(TxAutoReg,0x40);
    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//功    能：读RC632寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

     MF522_SCK_CLR;
     MF522_NSS_CLR;
     ucAddr = ((Address<<1)&0x7E)|0x80;

     for(i=8;i>0;i--)
     {
         if((ucAddr&0x80)==0x80)MF522_SI_SET;
			 else MF522_SI_CLR;
         MF522_SCK_SET;
         ucAddr <<= 1;
         MF522_SCK_CLR;
     }

     for(i=8;i>0;i--)
     {
         MF522_SCK_SET;
         ucResult <<= 1;
         if(MF522_SO_DAT)ucResult|=0x01;
         MF522_SCK_CLR;
     }

     MF522_NSS_SET;
     MF522_SCK_SET;
     return ucResult;
}

/////////////////////////////////////////////////////////////////////
//功    能：写RC632寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr;

    MF522_SCK_CLR;
    MF522_NSS_CLR;
    ucAddr = ((Address<<1)&0x7E);

    for(i=8;i>0;i--)
    {
        if((ucAddr&0x80)==0x80)MF522_SI_SET;
				else MF522_SI_CLR;
        MF522_SCK_SET;
        ucAddr <<= 1;
        MF522_SCK_CLR;
    }

    for(i=8;i>0;i--)
    {
        if((value&0x80)==0x80)MF522_SI_SET;
			else MF522_SI_CLR;
        MF522_SCK_SET;
        value <<= 1;
        MF522_SCK_CLR;
    }
    MF522_NSS_SET;
    MF522_SCK_SET;
}

/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
    do 
    {
         n = ReadRawRC(ComIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(ReadRawRC(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}


/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn()
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}



//#define MAXRLEN 18
////M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
////4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
//uint8  data1[16] ={0x12,0x34,0x56,0x78,0xED,0xCB,0xA9,0x87,0x12,0x34,0x56,0x78,0x03,0xFC,0x03,0xFC}; //钱包初始数据
//uint8  money[4]      = {1,0,0,0};//钱包扣费值
//uint8  DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //初始密码
//uint8  g_ucTempbuf[20];//处理过程数据暂存区
//uint8  blockselect=1;//定义块号和欲扣金额的空间 



///*******************************************************************************
//Func:GPIO初始化
//Date:2014-10-9
//Note:
//*******************************************************************************/
//void RC522_GPIO_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
//void RCC522_Delay(uint16 i)
//{
//	i*=100;
//	while(i--);
//}
//                              
///////////////////////////////////////////////////////////////////////
////功    能：寻卡
////参数说明: req_code[IN]:寻卡方式
////                0x52 = 寻感应区内所有符合14443A标准的卡
////                0x26 = 寻未进入休眠状态的卡
////          pTagType[OUT]：卡片类型代码
////                0x4400 = Mifare_UltraLight
////                0x0400 = Mifare_One(S50)
////                0x0200 = Mifare_One(S70)
////                0x0800 = Mifare_Pro(X)
////                0x4403 = Mifare_DESFire
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////
//char PcdRequest(unsigned char req_code,unsigned char *pTagType)
//{
//   char status;  
//   unsigned int  unLen;
//   unsigned char ucComMF522Buf[MAXRLEN]; 

//   ClearBitMask(Status2Reg,0x08);
//   WriteRawRC(BitFramingReg,0x07);
//   SetBitMask(TxControlReg,0x03);
// 
//   ucComMF522Buf[0] = req_code;

//   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
//   
//   if ((status == MI_OK) && (unLen == 0x10))
//   {    
//       *pTagType     = ucComMF522Buf[0];
//       *(pTagType+1) = ucComMF522Buf[1];
//   }
//   else
//   {   status = MI_ERR;   }
//   
//   return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：防冲撞
////参数说明: pSnr[OUT]:卡片序列号，4字节
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////  
//char PcdAnticoll(unsigned char *pSnr)
//{
//    char status;
//    unsigned char i,snr_check=0;
//    unsigned int  unLen;
//    unsigned char ucComMF522Buf[MAXRLEN]; 
//    

//    ClearBitMask(Status2Reg,0x08);
//    WriteRawRC(BitFramingReg,0x00);
//    ClearBitMask(CollReg,0x80);
// 
//    ucComMF522Buf[0] = PICC_ANTICOLL1;
//    ucComMF522Buf[1] = 0x20;

//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

//    if (status == MI_OK)
//    {
//    	 for (i=0; i<4; i++)
//         {   
//             *(pSnr+i)  = ucComMF522Buf[i];
//             snr_check ^= ucComMF522Buf[i];
//         }
//         if (snr_check != ucComMF522Buf[i])
//         {   status = MI_ERR;    }
//    }
//    
//    SetBitMask(CollReg,0x80);
//    return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：选定卡片
////参数说明: pSnr[IN]:卡片序列号，4字节
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////
//char PcdSelect(unsigned char *pSnr)
//{
//    char status;
//    unsigned char i;
//    unsigned int  unLen;
//    unsigned char ucComMF522Buf[MAXRLEN]; 
//    
//    ucComMF522Buf[0] = PICC_ANTICOLL1;
//    ucComMF522Buf[1] = 0x70;
//    ucComMF522Buf[6] = 0;
//    for (i=0; i<4; i++)
//    {
//    	ucComMF522Buf[i+2] = *(pSnr+i);
//    	ucComMF522Buf[6]  ^= *(pSnr+i);
//    }
//    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
//  
//    ClearBitMask(Status2Reg,0x08);

//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
//    
//    if ((status == MI_OK) && (unLen == 0x18))
//    {   status = MI_OK;  }
//    else
//    {   status = MI_ERR;    }

//    return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：验证卡片密码
////参数说明: auth_mode[IN]: 密码验证模式
////                 0x60 = 验证A密钥
////                 0x61 = 验证B密钥 
////          addr[IN]：块地址
////          pKey[IN]：密码
////          pSnr[IN]：卡片序列号，4字节
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////               
//char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
//{
//    char status;
//    unsigned int  unLen;
//    unsigned char i,ucComMF522Buf[MAXRLEN]; 

//    ucComMF522Buf[0] = auth_mode;
//    ucComMF522Buf[1] = addr;
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+2] = *(pKey+i);   }
//    for (i=0; i<6; i++)
//    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
// //   memcpy(&ucComMF522Buf[2], pKey, 6); 
// //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
//    
//    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
//    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
//    {   status = MI_ERR;   }
//    
//    return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：读取M1卡一块数据
////参数说明: addr[IN]：块地址
////          pData[OUT]：读出的数据，16字节
////返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////// 
//char PcdRead(unsigned char addr,unsigned char *pData)
//{
//    char status;
//    unsigned int  unLen;
//    unsigned char i,ucComMF522Buf[MAXRLEN]; 

//    ucComMF522Buf[0] = PICC_READ;
//    ucComMF522Buf[1] = addr;
//    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
//   
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
//    if ((status == MI_OK) && (unLen == 0x90))
// //   {   memcpy(pData, ucComMF522Buf, 16);   }
//    {
//        for (i=0; i<16; i++)
//        {    *(pData+i) = ucComMF522Buf[i];   }
//    }
//    else
//    {   status = MI_ERR;   }
//    
//    return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：写数据到M1卡一块
////参数说明: addr[IN]：块地址
////          pData[IN]：写入的数据，16字节
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////                  
//char PcdWrite(unsigned char addr,unsigned char *pData)
//{
//    char status;
//    unsigned int  unLen;
//    unsigned char i,ucComMF522Buf[MAXRLEN]; 
//    
//    ucComMF522Buf[0] = PICC_WRITE;
//    ucComMF522Buf[1] = addr;
//    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
// 
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

//    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
//    {   status = MI_ERR;   }
//        
//    if (status == MI_OK)
//    {
//        //memcpy(ucComMF522Buf, pData, 16);
//        for (i=0; i<16; i++)
//        {    ucComMF522Buf[i] = *(pData+i);   }
//        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

//        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
//        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
//        {   status = MI_ERR;   }
//    }
//    
//    return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：扣款和充值
////参数说明: dd_mode[IN]：命令字
////               0xC0 = 扣款
////               0xC1 = 充值
////          addr[IN]：钱包地址
////          pValue[IN]：4字节增(减)值，低位在前
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////                 
//char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
//{
//    char status;
//    unsigned int  unLen;
//    unsigned char i,ucComMF522Buf[MAXRLEN]; 
//    
//    ucComMF522Buf[0] = dd_mode;
//    ucComMF522Buf[1] = addr;
//    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
// 
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

//    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
//    {   status = MI_ERR;   }
//        
//    if (status == MI_OK)
//    {
//       // memcpy(ucComMF522Buf, pValue, 4);
//        for (i=0; i<16; i++)
//        {    ucComMF522Buf[i] = *(pValue+i);   }
//        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
//        unLen = 0;
//        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
//        if (status != MI_ERR)
//        {    status = MI_OK;    }
//    }
//    
//    if (status == MI_OK)
//    {
//        ucComMF522Buf[0] = PICC_TRANSFER;
//        ucComMF522Buf[1] = addr;
//        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
//   
//        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

//        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
//        {   status = MI_ERR;   }
//    }
//    return status;
//}

///////////////////////////////////////////////////////////////////////
////功    能：备份钱包
////参数说明: sourceaddr[IN]：源地址
////          goaladdr[IN]：目标地址
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////

//char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
//{
//    char status;
//    unsigned int  unLen;
//    unsigned char ucComMF522Buf[MAXRLEN]; 

//    ucComMF522Buf[0] = PICC_RESTORE;
//    ucComMF522Buf[1] = sourceaddr;
//    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
// 
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

//    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
//    {   status = MI_ERR;   }
//    
//    if (status == MI_OK)
//    {
//        ucComMF522Buf[0] = 0;
//        ucComMF522Buf[1] = 0;
//        ucComMF522Buf[2] = 0;
//        ucComMF522Buf[3] = 0;
//        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
// 
//        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
//        if (status != MI_ERR)
//        {    status = MI_OK;    }
//    }
//    
//    if (status != MI_OK)
//    {    return MI_ERR;   }
//    
//    ucComMF522Buf[0] = PICC_TRANSFER;
//    ucComMF522Buf[1] = goaladdr;

//    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
// 
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

//    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
//    {   status = MI_ERR;   }

//    return status;
//}


///////////////////////////////////////////////////////////////////////
////功    能：命令卡片进入休眠状态
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////
//char PcdHalt(void)
//{
//    //char status=0;
//    unsigned int  unLen;
//    unsigned char ucComMF522Buf[MAXRLEN]; 

//    ucComMF522Buf[0] = PICC_HALT;
//    ucComMF522Buf[1] = 0;
//    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
// 
//    //status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
//	PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
//    return MI_OK;
//}

///////////////////////////////////////////////////////////////////////
////用MF522计算CRC16函数
///////////////////////////////////////////////////////////////////////
//void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
//{
//    unsigned char i,n;
//    ClearBitMask(DivIrqReg,0x04);
//    WriteRawRC(CommandReg,PCD_IDLE);
//    SetBitMask(FIFOLevelReg,0x80);
//    for (i=0; i<len; i++)
//    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
//    WriteRawRC(CommandReg, PCD_CALCCRC);
//    i = 0xFF;
//    do 
//    {
//        n = ReadRawRC(DivIrqReg);
//        i--;
//    }
//    while ((i!=0) && !(n&0x04));
//    pOutData[0] = ReadRawRC(CRCResultRegL);
//    pOutData[1] = ReadRawRC(CRCResultRegM);
//}

///////////////////////////////////////////////////////////////////////
////功    能：复位RC522
////返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////////
//char PcdReset(void)
//{
//    MF522_NSS_SET;
//    RCC522_Delay(100);
//    MF522_NSS_CLR;
//    RCC522_Delay(100);
//    MF522_NSS_SET;
//     RCC522_Delay(100);
//    WriteRawRC(CommandReg,PCD_RESETPHASE);
//    RCC522_Delay(100);
//    
//    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
//    WriteRawRC(TReloadRegL,30);           
//    WriteRawRC(TReloadRegH,0);
//    WriteRawRC(TModeReg,0x8D);
//    WriteRawRC(TPrescalerReg,0x3E);
//    WriteRawRC(TxAskReg,0x40);
//   
//    return MI_OK;
//}

///////////////////////////////////////////////////////////////////////
////功    能：读RC632寄存器
////参数说明：Address[IN]:寄存器地址
////返    回：读出的值
///////////////////////////////////////////////////////////////////////
//unsigned char ReadRawRC(unsigned char Address)
//{
//     unsigned char i, ucAddr;
//     unsigned char ucResult=0;

//     MF522_SCK_CLR;
//     MF522_NSS_CLR;
//     ucAddr = ((Address<<1)&0x7E)|0x80;

//     for(i=8;i>0;i--)
//     {
//			 if((ucAddr&0x80)==0x80)MF522_SI_SET;
//			else MF522_SI_CLR;
//         MF522_SCK_SET;
//         ucAddr <<= 1;
//         MF522_SCK_CLR;
//     }

//     for(i=8;i>0;i--)
//     {
//         MF522_SCK_SET;
//         ucResult <<= 1;
//		  if(MF522_SO_DAT)ucResult|=0x01;
//         MF522_SCK_CLR;
//     }

//     MF522_NSS_SET;
//     MF522_SCK_SET;
//     return ucResult;
//     
//}

///////////////////////////////////////////////////////////////////////
////功    能：写RC632寄存器
////参数说明：Address[IN]:寄存器地址
////          value[IN]:写入的值
///////////////////////////////////////////////////////////////////////
//void WriteRawRC(unsigned char Address, unsigned char value)
//{  
//    unsigned char i, ucAddr;

//    MF522_SCK_CLR;
//    MF522_NSS_CLR;
//    ucAddr = ((Address<<1)&0x7E);

//    for(i=8;i>0;i--)
//    {
//        if((ucAddr&0x80)==0x80)MF522_SI_SET;
//			else MF522_SI_CLR;
//        MF522_SCK_SET;
//        ucAddr <<= 1;
//        MF522_SCK_CLR;
//    }

//    for(i=8;i>0;i--)
//    {
//         if((value&0x80)==0x80)MF522_SI_SET;
//			else MF522_SI_CLR;
//        MF522_SCK_SET;
//        value <<= 1;
//        MF522_SCK_CLR;
//    }
//    MF522_NSS_SET;
//    MF522_SCK_SET;
//}

///////////////////////////////////////////////////////////////////////
////功    能：置RC522寄存器位
////参数说明：reg[IN]:寄存器地址
////          mask[IN]:置位值
///////////////////////////////////////////////////////////////////////
//void SetBitMask(unsigned char reg,unsigned char mask)  
//{
//    char tmp = 0x0;
//    tmp = ReadRawRC(reg);
//    WriteRawRC(reg,tmp | mask);  // set bit mask
//}

///////////////////////////////////////////////////////////////////////
////功    能：清RC522寄存器位
////参数说明：reg[IN]:寄存器地址
////          mask[IN]:清位值
///////////////////////////////////////////////////////////////////////
//void ClearBitMask(unsigned char reg,unsigned char mask)  
//{
//    char tmp = 0x0;
//    tmp = ReadRawRC(reg);
//    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
//} 

///////////////////////////////////////////////////////////////////////
////功    能：通过RC522和ISO14443卡通讯
////参数说明：Command[IN]:RC522命令字
////          pInData[IN]:通过RC522发送到卡片的数据
////          InLenByte[IN]:发送数据的字节长度
////          pOutData[OUT]:接收到的卡片返回数据
////          *pOutLenBit[OUT]:返回数据的位长度
///////////////////////////////////////////////////////////////////////
//char PcdComMF522(unsigned char Command, 
//                 unsigned char *pInData, 
//                 unsigned char InLenByte,
//                 unsigned char *pOutData, 
//                 unsigned int  *pOutLenBit)
//{
//    char status = MI_ERR;
//    unsigned char irqEn   = 0x00;
//    unsigned char waitFor = 0x00;
//    unsigned char lastBits;
//    unsigned char n;
//    unsigned int i;
//    switch (Command)
//    {
//       case PCD_AUTHENT:
//          irqEn   = 0x12;
//          waitFor = 0x10;
//          break;
//       case PCD_TRANSCEIVE:
//          irqEn   = 0x77;
//          waitFor = 0x30;
//          break;
//       default:
//         break;
//    }
//   
//    WriteRawRC(ComIEnReg,irqEn|0x80);
//    ClearBitMask(ComIrqReg,0x80);
//    WriteRawRC(CommandReg,PCD_IDLE);
//    SetBitMask(FIFOLevelReg,0x80);
//    
//    for (i=0; i<InLenByte; i++)
//    {   WriteRawRC(FIFODataReg, pInData[i]);    }
//    WriteRawRC(CommandReg, Command);
//   
//    
//    if (Command == PCD_TRANSCEIVE)
//    {    SetBitMask(BitFramingReg,0x80);  }
//    
//    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
//    do 
//    {
//         n = ReadRawRC(ComIrqReg);
//         i--;
//    }
//    while ((i!=0) && !(n&0x01) && !(n&waitFor));
//    ClearBitMask(BitFramingReg,0x80);
//	      
//    if (i!=0)
//    {    
//         if(!(ReadRawRC(ErrorReg)&0x1B))
//         {
//             status = MI_OK;
//             if (n & irqEn & 0x01)
//             {   status = MI_NOTAGERR;   }
//             if (Command == PCD_TRANSCEIVE)
//             {
//               	n = ReadRawRC(FIFOLevelReg);
//              	lastBits = ReadRawRC(ControlReg) & 0x07;
//                if (lastBits)
//                {   *pOutLenBit = (n-1)*8 + lastBits;   }
//                else
//                {   *pOutLenBit = n*8;   }
//                if (n == 0)
//                {   n = 1;    }
//                if (n > MAXRLEN)
//                {   n = MAXRLEN;   }
//                for (i=0; i<n; i++)
//                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
//            }
//         }
//         else
//         {   status = MI_ERR;   }
//        
//   }
//   

//   SetBitMask(ControlReg,0x80);           // stop timer now
//   WriteRawRC(CommandReg,PCD_IDLE); 
//   return status;
//}

////////////////////////////////////////////////////////////////////////
////设置RC522的工作方式 
////////////////////////////////////////////////////////////////////////
//char M500PcdConfigISOType(unsigned char type)
//{
//   if (type == 'A')                     //ISO14443_A
//   { 
//       ClearBitMask(Status2Reg,0x08);
//       WriteRawRC(ModeReg,0x3D);
//       WriteRawRC(RxSelReg,0x86);
//       WriteRawRC(RFCfgReg,0x7F);   
//   	   WriteRawRC(TReloadRegL,30);
//	   WriteRawRC(TReloadRegH,0);
//       WriteRawRC(TModeReg,0x8D);
//	   WriteRawRC(TPrescalerReg,0x3E);
//	   RCC522_Delay(10000);
//       PcdAntennaOn();
//   }
//   else{ return 1; }  //-1
//   
//   return MI_OK;
//}

///////////////////////////////////////////////////////////////////////
////开启天线  
////每次启动或关闭天险发射之间应至少有1ms的间隔
///////////////////////////////////////////////////////////////////////
//void PcdAntennaOn()
//{
//    unsigned char i;
//    i = ReadRawRC(TxControlReg);
//    if (!(i & 0x03))
//    {
//        SetBitMask(TxControlReg, 0x03);
//    }
//}


///////////////////////////////////////////////////////////////////////
////关闭天线
///////////////////////////////////////////////////////////////////////
//void PcdAntennaOff()
//{
//    ClearBitMask(TxControlReg, 0x03);
//}

// /*******************************************************************************
//* 描  述  :  初始化RC522，开启天线并设置RC522的工作方式为 ISO14443_A
//* 输  入  :  无
//* 输  出  :  无
//* 返  回  :  无
//*******************************************************************************/
// void InitRc522(void)
//{
//	RC522_GPIO_Init();
//	Tos_TaskDelay(10);										  

//    PcdReset();
//    PcdAntennaOff();  
//    PcdAntennaOn();
//    M500PcdConfigISOType( 'A' );
//}
