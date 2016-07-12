
#ifndef _TTS_USB_KEYBOARD_H_
#define _TTS_USB_KEYBOARD_H_
#include <include.h>

#define b_REQUEST_GET_REPORT		0x01
#define b_REQUEST_GET_IDLE			0x02
#define b_REQUEST_GET_PROTOCOL		0x03
#define b_REQUEST_SET_REPORT		0x09
#define b_REQUEST_SET_IDLE			0x0A
#define b_REQUEST_SET_PROTOCOL		0x0B

/*-------------------------------------------------------------
 short item  [data(0-4)]...|[btag4|btype2|bsize2]
 long item	 [data(3-258)]|[bLongItemTag]|[bDataSize]|[11111110B]
 	bsize: 0/1/2/4	btype:0=main/1=global/2=local/3=recvd
	Main Items:
		---------------------------------------------------
		Value		|	Description
		---------------------------------------------------		
		1000 00XX	|	Input
		1001 00XX	|	Output
		1010 00XX	|	Feature
		1011 00XX 	|	Collection
		1100 00XX 	|	EndCollection
		1101 00XX-> |	Reserved
	Input/Output/Feature ItemsValue Note:
		Bits	|	=0				|	=1
		---------------------------------------------------
		B0		|	Data			|	Constant
		B1		|	Array			|	Variable
		B2		|	Absolute		|	Relative
		B3		|	No Wrap			|	Wrap
		B4		|	Linear			|	Nonlinear
		B5		|	Preferred State	|	No Preferred
		B6		|	No Null Position|	Null State
		B7		|	Non Volatile	|	Volatile
		B8		|	Bit Field		|	Buffered Bytes
		B9-31	|	Reserved
		---------------------------------------------------
	Collection/End Collection ItemsValue Note:
		---------------------------------------------------
		Value	|	Description
		---------------------------------------------------
		0x00	|	Physical
		0x01	|	Application
		0x02	|	Logical
		0x03	|	Report
		0x04	|	Named Array
		0x05	|	Usage Switch
		0x06	|	Usage modifier
		0x07-7F	|	Reserved
		0x80-FF	|	Vendor defined
	Global ItemsValue Note:
		---------------------------------------------------
		Value		|	Description
		---------------------------------------------------
		0000 01XX	|	Usage Page
		0001 01XX	|	Logical Minimum
		0010 01XX	|	Logical Maximum
		0011 01XX	|	Physical Minimum
		0100 01XX	|	Physical Maximum
		0101 01XX	|	Unit Exponent
		0110 01XX	|	Uint
		0111 01XX	|	Report Size
		1000 01XX	|	Report ID
		1001 01XX	|	Report Count
		1010 01XX	|	Push
		1011 01XX	|	Pop
		1100 01XX-> |	Reserved
	Local ItemsValue Note:
		---------------------------------------------------
		Value		|	Description
		---------------------------------------------------
		0000 10XX	|	Usage
		0001 10XX	|	Usage Minimum
		0010 10XX	|	Usage Maximum
		0011 10XX	|	Designator Index
		0100 10XX	|	Designator Minimum
		0101 10XX	|	Designator Maximim
		0110 10XX	|	
		0111 10XX	|	String Index
		1000 10XX	|	String Minimum
		1001 10XX	|	String Maximim
		1010 10XX	|	Delimiter
		1011 10XX-> |	Reserved														 
*--------------------------------------------------------------*/ 
 	
/*--------------------------------------------------------------
				Keyboard Report format
 Byte0: ×éºÏ¼ü
 	-----------------------------------------------------------
	  GUI	| ALT | SHIFT | CTRL | GUI | ALT | SHIFT | CTRL	
	  ---------------------------|--------------------------
			 Right Key			 |		  Left Key			
	-----------------------------------------------------------
 Byte1: Receved for Verdor Use
 Byte2-7: KeyCodes

 						Keyboard Leds format
    --------------------------------------------------------------
	 Const | Const |Const |	Kana | Compose | Scroll | Caps | Num
	--------------------------------------------------------------

*--------------------------------------------------------------*/

/*
	'A',0x04,
	'B',0x05,
	'C',0x06,
	'D',0x07,
	'E',0x08,
	'F',0x09,
	'G',0x0A,
	'H',0x0B,
	'I',0x0C,
	'J',0x0D,
	'K',0x0E,
	'L',0x0F,
	'M',0x10,
	'N',0x11,
	'O',0x12,
	'P',0x13,
	'Q',0x14,
	'R',0x15,
	'S',0x16,
	'T',0x17,
	'U',0x18,
	'V',0x19,
	'W',0x1A,
	'X',0x1B,
	'Y',0x1C,
	'Z',0x1D,
	'!',0x1E,
	'@',0x1F,
	'#',0x20,
	'$',0x21,
	'%',0x22,
	'^',0x23,
	'&',0x24,
	'*',0x25,
	'(',0x26,
	')',0x27,
	
	KEY_ENTER,	0x28
	KEY_ESC,	0x29,
	KEY_DEL,	0x2A,
	KEY_TAB,	0x2B,
	KEY_SPACE,	0x2C,

	'-',0x2D,
	'_',0x2D,
	'=',0x2E,
	'+',0x2E,
	'[',0x2F,
	'{',0x2F,
	']',0x30,
	'}',0x30,
	'\\',0x31,
	'|',0x31,
	'`',0x32,
	'~',0x32,
	';',0x33,
	':',0x33,
	''',0x34,
	'"',0x34,	
	',',0x36,
	'<',0x36,	
	'.',0x37,
	'>',0x37,
	'/',0x38,
	'?',0x38,
	
	KEY_CAPS,	0x39,	
	KYE_F1,	 	0x3A,
	KYE_F2,		0x3B,
	KEY_F3,		0x3C,
	KEY_F4,		0x3D,
	KEY_F5,		0x3E,
	KEY_F6,		0x3F,
	KEY_F7,		0x40,
	KEY_F8,		0x41,
	KEY_F9,		0x42,
	KEY_F10,	0x43,
	KEY_F11,	0x44,
	KEY_F12,	0x45,
	KEY_PRINT,	0x46,
	KEY_SCROLL,	0x47,
	KEY_PAUSE,	0x48,
	KEY_INSERT,	0x49,
	KEY_HOME,	0x4A,
	KEY_PAGEUP,	0x4B,
	KEY_DELET,	0x4C,
	KEY_END,	0x4D,
	KEY_PAGEDOWN,	0x4E,
	KEY_RIGHTARROW,	0x4F,
	KEY_LEFTARROW,	0x50,
	KEY_DOWNARROW,	0x51,
	KEY_UPARROW,	0x52,
	KEY_NUMLOCK,	0x53,
	KEYPAD_/,		0x54,
	KEYPAD_*,		0x55,
	KEYPAD_-,		0x56,
	KEYPAD_+,		0x57,
	KEYPAD_ENT,		0x58,
	KEYPAD_1,		0x59,
	KEYPAD_2,		0x5A,
	KEYPAD_3,		0x5B,
	KEYPAD_4,		0x5C,
	KEYPAD_5,		0x5D,
	KEYPAD_6,		0x5E,
	KEYPAD_7,		0x5F,
	KEYPAD_8,		0x60,
	KEYPAD_9,		0x61,
	KEYPAD_0,		0x62,
	KEYPAD_.,		0x63,
	KEY_POWER,		0x66
*/



#define USB_HID_ENDP_PACKETSIZE 64


void USB_HID_KeyBoard_Task(void *Tags);


#endif
