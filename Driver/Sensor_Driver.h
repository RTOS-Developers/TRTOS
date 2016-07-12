#ifndef _SENSOR_H_
#define _SENSOR_H_
#include <Include.h>


typedef enum
{
	
	SCFF_GetSensorTest=0,
	SCFF_GetSensorModel=1,//获取传感器型号
	SCFF_GetSensorNumber=2,//获取传感器编号
	SCFF_GetSensorName=3,//获取传感器名称
	SCFF_GetSensorNote=4,//获取传感器备注
	SCFF_GetBSD=5,//获取板载信息
	SCFF_GetCPUModel=6,//获取cpu型号
	SCFF_GetCPUID=7,//获取cpuID
	SCFF_GetCANSpeed=8,//获取CAN通信速率
	SCFF_GetSensorADC=9,//获取角度模数值
	SCFF_GetSensorValue=10,//获取角度值
	SCFF_SetSensorMax=11,//校准大角度
	SCFF_SetSensorMin=12,//校准小角度
	SCFF_InDeBug=13,//进入调试模式
	SCFF_OutDeBug=14,//退出调试模式
	SCFF_GetCANGroup=15,//获取can通信组
	SCFF_GetCANStdId=16,//获取can通信成员地址
	SCFF_SetCANGroup=17,//设置can通信组
	SCFF_SetCANStdId=18,//设置can通信成员地址
	SCFF_SetCTRBIT=19,//设置继电器按位
	SCFF_ClrCTRBIT=20,//清除继电器按位
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
	SCFF_GotoApplication=42,//运行程序
	SCFF_Cheack1024Byte=43,//校验1024字节
	SCFF_Write1024Byte=44,//写入1024字节
	SCFF_Read64Byte=45,//写入64字节
	SCFF_Read1024Byte=46,//写入1024字节
	SCFF_EraseFlase=47,//擦除flash
	SCFF_SendCenterRoom=48,
	SCFF_SendSkyBus=49,
	SCFF_SetReadValue=50,//获取恢复配置
	SCFF_SetFullWeight=51,//设置额定重量
}Sensor_Command_FrameFlag;


extern  char *String_Hello;
extern  char *String_Ok_ADC;
extern  char *String_Ok;
extern  char *String_Error;
extern  char *String_Wating;


#if  defined(BOARD_TANK_Ctr)
#define SensorModel "CX-5410"
#define SensorDefNumber "00000000"
#define SensorName "Remote Control Module"
#define SensorDefNote "-Upper"

#elif defined(BOARD_SENSOR)

	#ifdef SENSOR_TYPE_ANGLE
	#define SensorModel "SC-A-CAN/A-5V"
	#define SensorDefNumber "00000000"
	#define SensorName "Angle Sensor"
	#define SensorDefNote "NB"
	#endif
	#ifdef SENSOR_TYPE_COMPASS
	#include <App_Compass.h>
	#define SensorModel "SC-C-CAN/A-5V"
	#define SensorDefNumber "00000000"
	#define SensorName "Compass Sensor"
	#define SensorDefNote "NB"
	#endif
	#ifdef SENSOR_TYPE_0_20MV_TO_CAN
	#include <App_CAN_0_20mV.h>
	#define SensorModel "SC-G-CAN-5V"
	#define SensorDefNumber "00000000"
	#define SensorName "CAN 0-20mv Sensor"
	#define SensorDefNote "NB"
	#endif
	#ifdef SENSOR_TYPE_CXYG
	#define SensorModel "CX-YG-G"
	#define SensorDefNumber "00000000"
	#define SensorName "CAN 0-20mv Sensor"
	#define SensorDefNote "NB"
	#endif
	#else
	#define SensorModel "Null"
	#define SensorDefNumber "Null"
	#define SensorName "Null"
	#define SensorDefNote "Null"
	#endif

#endif
