
#ifndef _TTS_USB_BULKONLY_H_
#define _TTS_USB_BULKONLY_H_

#include <include.h>


#define USB_BOT_CSW_STATUS_PASSED		0x00
#define USB_BOT_CSW_STATUS_FAILED		0x01
#define USB_BOT_CSW_STATUS_ERROR		0x02

#define USB_BOT_CBW_SIGNALTURE			0x43425355
#define USB_BOT_CSW_SIGNALTURE			0x53425355

#define USB_BOT_STAGE_IDLE				0x00			//初始空闲阶段
#define USB_BOT_STAGE_SETUP				0x01			//设置阶段
#define USB_BOT_STAGE_DATAIN			0x02			//数据输入阶段
#define USB_BOT_STAGE_DATAOUT			0x03			//数据输入阶段
#define USB_BOT_STAGE_STATUS			0x04			//状态阶段




typedef struct
{
	uint32 Signalture;					//CBW标签 0x43425355
	uint32 Tag;							//匹配标记,匹配CSW
	uint32 DataTransferLength;			//数据传输长度
	uint8  Flags;						//B7 0/host->dev 1/dev->host
	uint8  Lun;							//逻辑块号
	uint8  CBLength;					//commd block长度
	uint8  CB[16];						//命令码	
}BulkOnly_CBW_TypeDef;

typedef struct							 
{
	uint32 Signalture;					//CSW标签,0x53425355
	uint32 Tag;						   	//匹配标记,匹配CBW
	uint32 DataResidue;					//数据指示
	uint8  Status;						//命令状态
}BulkOnly_CSW_TypeDef;

typedef struct		  	
{							
	uint8  Dtype;						//DBW->Data block wraper, Dtype:返回类型
	uint8  *wAddr;						//返回数据缓冲区
	uint16 Length;						//返回数据长度
}BulkOnly_DBW_TypeDef;

extern BulkOnly_CBW_TypeDef CBW;				//CBW块定义
extern BulkOnly_CSW_TypeDef CSW;				//CSW块定义
extern BulkOnly_DBW_TypeDef DBW;				//DBW块定义
extern uint8 USB_BOT_STAGE;

/*-------------------------------------------------------------------------
 Func: 实现CBW块的解码
-------------------------------------------------------------------------*/
uint8 USB_BulkOnly_CBWDecode(uint8 *Buffer);

/*-------------------------------------------------------------------------
 Func: 实现CSW块的解码
-------------------------------------------------------------------------*/
void USB_BulkOnly_CSWDecode(uint8 *Buffer);  

/*-------------------------------------------------------------------------
 Func: 实现CBW包的打包
-------------------------------------------------------------------------*/
void USB_BulkOnly_CSWEncode(uint8 *Buffer);




#endif

