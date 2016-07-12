#ifndef _CAN1_DRIVER_H_
#define _CAN1_DRIVER_H_

#include <include.h>

#define CAN_GROUP   0x0000

void CAN1_Init(void);
void CAN1_PoolBackTest(void);
void CAN1_SendMsg(void *Msg,uint8 Length);
void Tos_TaskGetCAN(void);
void CAN1_NVIC_Cfg(void);
void CAN1_GPIO_Init(void);
void CAN1_Filter_Configs(uint16 LocalAddr,BOOL Host);
void CAN_SIG_Init(void);
void CAN_SIG_SendMsg(uint8 *Data,uint32 Addr);
void CAN_SIG_Filter_Configs(void);
void DeviceMount_Can1(void);
#endif


