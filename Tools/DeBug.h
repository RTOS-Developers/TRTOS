#ifndef _DEBUG_H_
#define _DEBUG_H_
#include<Include.h>

#define DeBug_LCD    0x01
#define DeBug_USART1  0x02
#define DeBug_USART2 0x04
typedef enum{
	Infor_Error=1,
	Infor_Fail=2,
	Infor_Infor=3,
	Infor_Ask=4,
	Infor_Warning=5,
	Infor_All=0xff
}DeBug_InforType;

typedef void (*DeBug_CallBack)(void *Str,uint8 Length);

void DeBug_Init(void);
void DeBug(char *fmt,...);
void DeBug_Get(void);
void DeBug_Drop(void);
void DeBug_OutSet(uint8 OutSet);
void DeBug_SetType(uint8 Type);
void DeBug_SetCallBack(DeBug_CallBack DBCB);
uint16 USART_WriteLine(USART_TypeDef* USARTx,char *fmt,...);
#endif
