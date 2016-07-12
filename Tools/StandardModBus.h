#ifndef _STANDARDMODBUS_H_
#define _STANDARDMODBUS_H_
#include <Include.h>
#include <Tools_Include.h>
/*
127~~~~~~~110~~~~~~~~~100~~~~~~~~72~~~~~~~~~65~~~~~~~~~~1
  ����������   �û���      ������    �û���				������
*/

typedef enum
{
	MBCC_ReadBits=0x01,//��������ɢֵ
	MBCC_ReadDisValue=0x02,//��������ɢֵ
	MBCC_ReadHoldReg=0x03,//��ȡ���ּĴ���ֵ
	MBCC_ReadInPutReg=0x04,//������Ĵ���
	MBCC_WriteBit=0x05,//д������Ȧ 0XFF00=1  0X0000=0
	MBCC_WriteByte=0x06,//д�����Ĵ���
	MBCC_WriteBytes=0x10,//д����Ĵ���
	MBCC_WRBytes=0x17,//��д����Ĵ���
	MBCC_WriteShield=0x16,//����д�Ĵ���
	MBCC_ReadFileRcord=0x14,//���ļ���¼
	MBCC_WriteFileRcord=0x15,//д�ļ���¼
	MBCC_ReadDevIDInfor=0x2B,//���豸ʶ����
}ModBusCmdCode;

typedef enum
{
	MBEC_NotError=0x00,
	MBEC_CmdCodeError=0x01,
	MBEC_DataLenthError=0x02,
	MBEC_CrcCheckError =0x03,
	MBEC_RegRangeError=0x04,
}ModBusErrorCode;


typedef enum
{
	MBWM_BroadCastEN=S0,//����㲥
	
}ModBusWorkMode;

typedef struct
{
	uint8 LoclAddr;//���ص�ַ
	uint8 WorkMode;//����ģʽ
	uint8 *RxBuf;
	uint16 RxBuf_Length;
	uint8 *TxBuf;
	uint16 TxBuf_Length;
	void *RunValue;//��������ַ
	uint16 RunValue_Length;
	void *CfgValue;//��������ַ
	uint16 CfgValue_Length;
	
}ModBus_Base;

typedef struct
{
	ModBus_Base *BaseInfor;//Э��������Ϣ
	uint8 CmdCode;//������
	
}ModBus_Infor;

#define MB_BroadCast 0xff

void MB_Create(ModBus_Infor *MBI,uint8 LocalAddr,uint8 WorkMode);
uint16 MB_CreateReadMsg(uint8 *Buf,uint8 DevAddr,ModBusCmdCode CmdCode,uint16 Addr,uint16 Length);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUSЭ�鶨��
 Time: 2010-8-10
 Ver.: V1.0
 Note: 												   	   
--------------------------------------------------------------------------------------------------*/
#define S_Modbus_CmdCode_Read 	0x03								    //Modbus������������
#define S_Modbus_CmdCode_Write	0x10									//Modbusд����������

#define S_Modbus_MsgSuccess       0x00
#define S_Modbus_CmdCodeError     0x01
#define S_Modbus_DataLenthError   0x02
#define S_Modbus_CrcCheckError    0x03
#define S_Modbus_RegRangeError    0x04
#define S_Modbus_InvalidOperation 0x05

#define S_Modbus_HostMode         0x00
#define S_Modbus_SlaveMode        0x01

#define S_Modbus_DisBroadcast		0x00
#define S_Modbus_EnBroadcast		0x02

#define S_Modbus_VarBuffer		0x00
#define S_Modbus_ParBuffer		0x01
#define S_Modbus_CmdBuffer		0x02
#define S_Modbus_EvtBuffer		0x03



/*--------------------------------------------------------------------------------------------------
 Func: MODBUS���Ľṹ							   	   
--------------------------------------------------------------------------------------------------*/
typedef struct
{
	uint8  DevAddr;		 				
	uint8  CmdCode;
	uint16 RegStart;
	uint16 RegCount;
	uint16 DataLenth;
	uint8  *DataAddr;
}S_ModbusMsg;

typedef struct
{
	uint16 *MemAddr;
	uint16 Size;
	uint16 StartAddr;
}S_ModbusBuf;


typedef struct 
{
    uint8  LocalAddr;   													//���شӵ�ַ
	uint8  LocalMode;                                                       //ָ��������MODBUS���������Ǵӻ���	
	S_ModbusBuf TyBuf[4];													//Modbus������������ַ ReadOnly
	S_ModbusMsg RxMsg;													//Modbus��֡���ձ���
	S_ModbusMsg TxMsg;													//Modbus��֡���ͱ���		
	void (*OnMsgError) (uint8 ErrorCode);									//Modbus�������ݴ����¼��ص�	
	void (*OnReadRegs) (uint8 RegType,uint16 RegStart,uint16 RegCount);		//Modbus���Ķ������¼��ص�
	void (*OnWriteRegs)(uint8 RegType,uint16 RegStart,uint16 RegCount);		//Modbus����д�����¼��ص�
}S_Modbus;


/*--------------------------------------------------------------------------------------------------
 Func: ��ʼ��MODBUS���ͱ��Ľṹ					   	   
--------------------------------------------------------------------------------------------------*/
void S_Modbus_CreateTxHeader(S_Modbus *CurModbus,uint8 DevAddr,uint8 CmdCode,
							   uint16 RegStart,uint16 RegCount,uint16 DataLenth);
 
/*--------------------------------------------------------------------------------------------------
 Func: ����ModBus���ݱ���
 Note: 
       ��������������: ���ɲ�ѯ����
       ���������ڴӻ�: ��TxMsg->CmdCodeΪ������,�����ɷ������ݱ���
                       ��TxMsg->CmdCodeΪд����,������Ӧ����
                       ���յ��Ƿ�����ִ������,�����ɴ�����״̬����        
 	   ����������Msg->DataLenthָ�������Ƿ�Datas��
 	   Infor: DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+CRC16[2]
	   Input: *Msg/���Ľṹ	 *Buffer/����֡���ݻ�����
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_CreateTxMessage(S_Modbus *CurModbus,uint8 *Buffer);

/*--------------------------------------------------------------------------------------------------
 Func: ��ȡ�������е����ݶ�
 Note: ����������RegCount��Lenth�Զ��ж��Ƿ��к��ʵ�Datas��
 	   Infor:	DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+Datas[RegCount*2]+CRC16[2]
	   Input:	*Data/ԭ�����ݱ���  Lenth/�����ܳ���  *Msg/���صı�����Ϣ
 	   return:  0/������Ϣ��ȡ�ɹ�	11/���ĳ��Ȳ���	12/��������У�����	13/���ݱ��ֳ��ȴ��� 0xFF/��ַ��ƥ��
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_ExpendRxMessage(S_Modbus *CurModbus,uint8 *RxBuffer,uint8 RxLenth);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUSЭ�鸴���ջ���������ͷ�����ͻ�����
 Note: ErrorCode:  0/��ȷ����֡��1/δ֪������  2/δ֪�Ĵ���	 3/�Ƿ�����   4/�Ĵ������ȳ��� 
--------------------------------------------------------------------------------------------------*/
void S_Modbus_MsgInforWrite(S_Modbus *CurModbus,uint8 ErrorCode);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUSЭ�鴦��ֻ������
 Time: 2010-6-29
 Ver.: V1.0
 Note:
--------------------------------------------------------------------------------------------------*/
void S_Modbus_GetLocalRegs(S_Modbus *CurModbus);

/*--------------------------------------------------------------------------------------------------
 Func: ��ʼ��MODBUS�������ṹ					   	   
 	   BufferType: S_Modbus_VarBuffer/S_Modbus_ParBuffer/S_Modbus_CmdBuffer/S_Modbus_EvtBuffer
	   return : 0/���óɹ�  0xFF/δ֪������
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_SetBuffer(S_Modbus *Modbus,uint8 BufferType,
							uint16 *MemAddr,uint16 StartAddr,uint16 Size);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUSЭ�鴴����ʼ��
--------------------------------------------------------------------------------------------------*/		
void S_Modbus_Create(uint8 CurAddr,uint8 EnBroadcast,uint8 LocalMode,S_Modbus *NewModbus);










#endif
