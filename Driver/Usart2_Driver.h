#ifndef _USART2_DRIVER_H_
#define _USART2_DRIVER_H_
#include <include.h>

extern uint8  USART2_RX_Buffer[USART_RX_BUFFER_SIZE];
extern uint8  USART2_TX_Buffer[USART_TX_BUFFER_SIZE];

extern uint8 DeviceId_USART2;
extern uint8 DeviceId_USART2_RX;

uint16 USART2_ReadDatas(uint8 *Buf,uint16 Length,uint32 TimeOut);
void DeviceMount_Usart2(void);
void USART2_WriteDatas(u8* Data,u16 Length); 
void USART2_SendByte(u16 Data);

#endif

