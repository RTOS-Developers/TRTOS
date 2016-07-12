#ifndef _SENSOR_H_
#define _SENSOR_H_
#include <Include.h>


typedef enum
{
    VT_End=0x00,//没有任何数据
    VT_Menu=0x01,//菜单型
    VT_Task=0x02,
    VT_Value=0x03,
    VT_Float=0x04,
    VT_Uint16=0x05,
    VT_Int8=0x06,
    VT_TaskF32=0x07,
    VT_ROMFloat=0x08,
    VT_ROMUint32=0x09,
    VT_ROMUint16=0x0a,
    VT_Uint8=0x0b,
    VT_ROMUint8=0x0c,
    VT_ROMStr=0x0d,
    VT_ROMInt8=0x0e,
    VT_ROMInt16=0x0f,
    VT_Uint16Bool=0x10,
    VT_EEPFloat=0x011,
    VT_EEPUint8=0x12,  
    VT_ROMEEPUint8=0x13,
    VT_EEPUint8Task=0x14,
		VT_PageAct=0x15,
		VT_TaskUint8=0x16,
		VT_Uint8Task=0x17,
		VT_BOOL=0x18,
		VT_Null=0x19,
		VT_Str=0x1a,
		VT_IP=0x1b,
		VT_Color=0x1c,
		VT_U32BIT=0x1d
}Value_Type;//低位为小数点位数

typedef struct 
{
  const char *Option;
	void *Value;
  uint8 Type;
	uint8 Note;//菜单类型Note=菜单深度
}COM_MenuStruct;

typedef enum
{
	
	SCFF_GetSensorTest=0,
	SCFF_GetSensorModel=1,//获取传感器型号
	SCFF_GetSensorNumber=2,//获取传感器编号
	SCFF_GetSensorName=3,//获取传感器名称
	SCFF_GetSensorNote=4,//获取传感器备注
	SCFF_GetCPUModel=5,//获取cpu型号
	SCFF_GetHandVersion=6,//硬件版本
	SCFF_GetSoftVersion=7,//软件版本
	SCFF_GetSystemInfor=8,//系统信息
	SCFF_GetLibVersion=9,//获取固件库版本
	SCFF_GetProgTime=10,//获取烧录时间
	SCFF_GetFlashSize=11,//获取flash大小
	SCFF_SetCTRUINT32=21,//一次性设置继电器
	SCFF_GetSIGUINT32=22,//获取输入IO
 	SCFF_ButtonClick=23,//按钮事件
	SCFF_SetWriteAddr=24,//写入写偏移地址
	SCFF_WriteBuffer=25,//写入数组
	SCFF_SetChnenel = 26,//写入操作通道
	SCFF_CorrectWeight=27,//校准重量
	SCFF_ClearWeight=28,//清零重量
	SCFF_CorrectRange=29,//校准幅度
	SCFF_ClearRange=30,//清零幅度
	SCFF_ButtonOver=31,//停止按按钮
	SCFF_GetAllSensorValue=32,//获取所有传感器值	
	SCFF_GetStructOffSet=33,//获取结构体偏移值
	SCFF_SetStructOffSet=34,//设置结构体偏移值
	SCFF_RefurBishGet=35,//下载数据
	SCFF_RefurBishSet=36,//上传数据
	SCFF_SetSaveValue=37,//保存参数
	SCFF_TankSIGUINT32=38,//罐子发送输入信号
	SCFF_TankSetCTRUINT32=39,//罐子继电器设置U32
	SCFF_RefurBishSet1=40,//上传数据
	SCFF_GoToBootLoader=41,//进入下载模式
	SCFF_Write64Byte=42,//写入64字节
	SCFF_Cheack1024Byte=43,//校验1024字节
	SCFF_Write1024Byte=44,//写入1024字节
	SCFF_Read64Byte=45,//写入64字节
	SCFF_Read1024Byte=46,//写入1024字节
	SCFF_EraseFlase=47,//擦除flash
	SCFF_SendCenterRoom=48,
	SCFF_SendSkyBus=49,
	SCFF_SetReadValue=50,//获取恢复配置
	SCFF_SetFullWeight=51,//设置额定重量
	SCFF_SetReg=52,//写寄存器
	SCFF_SetAlarmList=53,//报警参数
	SCFF_GetAlarmList=54,//报警参数
	SCFF_SetIpPortList=55,//设置IP端口
	SCFF_GetIpPortList=56,//设置IP端口
	SCFF_SetCallList=57,//设置电话清单
	SCFF_GetCallList=58,//获取电话清单
	SCFF_GetSensorADC=59,//获取角度模数值
	SCFF_GetSensorValue=60,//获取角度值
	SCFF_SetSensorMax=61,//校准大角度
	SCFF_SetSensorMin=62,//校准小角度
	SCFF_InDeBug=63,//进入调试模式
	SCFF_OutDeBug=64,//退出调试模式
	SCFF_GetCANGroup=65,//获取can通信组
	SCFF_GetCANStdId=66,//获取can通信成员地址
	SCFF_SetCANGroup=67,//设置can通信组
	SCFF_SetCANStdId=68,//设置can通信成员地址
	SCFF_SetCTRBIT=69,//设置继电器按位
	SCFF_ClrCTRBIT=70,//清除继电器按位
	SCFF_GetLoginHead=71,//获取登陆头
	SCFF_SetLoginHead=72,//设置登陆头
	SCFF_GetHeartBeat=73,//获取心跳包
	SCFF_SetHeartBeat=74,//设置心跳包
	SCFF_COMMenuList=75,//串口菜单
	SCFF_SetDeBugMode=76,//设置调试模式
	SCFF_SetDateTime=77,//设置时间
	SCFF_PrintHistory=78,//输出历史记录
	SCFF_CleanHistory=79,//输出历史记录

}Sensor_Command_FrameFlag;


extern  char *String_Hello;
extern  char *String_Ok_ADC;
extern  char *String_Ok;
extern  char *String_Error;
extern  char *String_Wating;

#define SensorModel "Toprie-TP500"
#define SensorHandVersion "V 2.00"
#define SensorSoftVersion "V 1.02"
#define SensorDefNumber "00000000"
#define SensorName "RTU-TP"
#define SensorDefNote "NB"
#endif
