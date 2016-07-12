
#ifndef _TTS_USB_DRIVER_H_
#define _TTS_USB_DRIVER_H_

#include <Include.h>	
#include <..\USB\USB_Include.h>


/*定义端点缓冲区描述表*/			  
typedef struct
{		 
  __IO uint16_t TXADDR;	   	//0x00			//Cortex-M3 字对齐
  	   uint16_t REVD0;		//0x02
  __IO uint16_t TXCOUNT;  	//0x04
  	   uint16_t REVD1;		//0x06
  __IO uint16_t RXADDR;	   	//0x08
  	   uint16_t REVD2;		//0x0A
  __IO uint16_t RXCOUNT;    //0x0C
  	   uint16_t REVD3;		//0x0E
}USBEDP_TypeDef;
/*定义USB模块结构*/
typedef struct
{
  __IO uint32_t EPR[8];		//0x00-0x1C		//0x40005C00
  	   uint32_t REVD0[8];	//0x20-0x3F
  __IO uint32_t CNTR;		//0x40
  __IO uint32_t	ISTR;		//0x44
  __IO uint32_t FNR;		//0x48
  __IO uint32_t DADDR;		//0x4C
  __IO uint32_t BTABLE;		//0x50	   		//0x40005C50
  	   uint32_t REVD1[235];	//0x4
  USBEDP_TypeDef ENDP[8];	//0x00-0x80		//0x40006000
  	   uint8_t  Buffer;		//0x80
}USB_TypeDef;

#define USB_BASE            0x40005C00
#define USB_PMA_BASE		0x40006000
#define USB					((USB_TypeDef *) USB_BASE)

#define  USB_EPR_EA                         ((uint16_t)0x000F)            /*!<Endpoint Address */
#define  USB_EPR_STAT_TX                    ((uint16_t)0x0030)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EPR_STAT_TX_0                  ((uint16_t)0x0010)            /*!<Bit 0 */
#define  USB_EPR_STAT_TX_1                  ((uint16_t)0x0020)            /*!<Bit 1 */			  
#define  USB_EPR_DTOG_TX                    ((uint16_t)0x0040)            /*!<Data Toggle, for transmission transfers */
#define  USB_EPR_CTR_TX                     ((uint16_t)0x0080)            /*!<Correct Transfer for transmission */
#define  USB_EPR_EP_KIND                    ((uint16_t)0x0100)            /*!<Endpoint Kind */	  
#define  USB_EPR_EP_TYPE                    ((uint16_t)0x0600)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EPR_EP_TYPE_0                  ((uint16_t)0x0200)            /*!<Bit 0 */
#define  USB_EPR_EP_TYPE_1                  ((uint16_t)0x0400)            /*!<Bit 1 */			  
#define  USB_EPR_SETUP                      ((uint16_t)0x0800)            /*!<Setup transaction completed */
#define  USB_EPR_STAT_RX                    ((uint16_t)0x3000)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EPR_STAT_RX_0                  ((uint16_t)0x1000)            /*!<Bit 0 */
#define  USB_EPR_STAT_RX_1                  ((uint16_t)0x2000)            /*!<Bit 1 */						 
#define  USB_EPR_DTOG_RX                    ((uint16_t)0x4000)            /*!<Data Toggle, for reception transfers */
#define  USB_EPR_CTR_RX                     ((uint16_t)0x8000)            /*!<Correct Transfer for reception */

/*stm32_usb TX/RX_CODE*/
#define USB_EPR_STAT_TX_DIS						0x0000
#define USB_EPR_STAT_TX_STALL					0x0010
#define USB_EPR_STAT_TX_NAK						0x0020
#define USB_EPR_STAT_TX_VALID					0x0030

/*stm32_usb TX/RX_CODE*/
#define USB_EPR_STAT_RX_DIS						0x0000
#define USB_EPR_STAT_RX_STALL					0x1000
#define USB_EPR_STAT_RX_NAK						0x2000
#define USB_EPR_STAT_RX_VALID					0x3000

/*stm32 usb endptype code*/
#define USB_EPR_TYPE_BULK	  					0x0000
#define USB_EPR_TYPE_CONTROL					0x0200
#define USB_EPR_TYPE_ISO						0x0400
#define USB_EPR_TYPE_INTERRUPT					0x0600

#define USB_RESULT_SUCCESS 						0x00
#define USB_RESULT_UNSUPPORT					0x01
#define USB_RESULT_NOTREADY						0x02
#define USB_RESULT_ERROR						0x03
#define USB_RESULT_LASTDATA						0x04
#define USB_RESULT_RETURNCSW					0x05


#define USB_CONNECT_FULLSPEED	GPIOA->BSRR=S10;				   //DP->D+ ->1
#define USB_DISCONN_FULLSPEED	GPIOA->BRR=	S10;							   

#define USB_CONNECT_LOWSPEED			   //DM->D-	->1
#define USB_DISCONNECT_ALL				   //D+/D-  ->0

/*bmRequest_type*/
#define bm_REQUEST_TYPE_STANDARD	0x00				
#define bm_REQUEST_TYPE_CLASS		0x20
#define bm_REQUEST_TYPE_VENDOR		0x40						 

/*bmRequest_recipient*/
#define bm_REQUEST_RECIPIENT_DEVICE		0x00
#define bm_REQUEST_RECIPIENT_INTERFACE	0x01
#define bm_REQUEST_RECIPIENT_ENDPOINT	0x02
#define bm_REQUEST_RECIPIENT_OTHER		0x03

/*bRequest*/
#define b_REQUEST_GET_STATUS	  		0x00
#define b_REQUEST_CLEAR_FEATURE			0x01
#define b_REQUEST_SET_FEATURE			0x03
#define b_REQUEST_SET_ADDRESS		   	0x05
#define b_REQUEST_GET_DESCRIPTOR		0x06
#define b_REQUEST_SET_DESCRIPTOR		0x07
#define b_REQUEST_GET_CONFIGURATION		0x08
#define b_REQUEST_SET_CONFIGURATION		0x09
#define b_REQUEST_GET_INTERFACE			0x0A
#define b_REQUEST_SET_INTERFACE			0x0B
#define b_REQUEST_SYNCH_FRAME			0x0C

/*descriptor type GET/SET*/
#define USB_DESCRIPTOR_TYPE_DEVICE			0x01
#define USB_DESCRIPTOR_TYPE_CONFIGURATION	0x02
#define USB_DESCRIPTOR_TYPE_STRING			0x03
#define USB_DESCRIPTOR_TYPE_INTERFACE		0x04
#define USB_DESCRIPTOR_TYPE_ENDPOINT		0x05
#define USB_DESCRIPTOR_TYPE_DEVICEQUALIFIER 0x06
#define USB_DESCRIPTOR_TYPE_OTHER_CONFIG	0x07
#define USB_DESCRIPTOR_TYPE_INTERFACE_POWER	0x08
#define USB_DECSRIPTOR_TYPE_HID_CONFIG		0x21
#define USB_DECSRIPTOR_TYPE_HID_REPORT		0x22

/*feature type define*/
#define FEATURE_DEVICE_REMOTE_WAKEUP	0x01
#define FEATURE_ENDPOINT_HALT		   	0x00
#define FEATURE_TEST_MODE				0x02

/*端点类型*/
#define USB_ENDPOINT_TYPE_CONTROL		0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS	0x01
#define USB_ENDPOINT_TYPE_BULK			0x02
#define USB_ENDPOINT_TYPE_INTERRUPT		0x03

/*usb state type*/
#define USB_STATE_INIT					0x00			//初始状态
#define USB_STATE_SETUP					0x01			//建立事务阶段
#define USB_STATE_DATA_IN				0x02			//数据输入阶段
#define USB_STATE_DATA_OUT				0x03			//数据输出阶段
#define USB_STATE_WAIT_OUT				0x04			//等待状态输出阶段
#define USB_STATE_WAIT_IN				0x05			//等待状态输入阶段
#define USB_STATE_STALLED				0x06			//事务结束停止阶段
#define USB_STATE_SET_ADDRESS			0x07		  	//配置地址阶段----->也属于SETUP阶段


























/*定义USB请求结构*/
typedef struct
{
	uint8  bmRequestType;							   
	uint8  bRequest;
	uint16 wValue;
	uint16 wIndex;
	uint16 wLength;
}USB_RequestDef;

/*定义USB当前控制状态*/
typedef struct
{																					 
	uint8 CurState;		  				//当前状态
	uint8 CurFeature;					//当前功能
	uint8 CurInterface;					//当前有效接口
	uint8 CurConfiguration;				//当前配置
	//uint8 *WriteAddr;					//控制端点0写缓冲地址
	//uint16 WriteLength;					//控制端点0写缓冲长度
}USB_StateDef;

/*定义USB端点状态*/
typedef struct
{
	uint8  CurState;
	uint8 *wAddr;	
	uint8 *rAddr;	
	uint16 wLength;
	uint16 rLength;
	uint16 wOffset;	
	uint16 rOffset;
}USB_EndpDef;


#define USB_ENDP0_PACKETSIZE	32			//定义USB控制端点0的缓冲区大小 双向

extern USB_RequestDef 	USB_REQ;			//当前的数据请求包
extern USB_StateDef	  	USB_STATE;			//当前USB相关状态
extern USB_EndpDef      USB_Endp0;			//控制端点0

extern const uint8 *USB_DeviceDesc;			//用户定义的设备描述表
extern const uint8 *USB_ConfigDesc;			//用户定义的配置描述表
extern const uint8 *USB_ReportDesc;			//用户定义的报告描述表
extern const uint8 *USB_StringDesc[5];		//用户定义的语言描述表
extern uint8 (*USB_ClassSpecial_Request)(void);
extern uint8 (*USB_VendorType_Request)(void);
extern Func USB_EndpointConfig;				//USB在reset之后回调,端点0已由系统初始化
extern Func USB_ENDP_DATA_IN[8];			//用户处理非控制端点DATA_IN事务
extern Func USB_ENDP_DATA_OUT[8];			//用户处理非控制端点DATA_OUT事务

void SystemUSB_Config(uint8 Mode);
void USB_ConfigEndp(uint8 EndpId,uint16 BufferSize,uint16 EndpFlag);
/*-----------------------------------------------------------------------
 Func: USB驱动 从端点接收数据包
-----------------------------------------------------------------------*/
uint16 USB_RecvPacket(uint8 EndpId,USB_EndpDef *Endp,uint8 *rBuffer,uint16 RxCount);
/*-----------------------------------------------------------------------
 Func: USB驱动 向端点发送数据包
-----------------------------------------------------------------------*/
uint8 USB_SendPacket(uint8 EndpId,USB_EndpDef *Endp,void *WriteBuffer,uint16 TxCount);

/*-----------------------------------------------------------------------
 Func: USB驱动 从端点缓冲区n获取数据
 ----------------------------------------------------------------------*/
uint8 USB_ReadBuffer(uint8 nEDP,uint8 *RxBuffer,uint16 RxCount);


void USB_Reset(void);
void USB_Init(void);
void USB_SetTxRxMode(uint8 EndpId,uint16 TxRxMode);


#endif




