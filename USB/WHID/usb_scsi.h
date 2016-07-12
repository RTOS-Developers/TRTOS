
#ifndef _TTS_USB_SCSI_H_
#define _TTS_USB_SCSI_H_
													   
#include <include.h>

typedef struct
{											  	
	uint8 	Opcode;				//操作码			BY0
	uint8 	LUN;				//卷编号			BY1
	uint32	LBA;				//操作LBA地址		BY2-5
	uint16	Length;				//返回数据长度
	uint16  Len16;				//16位传送长度		BY7-8
	uint32	Len32;				//32位传送长度		BY6-9
}Scsi_Cmd_TypeDef;



#define SCSI_CMD_FORMAT_UNIT                        0x04
#define SCSI_CMD_INQUIRY                            0x12
#define SCSI_CMD_MODE_SELECT                        0x55
#define SCSI_CMD_MODE_SENSE6 						0x1A
#define SCSI_CMD_MODE_SENSE10	                    0x5A
#define SCSI_CMD_ALLOW_MEDIUM_REMOVAL               0x1E
#define SCSI_CMD_READ10                             0x28
#define SCSI_CMD_READ12                             0xA8
#define SCSI_CMD_READ_CAPACITY                      0x25
#define SCSI_CMD_READ_FORMAT_CAPACITIES             0x23
#define SCSI_CMD_REQUEST_SENSE                      0x03
#define SCSI_CMD_REZERO								0x01
#define SCSI_CMD_SEEK10								0x2B
#define SCSI_CMD_SEND_DIAGNOSTIC                    0x1D
#define SCSI_CMD_START_STOP_UNIT                    0x1B
#define SCSI_CMD_TEST_UNIT_READY                    0x00
#define SCSI_CMD_VERIFY	                            0x2F
#define SCSI_CMD_WRITE10                            0x2A
#define SCSI_CMD_WRITE12                            0xAA
#define SCSI_CMD_WRITE_AND_VERIFY					0x2E



#define SCSI_NO_SENSE		    	                0x00
#define SCSI_RECOVERED_ERROR		                0x01
#define SCSI_NOT_READY		                    	0x02
#define SCSI_MEDIUM_ERROR		                    0x03
#define SCSI_HARDWARE_ERROR		                    0x04
#define SCSI_ILLEGAL_REQUEST		                0x05
#define SCSI_UNIT_ATTENTION		                    0x06
#define SCSI_DATA_PROTECT		                    0x07
#define SCSI_BLANK_CHECK		                    0x08
#define SCSI_VENDOR_SPECIFIC		                0x09
#define SCSI_COPY_ABORTED		                    0x0A
#define SCSI_ABORTED_COMMAND		                0x0B
#define SCSI_VOLUME_OVERFLOW		                0x0D
#define SCSI_MISCOMPARE		                    	0x0E

#define SCSI_INVALID_COMMAND                        0x20
#define SCSI_INVALID_FIELED_IN_COMMAND              0x24
#define SCSI_PARAMETER_LIST_LENGTH_ERROR            0x1A
#define SCSI_INVALID_FIELD_IN_PARAMETER_LIST        0x26
#define SCSI_ADDRESS_OUT_OF_RANGE                   0x21
#define SCSI_MEDIUM_NOT_PRESENT 			    	0x3A
#define SCSI_MEDIUM_HAVE_CHANGED			    	0x28

extern Scsi_Cmd_TypeDef SCSI_CMD;

/*-------------------------------------------------------------------------
 Func: SCSI命令解码
-------------------------------------------------------------------------*/
uint8 USB_ScsiDecode(uint8 *Buffer);

#endif

