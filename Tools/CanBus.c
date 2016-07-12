#include <include.h>
#include <Tools_Include.h>
#include <Driver_Include.h>
#include <CanBus.h>
uint8 CANBUS_CFG_OffSet=FlashConfigNull;
CanBus_RxMessage CBRM_Msg[CAN_DeviceTotal];
CANBUS_DataInfor CBDataI[CAN_DataPoolTotal];
CanBus_DevInfor CBDevI;//协议栈基本信息
f32 TestValue=14.332;
/************************************************************* 
*地址划分   主机与主机交流 使用基本ID    主机与从机交流使用扩展ID
*主机通过数据帧与从机进行数据传输，通过远程帧与从机进行危险操作
*被分配了地址的从机在上电后，按一定时间间隔往主机发送心跳包，提醒自己已经连接上了，请求主机处理，主机回发gowork指令命令从机进入工作状态
*主机跟从机发送数据，数据地址栏带从机或广播地址，从机跟主机发送数据时，地址栏却需要带自己地址
主机接收  0xxxxx  0x0003   
从机接收  0x0001~0xfffe   0x0003
*主机数据接口     主机提供数据读出接口为从机提供配置，写入接口用来接收存放从机发过来的实时数据 ，数据实时更新  ，主机在使用完一次数据后对该数据位置进行清理
*从机数据接口     从机提供的数据读出接口供主机进行访问  数据中包含 传感器采集数据或者本地参数     写入接口包括一些控制参数或操作指令
*另外扩展部分包含   传感器与输出控制器直接的传输数据 这些接口都是由主机进行配置  
*************************************************************/

/*************************************************************
Func:  读取CANBUS的通信地址
Date:  2014-10-11
Note:  
*************************************************************/
void Load_CanBus_Addr()
{
	FLASH_ReadDatas(CANBUS_CFG_OffSet,&CBDevI.Addr,1);
}
/*************************************************************
Func:  本地数据供外部访问数据
Date:  2014-10-11
Note:  
*************************************************************/
void Save_CanBus_Addr()
{
	FLASH_WriteDatas(CANBUS_CFG_OffSet,&CBDevI.Addr,1);
}

/*************************************************************
Func:  本地数据供外部访问数据
Date:  2014-10-11
Note:  
*************************************************************/
void CANBUS_CBDataI_Init()
{
	uint16 i;
	for(i=0;i<CAN_DataPoolTotal;i++)
		{
			CBDataI[i].DataType=CBDT_NullData;
			CBDataI[i].Data_P=0;
		}
		CBDevI.CBDataI_Size=0;
}
/*************************************************************
Func:  本地数据供外部访问数据
Date:  2014-10-11
Note:  
*************************************************************/
void CANBUS_JoinVoidValue(uintbus Data,CanBus_DataType Type)
{
	CBDataI[CBDevI.CBDataI_Size].DataType=Type;
	CBDataI[CBDevI.CBDataI_Size++].Data_P=Data;
}

/*************************************************************
Func:  CANBUS协议栈创建
Date:  2014-10-11
Note:  
*************************************************************/
void CANBUS_Create(BOOL HostTrue)
{
	FlashGetConfigRoom(&CANBUS_CFG_OffSet,1);//获取配置，配置大小1个uint16长度
	CBDevI.HostTrue=HostTrue;
}
/*************************************************************
Func:  CANBUS协议栈启动
Date:  2014-10-11
Note:  
*************************************************************/
void CANBUS_Start()
{
	Load_CanBus_Addr();
	CAN1_Init();
	CAN1_Filter_Configs(CBDevI.Addr,CBDevI.HostTrue);
	CBDevI.WorkTrue=True;
}
/*************************************************************
Func:  重启CANBUS协议栈
Date:  2014-10-11
Note:  更改滤波器或地址参数需要重新启动
*************************************************************/
void CANBUS_ReStart()
{
	Load_CanBus_Addr();
	CAN1_Filter_Configs(CBDevI.Addr,CBDevI.HostTrue);
	CBDevI.WorkTrue=True;
}
/*************************************************************
Func:  主机到从机发消息
Date:  2014-10-11
Note:  对方机地址  和数据包
*************************************************************/
BOOL CANBUS_SendMsg(uint16 Addr,CanBus_TxMessage *CBTM)
{
	uint16 TransmitMailbox=0,i=0;
	CanTxMsg  TxMessage;
	TxMessage.ExtId=Addr<<13;
	TxMessage.RTR=CAN_RTR_DATA;//发送数据帧
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.DLC=sizeof(CanBus_TxMessage);
	BufferCoppy(CBTM,&TxMessage.Data[0],sizeof(CanBus_TxMessage));
	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);
	while((CAN_TransmitStatus(CAN1,TransmitMailbox)!=CANTXOK)&&(i!=0xff)) 
	{
	i++;
		Tos_TaskDelay(1);
	}
	if(i==0xff)return False;
	else return True;
}

/*************************************************************
Func:  从设备地址全部清除
Date:  2014-10-11
Note:  针对全部设备有效
*************************************************************/
BOOL CANBUS_TXD_DeviceReset(uint16 Addr)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=Addr;
	CBTM.Order=CBO_DeviceReset;//恢复指令
	R=CANBUS_SendMsg(CanBus_Broadcast,&CBTM);
	return R;
}
/*************************************************************
Func:  从设备地址全部清除
Date:  2014-10-11
Note:  针对全部设备有效
*************************************************************/
void CANBUS_RXD_DeviceReset()
{
	CBDevI.Addr=CanBus_Broadcast;
	Save_CanBus_Addr();
	CANBUS_ReStart();
#ifdef COMPILE_MODE_DEBUG
	DeBug("Device Reset Default",Infor_Infor);
#endif
}
/*************************************************************
Func:  为设备设置地址，已经设置好的地址直接忽略
Date:  2014-10-11
Note: 该地址是为设备分配的地址
*************************************************************/
BOOL CANBUS_TXD_DeviceSetAddr(uint16 Addr)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=CanBus_Broadcast;//通过广播地址发送设置地址指令
	CBTM.Order=CBO_DeviceSetAddr;
	CBTM.DataType=CBDT_Uint16Data;
	CBTM.Data[0]=Addr>>8;
	CBTM.Data[1]=Addr&0xff;
	R=CANBUS_SendMsg(CanBus_Broadcast,&CBTM);//往广播地址写
	return R;
}
/*************************************************************
Func:  为设备设置地址，已经设置好的地址直接忽略
Date:  2014-10-11
Note: 该地址是为设备分配的地址
*************************************************************/
void CANBUS_RXD_DeviceSetAddr(CanBus_TxMessage *CBTM)
{
	Load_CanBus_Addr();
	if(CBDevI.Addr==CanBus_Broadcast)
	{
		CBDevI.Addr=CBTM->Data[0];
		CBDevI.Addr<<=8;
		CBDevI.Addr|=CBTM->Data[1];
		Save_CanBus_Addr();
		CANBUS_ReStart();
	}
}
/*************************************************************
Func:  启动指定设备
Date:  2014-10-11
Note:  Addr为设备地址
*************************************************************/
BOOL CANBUS_TXD_DeviceStartWork(uint16 Addr)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=Addr;//通过广播地址发送设置地址指令
	CBTM.Order=CBO_DeviceStartWork;
	R=CANBUS_SendMsg(Addr,&CBTM);
	return R;
}
/*************************************************************
Func:  停止指定设备
Date:  2014-10-11
Note:  
*************************************************************/
BOOL CANBUS_TXD_DeviceStopWork(uint16 Addr)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=Addr;//通过广播地址发送设置地址指令 
	CBTM.Order=CBO_DeviceStopWork;
	R=CANBUS_SendMsg(Addr,&CBTM);
	return R;
}
/*************************************************************
Func:  像指定的地址发送32位长度数据
Date:  2014-10-11
Note:  
*************************************************************/
BOOL CANBUS_TXD_DeviceSetU32Data(uint16 Addr,uint32 Data)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=Addr;
	CBTM.Order=CBO_DeviceSetData;
	CBTM.DataType=CBDT_Uint32Data;
	D32ToArray(&CBTM.Data[0],Data,0);
	R=CANBUS_SendMsg(Addr,&CBTM);
	return R;
}
/*************************************************************
Func:  按照指定类型发送数据到指定地址
Date:  2014-10-11
Note:  
*************************************************************/
BOOL CANBUS_TXD_DeviceSetData(uint16 Addr,CanBus_DataType CBDT,uintbus Data)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=Addr;
	CBTM.Order=CBO_DeviceSetData;
	CBTM.DataType=CBDT;
	BufferCoppy((void *)Data,&CBTM.Data[0],4);
	R=CANBUS_SendMsg(Addr,&CBTM);
	return R;
}
BOOL CANBUS_RXD_DeviceSetData(CanBus_TxMessage *CBTM)
{
	BOOL R;
	uint8 i;
	BufferCoppy(&CBTM->Data[0],&i,2);
	if(i<CBDevI.CBDataI_Size)
	{
		if(CBTM->DataType==CBDataI[i].DataType)
		{
		BufferCoppy((void *)&CBTM->Data[0],(void *)CBDataI[i].Data_P,4);
		}else return False;
	}
	return R;
}
BOOL CANBUS_TXD_Object(uint16 Addr,CanBus_Order Order,CanBus_DataType CBDT,uintbus Data)
{
	BOOL R;
	CanBus_TxMessage CBTM;
	CBTM.Addr=Addr;
	CBTM.Order=Order;
	CBTM.DataType=CBDT;
	BufferCoppy((void *)Data,&CBTM.Data[0],4);
	R=CANBUS_SendMsg(Addr,&CBTM);
	return R;
}
BOOL CANBUS_TXD_DeviceGetData(uint16 Addr,uint16 Index)
{
	BOOL R;
	R=CANBUS_TXD_Object(Addr,CBO_DeviceGetData,CBDT_Uint16Data,(uintbus)&Index);
	return R;
}
