#ifndef _CANBUS_H_
#define _CANBUS_H_
#include <Include.h>

#ifndef CAN_DeviceTotal
	#define CAN_DeviceTotal 10
#endif
#define CAN_DataPoolTotal 10

#define CanBus_Broadcast 0xffff
typedef enum
{
	CBO_DeviceReset=1,
	CBO_DeviceSetAddr=2,
	CBO_DeviceStartWork=3,
	CBO_DeviceStopWork=4,
	CBO_DeviceGetData=5,
	CBO_DeviceSetData=6,
	
}CanBus_Order;
typedef enum
{
	CBT_WorkOk=0,
	CBT_SensorError=1,
	CBT_CollectionTimeout=2,
	CBT_WorkStop=3,
	
}CanBus_Status;

typedef enum
{
	CBDT_NullData=0,
	CBDT_Bit8Data=1,
	CBDT_BytesData=2,
	CBDT_Uint8Data=3,
	CBDT_Int8Data=4,
	CBDT_Uint16Data=5,
	CBDT_Int16Data=6,
	CBDT_Uint32Data=7,
	CBDT_Int32Data=8,
	CBDT_Float32Data=9,
	CBDT_Bit16Data=10,
	CBDT_Bit32Data=11,
}CanBus_DataType;
typedef struct
{
	uint16 Addr; //从机地址
	uint8 Status;//从机状态
	CanBus_DataType DataType;//数据类型
	uint8 Data[4];
}CanBus_RxMessage;
typedef struct CanBus_DevInfor
{
	BOOL HostTrue;
	BOOL WorkTrue;
	uint16 CBDataI_Size;
	uint16 Addr;
}CanBus_DevInfor;

typedef struct
{
	uint16 Addr; //从机地址
	CanBus_Order Order;//命令
	CanBus_DataType DataType;//数据类型
	uint8 Data[4];
}CanBus_TxMessage;

typedef struct
{
	CanBus_DataType DataType;//数据类型
	uintbus Data_P;
}CANBUS_DataInfor;


extern CanBus_RxMessage CBRM_Msg[CAN_DeviceTotal];
extern CanBus_DevInfor CBDevI;
extern CANBUS_DataInfor CBDataI[CAN_DataPoolTotal];

void Load_CanBus_Addr(void);
void Save_CanBus_Addr(void);
BOOL CANBUS_SendMsg(uint16 SlaveAddr,CanBus_TxMessage *CBTM);
BOOL CANBUS_TXD_DeviceReset(uint16 Addr);
BOOL CANBUS_TXD_DeviceSetAddr(uint16 Addr);
BOOL CANBUS_TXD_DeviceStartWork(uint16 Addr);
BOOL CANBUS_TXD_DeviceSetU32Data(uint16 Addr,uint32 Data);
BOOL CANBUS_TXD_DeviceSetData(uint16 Addr,CanBus_DataType CBDT,uintbus Data);
BOOL CANBUS_TXD_DeviceGetData(uint16 Addr,uint16 Index);

void CANBUS_Create(BOOL HostTrue);
void CANBUS_Start(void);
void CANBUS_ReStart(void);
void CANBUS_RXD_DeviceReset(void);
void CANBUS_RXD_DeviceSetAddr(CanBus_TxMessage *CBTM);
BOOL CANBUS_RXD_DeviceSetData(CanBus_TxMessage *CBTM);

void Printf_CanBus_TxMessage(CanBus_TxMessage *CBTM);
void CANBUS_CBDataI_Init(void);
void CANBUS_JoinVoidValue(uintbus Data,CanBus_DataType Type);
#endif
