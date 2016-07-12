
#include <Include.h>
#include <sysruntime.h>

#define _DEBUG_MODE


const uint32 CpuIdCode[]={0x55AAAA55,0x22334455,0xC33C3CC3};
const uint8 HardWareVersion=42;
const uint8 SoftWareVersion=50; 
const uint8 SerialNumber[]={'5','A','A','5','0','C','C','0','3','C','C','0','A','B','B','A','F','F','E','Z'};
const uint8  FactoryName[]={'_','H','B','Y','L','K','J','_','T','T','S','_','W','R','T','O','S','_', 0 , 0 };
const uint32 ProductTime=0;

uint8  SysDogState;
uint8  SysDogTimer0;
uint8  SysDogTimer1;
uint8  SysDogTimer2;
uint8  SysDogTimer3;
uint8  SysDogTimer4;


//uint8 System_CheckCode()
//{
//	uint8 R=0;
//	#ifndef _DEBUG_MODE
//		uint32 *Addr=(uint32 *)(0x1FFFF7E8);
//		if((*Addr)==CpuIdCode[2])R|=S2;	Addr++;
//		if((*Addr)==CpuIdCode[1])R|=S1;	Addr++;
//		if((*Addr)==CpuIdCode[0])R|=S0;
//	#endif
//	R|=(SysCallIndex<<4);
//	SysCallIndex++;
//	return R;
//}




void System_SoftDogUpdate()
{
	if(SysDogTimer0<250)SysDogTimer0++;
	if(SysDogTimer1<250)SysDogTimer1++;
	if(SysDogTimer2<250)SysDogTimer2++;
	if(SysDogTimer3<250)SysDogTimer3++;
	if(SysDogTimer4<250)SysDogTimer4++;     
}

/********************************************
    返回看门狗状态，>0说明有事件超时
********************************************/
uint8 System_GetRunState()
{
	uint8  R=0;
	if( ( (SysDogState&S0)>0 )&&(SysDogTimer0>5) ) R=1;     //监控OS后台任务
	if( ( (SysDogState&S1)>0 )&&(SysDogTimer1>20) ) R=2;    //监控SSD1963芯片是否死机
	if( ( (SysDogState&S2)>0 )&&(SysDogTimer2>20) ) R=3;    //监控GUI线程2秒之内超时
	if( ( (SysDogState&S3)>0 )&&(SysDogTimer3>20) ) R=4;    //监控触屏驱动线程2秒之内超时
	if( ( (SysDogState&S4)>0 )&&(SysDogTimer4>20) ) R=5;    //监控485通讯线程2秒之内超时
	return R;
}
