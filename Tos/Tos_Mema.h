#ifndef _TOS_MEMA_H_
#define _TOS_MEMA_H_
#include <Include.h>

typedef struct TOS_MEMA_Ptr
{
	struct TOS_MEMA_Ptr *NextPtr;
	uint32 Size;
}TOS_MEMA_Ptr;
extern uint8 MemBuf[MEMA_BUFFER_SIZE];
void Mema_Init(uint8 *Addr);
void *Mema_Malloc(uint16 Size);
void Mema_Free(void *Addr);
uint32 Mema_Count(void);
void *Mema_Extend(void *Addr,uint16 Size);
#endif
