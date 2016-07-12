#ifndef _USART1_DRIVER_H_
#define _USART1_DRIVER_H_
#include <Include.h>


extern uint8  USART1_RX_Buffer[USART_RX_BUFFER_SIZE];
extern uint8  USART1_TX_Buffer[USART_TX_BUFFER_SIZE];
extern uint8 *USART1_RX_Addr;
extern uint16 USART1_RX_Count;
extern uint8 DeviceId_USART1;
extern uint8 DeviceId_USART1_RX;


void USART1_Configuration(void);
void USART1_SendByte(u16 Data);
void USART1Write(u8* data,u16 len);
void USART1_DMAWriteDatas(u8* Data,u16 Length);
void USART1_IRQHandler(void);
void DeviceMount_Usart1(void);
void USART1_Printf(u8 *Data);
void USART1_DMAConfiguration(void);
uint16 USART1_ReadDatas(uint8 *Buf,uint16 Length,uint32 TimeOut);
void USART1_Struct_Init(uint32 Baud);

#endif
