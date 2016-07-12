#ifndef _GPRS_DRIVER_H_
#define _GPRS_DRIVER_H_
#include <Include.h>



typedef struct 
{
	uint16 Key;
	void *Value;
  uint8 Type;
	uint8 Note;//菜单类型Note=菜单深度
}GPRS_KeyStruct;


extern Tos_Timer GPRS_Timer[];
void GPRS_GeartBeat(void);
void GPRS_Register(void);
void GPRS_CalibrationTime(void);
void GPRS_RealTimeData(void);
void GPRS_DriverName(void);
void GPRS_Task2(void *Tags);
void GPRS_Maintenance(void);
void GPRS_MaintenanceStart(void);
void GPRS_MaintenanceStop(void);
void TaskSPK(void);
void GPRS_GetLimitSensorConfigs(void);
void GPRS_GetSpeedSensor(void);
void GPRS_GetCurrentLimitSensorADC(void);
void GPRS_GetCurrentFrequencyFrame(void);
uint8 Coppy_DateToBcd(Date *DT,uint8 *Buf);
uint8 Coppy_DateFromBcd(Date *DT,uint8 *Buf);
void GPRS_SendOne(uint8 Offset,uint16 Delay,uint16 LiftCry,Func Task);
uint8 GPRS_CreatePicture(uint8 *Buf,uint8 *Src,uint8 PageSize,uint16 Count,uint16 Index);
#endif
