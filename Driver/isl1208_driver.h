
#ifndef _ISL1208_DRIVER_H_
#define _ISL1208_DRIVER_H_

#include <include.h>
#include <Tos_Include.h>

#define ISL1208_ADDR			0x6F
#define ISL1208_RTC_SECOND 		0x00
#define ISL1208_RTC_MINUTE		0x01
#define ISL1208_RTC_HOUR		0x02
#define ISL1208_RTC_DAY			0x03
#define ISL1208_RTC_MONTH		0x04
#define ISL1208_RTC_YEAR		0x05
#define ISL1208_RTC_WEEK		0x06
#define ISL1208_SR_REG			0x07
#define ISL1208_INT_REG			0x08
#define ISL1208_ATR_REG			0x0A
#define ISL1208_DTR_REG			0x0B
#define ISL1208_ALM_SECOND 		0x0C
#define ISL1208_ALM_MINUTE		0x0D
#define ISL1208_ALM_HOUR		0x0E
#define ISL1208_ALM_DAY			0x0F
#define ISL1208_ALM_MONTH		0x10
#define ISL1208_ALM_WEEK		0x11
#define ISL1208_USER_DAT0		0x12
#define ISL1208_USER_DAT1		0x13

uint8 ISL1208_Init(void *GIS);
uint8 ISL1208_GetDateTime(Date_Time *DateTime);
uint8 ISL1208_SetDateTime(Date_Time *DateTime);
uint8 ISL1208_SetAlarm(uint8 Hour,uint8 Min,uint8 Sec,uint8 AlarmFlag);

void DeviceMount_ISL1208(const GPIO_InitStruct *GIS);

#endif
