/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptors for Joystick Mouse Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const u8 Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {	 //设备描述符
    0x12,                       /*bLength 描述符的长度，设备描述符的长度为18个字节*/
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType描述符的类型，设备描述符的类型为0x01*/
    0x00,                      
    0x02,                       /*bcdUSB USB设备所遵循的协议版本号，例如2.0协议为0x0200*/
    0x00,                       /*bDeviceClassUSB设备类代码，由USB-IF分配，如果该字段为0x00，表示由接口描述符来指定（有可能该USB设备是一个复合设备，USB设备的各个接口相互独立，分别属于不同的设备类）。如果是0x01~0xfe，表示为USB-IF定义的设备类，例如0x03为HID设备，0x09为HUB设备。如果是0xff，表示由厂商自定义设备类型。*/
    0x00,                       /*bDeviceSubClass USB子类代码，由USB-IF分配，如果bDeviceClass为0x00，那么该字段也必须为 0x00，其它情况可以参考USB关于对于USB Device Class的定义*/
    0x00,                       /*bDeviceProtocol 协议代码，由USB-IF分配，如果bDeviceClass和bDeviceSubClass定义为0x00，那么该字段也必须为0x00。*/
    0x40,                       /*bMaxPacketSize40端点0最大数据包长度，必须为8、16、32和64*/
    0x34,                       /*idVendor (0x0483)*///为了飞车我改为0x1234
    0x12,                       //厂商ID，由USB-IF分配，需要向USB-IF组织申请
    0x21,                       /*idProduct = 0x5710为了飞车改为0x4321
    0x49,                       产品ID，由厂商指定*/
    0x00,                       
    0x02,                       /*bcdDevice rel. 2.00设备序列号，由厂商自行设置*/
    1,                          /*Index of string descriptor describing
                                                  manufacturer  用于描述厂商的字符串描述符索引*/
    2,                          /*Index of string descriptor describing
                                                 product用于描述产品的字符串描述符索引*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number用于描述产品序列号的字符串描述符索引，注意，所有的字符串描述符是可选的，如果没有字符串描述符，指定这些索引为0x00。 */
    0x01                        /*bNumConfigurations 配置描述符数量*/
  }
  ; /* Joystick_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuation Descriptor size配置描述符长度，配置描述符长度为9字节大小 *///配置描述符
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration 描述符类型，配置描述符类型为0x02*/
    JOYSTICK_SIZ_CONFIG_DESC,/* wTotalLength: Bytes returned 配置描述符信息总的大小，包括接口描述符、端点描述符等等。*/
    0x00,
    0x01,         /*bNumInterfaces: 1 interface USB接口数量*/
    0x01,         /*bConfigurationValue: Configuration value当使用SetConfiguration和GetConfiguration请求时所指定的配置索引值*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration描述配置的字符串描述符索引*/
    0x80,         /*bmAttributes: bus powered 供电配置，位详细定义如下：

                          D7              保留，必须置1

                          D6              自供电模式

                          D5              远程唤醒

                          D4～D0      保留
                         */ //这边被改过
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus  最大功耗，以2mA为单位，例如0x32为50*2=100mA。*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size描述符长度，接口描述符长度为9个字节。*///接口描述符
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type描述符类型，接口描述符的类型为0x04*/
    0x00,         /*bInterfaceNumber: Number of Interface该接口编号，接口编号从0开始分配，当一个配置有多个接口时，就用该字段来区分不同的接口。*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints端点数量，不包括端点0*/	//为了飞车这里增加了一个
    0x03,         /*bInterfaceClass:0x03为hid设备 固定HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot系统运行前使用*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse，键盘或者鼠标或者其他*/
    0,            /*iInterface: Index of string descriptor描述该接口的字符串描述符索引。*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/ //HID类描述符
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID描述符类型，端点描述符类型为0x05。*/
    0x00,         /*bcdHID: HID Class Spec release number*/
    0x01,         //端点地址，详细定义如下 D7 端点方向 0 OUT端点1 IN D6～D4       保留 D3～D0       端点编号
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Joystick Mouse endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*///端点描述符
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 4 Byte max *///为了飞车这里增加到8
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
    /* 34 */
		//下面是为了飞车而增加的一个端点。
	0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x01,          /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 8 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
  }; /* MOUSE_ConfigDescriptor */

const u8 Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
{
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 0x09, 0x06, // USAGE (Keyboard)
 0xa1, 0x01, // COLLECTION (Application)
 0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)
 0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
 0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 0x95, 0x08, //     REPORT_COUNT (8)
 0x75, 0x01, //     REPORT_SIZE (1)
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 0x95, 0x01, //     REPORT_COUNT (1)
 0x75, 0x08, //     REPORT_SIZE (8)
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 0x95, 0x06, //   REPORT_COUNT (6)
 0x75, 0x08, //   REPORT_SIZE (8)
 0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
 0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
 0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
 0x81, 0x00, //     INPUT (Data,Ary,Abs)
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 0x95, 0x02, //   REPORT_COUNT (2)
 0x75, 0x01, //   REPORT_SIZE (1)
 0x05, 0x08, //   USAGE_PAGE (LEDs)
 0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
 0x29, 0x02, //   USAGE_MAXIMUM (Caps Lock)
 0x91, 0x02, //   OUTPUT (Data,Var,Abs)
 0x95, 0x01, //   REPORT_COUNT (1)
 0x75, 0x06, //   REPORT_SIZE (6)
 0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
 0xc0        // END_COLLECTION
}; /* Joystick_ReportDescriptor */


/* USB String Descriptors (optional) */
const u8 Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const u8 Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
   0x81,0x67, 
   0xc1,0x54,  
   0xde,0x98,   		
   0x66,0x8f,		
  };

//使用UNICODE编码
const u8 Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	0x81,0x67, 
    0xc1,0x54,  
    0xde,0x98,   	
    0x66,0x8f,
  };

u8 Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
		'T',0,'R',0,'T',0,'O',0,'S',0,' ',0,'H',0,'I',0,'D',0,
	  0x1a,0x90,  
    0xe1,0x4f,   
    0x9e,0x5b, 	
    0x8c,0x9a,
  };

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

