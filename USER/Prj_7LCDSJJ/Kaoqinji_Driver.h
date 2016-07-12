#ifndef KAOQINJI_DRIVER_H_
#define KAOQINJI_DRIVER_H_
#include <Include.h>


typedef struct
{
	uint8 DriverID[18];
	uint8 UserName[10];
	uint8 UserID[8];
	uint16 IDCrc;
	uint8 DateTimeTemp[8];
	uint32 PictureSize;
	uint8 PicBuf[10000];
	uint8 TempBuf[20];
	uint8 DeverType;
	uint8 FaceFesh;
}UserInforStruct;

void KQ_Command(void *Tags);
void TaskSendPicture(void *Tags);

#endif
