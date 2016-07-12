#ifndef _TEXT_H_
#define _TEXT_H_
#include <Include.h>
#include <stdarg.h>


typedef struct 
{
	uint8 R;
	uint8 G;
	uint8 B;
}Color_RGB;
typedef struct
{
	uint8 Ip[4];
	uint16 Port;
}IpPortStruct;
extern void vsprintf(char *,char *fmt,va_list ap);
uint16 GetTextLength(void *Tags);
void Coppy_FromConst(uint8 *D,const uint8 *S,uint16 Length);
uint16 GetCountSize(const uint8 *Tags);
uint16 GetArraySum(void *Tags,uint16 Lentgh);
uint8 GetStringFromNumber(uint8 *Dstr,f32 Number,uint8 NumType,uint8 DotLenth);
uint16 StringCoppy(void *S,void *Out);
void StringEndingAppend(void *End,void *Out);
void StringHeadAppend(void *Head,void *Out);
f32 GetStringToNumber(uint8 *Sstr);
uint16  BufferCoppy(void *S,void *Out,uint16 Length);
uint8 MQTT_LegnthToByte(uint32 Length,uint8 *Bytes);
uint8 MQTT_LegnthToByte(uint32 Length,uint8 *Bytes);
void MidFilter_ByWords(uint16 *Sdata,uint16 Length);
uint16 AvrFilter_ByWords(uint16 *Sdata,uint16 Length,uint16 SumLen);
uint16 FlowPoolFilter(uint32 *Pool,uint16 Data,uint8 *PoolIndex,uint8 PoolSize);
uint8  D16ToArray(void *Array,uint16 Data,uint8 EndMode);
uint16 D16FromArray(void *Array,uint8 EndMode);
uint32 D32FromArray(void *Array,uint8 EndMode);
uint32 DispersionRatioByWords(uint16 *Sdata,uint16 Length);
uint16 GetWordFromBytes(string Buf,uint8 Mode);
uint8 SetWordToBytes(string Buf,uint16 Data,uint8 Mode);
uint16 FindChar_Count(string Str,char Key);
void BufferFill(void *S,uint8 Data,uint16 Length);
void GetFileNameFormat(uint8 *File,uint8 *FileName,uint8 *FileFormat);
void UniteNameFormat(uint8 *File,uint8 *FileName,uint8 *FileFormat);
void CreateTrueString(void *S);
uint16 SerchKeyFromBytes(uint8 *Buf,uint8 Key,uint16 Length);
uint16 StringSearchKey(uint8 *String,uint8 Key);
BOOL CompareWithString(uint8 *S,uint8 *K);
BOOL CompareWithBuffer(uint8 *S,uint8 *K,uint16 Length);
uint8 SplitWithString(uint8 *S,uint8 Key,uint8 *Buf);
void ClearWithBuffer(uint8 *S,uint16 Length);
uint16 Float32ToByte(uint8 *S,float Data);
float Float32FromByte(uint8 *S);
float Opera_WhithADCFloat(float m,float moshu1,float shuzhi1,float moshu2,float shuzhi2,float xianzhi);
BOOL VoidGetBool(uintbus Value);
uint32 VoidGetU32(uintbus Value);
f32 VoidGetF32(uintbus Value);
BOOL GetStringToBool(uint8 *Str);
uint8 D32ToArray(void *Array,uint32 Data,uint8 EndMode);
uint32 Uint32FromByte(uint8 *S);
uint16 Uint32ToByte(uint8 *S,uint32 Data);
uint16 Uint16ToByte(uint8 *S,uint32 Data);
uint32 StringToIPUint32(uint8 *String);
uint8 IPUint32ToString(uint8 *String,uint32 IP);
uint8 Bcd_Decimal(uint8 bcd);
uint8 Decimal_Bcd(uint8 decimal);
uint8 GetStringByFmt(char *String,char *fmt,...);
void FmtToBuf(char *String,char *fmt,...);
uint16 BytesFindString(char *S,const char *T,int Pos);
void StringReplaceChar(uint8 *String,uint8 Key,uint8 New);
void StringTrmChar(uint8 *String,uint8 Key);
uint8 GetHexFromString(uint8 *String);
void HtmlGetGB2312(uint8 *String);
Color_RGB TFT_RGB(uint16 Color);
uint16 ALL_TFT(uint32 Color);
uint16 RGB_TFT(Color_RGB T);
uint8 IpPortToString(IpPortStruct *IPPort,uint8 *String);
void IpPortFromString(IpPortStruct *IPPort,uint8 *String);
BOOL GPRS_CheckIpTrue(char *S);
float Abs(float Data);
uint8 FindNumFromString(uint8 *Buf,uint8 *Data,uint8 index);
char *Strstr(const char*s1,const char*s2);
int FromHex64(char t);
char ToHex64(int t);
uint8 FindHexFromString(uint8 *Buf,uint8 *Data,uint8 index);
uint8 FindFloatFromString(uint8 *Buf,uint8 *Data,uint8 index);
#endif
