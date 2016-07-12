#ifndef _TOS_DERICE_H_
#define _TOS_DERICE_H_
#include <include.h>


typedef struct
{
	char *DeviceName;    //设备名称
	uint8 DeviceId;       //设备Id
	uint8 DeviceOwnerId;  //设备所属于哪个任务拥有
	uint8 DeviceWaitId;   //等待获取该设备的任务ID
	uint8 DeviceVirtue;   //设备属性
	uint8 DeviceState;    //设备状态
	DeviceWRFunc Write;  //设备写接口
	DeviceWRFunc Read;   //设备读接口
	void  *Init;   //设备初始化接口
	void  *Exit;   //设备退出接口
	uintbus Note;
}DeviceInfor;

enum DericeVirtue//设备属性
{
	DV_Array=1,//组设备
	DV_IOList=2,//IO清单设备
	DV_Task=3,//普通设备
	DV_TaskValue=4,//带参
};
enum DeviceState
{
	Delete   =0,//删除不存在
	NotBusy  =1,//设备空闲状态
	Busy     =2,//设备忙
	Ready    =3 //设备就绪
};

#define DeviceNull 0xff 
#define TosErrorID 0xff
#define FlashConfigNull 0xff
extern DeviceInfor	*Tos_Device_Tab;
extern uint8 DeviceId_Index;
void DeviceList_Init(void);
void Device_Init(void);
BOOL Device_Ok(uint8 ID);
BOOL DeviceUnMount(uint8 ID);
BOOL Tos_TaskGetDev(uint8 ID,uint32 Timeout);
BOOL Tos_TaskDropDev(uint8 ID);
uint8 Device_Count(void);
void DeviceList_Printf(void);
#endif
