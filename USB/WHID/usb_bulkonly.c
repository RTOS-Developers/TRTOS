
#include <include.h>
#include <..\USB\USB_Include.h>
#include <Text.h>

BulkOnly_CBW_TypeDef CBW;				//CBW块定义
BulkOnly_CSW_TypeDef CSW;				//CSW块定义
BulkOnly_DBW_TypeDef DBW;				//DBW块定义

uint8 USB_BOT_STAGE;					//Bulkonly状态

/*-------------------------------------------------------------------------
 Func: 实现CBW块的解码
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
uint8 USB_BulkOnly_CBWDecode(uint8 *Buffer)
{
	uint8 R;
	CBW.Signalture			=D32FromArray(Buffer,0);				//返回小端格式下32位整型
	CBW.Tag					=D32FromArray(Buffer+4,0);				//
	CBW.DataTransferLength	=D32FromArray(Buffer+8,0);
	CBW.Flags				=*(Buffer+12);
	CBW.Lun					=*(Buffer+13);
	CBW.CBLength			=*(Buffer+14);
	BufferCoppy(Buffer+15,CBW.CB,CBW.CBLength);
	DBW.Length				=CBW.DataTransferLength;					//默认返回数据长度
	R=USB_ScsiDecode(CBW.CB);										   	//解码command block域
	CSW.Signalture			=USB_BOT_CSW_SIGNALTURE;
	CSW.Tag					=CBW.Tag;
	CSW.DataResidue			=CBW.DataTransferLength;		
	if(R==USB_RESULT_SUCCESS){
		CSW.Status=USB_BOT_CSW_STATUS_PASSED;
		CSW.DataResidue -= DBW.Length;
	}else if(R==USB_RESULT_RETURNCSW){
		CSW.Status=USB_BOT_CSW_STATUS_PASSED;
		CSW.DataResidue -= DBW.Length;	 	
	}else if(R==USB_RESULT_UNSUPPORT){
		CSW.Status=USB_BOT_CSW_STATUS_FAILED;
	}else if(R==USB_RESULT_NOTREADY){
		CSW.Status=USB_BOT_CSW_STATUS_PASSED;
	}
	return R;
}


/*-------------------------------------------------------------------------
 Func: 实现CSW块的解码
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_BulkOnly_CSWDecode(uint8 *Buffer)
{
	CSW.Signalture			=D32FromArray(Buffer,0);				//返回小端格式下32位整型
	CSW.Tag					=D32FromArray(Buffer+4,0);				//
	CSW.DataResidue			=D32FromArray(Buffer+8,0);
	CSW.Status				=*(Buffer+12);
}													   

/*-------------------------------------------------------------------------
 Func: 实现CBW包的打包PAI
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_BulkOnly_CSWEncode(uint8 *Buffer)
{
	D32ToArray(Buffer,CSW.Signalture,0);
	D32ToArray(Buffer+4,CSW.Tag,0);
	D32ToArray(Buffer+8,CSW.DataResidue,0);
	*(Buffer+12)=CSW.Status;													  
}




