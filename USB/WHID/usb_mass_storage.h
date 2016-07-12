
#ifndef _TTS_USB_MASS_STORAGE_H_
#define _TTS_USB_MASS_STORAGE_H_

#include <include.h>

#define USB_MASS_ENDP_OUT				2
#define USB_MASS_ENDP_IN				1
#define USB_MASS_ENDP_PACKETSIZE		64

#define b_REQUEST_GET_MAX_LUN			0xFE
#define b_REQUEST_MASS_STORAGE_RESET	0xFF

#define USB_MASS_STORAGE_MAX_LUN		0


extern uint8 USB_MASS_TASKID;

/*-------------------------------------------------------------------------
 Func: 实现Mass Storage的初始化
-------------------------------------------------------------------------*/
void USB_Mass_Storage_Init(void);

/*-------------------------------------------------------------------------
 Func: USB批量数据处理任务
-------------------------------------------------------------------------*/
void USB_MassStorage_Task(void *Tags);


#endif
