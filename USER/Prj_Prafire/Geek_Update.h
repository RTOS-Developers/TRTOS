#ifndef _GEEK_UPDATE_H_
#define _GEEK_UPDATE_H_
#include <Include.h>
#include <AppConfig.h> 
#define SPASETIME 500
#define BUFFERSIZE 64
#define HOSTADDRESS 0x00
//#define USERS485 
#define USERS232

typedef enum
{
	GC_Erase=0x01,
	GC_Write=0x02,
	GC_Read=0x03,
	GC_Stop=0x04,
	GC_Run=0x05,
	GC_Success=0x06,
	GC_Failure=0x07,
	GC_Send=0x08,
	GC_State=0x09,
	GC_Data=0x0a,
}Geek_Cmd;

typedef struct
{
	ConfigHeadStruct CHS;
	uint8 Addr;
	uint8 Cmd;
	uint16 Length;
	uint32 Tags1;
	uint32 Tags2;
	uint8 *Buf;
}Geek_DatStruct;


#endif
