
#ifndef _TTS_USB_DESC_H_
#define _TTS_USB_DESC_H_

#include <include.h>

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

/*endpoint_type*/
#define USB_ENDPOINT_TYPE_CONTROL				0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS			0x01
#define USB_ENDPOINT_TYPE_BULK					0x02
#define USB_ENDPOINT_TYPE_INTERRUPT				0x03


#define USB_CONFIGURATION_DESCRIPTOR_SIZE 		32


#define HID_DESCRIPTOR_TYPE                     0x21
#define CUSTOMHID_SIZ_HID_DESC                  0x09
#define CUSTOMHID_OFF_HID_DESC                  0x12

#define CUSTOMHID_SIZ_DEVICE_DESC               18
#define CUSTOMHID_SIZ_CONFIG_DESC               41
#define CUSTOMHID_SIZ_REPORT_DESC               162
#define CUSTOMHID_SIZ_STRING_LANGID             4
#define CUSTOMHID_SIZ_STRING_VENDOR             38
#define CUSTOMHID_SIZ_STRING_PRODUCT            32
#define CUSTOMHID_SIZ_STRING_SERIAL             26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#endif
