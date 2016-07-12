#include <Include.h>
#include <CheakOut.h>
#include <LockCode.h>
/********************************************************************************
Func:认证码
Date:2014-6-10
Note:认证使能的CPU程序需要录入该CPU的唯一ID  Crc校验码
********************************************************************************/
const uint16 CodeTab[]={
0x5568,0xf451,0x6266,0x6B38,0x2423,0x1F39,0x4442,0x8461,0x44b2,0x3613,0x8E19,0xc650,0x2541,
0xbb29,0x529e,0X4887
};

uint16 GetLockCrcCode()//获取唯一编号
{
	uint16 CrcValue;
	CrcValue=Tools_GetCRC16((uint8 *)(0x1ffff7e8),12);  
	return CrcValue;
}
/********************************************************************************
Func:认证
Date:2014-6-10
Note:
********************************************************************************/
BOOL AppLicense()
{
	uint16 i;
	uint16 Code=GetLockCrcCode();
	for(i=0;i<sizeof(CodeTab);i++)if(Code==CodeTab[i])return True;
	return False;
}
