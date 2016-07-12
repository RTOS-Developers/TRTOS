#ifndef _USART3_DRIVER_H_
#define _USART3_DRIVER_H_
#include <Include.h>


extern uint8  USART3_RX_Buffer[USART_RX_BUFFER_SIZE];
extern uint8  USART3_TX_Buffer[USART_TX_BUFFER_SIZE];
extern uint8 *USART3_RX_Addr;
extern uint16 USART3_RX_Count;
extern uint8 DeviceId_USART3;
extern uint8 DeviceId_USART3_RX;


void USART3_Configuration(void);
void USART3_SendByte(u16 Data);
void USART3Write(u8* data,u16 len);
void USART3_DMAWriteDatas(u8* Data,u16 Length);
void USART3_IRQHandler(void);
void DeviceMount_Usart3(void);
void USART3_Printf(u8 *Data);
void USART3_DMAConfiguration(void);
uint16 USART3_ReadDatas(uint8 *Buf,uint16 Length,uint32 TimeOut);
void Tos_TaskGetUSART3(void);
#endif
