
#ifndef _MODBUS_H_
#define _MODBUS_H_

#include <Include.h>

#define ModBus_StartFlag 0x7e
#define ModBus_EndFlag   0x7f

#define ModBus_Broadcast 0xff


typedef enum
{
	ModBus_Ok=0,
	ModBus_SizeError=1,//数据长度错误
	ModBus_CheakError=2,//校验不通过
	ModBus_MsgError=3,//未知命令
	ModBus_DataOver=4, //访问内容不存在
	ModBus_FlagError=5,//起始结束标志错误
	ModBus_AddrError=6,//地址错误
	ModBus_Broad=6,//为广播消息（当然广播消息肯定是有效数据类型
}ModBus_Status;
typedef enum
{
	MBCM_Crc=1,
	MBCM_Xor=2
}ModBus_CheackMode;
typedef enum
{
	WorkMode_Master=0,//主机模式
	WorkMode_Slave=1,//从机模式
}MasterSlaveMode;
typedef struct
{
   uint8 StartFlag;//开始标识（不受接收数据改变）  
   uint8 MasterAddr;//设备地址（不受接收数据改变)
   uint8 SlaveAddr;//主机地址(不受接收数据改变)
   uint8 MsgFlag;//消息标识
   uint8 MsgNum;//消息流水号（通常随机)
   uint16 MsgLength;//消息长度
   uint8 DataFlag;//数据标识
   uint16 DataLength;//数据长度
   uint8 EndFlag;//数据包结束符（不受接收数据改变)
	 uint8 CheakMode;//检验类型(不受接收数据改变)
	 uint8 ErrorFlag;//数据错误存放位置
	 uint8 WorkMode;//modbus协议工作模式
}ModBus_Message;
BOOL MyCom_CheckCrc(uint8 *Buf);
uint16 MyCom_AppendCrc(uint8 *Buf,uint16 Length);
uint8 MyCom_SendMsg(uint8 *Buf,uint16 OnlyAddr,uint8 Cmd,uint8 *Data,uint8 DataLegnth);
uint16 Load_ModBus_Addr(void);
void Save_ModBus_Addr(uint16 Addr);
void ModBus_Create(ModBus_Message *ModBusMsg,uint8 MasterAddr,uint8 SlaveAddr,MasterSlaveMode WorkMode,uint8 CheakMode);
uint16 ModBus_CreateMsg(string Buf,ModBus_Message *ModBusMsg,uint8 MsgFlag,uint8 MsgNum,uint8 DataFlag,uint8 *Data,uint16 DataLength);
uint16 ModBus_Expend(string Buf,uint16 Length,ModBus_Message *ModBusMsg);
void ModBus_Clear(ModBus_Message *ModBusMsg);

#endif
