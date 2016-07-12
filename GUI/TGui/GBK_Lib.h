#ifndef _GBK0816Lib_H_
#define _GBK0816Lib_H_
#include <Include.h>


typedef struct{
	uint8 Wide;
	uint8 High;
	const uint8 *ELib;
	const uint8 *EIndex;
	const uint8 *CLib;
	const uint8 *CIndex;
}FontTypeStruct;
extern const uint8 GBK0612Lib[];
extern const uint8 GBK1212_Index[];
extern const uint8 GBK1212Lib[];

extern const uint8 GBK0816Lib[];
extern const uint8 GBK1616_Index[];
extern const uint8 GBK1616Lib[];

extern const uint8 GBK0824Lib[];
extern const uint8 GBK1624_Index[];
extern const uint8 GBK1624Lib[];

extern const uint8 GBK1224Lib[];
extern const uint8 GBK2424_Index[];
extern const uint8 GBK2424Lib[];

extern const uint8 GBK1632Lib[];
extern const uint8 GBK3232_Index[];
extern const uint8 GBK3232Lib[];

extern const uint8 GBK2448Lib[];
extern const uint8 GBK4848_Index[];
extern const uint8 GBK4848Lib[];


extern const FontTypeStruct GBKLib_FontType1224;
extern const FontTypeStruct GBKLib_FontType1616;
extern const FontTypeStruct GBKLib_FontType1624;
extern const FontTypeStruct GBKLib_FontType3232;
extern const FontTypeStruct GBKLib_FontType2424;
extern const FontTypeStruct GBKLib_FontType4848;
#endif

