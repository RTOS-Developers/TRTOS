#include <Include.h>
#include <text.h>
/******************************************************************************
 Func: 获取字符串长度
 Time: 2014-4-28
 Ver.: V2.0
 By:童金吕
 Note:返回字符串长度包括结束符
******************************************************************************/
uint16 GetTextLength(void *Tags)
{
	uint8 *P;
	uint16 i=0;
	P=(uint8 *)Tags;
	while(*P++)i++;
	return i;
}
/******************************************************************************
 Func: 区分IP与域名
 Time: 2014-4-28
 Ver.: V2.0
 By：童金吕
 Note:是IP返回真
******************************************************************************/
BOOL GPRS_CheckIpTrue(char *S)
{
	char *P=S;
	while(*P)
	{
		if(*P=='.')
		{
			if((*(P-1)>='0')&(*(P-1)<='9'))
			if((*(P+1)>='0')&(*(P+1)<='9'))
			return True;//是IP
		}
		P++;
	}
	return False;
}
/******************************************************************************
 Func: 寻找字符串中的数字串
 Time: 2014-4-28
 Ver.: V2.0
 Note:Buf为源串，Data输出串，Index 数字串索引
******************************************************************************/
uint8 FindNumFromString(uint8 *Buf,uint8 *Data,uint8 index)
{
	uint8 Length,i,c=0,t,has=0,z=0;
	Length=GetTextLength(Buf);
	if(Length)
	{
		Data[0]=0;
		for(i=0;i<Length;i++)
		{
			t=Buf[i];
			if((t>='0')&(t<='9')){if(index==z)Data[c++]=t;has=1;}
			else if(has){if(index==z)Data[c++]=0;has=0;z++;}
		}	
	}
	return c;
}
uint8 FindFloatFromString(uint8 *Buf,uint8 *Data,uint8 index)
{
	uint8 Length,i,c=0,t,has=0,z=0;
	Length=GetTextLength(Buf);
	if(Length)
	{
		Data[0]=0;
		for(i=0;i<Length;i++)
		{
			t=Buf[i];
			if(((t>='0')&(t<='9'))|(t=='.')){if(index==z)Data[c++]=t;has=1;}
			else if(has){if(index==z)Data[c++]=0;has=0;z++;}
		}	
	}
	return c;
}
/******************************************************************************
 Func: 寻找字符串中的数字串
 Time: 2014-4-28
 Ver.: V2.0
 Note:Buf为源串，Data输出串，Index 数字串索引
******************************************************************************/
uint8 FindHexFromString(uint8 *Buf,uint8 *Data,uint8 index)
{
	uint8 Length,i,c=0,t,has=0,z=0;
	Length=GetTextLength(Buf);
	if(Length)
	{
		Data[0]=0;
		for(i=0;i<Length;i++)
		{
			t=Buf[i];
			if(((t>='0')&(t<='9'))||((t<='F')&(t>='A'))){if(index==z)Data[c++]=t;has=1;}
			else if(has){if(index==z)Data[c++]=0;has=0;z++;}
		}	
	}
	return c;
}
/******************************************************************************
 Func: 匹配字符串
 Time: 2014-4-28
 Ver.: V2.0
 Note:返回匹配到的字符串首字母地址，没找到返回0
******************************************************************************/
char *Strstr(const char*s1,const char*s2)
{
    int n;
    if(*s2)
    {
        while(*s1)
        {
            for(n=0;*(s1+n)==*(s2+n);n++)
            {
                if(!*(s2+n+1))
                    return(char*)s1;
            }
            s1++;
        }
        return NULL;
    }
    else
        return (char*)s1;
}
/******************************************************************************
 Func: 数据从64进制转换，目的是为了压缩数据
 Time: 2014-4-28
 Ver.: V2.0
 Note:返回数据int
******************************************************************************/
int FromHex64(char t)//字符--->>数字 
{
     if(t>='0'&&t<='9')return t-'0';//数字
    if(t>='A'&&t<='Z')return t-'A'+10;//大写字母
    else return t-'a'+36;//小写字母
}
 /******************************************************************************
 Func: 向64进制转换
 Time: 2014-4-28
 Ver.: V2.0
 Note:
******************************************************************************/
char ToHex64(int t)//数字-->>>字符
{
  if(t>=0&&t<=9)return (t+'0');
  if(t>=10&&t<=35)return (t-10+'A');
  else return (t-36+'a');
}
/*---------------------------------------------------------------------------------------------------------
 Func: 复制数据到RAM
 Time: 2014-2-2
 Ver.: V1.0
 Note: 以值为零结束
---------------------------------------------------------------------------------------------------------*/
void Coppy_FromConst(uint8 *D,const uint8 *S,uint16 Length)
{
		while(Length--)*D++=*S++;
}
/*---------------------------------------------------------------------------------------------------------
 Func: 获取count 类型字符串长度
 Time: 2014-5-2
 Ver.: V1.0
 Note: 以值为零结束
---------------------------------------------------------------------------------------------------------*/
uint16 GetCountSize(const uint8 *Tags)
{
	uint16 i;
	for(i=0;i<0xffff;i++)
	{
		if(Tags[i]==0)return i;
	}
	return 0xffff;
}
/*******************************************************************************
Func:搜索关键字
Date:2014-5-22
Note:返回位置
*******************************************************************************/
uint16 StringSearchKey(uint8 *String,uint8 Key)
{
	uint16 i=0;
	while(*String)
	{
		i++;
		if(*String++==Key)return i;
	}
	return 0xffff;
}
void StringReplaceChar(uint8 *String,uint8 Key,uint8 New)
{
	uint8 *P=String;
	while(*P)
	{
		if(*P==Key)*P=New;P++;
	}
}
/******************************************************************************
 Func: 删除非法字符
 Time: 2014-7-23
 Ver.: V1.0
 Note:
******************************************************************************/
void StringTrmChar(uint8 *String,uint8 Key)
{
	uint8 *P=String,*P1=String;
	while(*P)
	{
		if(*P!=Key)*P1++=*P;
		P++;
	}
	*P1=0;
}
void HtmlGetGB2312(uint8 *String)
{
	uint8 *P=String,*P1=String;
	while(*P)
	{
		if(*P=='%'){*P1++=GetHexFromString(P+1);P+=3;}
		else *P1++=*P++;
	}
	*P1=0;
}
/******************************************************************************
 Func: 获取文件文件名和文件文件格式
 Time: 2014-7-23
 Ver.: V1.0
 Note:
******************************************************************************/
void GetFileNameFormat(uint8 *File,uint8 *FileName,uint8 *FileFormat)
{
	uint16 i=StringSearchKey(&File[0],'.');
	if(FileName)
	{
		while(*File)
		{
			*FileName++=*File++;
			if(*File=='.'){File++;break;}
		}
		*FileName=0;
	}else File+=i;
	if(FileFormat&&i!=0xffff)
	{
		while(*File)
		{
			*FileFormat++=*File++;
		}
		*FileFormat=0;
	}
}
/******************************************************************************
 Func: 合并文件文件名和文件文件格式
 Time: 2014-8-1
 Ver.: V1.0
 Note:
******************************************************************************/
void UniteNameFormat(uint8 *File,uint8 *FileName,uint8 *FileFormat)
{
	while(*FileName)
	{
		*File++=*FileName++;
	}
	*File++='.';
	while(*FileFormat)
	{
		*File++=*FileFormat++;
	}
	*File=0;
}
/*---------------------------------------------------------------------------------------------------------------------------
 Func: 字展开
 Time: 2010-7-28
 Ver.: V1.0
 Note: 本方法用于将字按字节展开到目标空间
 	   Array/目标空间   Data/源字  EndMode/大小端格式  0/小端 1/大端
 	   return P/对于Array更新后的位置
---------------------------------------------------------------------------------------------------------------------------*/
uint8  D16ToArray(void *Array,uint16 Data,uint8 EndMode)
{
    uint8 *P;
	P=(uint8 *)Array;
	if(EndMode){
        *P++=(uint8)(Data>>8);
        *P++=0xff&Data;        
	}else{
        *P++=0xff&Data;
        *P++=(uint8)(Data>>8);
	}	
	return 2;
}


/*---------------------------------------------------------------------------------------------------------------------------
 Func: 双字展开
 Time: 2010-7-28
 Ver.: V1.0
 Note: 本方法用于将双字按字节展开到目标空间
 	   Array/目标空间   Data/源双字  EndMode/大小端格式  0/小端 1/大端
 	   return P/对于Array更新后的位置
---------------------------------------------------------------------------------------------------------------------------*/
uint8 D32ToArray(void *Array,uint32 Data,uint8 EndMode)
{
    uint8 *P;
	P=(uint8 *)Array;
	if(EndMode){
        *P++=(uint8)(Data>>24);
	    *P++=(uint8)(Data>>16);
        *P++=(uint8)(Data>>8);
        *P++=(uint8)Data;
	}else{	    
        *P++=(uint8)Data;
        *P++=(uint8)(Data>>8);
        *P++=(uint8)(Data>>16);
        *P++=(uint8)(Data>>24);
	}	
	return 4;
}


/*---------------------------------------------------------------------------------------------------------------------------
 Func: 字组合
 Time: 2010-7-28
 Ver.: V1.0
 Note: 本方法用于将目标空间按字节组和成字
 	   Array/目标空间   Data/源字  EndMode/大小端格式  0/小端 1/大端
 	   return P/对于Array更新后的位置
---------------------------------------------------------------------------------------------------------------------------*/
uint16 D16FromArray(void *Array,uint8 EndMode)
{
    uint8 *P;
    uint16 Data;
    P=(uint8 *)Array;
    if(EndMode)
    {
        Data =*P++; Data<<=8;
        Data|=*P++;
    }else{
        Data=*(P+1);Data<<=8;
        Data|=*P;
    }
    return Data;
}


/*---------------------------------------------------------------------------------------------------------------------------
 Func: 双字组合
 Time: 2010-7-28
 Ver.: V1.0
 Note: 本方法用于将目标空间按字节组和成双字
 	   Array/目标空间   EndMode/大小端格式  0/小端 1/大端
 	   return P/对于Array更新后的位置
---------------------------------------------------------------------------------------------------------------------------*/
uint32 D32FromArray(void *Array,uint8 EndMode)
{
    uint8 *P;
    uint32 Data;
    P=(uint8 *)Array;
    if(EndMode)
    {
        Data =*P++;Data<<=8;
        Data|=*P++;Data<<=8;
        Data|=*P++;Data<<=8;
        Data|=*P;
    }else{
        P+=3;
        Data =*P--;Data<<=8;
        Data|=*P--;Data<<=8;
        Data|=*P--;Data<<=8;
        Data|=*P;
    }
    return Data;
}


/*---------------------------------------------------------------------------------------------------------
 Func: 获取字符串的和，以值为零结束
 Time: 2014-2-2
 Ver.: V1.0
 Note: *
---------------------------------------------------------------------------------------------------------*/
uint16 GetArraySum(void *Tags,uint16 Lentgh)
{
	uint16 i;
	uint8 *Msg=(uint8 *)Tags;
	uint16 Sum=0;
	for(i=0;i<Lentgh;i++)
	{
		Sum+=Msg[i];
		if(Msg[i]==0)return Sum;
	}
	return Sum;
}
uint8 GetStringByFmt(char *String,char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf(String,fmt,ap);
	va_end(ap);
	return GetTextLength(String);
}
/*---------------------------------------------------------------------------------------------------------
 Func: TTS基础算法 数字转化为字符串
 Time: 2014-2-2
 Ver.: V1.0
 Note: *Dstr/转换后的字符串 Number/待转换的数字 DotLenth/小数点后保留位数
---------------------------------------------------------------------------------------------------------*/
uint8 GetStringFromNumber(uint8 *Dstr,f32 Number,uint8 NumType,uint8 DotLenth)
{
	uint8 Temp[40],j=0,i=0,k;
	int32 T,M=0;
	switch(NumType){	
		case 'B':	M=2;	break;
		case 'O':	M=8;	break;
		case 'D':	M=10;	break;
		case 'H':	M=16;	break;
	}
	if(Number<0){					//取负号
		*Dstr++='-';
		Number=0-Number;
		j++;
	}
	T=(int32)Number;				//取整数部分
	Number-=T;						//取小数部分
	do
	{								//拆分整数部分
        k=T%M;
        Temp[i++] = k>9 ? k-10+'A' : k+'0';
		T/=M;		
	}while(T>0);
	j+=i;
	while(i--)*Dstr++=Temp[i];		//存储整数部分	
	if((DotLenth>0)&(NumType=='D')){	 //对10进制数求小数
		j++;
		*Dstr++='.';		
		while(DotLenth--){
			Number*=M;
			i=(uint8)Number;			
			*Dstr++=i+'0';
			Number-=i;
			j++;	
		}
	}	
	*Dstr=0;
	return ++j;
}
/******************************************************************************
 Func:获取Bool类型的值
 Time: 2014-10-12
 Ver.: V1.0
 Note:
******************************************************************************/
BOOL VoidGetBool(uintbus Value)
{
	BOOL V;
	V=*(BOOL *)Value;
	if(V!=False)V=True;
	return V;
}
/******************************************************************************
 Func:获取uint32类型的值
 Time: 2014-10-12
 Ver.: V1.0
 Note:
******************************************************************************/
uint32 VoidGetU32(uintbus Value)
{
	uint32 V;
	V=*(uint32 *)Value;
	return V;
}
/******************************************************************************
 Func:获取Bool类型的值
 Time: 2014-10-12
 Ver.: V1.0
 Note:
******************************************************************************/
f32 VoidGetF32(uintbus Value)
{
	f32 V;
	V=*(f32 *)Value;
	return V;
}
/******************************************************************************
 Func: 字符串转换到浮点型
 Time: 2015-8-3
 Ver.: V1.0
 Note: 
******************************************************************************/
f32 GetStringToNumber(uint8 *Sstr)
{
	uint8 T,F=0,D=0;
	f32 RealValue=0;
	f32 Factor=1;
	while(*Sstr>0){
		T=*Sstr++;
		if(T=='-')F=1;
		else if(T=='.'){D=1;}
		else if((T>='0')&&(T<='9')){
			if(!D){RealValue*=10;RealValue+=T-'0';}
			else 
			{
				Factor*=0.1;
				RealValue+=(Factor*(T-'0'));
			}
		}else 
		{
			if(F)RealValue*=(-1);//遇到非法字符认为结束了
			return RealValue;
		}
	}
	if(F)RealValue*=(-1);
	return RealValue;
}
/*---------------------------------------------------------------------------------------------------------
 Func: TTS基础算法 字符串转化为数字//这个算法是王三槐写的有问题
 Time: 2010-3-25
 Ver.: V1.0
 Note: *Sstr/待转换的源数字字符串 return/转换后的数值
 	   若字符串中含有非法字符返回0值
---------------------------------------------------------------------------------------------------------*/
f32 _GetStringToNumber(uint8 *Sstr)
{
	uint8 T,F=0,D=0,Factor=0;
	f32 RealValue=0;
	while(*Sstr>0){
		T=*Sstr++;
		if(T=='-')F=1;
		else if(T=='.'){
			D=1;Factor=10;
		}else if((T>='0')&&(T<='9')){		
			T=T-'0';	
			if(D==0)RealValue=RealValue*10+T;
			else	{
				RealValue=RealValue+(f32)T/(f32)Factor;
				Factor*=10;
			}
		}else return 0;		
	}
	if(F)RealValue*=(-1);
	return RealValue;
}
/*******************************************************************************
Func:BOOL类型从字符串转到
Date:2014-5-22
Note:
*******************************************************************************/
BOOL GetStringToBool(uint8 *Str)
{
	while(*Str)
	{
		if(*Str=='Y')return True;
		if(*Str=='N')return False;
		Str++;
	}
	return False;
}
/*******************************************************************************
Func:Buf填充
Date:2014-5-22
Note:
*******************************************************************************/
void BufferFill(void *S,uint8 Data,uint16 Length)
{
	uint8 *P;
	P=(uint8 *)S;
	while(Length--)*P++=Data;
}

/*---------------------------------------------------------------------------------------------------------
 Func: 复制字符串
 Time: 2014-2-2
 Ver.: V1.0
 Note: *
---------------------------------------------------------------------------------------------------------*/
uint16 BufferCoppy(void *S,void *Out,uint16 Length)
{
	uint16 Count=0;
	INT8U *PS,*PD;
	PS=(INT8U *)S;
	PD=(INT8U *)Out;
	Count =Length;
	while (Length--)
	{
		(*PD++)=(*PS++);
	}
	return Count;
} 
/*---------------------------------------------------------------------------------------------------------
 Func: 复制字符串
 Time: 2014-2-2
 Ver.: V1.0
 Note: *
---------------------------------------------------------------------------------------------------------*/
uint16 StringCoppy(void *S,void *Out)
{
	uint16 i=0;
	INT8U *PS,*PD;
	PS=(INT8U *)S;
	PD=(INT8U *)Out;
	while (*PS)
	{
		(*PD++)=(*PS++);
		i++;
	}
	*PD=0;
	return i;
}
/*******************************************************************************
Func:TQFS里用到的   获取正确的字符串
Date:2014-5-22
Note:字符串结尾处为未填充的0xff 改为0
*******************************************************************************/
void CreateTrueString(void *S)
{
	uint8 i;
	uint8 *P=(uint8 *)S;
	for(i=0;i<0xff;i++)
	{
		if(P[i]==0xff&&P[i+1]==0xff)break;
		if(P[i]==0x00)return;
	}
	P[i]=0x00;
}
/*---------------------------------------------------------------------------------------------------------
 Func:字符大小写转换
 Time: 2014-2-2
 Ver.: V1.0
 Note: Dir=False转为大写  Dir=True 转为小写
---------------------------------------------------------------------------------------------------------*/
void CharToUpperLower(uint8 Byte,BOOL Dir)
{
	if(Dir)
	{
		if(Byte<='z'&Byte>='a')Byte-=('a'-'A');
	}else 
	{
		if(Byte<='Z'&Byte>='A')Byte+=('a'-'A');
	}
}
/*---------------------------------------------------------------------------------------------------------
 Func: 字符串大小写转换
 Time: 2014-2-2
 Ver.: V1.0
 Note: Dir=False转为大写  Dir=True 转为小写
---------------------------------------------------------------------------------------------------------*/
void StringToUpperLower(uint8 *String,BOOL Dir)
{
	while(*String>0)
	{
		CharToUpperLower(*String++,Dir);
	}
}
/*---------------------------------------------------------------------------------------------------------
 Func: 字符串添加尾
 Time: 2014-2-2
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------*/
void StringEndingAppend(void *End,void *Out)
{
	int Length;
	INT8U *PS,*PD;
	PS=(INT8U *)Out;
	PD=(INT8U *)End;
	PS+=GetTextLength(Out);
	Length=GetTextLength(End);
	if(Length==-1)return;
	while(Length--)
	{
		(*PS++)=(*PD++);
	}
	(*PS++)=0;
}
/*---------------------------------------------------------------------------------------------------------
 Func: 字符串添加头
 Time: 2014-2-2
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------*/
void StringHeadAppend(void *Head,void *Out)
{
	INT8U Temp[40];
	StringCoppy(Head,&Temp[0]);
	StringEndingAppend(Out,&Temp[0]);
	StringCoppy(&Temp[0],Out);
}
/*---------------------------------------------------------------------------------------------------------
 Func: 中值滤波算法,去掉最小值和最大值
 Time: 2013-7-5
 Ver.: V1.0
 Note: 去掉一个最小值,去掉一个最大值,被去掉的值将被改写为0
 	   本方法要求输入值必须大于0,若为零则被忽略
---------------------------------------------------------------------------------------------------------*/
void MidFilter_ByWords(uint16 *Sdata,uint16 Length)
{
	uint16 *Sptr,ID,Min,Max,MinID,MaxID;
	if(Length<3)return;
	Sptr=Sdata;Min=0xFFFF;Max=0;ID=0;MinID=0;MaxID=0;
	do{
		if(*Sdata<Min)	{Min=*Sdata;MinID=ID;}
		if(*Sdata>=Max) {Max=*Sdata;MaxID=ID;}
		Sdata++;ID++;
	}while(--Length);
	Sptr[MinID]=0;	Sptr[MaxID]=0;
}
/***************************************************************************************
Func:MQTT长度打包
Date:2016-4-22
Note:
***************************************************************************************/
uint8 MQTT_LegnthToByte(uint32 Length,uint8 *Bytes)
{
	uint8 digit,i=0;
	do
	{
  digit =Length%128;
  Length = Length/128;
  if (Length>0)
    digit = digit|0x80;
		Bytes[i++]=digit;
	}
	while(Length>0);
	return i;
}
/***************************************************************************************
Func:MQTT长度解包
Date:2016-4-22
Note:
***************************************************************************************/
uint32 MQTT_GetLength(uint8 *Bytes)
{
	uint8 multiplier =1,i=0,digit;
	uint32 value=0;
	do
	{
  digit =Bytes[i++];
  value +=(digit&127) * multiplier;
  multiplier *= 128;
	}
	while((digit&128) != 0);
	return value;
}
float Abs(float Data)
{
	if(Data>0)return Data;
	else return (0-Data);
}
/*---------------------------------------------------------------------------------------------------------
 Func: 平均值滤波,求取数组的平均值
 Time: 2013-7-5
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------*/
uint16 AvrFilter_ByWords(uint16 *Sdata,uint16 Length,uint16 SumLen)
{
	uint32 Tsum=0;
	if(Length==0)return 0;
	do{	Tsum+=*Sdata++;	}while(--Length);
	return (uint16)(Tsum/SumLen);
}
/*---------------------------------------------------------------------------------------------------------
 Func: 求离散率
 Time: 2013-7-5
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------------------*/
uint32 DispersionRatioByWords(uint16 *Sdata,uint16 Length)
{
	uint16 Avr;
	uint32 Dis=0;
	Avr=AvrFilter_ByWords(Sdata,Length,Length);
	do{	Dis+=__fabs(Avr-(*Sdata++));}while(--Length);
 return Dis;
}
/*---------------------------------------------------------------------------------------------------------
 Func: 流池滤波法
 Time: 2014-9-18
 Ver.: V2.0
 Note: 
---------------------------------------------------------------------------------------------------------*/
uint16 FlowPoolFilter(uint32 *Pool,uint16 Data,uint8 *PoolIndex,uint8 PoolSize)
{
	uint16 Old_Filter=0;
	if(*PoolIndex)Old_Filter=((*Pool)/(*PoolIndex));
	(*Pool)+=Data;
	if((*PoolIndex)<PoolSize)(*PoolIndex)++;
	else (*Pool)-=Old_Filter; 
	return (*Pool)/(*PoolIndex);
}
/***************************************************************************************
Func:在数组中获取16位长度的数据
Date:2014-6-14
Note:Mode=0小端,Mode=1大端
***************************************************************************************/
uint16 GetWordFromBytes(string Buf,uint8 Mode)
{
	uint16 Temp=0;
	if(Mode==0)
	{
		Temp=Buf[1];
		Temp<<=8;
		Temp|=Buf[0];
	}else
	{
		Temp=Buf[0];
		Temp<<=8;
		Temp|=Buf[1];
	}
	return Temp;
}
/***************************************************************************************
Func:插入16位宽度的数据到数组中
Date:2014-6-14
Note:Mode=0小端,Mode=1大端
***************************************************************************************/
uint8 SetWordToBytes(string Buf,uint16 Data,uint8 Mode)
{
	if(Mode==0)
	{
		Buf[1]=Data>>8;
		Buf[0]=Data&0xff;
	}else
	{
		Buf[0]=Data>>8;
		Buf[1]=Data&0xff;
	}
	return 2;
}
/***************************************************************************************
Func:寻找字符计数
Date:2014-6-14
Note:
***************************************************************************************/
uint16 FindChar_Count(string Str,char Key)
{
	uint16 Count=0;
	while(*Str)
	{
		if(*(Str++)==Key)Count++;
	}
	return Count;
}
/***************************************************************************************
Func:寻找关键字
Date:2014-6-14
Note:
***************************************************************************************/
uint16 SerchKeyFromBytes(uint8 *Buf,uint8 Key,uint16 Length)
{
	uint16 i;
	for(i=0;i<Length;i++)
	{
		if((*Buf++)==Key)return i;
	}
	return 0xffff;
}
/***************************************************************************************
Func:比较数组
Date:2014-6-14
Note:
***************************************************************************************/
BOOL CompareWithBuffer(uint8 *S,uint8 *K,uint16 Length)
{
	while(Length--)
	{
		if(*S++!=*K++)return False;
	}
	return True;
}
/***************************************************************************************
Func:比较字符串
Date:2014-6-14
Note:
***************************************************************************************/
BOOL CompareWithString(uint8 *S,uint8 *K)
{
	if(GetTextLength(S)!=GetTextLength(K))return False;
	while(*S&&*K)
	{
		if(*S++!=*K++)return False;
	}
	return True;
}
/***************************************************************************************
Func:搜索字符位置并记录位置
Date:2014-6-14
Note:
***************************************************************************************/
uint8 SplitWithString(uint8 *S,uint8 Key,uint8 *Buf)
{
	uint8 Index=0;
	uint8 i=0;
	while(*S)
	{
		if(*S==Key){Buf[i++]=Index;}
		Index++;
		S++;
	}
	if(Index==0)return 0;
	return i+1;
}
/***************************************************************************************
Func:清除缓冲区
Date:2014-6-14
Note:
***************************************************************************************/
void ClearWithBuffer(uint8 *S,uint16 Length)
{
	while(Length--)
	{
		*S++=0;
	}
}
/***************************************************************************************
Func:浮点到数组
Date:2014-6-14
Note:
***************************************************************************************/
uint16 Float32ToByte(uint8 *S,float Data)
{
	uint8 *P;
	uint8 i=4;
	P=(uint8 *)&Data;
	while(i--)
	{
		*S++=*P++;
	}
	return 4;
}
/***************************************************************************************
Func:整形到数组
Date:2014-6-14
Note:
***************************************************************************************/
uint16 Uint32ToByte(uint8 *S,uint32 Data)
{
	uint8 *P;
	uint8 i=4;
	P=(uint8 *)&Data;
	while(i--)
	{
		*S++=*P++;
	}
	return 4;
}
uint16 Uint16ToByte(uint8 *S,uint32 Data)
{
	uint16 *P;
	uint8 i=4;
	P=(uint16 *)&Data;
	while(i--)
	{
		*S++=*P++;
	}
	return 2;
}
float Float32FromByte(uint8 *S)
{
	float *P;
	P=(float *)S;
	return *P;
}
uint32 Uint32FromByte(uint8 *S)
{
	uint32 *P;
	P=(uint32 *)S;
	return *P;
}
uint8 IpPortToString(IpPortStruct *IPPort,uint8 *String)
{
	uint8 n,Length,Index=0;
	for(n=0;n<4;n++)
	{
		Length=GetStringFromNumber(&String[Index],IPPort->Ip[n],'D',0);
		Index+=(Length);
		if(n==3)String[Index-1]=':';else String[Index-1]='.';
	}
	Length=GetStringFromNumber(&String[Index],IPPort->Port,'D',0);
	Index+=(Length);
	return Index;
}
void IpPortFromString(IpPortStruct *IPPort,uint8 *String)
{
	uint8 i=0,c=0;
	uint16 T=0;
	while(*String)
	{
		if((*String>='0')&(*String<='9'))
		{
			T*=10;
			T+=(*String-'0');
		}
		if(*String=='.'){IPPort->Ip[c++]=T;T=0;}
		if(*String==':'){IPPort->Ip[c++]=T;T=0;}
		String++;
		if(i++>20)break;
	}
	IPPort->Port=T;
}
uint32 StringToIPUint32(uint8 *String)
{
	uint8 i=0;
	uint16 T=0;
	uint32 Temp=0;
	while(*String)
	{
		if((*String>='0')&(*String<='9'))
		{
			T*=10;
			T+=(*String-'0');
		}
		if(*String=='.'){Temp<<=8;if(T>0xff)T=0xff;Temp|=T;T=0;}//怀疑这个有问题，暂时没时间看
		String++;
		if(i++>15)break;
	}
	Temp<<=8;Temp|=T;
	return Temp;
}
uint8 IPUint32ToString(uint8 *String,uint32 IP)
{
	uint8 n,Length,Temp,Index=0;
	for(n=0;n<4;n++)
	{
		Temp=IP/0xffffff;
		IP<<=8;
		Length=GetStringFromNumber(&String[Index],Temp,'D',0);
		Index+=(Length);
		if(n!=3)String[Index-1]='.';	
	}
	return Index;
}
uint8 Bcd_Decimal(uint8 bcd)
{
 return bcd-(bcd >> 4)*6;
}

uint8 Decimal_Bcd(uint8 decimal)
{
	return (uint8) (decimal+(decimal / 10)*6);
}
void FmtToBuf(char *String,char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    vsprintf(String,fmt,ap);
    va_end(ap);
}
/*******************************************************************************
Func:模式匹配
Date:2015-09-05
Note:系统库strstr函数居然有问题，只好自己实现了
*******************************************************************************/
uint16 BytesFindString(char *S,const char *T,int Length)
{
	uint16 i=0,j=0;
	while(Length-->0&T[j]>0)
	{
		if(S[i]==T[j])
    {i++;j++;}
		else {i=i-j+1;j=0;}
	}
	if(T[j]==0)return i;
	else return 0;
}
/*******************************************************************************
Func:从字符串转到16进制数
Date:2015-09-05
Note:
*******************************************************************************/
uint8 GetHexFromString(uint8 *String)
{
	uint8 Data=0x00;
	if((String[0]>='0')&(String[0]<='9'))Data=(String[0]-'0')*16;
	else if((String[0]>='A')&(String[0]<='F'))Data=(String[0]-'A'+10)*16;
	else if((String[0]>='a')&(String[0]<='f'))Data=(String[0]-'a'+10)*16;
	if((String[1]>='0')&(String[1]<='9'))Data+=(String[1]-'0');
	else if((String[1]>='A')&(String[1]<='F'))Data+=(String[1]-'A'+10);
	else if((String[1]>='a')&(String[1]<='f'))Data+=(String[1]-'a'+10);
	return Data;
}
/*******************************************************************************
Func:从TFT565色转到三基色
Date:2015-09-15
Note:
*******************************************************************************/
Color_RGB TFT_RGB(uint16 Color)
{
	Color_RGB T;
	T.R=0x1f&(Color>>11);
	T.G=0x3f&(Color>>5);
	T.B=0x1f&(Color);
	return T;
}
/*******************************************************************************
Func:24bit色转TFT565色
Date:2015-09-15
Note:
*******************************************************************************/
uint16 RGB_TFT(Color_RGB T)
{
	uint16 C;
	C=0x1f&(T.R);
	C<<=6;
	C|=(0x3f&(T.R));
	C<<=5;
	C|=(0x1f&(T.B));
	return C;
}
uint16 ALL_TFT(uint32 Color)
{
	uint16 C;
	C=0x1f&(Color>>16);
	C<<=6;
	C|=(0x3f&(Color>>8));
	C<<=5;
	C|=(0x1f&(Color));
	return C;
}

