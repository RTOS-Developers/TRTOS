#include <StandardModBus.h>




void MB_Create(ModBus_Infor *MBI,uint8 LocalAddr,uint8 WorkMode)
{
//	MBI->BaseInfor.LoclAddr=LocalAddr;
	//MBI->BaseInfor.WorkMode=WorkMode;
}
uint16 MB_AppendCrc(uint8 *Buf,uint16 Length)
{
	uint16 CrcValue;
	CrcValue=Tools_GetCRC16(&Buf[0],Length); 
	Buf[Length++]=CrcValue&0xff;
	Buf[Length++]=CrcValue>>8;
	return Length;
}
BOOL MB_CheckCrc(uint8 *Buf,uint16 Length)
{
	uint16 CrcValue,CrcSource;
	CrcValue=Tools_GetCRC16(&Buf[0],Length-2); 
	CrcSource=Buf[Length-1];CrcSource<<=8;
	CrcSource|=Buf[Length-2];
	if(CrcSource==CrcValue)return True;
	return False;
}
uint16 MB_CreateReadMsg(uint8 *Buf,uint8 DevAddr,ModBusCmdCode CmdCode,uint16 Addr,uint16 Length)
{
	uint16 i=0;
	Buf[i++]=DevAddr;
	Buf[i++]=CmdCode;
	Buf[i++]=Addr>>8;
	Buf[i++]=Addr&0xff;
	Buf[i++]=Length>>8;
	Buf[i++]=Length&0xff;
	i=MB_AppendCrc(&Buf[0],i);
	return i;
}
uint16 MB_CreateCmd(uint8 *Buf,uint8 DevAddr,ModBusCmdCode CmdCode,uint16 Addr,uint16 Length)
{
	return 0;
}
uint16 MB_ExpendMsg0x03(ModBus_Infor *MBI)
{
	uint16 i=2;
	uint16 Addr,Length;
	uint8 *RxBuf=MBI->BaseInfor->RxBuf;
	uint8 *TxBuf=MBI->BaseInfor->TxBuf;
	Addr=RxBuf[i++];Addr<<=8;
	Addr|=RxBuf[i++];
	Length=RxBuf[i++];Length<<=8;
	Length|=RxBuf[i++];
	i=0;
	if(MBI->BaseInfor->RunValue_Length>(Length+Addr))// 地址正确
	{
		TxBuf[i++]=RxBuf[0];
		TxBuf[i++]=RxBuf[1];
		TxBuf[i++]=Length*2;
		i+=BufferCoppy((uint8 *)((uintbus)MBI->BaseInfor->RunValue+Addr),&TxBuf[i],2*Length);
		i=MB_AppendCrc(&TxBuf[0],i);
	}else
	{
		TxBuf[i++]=RxBuf[0];
		TxBuf[i++]=RxBuf[1]|0x80;
		i=MB_AppendCrc(&TxBuf[0],i);
	}
	return i;
}
uint8 MB_ExpendMsg(ModBus_Infor *MBI)
{
	uint16 Length=0;
	uint8 *RxBuf=MBI->BaseInfor->RxBuf;
	if(MBI->BaseInfor->LoclAddr==RxBuf[0])
	{
		switch(RxBuf[1])
		{
			case 0x03:if(MB_CheckCrc(RxBuf,8))Length=MB_ExpendMsg0x03(MBI);break;
		}
	}
	if(MBI->BaseInfor->WorkMode&MBWM_BroadCastEN)//广播
	if(MB_BroadCast==RxBuf[0])
	{
		switch(RxBuf[1])
		{
			
		}
	}
	return Length;
}

/*--------------------------------------------------------------------------------------------------
 Func: 初始化MODBUS发送报文结构
 Time: 2010-6-29
 Ver.: V1.0
 Note: 												   	   
--------------------------------------------------------------------------------------------------*/
void S_Modbus_CreateTxHeader(S_Modbus *CurModbus,uint8 DevAddr,uint8 CmdCode,
							   uint16 RegStart,uint16 RegCount,uint16 DataLenth)
{
	S_ModbusMsg *Msg=&(CurModbus->TxMsg);
	Msg->DevAddr=DevAddr;
	Msg->CmdCode=CmdCode;
	Msg->RegStart=RegStart;
	Msg->RegCount=RegCount;
	Msg->DataLenth=DataLenth;
}

 
/*--------------------------------------------------------------------------------------------------
 Func: 生成ModBus数据报文
 Time: 2010-6-28
 Ver.: V1.0
 Note: 
       本方法对于主机: 生成查询报文
       本方法对于从机: 若TxMsg->CmdCode为读命令,则生成返回数据报文
                       若TxMsg->CmdCode为写命令,则生成应答报文
                       若收到非法不可执行命令,则生成错误码状态报文
       
 	   本方法根据Msg->DataLenth指定生成是否含Datas域
 	   Infor: DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+CRC16[2]
	   Input: *Msg/报文结构	 *Buffer/报文帧数据缓冲区
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_CreateTxMessage(S_Modbus *CurModbus,uint8 *TxBuffer)
{
	uint8  *D;
    uint16  L,T;
	uint16  *S;
	S_ModbusMsg *Msg=&(CurModbus->TxMsg);
	TxBuffer[0]=Msg->DevAddr;												//写入设备地址字节
	TxBuffer[1]=Msg->CmdCode;												//写入命令码字节
	if(!(CurModbus->LocalMode&S_Modbus_SlaveMode)){
		D16ToArray(TxBuffer+2,Msg->RegStart,1);							//写入起始地址
		D16ToArray(TxBuffer+4,Msg->RegCount,1);							//写入寄存器个数
		BufferCoppy(Msg->DataAddr,TxBuffer+6,Msg->DataLenth);			//写入数据域 
		
	    L=Msg->DataLenth+6;
	}else{
        if(Msg->CmdCode&0x80){                                              //生成错误报文,命令不可执行
			TxBuffer[2]=(uint8)Msg->DataLenth;
            L=3; 				                                            //写入错误码
        }else if(Msg->CmdCode==S_Modbus_CmdCode_Write){                  //生成从机的返回数据报文
            TxBuffer[2]=(uint8)Msg->DataLenth;								//写入数据长度域
            L=3;
	    }else if(Msg->CmdCode==S_Modbus_CmdCode_Read){                   //生成从机写应答报文
			TxBuffer[2]=(uint8)Msg->DataLenth;								//写入数据长度域			
			S=(uint16 *)Msg->DataAddr;
			D=TxBuffer+3;
			for(T=0;T<Msg->DataLenth;T+=2){
				D16ToArray(D,*S++,1);
				D+=2;
			}
			//S_BytesCopy(TxBuffer+3,Msg->DataAddr,Msg->DataLenth);			//写入数据域
            L=Msg->DataLenth+3;
	    }
    }
	T=Tools_GetCRC16(TxBuffer,L);          								//求本帧数据校验码
	D16ToArray(TxBuffer+L,T,0);											//写入CRC16到帧尾部
	return L+2;								                				//设定发送帧长度
}


/*--------------------------------------------------------------------------------------------------
 Func: 截取请求报文中的数据段
 Time: 2010-6-28
 Ver.: V1.0
 Note: 本方法根据RegCount和Lenth自动判断是否有合适的Datas域
 	   Infor:	DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+Datas[RegCount*2]+CRC16[2]
	   Input:	*Data/原如数据报文  Lenth/报文总长度  *Msg/返回的报文信息
 	   return:  0/报文信息提取成功	11/报文长度不足	12/报文数据校验错误	13/数据报分长度错误 0xFF/地址不匹配
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_ExpendRxMessage(S_Modbus *CurModbus,uint8 *RxBuffer,uint8 RxLenth)
{	
	uint16 T,P;
	S_ModbusMsg *Msg=&(CurModbus->RxMsg);
	if(RxLenth<5)return S_Modbus_DataLenthError;								//判断报文长度是否正常
	if((*RxBuffer)!=(CurModbus->LocalAddr)){									//检查单播地址选项
		if((*RxBuffer)==0xFF){
			if(!(CurModbus->LocalMode&S_Modbus_EnBroadcast)){					//检查广播选项
				return 0xFF;
			}
		}else return 0xFF;
	}	
	T=D16FromArray(RxBuffer+RxLenth-2,0);									//取出报文CRC16节
	P=Tools_GetCRC16(RxBuffer,RxLenth-2);									//获取报文CRC16检验码
	if(T!=P)return S_Modbus_CrcCheckError;									//检验CRC16码是否正确
	Msg->DevAddr=*RxBuffer++;													//取设备地址
	Msg->CmdCode=*RxBuffer++;													//取命令码
	if(!(CurModbus->LocalMode&S_Modbus_SlaveMode)){
        if(Msg->CmdCode==S_Modbus_CmdCode_Read){
            Msg->DataLenth=*RxBuffer++;
            Msg->DataAddr=RxBuffer;            
        }else if(Msg->CmdCode&0x80){
			Msg->DataLenth=1;
            Msg->DataAddr=RxBuffer;
        }else if(Msg->CmdCode==S_Modbus_CmdCode_Write){
			Msg->DataLenth=*RxBuffer++;
            Msg->DataAddr=RxBuffer;
        }
	}else{		
		Msg->RegStart=D16FromArray(RxBuffer,1);								//取起始寄存器地址	
		Msg->RegCount=D16FromArray(RxBuffer+2,1);							//取寄存器个度
	    RxLenth-=8;Msg->DataLenth=RxLenth;T=Msg->RegCount<<1;					//求取报文长度
	    if(RxLenth==0){return S_Modbus_MsgSuccess;}   						//是否查询帧
	    if(T!=RxLenth){Msg->DataLenth=0;return S_Modbus_DataLenthError;}		//判断数据部分是否长度正确			
        Msg->DataAddr=RxBuffer+4;         										//取报文数据部分	
	}
	return S_Modbus_MsgSuccess;
}


/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议复接收缓冲区报文头到发送缓冲区
 Time: 2010-6-29
 Ver.: V1.0
 Note: ErrorCode:  0/正确发送帧　1/未知功能码  2/未知寄存器	 3/非法操作   4/寄存器长度超限 
--------------------------------------------------------------------------------------------------*/
void S_Modbus_MsgInforWrite(S_Modbus *CurModbus,uint8 ErrorCode)
{
	S_ModbusMsg *RxMsg=&(CurModbus->RxMsg);
	S_ModbusMsg *TxMsg=&(CurModbus->TxMsg);
	TxMsg->DevAddr=RxMsg->DevAddr;
	TxMsg->CmdCode=RxMsg->CmdCode;
	if(ErrorCode){
		TxMsg->CmdCode|=0x80;	
		TxMsg->RegStart=0;
		TxMsg->RegCount=1;
		TxMsg->DataLenth=ErrorCode;
		if(CurModbus->OnMsgError>0)CurModbus->OnMsgError(ErrorCode);
	}else{
		TxMsg->RegStart=RxMsg->RegStart;
		TxMsg->RegCount=RxMsg->RegCount;
		TxMsg->DataLenth=((RxMsg->RegCount)<<1);
	}
}


/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议处理只读变量
 Time: 2010-6-29
 Ver.: V1.0
 Note:
--------------------------------------------------------------------------------------------------*/
void S_Modbus_GetLocalRegs(S_Modbus *CurModbus)
{
	uint8  i,j,*P;
	uint16 T,K,*RegAddr=0;
	S_ModbusBuf *Buf;
	T=CurModbus->RxMsg.RegStart;	
	K=T+CurModbus->RxMsg.RegCount;
	Buf=CurModbus->TyBuf;
	for(i=0;i<4;i++,Buf++){if((T>=Buf->StartAddr)&&(K<=Buf->StartAddr+Buf->Size))break;}
	if(i==4){S_Modbus_MsgInforWrite(CurModbus,S_Modbus_RegRangeError);return ;}			//未知寄存器
	K=CurModbus->RxMsg.RegCount;															//得到数据长度
	switch(CurModbus->RxMsg.CmdCode){
		case S_Modbus_CmdCode_Read:														//读取数据命令			
			T-=Buf->StartAddr;
			RegAddr=Buf->MemAddr+T;
			if(CurModbus->OnReadRegs>0)CurModbus->OnReadRegs(i,T,K);						//调用读数据回调方法
			S_Modbus_MsgInforWrite(CurModbus,S_Modbus_MsgSuccess);						//填充好发送报文头部
			CurModbus->TxMsg.DataAddr=(uint8 *)RegAddr;										//取寄存器数据到缓冲区						
			break;
		case S_Modbus_CmdCode_Write:														//写数据命令			
			if((i==S_Modbus_ParBuffer)||(i==S_Modbus_CmdBuffer)){						//是否写允许
				S_Modbus_MsgInforWrite(CurModbus,S_Modbus_MsgSuccess);					//重新回发数据								
				T-=Buf->StartAddr;
				RegAddr=Buf->MemAddr+T;
				P=CurModbus->RxMsg.DataAddr;												//计算参数区地址
				for(j=0;j<K;j++){
					*RegAddr++=D16FromArray(P,1);
					P+=2;
				}
				if(CurModbus->OnWriteRegs>0)CurModbus->OnWriteRegs(i,T,K);					//参数设置回调													
			}else{
			 	S_Modbus_MsgInforWrite(CurModbus,S_Modbus_InvalidOperation);			//非法操作
			}
			break;
		default:
			S_Modbus_MsgInforWrite(CurModbus,S_Modbus_CmdCodeError);					//未知功能码错误
			break;
	}
}


/*--------------------------------------------------------------------------------------------------
 Func: 初始化MODBUS缓冲区结构					   	   
 	   BufferType: S_Modbus_VarBuffer/S_Modbus_ParBuffer/S_Modbus_CmdBuffer/S_Modbus_EvtBuffer
	   return : 0/设置成功  0xFF/未知缓冲区
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_SetBuffer(S_Modbus *Modbus,uint8 BufferType,uint16 *MemAddr,uint16 StartAddr,uint16 Size)
{
	if(BufferType<4){
		Modbus->TyBuf[BufferType].MemAddr=MemAddr;
		Modbus->TyBuf[BufferType].StartAddr=StartAddr;
		Modbus->TyBuf[BufferType].Size=Size;
		return 0;
	}
	return 0xFF;
}


/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议创建初始化
 Time: 2010-6-29
 Ver.: V1.0
 Note:
--------------------------------------------------------------------------------------------------*/		
void S_Modbus_Create(uint8 CurAddr,uint8 EnBroadcast,uint8 LocalMode,S_Modbus *NewModbus)
{
	NewModbus->LocalAddr=CurAddr;
	NewModbus->LocalMode=LocalMode|EnBroadcast;
	S_Modbus_SetBuffer(NewModbus,S_Modbus_VarBuffer,0,0,0);
	S_Modbus_SetBuffer(NewModbus,S_Modbus_ParBuffer,0,0,0);
	S_Modbus_SetBuffer(NewModbus,S_Modbus_CmdBuffer,0,0,0);
	S_Modbus_SetBuffer(NewModbus,S_Modbus_EvtBuffer,0,0,0);
	NewModbus->OnMsgError=0;
	NewModbus->OnWriteRegs=0;
	NewModbus->OnReadRegs=0;
}


