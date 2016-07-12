
#include <include.h>
#include <stm32f10x.h>
#include <usb_mass_storage.h>
#include <usb_driver.h>
#include <usb_bulkonly.h>
#include <usb_scsi.h>
#include <board_driver.h>

USB_EndpDef Endp1,Endp2;
uint8 USB_MASS_TASKID;



/*--------------------------------------------------------------------------------------
 Func: USB标准设备描述
 Time: 2011-4-27
 Note: 
------------------------------------------------------------------------------------- */
const uint8 USB_MASS_DeviceDesc[] =
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
    0x50,                       				//产品ID = 0x5750
    0x57,
    0x00,                       				//设备版本
    0x02,
    1,                          				//厂商字符描述表位置索引
    2,                          				//产品字符描述表位置索引
    3,                          				//设备串号字符描述表索引
    0x01                        				//配置描述表项数
  };											


/*--------------------------------------------------------------------------------------
 Func: USB标准设备配置表
 Time: 2011-4-27
 Note: 
------------------------------------------------------------------------------------- */
const uint8 USB_MASS_ConfigDesc[] =
  {
    /*-------------------------配置描述----------------------------*/
	0x09, 										//配置表长度
    USB_DESCRIPTOR_TYPE_CONFIGURATION, 			//配置表类型
    0x20,										//配置描述表总长度											  	    
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
    0x08,        								//接口类型 		HID: 0x03	MASS:0x08
    0x06,         								//接口子类型 	HID: 1=BOOT, 0=no boot
    0x50,         								//接口协议类型 	HID: 0=none, 1=keyboard, 2=mouse */
    4,											//接口字符串描述索引
	/*-------------------------端点描述----------------------------*/
    0x07,   									//端点描述表长度
    USB_DESCRIPTOR_TYPE_ENDPOINT,				//端点描述表类型  										//
    0x81,   									//端点方向及端点号(IN)  B7|B[3:0]
    USB_ENDPOINT_TYPE_BULK,						//端点类型[BULK]
    USB_MASS_ENDP_PACKETSIZE, 					//bulk in 缓冲区
    0x00,										//端点最大缓冲区大小
    0x00,   									//端点中断间隔
    
    0x07,   									//端点描述表长度
    USB_DESCRIPTOR_TYPE_ENDPOINT,				//端点描述表类型  	
    0x02,   									//端点方向及端点号(OUT)  B7|B[3:0]
    USB_ENDPOINT_TYPE_BULK,						//端点类型[BULK]
    USB_MASS_ENDP_PACKETSIZE,					//bulk out缓冲区
    0x00,										//端点最大缓冲区大小
    0x00     									//端点中断间隔
};


/*----------------------------------------------------------------------------------------
 Func: 字符串描述表		/ * LangID = 0x0409: U.S. English * /
----------------------------------------------------------------------------------------*/
const uint8 USB_MASS_StringLangID[] =
{
    4,
    USB_DESCRIPTOR_TYPE_STRING,							  
    0x09,
    0x04												   
}; 

/*----------------------------------------------------------------------------------------
 Func: 厂商字符串描述串
----------------------------------------------------------------------------------------*/
const uint8 USB_MASS_StringVendor[] =
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
const uint8 USB_MASS_StringProduct[] =
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
const uint8 USB_MASS_StringSerial[] =
{
    15,									//字符串长度
	USB_DESCRIPTOR_TYPE_STRING,        	//串类型		   //0200310
    '0', 0, '2', 0, '0', 0,'0', 0,'3', 0, '1', 0, '0', 0
};

/*----------------------------------------------------------------------------------------
 Func: 接口字符串描述串
----------------------------------------------------------------------------------------*/
const uint8 USB_MASS_StringInterface[] =
{	    
	18, 										/* Size of Vendor string */
    USB_DESCRIPTOR_TYPE_STRING,  				/* bDescriptorType*/
    /*QYKJMASS*/
    'Q', 0, 'Y', 0, 'K', 0, 'J', 0, 'M', 0, 'A', 0, 'S', 0, 'S', 0,
};




/*-------------------------------------------------------------------------
 Func: 实现Mass Storage的ClassSpecial->Setup
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
uint8 USB_Mass_Storage_Setup()
{
	uint8 R;
	if(USB_REQ.wLength==0){
		if(USB_REQ.bRequest==b_REQUEST_MASS_STORAGE_RESET){
			USB_SendPacket(0,&USB_Endp0,&R,0);
			return USB_RESULT_SUCCESS;
		
		}else return USB_RESULT_UNSUPPORT; 
	
	}else{
		if(USB_REQ.bRequest==b_REQUEST_GET_MAX_LUN){
			R=USB_MASS_STORAGE_MAX_LUN;
		 	USB_SendPacket(0,&USB_Endp0,&R,0);
			return USB_RESULT_SUCCESS;
		
		}else return USB_RESULT_UNSUPPORT; 	
	}
}

/*-------------------------------------------------------------------------
 Func: 实现Mass Storage的ENDP_IN			   
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_Mass_Storage_DataIn()
{
	uint8 R;
	switch(USB_BOT_STAGE){
		case USB_BOT_STAGE_SETUP:			
			if(Endp1.wAddr>0){
				R=USB_SendPacket(1,&Endp1,Endp1.wAddr,Endp1.wLength);			//发送后续数据
				CSW.DataResidue-=R;
			}else{				
				R=USB_SendPacket(1,&Endp1,(uint8 *)(&CSW),13);					//发送CSW
				USB_BOT_STAGE=USB_BOT_STAGE_STATUS;
				USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);				
				break;
			}
			
			break;
		case USB_BOT_STAGE_DATAIN:
			if(Endp1.wAddr>0){				
				R=USB_SendPacket(1,&Endp1,Endp1.wAddr,Endp1.wLength);			//发送后续数据
				CSW.DataResidue-=R;
			}else{
				Tos_TaskSmail(USB_MASS_TASKID,SCSI_CMD_READ10);
				Tos_TaskRecover(USB_MASS_TASKID);
			}
			//USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
			break;		
		case USB_BOT_STAGE_STATUS:									
			USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
			break;					
	}
}


/*-------------------------------------------------------------------------
 Func: 实现Mass Storage的ENDP_IN			   
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_Mass_Storage_DataOut()
{
	uint8  Buffer[100];		
	uint16 R;
	switch(USB_BOT_STAGE){
		case USB_BOT_STAGE_STATUS:
			USB_BOT_STAGE=USB_BOT_STAGE_SETUP;
			R=USB_ReadBuffer(2,Buffer,100);
			R=USB_BulkOnly_CBWDecode(Buffer);					//解码CBW块
			if(R==USB_RESULT_SUCCESS){
				USB_SendPacket(1,&Endp1,DBW.wAddr,DBW.Length); 	//有返回数据要发送
			}else if(R==USB_RESULT_RETURNCSW){
				USB_BOT_STAGE=USB_BOT_STAGE_STATUS;
				USB_SendPacket(1,&Endp1,(uint8 *)(&CSW),13);	//直接返回状态,无数阶段								
				USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
			}else if(R==USB_RESULT_NOTREADY){
			}
			break;
		case USB_BOT_STAGE_DATAOUT:
			if(Endp2.rLength>0){
				R=USB_RecvPacket(2,&Endp2,Endp2.rAddr,Endp2.rLength);
				CSW.DataResidue-=R;
				if(R==0){					
					Tos_TaskSmail(USB_MASS_TASKID,SCSI_CMD_WRITE10);
					Tos_TaskRecover(USB_MASS_TASKID);	
				}else{
					USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
				}
			}else USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
			break;
		case USB_BOT_STAGE_DATAIN:
			USB_BOT_STAGE=USB_BOT_STAGE_STATUS;
			R=USB_ReadBuffer(1,(uint8 *)(&CSW),13);
			USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
			break;
		default: 	break; 	
	}
}


void USB_Mass_EndpoingConfig(void)
{
	USB_ConfigEndp(1,USB_MASS_ENDP_PACKETSIZE,USB_EPR_TYPE_BULK|USB_EPR_STAT_TX_VALID|USB_EPR_STAT_RX_DIS);	  	//IN		
	USB_ConfigEndp(2,USB_MASS_ENDP_PACKETSIZE,USB_EPR_TYPE_BULK|USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);		//OUT
}


/*-------------------------------------------------------------------------
 Func: 实现Mass Storage的初始化
 Time: 2011-5-6
 Ver.: V1.0
 Note:
-------------------------------------------------------------------------*/
void USB_Mass_Storage_Init(void)
{
	USB_DeviceDesc			=USB_MASS_DeviceDesc;
	USB_ConfigDesc			=USB_MASS_ConfigDesc;
	USB_ReportDesc			=0;
	USB_StringDesc[0]		=USB_MASS_StringLangID;
	USB_StringDesc[1]		=USB_MASS_StringVendor;
	USB_StringDesc[2]		=USB_MASS_StringProduct;
	USB_StringDesc[3]		=USB_MASS_StringSerial;
	USB_StringDesc[4]		=USB_MASS_StringInterface;
	USB_EndpointConfig 		=USB_Mass_EndpoingConfig;
	USB_ClassSpecial_Request=USB_Mass_Storage_Setup;	 	//重定义用户SETUP
	USB_ENDP_DATA_OUT[2]	=USB_Mass_Storage_DataOut;		//重定义Mass 输出端点
	USB_ENDP_DATA_IN[1]		=USB_Mass_Storage_DataIn;		//重定义Mass 输入端点	
	USB_BOT_STAGE=USB_BOT_STAGE_STATUS;
}

/*-------------------------------------------------------------------------
 Func: USB批量数据处理任务
 Time: 2011-5-10
 Ver.: V1.0
 Note: 将USB数据处理部分放在中断中完成占用时间长
-------------------------------------------------------------------------*/
void USB_MassStorage_Task(void *Tags)
{		
	uint8  i,Buffer[2048];									//分配一个大页的缓冲区
	uint16 T,Length;
	Tos_TaskDelay(500);
	SystemUSB_Config(1);
	USB_Init();
	USB_Mass_Storage_Init();									//初始化
	USB_MASS_TASKID	=Tos_TaskCurrent;							//取当前任务ID
	USB_CONNECT_FULLSPEED
	while(1){
		//Tos_TaskSuspend();
		switch(Tos_TaskMail){
			//case (SCSI_CMD_READ10|0x8000):				
			//	break;
			case SCSI_CMD_READ10:
				if(SCSI_CMD.Len16>0){
					/*					
					if(SCSI_CMD.LBA&0x03){	T=(SCSI_CMD.LBA|0x03)-SCSI_CMD.LBA+1;}					
					else				 {	T=4;}
					if(SCSI_CMD.Len16<T)T=SCSI_CMD.Len16;
					Length=T<<9;
					*/
				//	WFS_ReadMem_Sectors(SCSI_CMD.LUN,SCSI_CMD.LBA,Buffer,0,512);
					i=USB_SendPacket(1,&Endp1,Buffer,512);
					CSW.DataResidue-=i;
					SCSI_CMD.Len16--;
					SCSI_CMD.LBA++;
					USB_BOT_STAGE=USB_BOT_STAGE_DATAIN;					
				}else{				
					USB_SendPacket(1,&Endp1,(uint8 *)(&CSW),13);					//发送CSW					
					USB_BOT_STAGE=USB_BOT_STAGE_STATUS;					
				}									
				USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);												 								
				break;
			case (SCSI_CMD_WRITE10|0x8000):				
				if(SCSI_CMD.LBA&0x07){	T=(SCSI_CMD.LBA|0x07)-SCSI_CMD.LBA+1;}
				else				 {	T=8;}
				if(SCSI_CMD.Len16<T)T=SCSI_CMD.Len16;
				Length=T<<9;
				Endp2.rAddr=0;
				USB_RecvPacket(2,&Endp2,Buffer,Length);
				USB_BOT_STAGE=USB_BOT_STAGE_DATAOUT;
				USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);
				break;
			case SCSI_CMD_WRITE10:				
				if(SCSI_CMD.Len16>0){
				//	WFS_WriteMem_Sectors(SCSI_CMD.LUN,SCSI_CMD.LBA,Buffer,0,Endp2.rOffset);
					SCSI_CMD.Len16-=(Endp2.rOffset>>9);
					SCSI_CMD.LBA+=(Endp2.rOffset>>9);
					USB_BOT_STAGE=USB_BOT_STAGE_DATAOUT;
				}
				if(SCSI_CMD.Len16>0){					
					if(SCSI_CMD.Len16<8)T=SCSI_CMD.Len16;
					else				T=8;
					Length=T<<9;
					USB_RecvPacket(2,&Endp2,Buffer,Length);
				}else{					
					USB_BOT_STAGE=USB_BOT_STAGE_STATUS;				
					USB_SendPacket(1,&Endp1,(uint8 *)(&CSW),13);					//发送CSW
				}
				USB_SetTxRxMode(2,USB_EPR_STAT_TX_DIS|USB_EPR_STAT_RX_VALID);				
				break;		
		}
	}
}



