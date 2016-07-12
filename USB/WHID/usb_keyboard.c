
#include <include.h>
#include <..\USB\WHID\usb_keyboard.h>
#include <usb_driver.h>
#include <tos.h>
#include <Tools_Include.h>


/*--------------------------------------------------------------------------------------
 Func: USB标准设备描述
 Time: 2011-4-27
 Note: 
------------------------------------------------------------------------------------- */
const uint8 USB_KEY_DeviceDesc[] =
  {
    0x12,                       				//device_desc表长度
    USB_DESCRIPTOR_TYPE_DEVICE, 				//设备描述表类
    0x00,                       				//bcdUSB-version	USB2.0
    0x02,
    0x00,                       				//设备类
    0x00,                       				//设备子类
    0x00,                       				//设备协议
    32,					          				//最大包长度	8/16/32/64	ENDP0
    0x83,                       				//厂商ID (0x0483)
    0x04,
    0x10,                       				//产品ID = 0x5750
    0x57,
    0x00,                       				//设备版本
    0x02,
    1,                          				//厂商字符描述表位置索引
    2,                          				//产品字符描述表位置索引
    3,                          				//设备串号字符描述表索引
    0x01                        				//配置描述表项数
  };											


/*----------------------------------------------------------------------------------------
 Func: 数据报字符串描述串	  total 9bytes packet
----------------------------------------------------------------------------------------*/
const uint8 USB_KEY_ReportDesc[] =
{			
	0x05,0x01,	//	Usage Page(Generic Desktop) 	Global|Size1	  01/GenDestop 07/Keyboard 08/LEDs 09/Button
	0x09,0x06,	//	Usage	(Keyboard)			  	Local |Size1	  01/Pointer 02/Mouse 04/Joystick 06/Keyboard
	0xA1,0x01,	//	Collection(Application)		Main  |Size1 ->Collection
	/*1bytes 8个静态复合按键*/
	0x05,0x07,	//	Usage Page(KeyCodes)
	0x19,0xE0,	//	Usage Minimum(224)			Local |Size1
	0x29,0xE7,	//	Usage Maximum(231)			Local |Size1
	0x15,0x00,	//	Logical Minimum(0)			Global|Size1
	0x25,0x01,	//	Logical Maximum(1)			Global|Size1
	0x75,0x01,	//	Report Size(1)				Global|Size1
	0x95,0x08,	//	Report Count(8)				Global|Size1
	0x81,0x02,	//	Input(Data,Variable,Absolute)Main |Size1
	/*1bytes 保留字节*/	
	0x95,0x01,	//	Report Count(1)				Global|Size1	  
	0x75,0x08,	//	Report Size(8)				Global|Size1	
	0x81,0x01,	//	Input(Constant)
	/*1bytes 低5位控制LED键盘灯 高3位保留*/
	0x95,0x05,	//	Report Count(5)
	0x75,0x01,	//	Report Size(1)
	0x05,0x08,	//	Usage Page(Page# for Leds)
	0x19,0x01,	//	Usage Minimum(1)
	0x29,0x05,	//	Usage Maxumum(5)
	0x91,0x02,	//	Output(Data,Variable,Absolute)
	0x95,0x01,	//	Report Count(1)
	0x75,0x03,	//	Report Size(3)
	0x91,0x01,	//	Output (Constant)
	/*6bytes 传递键值*/
	0x95,0x06,	//	Report Count(6)
	0x75,0x08,	//	Report Size(8)
	0x15,0x00,	//	Logical Minimum(0)
	0x25,0x65,	//	Logical Maximum(101)
	0x05,0x07,	//	Usage Page(Key Codes)
	0x19,0x00,	//	Usage Minimum(0)
	0x29,0x65,	//	Usage Maximim(101)
	0x81,0x00,	//	Input (Data,Array)
	
	0xC0		//  End Collection
};


/*--------------------------------------------------------------------------------------
 Func: USB标准设备配置表
 Time: 2011-4-27
 Note: 
------------------------------------------------------------------------------------- */
const uint8 USB_KEY_ConfigDesc[] =
  {
    /*-------------------------配置描述----------------------------*/
	0x09, 										//配置表长度
    USB_DESCRIPTOR_TYPE_CONFIGURATION, 			//配置表类型
    0x29,										//配置描述表总长度											  	    
    0x00,
    0x01,         								//设备支持的接口数
    0x01,         								//本配置选中值Set_Configuration()
    0x00,         								//本配置字符描述索引
    0xC0,         								//总线电源属性 D6/自供电 D5/行程唤醒 D7->1
    0x32,         								//最大电流 N*2 mA
    /*-------------------------接口描述----------------------------*/
    0x09,         								//接口描述表长度
    USB_DESCRIPTOR_TYPE_INTERFACE,				//接口描述表类型
    0x00,         								//接口编号
    0x00,         								//接口编码 SetInterface()
    0x02,         								//接口包含的端点数 0/control pipe
    0x03,        								//接口类型 		HID: 0x03	MASS:0x08
    0x01,         								//接口子类型 	HID: 1=BOOT, 0=no boot  MASS: 6=BOT
    0x01,         								//接口协议类型 	HID: 0=none, 1=keyboard, 2=mouse */ MASS: 80=SCSI
    4,											//接口字符串描述索引
	/*-------------------------HID接口描述----------------------------*/
    0x09,         								//HID 描述表长度
    USB_DECSRIPTOR_TYPE_HID_CONFIG,				//HID 描述表类型
    0x01,         								//V1.01
    0x01,         								//HID class spec release 版本
    0x00,         								//HID countrycode 0=not supported
    0x01,        								//HID number class descriptors
    0x22,         								//HID constant name type of class descriptor
    sizeof(USB_KEY_ReportDesc),					//
    0x00,										//HID 报文长度
	/*-------------------------输入端点描述----------------------------*/
    0x07,   									//端点描述表长度
    USB_DESCRIPTOR_TYPE_ENDPOINT,				//端点描述表类型  										//
    0x81,   									//端点方向及端点号(IN)  B7|B[3:0]
    USB_ENDPOINT_TYPE_INTERRUPT,				//端点类型[INTERRUPT]
    USB_HID_ENDP_PACKETSIZE, 					//bulk in 缓冲区
    0x00,										//端点最大缓冲区大小
    0x0A,   									//端点中断间隔 32ms   

	/*-------------------------输出端点描述----------------------------*/
    0x07,   									//端点描述表长度
    USB_DESCRIPTOR_TYPE_ENDPOINT,				//端点描述表类型  										//
    0x02,   									//端点方向及端点号(IN)  B7|B[3:0]
    USB_ENDPOINT_TYPE_INTERRUPT,				//端点类型[INTERRUPT]
    USB_HID_ENDP_PACKETSIZE, 					//bulk in 缓冲区
    0x00,										//端点最大缓冲区大小
    0x0A,   									//端点中断间隔 32ms  
};


/*----------------------------------------------------------------------------------------
 Func: 字符串描述表		/ * LangID = 0x0409: U.S. English * /
----------------------------------------------------------------------------------------*/
const uint8 USB_KEY_StringLangID[] ={4,USB_DESCRIPTOR_TYPE_STRING,0x09,0x04};

/*----------------------------------------------------------------------------------------
 Func: 厂商字符串描述串
----------------------------------------------------------------------------------------*/
const uint8 USB_KEY_StringVendor[] =
{	    
	34, 										/* Size of Vendor string */
    USB_DESCRIPTOR_TYPE_STRING,  				/* bDescriptorType*/
    /*HUBEI-QYKJ@2011*/
    'H', 0, 'U', 0, '-', 0, 'B', 0, 'E', 0, 'I', 0, '-', 0, 'Q', 0,
    'Y', 0, 'K', 0, 'J', 0, '@', 0, '2', 0, '0', 0, '1', 0, '1', 0
};

/*----------------------------------------------------------------------------------------
 Func: 产品字符串描述串
----------------------------------------------------------------------------------------*/
const uint8 USB_KEY_StringProduct[] =
{
	34,          						/* bLength */
    USB_DESCRIPTOR_TYPE_STRING,         /* bDescriptorType */
	/*HUBEI-QYKJ-TTS-USB*/
    'H', 0, 'B', 0, '-', 0, 'Q', 0,	'Y', 0, 'K', 0, 'J', 0, '-', 0, 
	'T', 0, 'T', 0, 'S', 0, '-', 0, 'U', 0, 'S', 0, 'B', 0, ' ', 0
};

/*----------------------------------------------------------------------------------------
 Func: 串号字符串
----------------------------------------------------------------------------------------*/
const uint8 USB_KEY_StringSerial[] =
{
    15,									//字符串长度
	USB_DESCRIPTOR_TYPE_STRING,        	//串类型		   //0200310
    '0', 0, '2', 0, '0', 0,'0', 0,'3', 0, '1', 0, '0', 0
};

/*----------------------------------------------------------------------------------------
 Func: 接口字符串描述串
----------------------------------------------------------------------------------------*/
const uint8 USB_KEY_StringInterface[] =
{	    
	18, 										/* Size of Vendor string */
    USB_DESCRIPTOR_TYPE_STRING,  				/* bDescriptorType*/
    /*QYKJMASS*/
    'Q', 0, 'Y', 0, 'K', 0, 'J', 0, 'M', 0, 'A', 0, 'S', 0, 'S', 0,
};




const uint8 USB_HID_KeyCode[]=
{
	 0x2C,0x9E,0xB4,0xA0,0xA1,0xA2,0xA4,0x34,0xA6,0xA7,0xA5,0xAE,0x36,0x2D,0x37,0x38,		// !"#$%&'()*+,-./	 
	 0x27,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0xB3,0x33,0xB6,0x2E,0xB7,0xB8,		//0123456789:;<=>?	 
	 0x9F,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,0x92,		//@ABCDEFGHIJKLMNO	 
	 0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x2F,0x31,0x30,0xA3,0xAD,		//PQRSTUVWXYZ[\]^_	 
	 0x32,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,		//`abcdefghijklmno	 
	 0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0xAF,0xB1,0xB0,0xB2,0x2A		//pqrstuvwxyz{|}~del
};



uint8 USB_HID_ProtocolValue;
uint8 USB_HID_KEY_ReportOutBuffer;
uint8 USB_HID_TASKID;
USB_EndpDef USB_Endp2;
USB_EndpDef USB_Endp1;


/*-------------------------------------------------------------------------
 Func: 实现HID数据报输入
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_HID_KEY_ReportIn(uint8 KeyValue,uint8 KeyState)
{
	
}


/*-------------------------------------------------------------------------
 Func: 实现HID数据报输出
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_HID_KEY_ReportOut(void)
{
	USB_RecvPacket(2,&USB_Endp2,&USB_HID_KEY_ReportOutBuffer,1);
	Tos_TaskRecover(USB_HID_TASKID);
}


/*-------------------------------------------------------------------------
 Func: 通过键盘模拟发送字符
 Time: 2011-5-6
 Ver.: V1.0
 Note: 
-------------------------------------------------------------------------*/
void USB_HID_KEY_SendKey(uint8 Char,uint8 Modify,uint8 Hold)
{
	uint8 i,Buffer[8];	
	for(i=0;i<8;i++)Buffer[i]=0;
	Buffer[0]=Modify;
	Buffer[2]=Char;
	USB_SendPacket(1,&USB_Endp1,Buffer,8);
	Tos_TaskDelay(10);
	if((Hold&0x10)==0x00)Buffer[0]=0;				//是否保持modify
	if((Hold&0x01)==0x00)return;					//是否发送断码
	Buffer[2]=0;
	USB_SendPacket(1,&USB_Endp1,Buffer,8);
	Tos_TaskDelay(10);
}


/*-------------------------------------------------------------------------
 Func: 通过键盘模拟发送字符串
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_HID_KEY_SendString(void *Pstr)
{
	uint8 T,*Dstr,Modify;
	Dstr=(uint8 *)Pstr;
	while(*Dstr>0){
		T=*Dstr++;
		T=USB_HID_KeyCode[T-0x20];
		if(T&0x80)Modify=0x20;
		else Modify=0;
		USB_HID_KEY_SendKey(T&0x7F,Modify,0x01);
	}
}

/*-------------------------------------------------------------------------
 Func: 实现HID KeyBoard的ClassSpecial->Setup
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
uint8 USB_HID_KEY_Setup()
{
	uint8 R;
	R=USB_RESULT_UNSUPPORT;
	if(USB_REQ.wLength==0){																
		if(USB_REQ.bRequest==b_REQUEST_SET_PROTOCOL){
			USB_HID_ProtocolValue=USB_REQ.wValue&0xFF;
			R=USB_RESULT_SUCCESS;	
		}
	}else{
		if(USB_REQ.bRequest==b_REQUEST_GET_PROTOCOL){
			USB_SendPacket(0,&USB_Endp0,&USB_HID_ProtocolValue,1);
			R=USB_RESULT_SUCCESS;
		}
	}
	return R;
}




void USB_HID_EndpointConfig(void)
{
	USB_ConfigEndp(1,USB_HID_ENDP_PACKETSIZE,USB_EPR_TYPE_BULK|USB_EPR_STAT_TX_VALID|USB_EPR_STAT_RX_DIS);	  	//IN		
	USB_ConfigEndp(2,USB_HID_ENDP_PACKETSIZE,USB_EPR_TYPE_BULK|USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);		//OUT
}


/*-------------------------------------------------------------------------
 Func: 实现Mass Storage的初始化
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_HID_KEY_Init(void)
{
	USB_DeviceDesc			=USB_KEY_DeviceDesc;
	USB_ConfigDesc			=USB_KEY_ConfigDesc;
	USB_ReportDesc			=USB_KEY_ReportDesc;
	USB_StringDesc[0]		=USB_KEY_StringLangID;
	USB_StringDesc[1]		=USB_KEY_StringVendor;
	USB_StringDesc[2]		=USB_KEY_StringProduct;
	USB_StringDesc[3]		=USB_KEY_StringSerial;
	USB_StringDesc[4]		=USB_KEY_StringInterface;
	USB_EndpointConfig 		=USB_HID_EndpointConfig;
	USB_ENDP_DATA_OUT[2]	=USB_HID_KEY_ReportOut;
	USB_ClassSpecial_Request=USB_HID_KEY_Setup;	 			//重定义用户SETUP
	USB_RecvPacket(2,&USB_Endp2,&USB_HID_KEY_ReportOutBuffer,1);	
}

/*-------------------------------------------------------------------------
 Func: USB批量数据处理任务
 Time: 2011-5-10
 Ver.: V1.0
 Note: 将USB数据处理部分放在中断中完成占用时间长
-------------------------------------------------------------------------*/
void USB_HID_KeyBoard_Task(void *Tags)
{		
	//uint8 i,USB_KEY_Buffer[8];
	//for(i=0;i<8;i++)USB_KEY_Buffer[i]=0;	
	Tos_TaskDelay(500);
	USB_HID_TASKID=Tos_TaskCurrent;
	USB_Init();			
SystemUSB_Config(1);	
	USB_HID_KEY_Init();												//初始化
	USB_CONNECT_FULLSPEED
	while(1){
		Tos_TaskDelay(5000);
		DeBug("open: http://www.google.com");
		USB_HID_KEY_SendKey(0x00,0x08,0x00);						//按下GUI键
		USB_HID_KEY_SendKey(USB_HID_KeyCode['r'-0x20],0x08,0x11);	//按下R键
		USB_HID_KEY_SendKey(0x00,0x00,0x00);						//放开GUI键
		USB_HID_KEY_SendString("http://www.google.com");
		USB_HID_KEY_SendKey(0x28,0x00,0x01);		//Enter
		Tos_TaskDelay(5000);
		DeBug("open: http://www.baiduh.com");
		USB_HID_KEY_SendKey(0x00,0x08,0x00);						//按下GUI键
		USB_HID_KEY_SendKey(USB_HID_KeyCode['r'-0x20],0x08,0x11);	//按下R键
		USB_HID_KEY_SendKey(0x00,0x00,0x00);						//放开GUI键
		USB_HID_KEY_SendString("http://www.baidu.com");
		USB_HID_KEY_SendKey(0x28,0x00,0x01);		//Enter
				
	}
}



