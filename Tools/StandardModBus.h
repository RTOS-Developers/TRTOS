#ifndef _STANDARDMODBUS_H_
#define _STANDARDMODBUS_H_
#include <Include.h>
#include <Tools_Include.h>
/*
127~~~~~~~110~~~~~~~~~100~~~~~~~~72~~~~~~~~~65~~~~~~~~~~1
  公共功能码   用户码      公共码    用户码				公共码
*/

typedef enum
{
	MBCC_ReadBits=0x01,//读输入离散值
	MBCC_ReadDisValue=0x02,//读输入离散值
	MBCC_ReadHoldReg=0x03,//读取保持寄存器值
	MBCC_ReadInPutReg=0x04,//读输入寄存器
	MBCC_WriteBit=0x05,//写单个线圈 0XFF00=1  0X0000=0
	MBCC_WriteByte=0x06,//写单个寄存器
	MBCC_WriteBytes=0x10,//写多个寄存器
	MBCC_WRBytes=0x17,//读写多个寄存器
	MBCC_WriteShield=0x16,//屏蔽写寄存器
	MBCC_ReadFileRcord=0x14,//读文件记录
	MBCC_WriteFileRcord=0x15,//写文件记录
	MBCC_ReadDevIDInfor=0x2B,//读设备识别码
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
	MBWM_BroadCastEN=S0,//允许广播
	
}ModBusWorkMode;

typedef struct
{
	uint8 LoclAddr;//本地地址
	uint8 WorkMode;//工作模式
	uint8 *RxBuf;
	uint16 RxBuf_Length;
	uint8 *TxBuf;
	uint16 TxBuf_Length;
	void *RunValue;//缓存区地址
	uint16 RunValue_Length;
	void *CfgValue;//缓存区地址
	uint16 CfgValue_Length;
	
}ModBus_Base;

typedef struct
{
	ModBus_Base *BaseInfor;//协议配置信息
	uint8 CmdCode;//命令码
	
}ModBus_Infor;

#define MB_BroadCast 0xff

void MB_Create(ModBus_Infor *MBI,uint8 LocalAddr,uint8 WorkMode);
uint16 MB_CreateReadMsg(uint8 *Buf,uint8 DevAddr,ModBusCmdCode CmdCode,uint16 Addr,uint16 Length);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议定义
 Time: 2010-8-10
 Ver.: V1.0
 Note: 												   	   
--------------------------------------------------------------------------------------------------*/
#define S_Modbus_CmdCode_Read 	0x03								    //Modbus读变量命令码
#define S_Modbus_CmdCode_Write	0x10									//Modbus写命令命令码

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
 Func: MODBUS报文结构							   	   
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
    uint8  LocalAddr;   													//本地从地址
	uint8  LocalMode;                                                       //指定本机是MODBUS主机方还是从机方	
	S_ModbusBuf TyBuf[4];													//Modbus变量缓冲区地址 ReadOnly
	S_ModbusMsg RxMsg;													//Modbus本帧接收报文
	S_ModbusMsg TxMsg;													//Modbus本帧发送报文		
	void (*OnMsgError) (uint8 ErrorCode);									//Modbus报文内容错误事件回调	
	void (*OnReadRegs) (uint8 RegType,uint16 RegStart,uint16 RegCount);		//Modbus报文读参数事件回调
	void (*OnWriteRegs)(uint8 RegType,uint16 RegStart,uint16 RegCount);		//Modbus报文写参数事件回调
}S_Modbus;


/*--------------------------------------------------------------------------------------------------
 Func: 初始化MODBUS发送报文结构					   	   
--------------------------------------------------------------------------------------------------*/
void S_Modbus_CreateTxHeader(S_Modbus *CurModbus,uint8 DevAddr,uint8 CmdCode,
							   uint16 RegStart,uint16 RegCount,uint16 DataLenth);
 
/*--------------------------------------------------------------------------------------------------
 Func: 生成ModBus数据报文
 Note: 
       本方法对于主机: 生成查询报文
       本方法对于从机: 若TxMsg->CmdCode为读命令,则生成返回数据报文
                       若TxMsg->CmdCode为写命令,则生成应答报文
                       若收到非法不可执行命令,则生成错误码状态报文        
 	   本方法根据Msg->DataLenth指定生成是否含Datas域
 	   Infor: DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+CRC16[2]
	   Input: *Msg/报文结构	 *Buffer/报文帧数据缓冲区
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_CreateTxMessage(S_Modbus *CurModbus,uint8 *Buffer);

/*--------------------------------------------------------------------------------------------------
 Func: 截取请求报文中的数据段
 Note: 本方法根据RegCount和Lenth自动判断是否有合适的Datas域
 	   Infor:	DevAddr[1]+CmdCode[1]+StartReg[2]+RegCount[2]+Datas[RegCount*2]+CRC16[2]
	   Input:	*Data/原如数据报文  Lenth/报文总长度  *Msg/返回的报文信息
 	   return:  0/报文信息提取成功	11/报文长度不足	12/报文数据校验错误	13/数据报分长度错误 0xFF/地址不匹配
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_ExpendRxMessage(S_Modbus *CurModbus,uint8 *RxBuffer,uint8 RxLenth);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议复接收缓冲区报文头到发送缓冲区
 Note: ErrorCode:  0/正确发送帧　1/未知功能码  2/未知寄存器	 3/非法操作   4/寄存器长度超限 
--------------------------------------------------------------------------------------------------*/
void S_Modbus_MsgInforWrite(S_Modbus *CurModbus,uint8 ErrorCode);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议处理只读变量
 Time: 2010-6-29
 Ver.: V1.0
 Note:
--------------------------------------------------------------------------------------------------*/
void S_Modbus_GetLocalRegs(S_Modbus *CurModbus);

/*--------------------------------------------------------------------------------------------------
 Func: 初始化MODBUS缓冲区结构					   	   
 	   BufferType: S_Modbus_VarBuffer/S_Modbus_ParBuffer/S_Modbus_CmdBuffer/S_Modbus_EvtBuffer
	   return : 0/设置成功  0xFF/未知缓冲区
--------------------------------------------------------------------------------------------------*/
uint8 S_Modbus_SetBuffer(S_Modbus *Modbus,uint8 BufferType,
							uint16 *MemAddr,uint16 StartAddr,uint16 Size);

/*--------------------------------------------------------------------------------------------------
 Func: MODBUS协议创建初始化
--------------------------------------------------------------------------------------------------*/		
void S_Modbus_Create(uint8 CurAddr,uint8 EnBroadcast,uint8 LocalMode,S_Modbus *NewModbus);










#endif
