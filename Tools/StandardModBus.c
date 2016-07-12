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
	if(MBI->BaseInfor->RunValue_Length>(Length+Addr))// ��ַ��ȷ
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
	if(MBI->BaseInfor->WorkMode&MBWM_BroadCastEN)//�㲥
	if(MB_BroadCast==RxBuf[0])
	{
		switch(RxBuf[1])
		{
			
		}
	}
	return Length;
}

/*--------------------------------------------------------------------------------------------------
 Func: ��ʼ��MODBUS���ͱ��Ľṹ
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
 Func: ����ModBus���ݱ���
 Time: 2010-6-28
 Ver.: V1.0
 Note: 
       ��������������: ���ɲ�ѯ����
       ���������ڴӻ�: ��TxMsg->CmdCodeΪ������,�����ɷ������ݱ���
                       ��TxMsg->CmdCodeΪд����,������Ӧ����
                       ���յ��Ƿ�����ִ������,�����ɴ�����״̬����
       
 	   ����������Msg->DataLenthָ�������Ƿ�Datas��
 	   Infor: DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+CRC16[2]
	   Input: *Msg/���Ľṹ	 *Buffer/����֡���ݻ�����
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_CreateTxMessage(S_Modbus *CurModbus,uint8 *TxBuffer)
{
	uint8  *D;
    uint16  L,T;
	uint16  *S;
	S_ModbusMsg *Msg=&(CurModbus->TxMsg);
	TxBuffer[0]=Msg->DevAddr;												//д���豸��ַ�ֽ�
	TxBuffer[1]=Msg->CmdCode;												//д���������ֽ�
	if(!(CurModbus->LocalMode&S_Modbus_SlaveMode)){
		D16ToArray(TxBuffer+2,Msg->RegStart,1);							//д����ʼ��ַ
		D16ToArray(TxBuffer+4,Msg->RegCount,1);							//д��Ĵ�������
		BufferCoppy(Msg->DataAddr,TxBuffer+6,Msg->DataLenth);			//д�������� 
		
	    L=Msg->DataLenth+6;
	}else{
        if(Msg->CmdCode&0x80){                                              //���ɴ�����,�����ִ��
			TxBuffer[2]=(uint8)Msg->DataLenth;
            L=3; 				                                            //д�������
        }else if(Msg->CmdCode==S_Modbus_CmdCode_Write){                  //���ɴӻ��ķ������ݱ���
            TxBuffer[2]=(uint8)Msg->DataLenth;								//д�����ݳ�����
            L=3;
	    }else if(Msg->CmdCode==S_Modbus_CmdCode_Read){                   //���ɴӻ�дӦ����
			TxBuffer[2]=(uint8)Msg->DataLenth;								//д�����ݳ�����			
			S=(uint16 *)Msg->DataAddr;
			D=TxBuffer+3;
			for(T=0;T<Msg->DataLenth;T+=2){
				D16ToArray(D,*S++,1);
				D+=2;
			}
			//S_BytesCopy(TxBuffer+3,Msg->DataAddr,Msg->DataLenth);			//д��������
            L=Msg->DataLenth+3;
	    }
    }
	T=Tools_GetCRC16(TxBuffer,L);          								//��֡����У����
	D16ToArray(TxBuffer+L,T,0);											//д��CRC16��֡β��
	return L+2;								                				//�趨����֡����
}


/*--------------------------------------------------------------------------------------------------
 Func: ��ȡ�������е����ݶ�
 Time: 2010-6-28
 Ver.: V1.0
 Note: ����������RegCount��Lenth�Զ��ж��Ƿ��к��ʵ�Datas��
 	   Infor:	DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+Datas[RegCount*2]+CRC16[2]
	   Input:	*Data/ԭ�����ݱ���  Lenth/�����ܳ���  *Msg/���صı�����Ϣ
 	   return:  0/������Ϣ��ȡ�ɹ�	11/���ĳ��Ȳ���	12/��������У�����	13/���ݱ��ֳ��ȴ��� 0xFF/��ַ��ƥ��
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_ExpendRxMessage(S_Modbus *CurModbus,uint8 *RxBuffer,uint8 RxLenth)
{	
	uint16 T,P;
	S_ModbusMsg *Msg=&(CurModbus->RxMsg);
	if(RxLenth<5)return S_Modbus_DataLenthError;								//�жϱ��ĳ����Ƿ�����
	if((*RxBuffer)!=(CurModbus->LocalAddr)){									//��鵥����ַѡ��
		if((*RxBuffer)==0xFF){
			if(!(CurModbus->LocalMode&S_Modbus_EnBroadcast)){					//���㲥ѡ��
				return 0xFF;
			}
		}else return 0xFF;
	}	
	T=D16FromArray(RxBuffer+RxLenth-2,0);									//ȡ������CRC16��
	P=Tools_GetCRC16(RxBuffer,RxLenth-2);									//��ȡ����CRC16������
	if(T!=P)return S_Modbus_CrcCheckError;									//����CRC16���Ƿ���ȷ
	Msg->DevAddr=*RxBuffer++;													//ȡ�豸��ַ
	Msg->CmdCode=*RxBuffer++;													//ȡ������
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
		Msg->RegStart=D16FromArray(RxBuffer,1);								//ȡ��ʼ�Ĵ�����ַ	
		Msg->RegCount=D16FromArray(RxBuffer+2,1);							//ȡ�Ĵ�������
	    RxLenth-=8;Msg->DataLenth=RxLenth;T=Msg->RegCount<<1;					//��ȡ���ĳ���
	    if(RxLenth==0){return S_Modbus_MsgSuccess;}   						//�Ƿ��ѯ֡
	    if(T!=RxLenth){Msg->DataLenth=0;return S_Modbus_DataLenthError;}		//�ж����ݲ����Ƿ񳤶���ȷ			
        Msg->DataAddr=RxBuffer+4;         										//ȡ�������ݲ���	
	}
	return S_Modbus_MsgSuccess;
}


/*--------------------------------------------------------------------------------------------------
 Func: MODBUSЭ�鸴���ջ���������ͷ�����ͻ�����
 Time: 2010-6-29
 Ver.: V1.0
 Note: ErrorCode:  0/��ȷ����֡��1/δ֪������  2/δ֪�Ĵ���	 3/�Ƿ�����   4/�Ĵ������ȳ��� 
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
 Func: MODBUSЭ�鴦��ֻ������
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
	if(i==4){S_Modbus_MsgInforWrite(CurModbus,S_Modbus_RegRangeError);return ;}			//δ֪�Ĵ���
	K=CurModbus->RxMsg.RegCount;															//�õ����ݳ���
	switch(CurModbus->RxMsg.CmdCode){
		case S_Modbus_CmdCode_Read:														//��ȡ��������			
			T-=Buf->StartAddr;
			RegAddr=Buf->MemAddr+T;
			if(CurModbus->OnReadRegs>0)CurModbus->OnReadRegs(i,T,K);						//���ö����ݻص�����
			S_Modbus_MsgInforWrite(CurModbus,S_Modbus_MsgSuccess);						//���÷��ͱ���ͷ��
			CurModbus->TxMsg.DataAddr=(uint8 *)RegAddr;										//ȡ�Ĵ������ݵ�������						
			break;
		case S_Modbus_CmdCode_Write:														//д��������			
			if((i==S_Modbus_ParBuffer)||(i==S_Modbus_CmdBuffer)){						//�Ƿ�д����
				S_Modbus_MsgInforWrite(CurModbus,S_Modbus_MsgSuccess);					//���»ط�����								
				T-=Buf->StartAddr;
				RegAddr=Buf->MemAddr+T;
				P=CurModbus->RxMsg.DataAddr;												//�����������ַ
				for(j=0;j<K;j++){
					*RegAddr++=D16FromArray(P,1);
					P+=2;
				}
				if(CurModbus->OnWriteRegs>0)CurModbus->OnWriteRegs(i,T,K);					//�������ûص�													
			}else{
			 	S_Modbus_MsgInforWrite(CurModbus,S_Modbus_InvalidOperation);			//�Ƿ�����
			}
			break;
		default:
			S_Modbus_MsgInforWrite(CurModbus,S_Modbus_CmdCodeError);					//δ֪���������
			break;
	}
}


/*--------------------------------------------------------------------------------------------------
 Func: ��ʼ��MODBUS�������ṹ					   	   
 	   BufferType: S_Modbus_VarBuffer/S_Modbus_ParBuffer/S_Modbus_CmdBuffer/S_Modbus_EvtBuffer
	   return : 0/���óɹ�  0xFF/δ֪������
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
 Func: MODBUSЭ�鴴����ʼ��
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


