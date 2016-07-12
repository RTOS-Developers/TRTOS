#include <Tos_Mema.h>
#include <text.h>
TOS_MEMA_Ptr *RootMema;

uint8 MemBuf[MEMA_BUFFER_SIZE];
/********************************************************************************
 Func: 内存初始化
 Time: 2015-09-14
 Ver.: V1.0
 Note; 传入内存堆地址
********************************************************************************/
void Mema_Init(uint8 *Addr)
{
	RootMema=(TOS_MEMA_Ptr *)Addr;//绑定位置到内存块头
	RootMema->NextPtr=0;
	RootMema->Size=sizeof(TOS_MEMA_Ptr);
}
/********************************************************************************
 Func: 分配内存
 Time: 2015-09-14
 Ver.: V1.0
 Note; 去掉了父节点
********************************************************************************/
void *Mema_Malloc(uint16 Size)
{
	TOS_MEMA_Ptr *P=RootMema,*TP;
	Size+=sizeof(TOS_MEMA_Ptr);
	while(P->NextPtr!=0)
	{
		if(((uint32)P->NextPtr-(uint32)P-P->Size)>=Size)//当当前节点和后节点之间能插入请求块
		{
			TP=(TOS_MEMA_Ptr *)((uint32)P+P->Size);
			TP->Size=Size;
			TP->NextPtr=P->NextPtr;
			P->NextPtr=TP;
			return (uint8 *)((uint32)TP+sizeof(TOS_MEMA_Ptr));
		}
		P=P->NextPtr;
	}
	TP=(TOS_MEMA_Ptr *)((uint32)P+P->Size);
	TP->NextPtr=0;
	TP->Size=Size;
	P->NextPtr=TP;
	
	return (uint8 *)((uint32)TP+sizeof(TOS_MEMA_Ptr));
}
/********************************************************************************
 Func: 计算内存使用情况
 Time: 2015-09-14
 Ver.: V1.0
 Note; 去掉了父节点
********************************************************************************/
uint32 Mema_Count()
{
	TOS_MEMA_Ptr *P=RootMema;
	uint32 Size=P->Size;
	while(P->NextPtr!=0)
	{
		P=P->NextPtr;
		Size+=P->Size;
	}
	return Size;
}
/********************************************************************************
 Func:释放占用内存
 Time: 2015-09-14
 Ver.: V1.0
 Note; 去掉了父节点
********************************************************************************/
void Mema_Free(void *Addr)
{
	TOS_MEMA_Ptr *P=RootMema,*TP=0;
	while(P->NextPtr)
	{
		if((uint32)P==((uint32)(uint8 *)Addr-sizeof(TOS_MEMA_Ptr)))goto Free;
		TP=P;
		P=P->NextPtr;
	}
	if(TP)TP->NextPtr=0;return;
Free:
	TP->NextPtr=P->NextPtr;//当本节点存在子节点父节点的子节点等于自身节点子节点
}
/********************************************************************************
 Func:改变容量
 Time: 2015-11-12
 Ver.: V1.0
 Note; 
********************************************************************************/
void *Mema_Extend(void *Addr,uint16 Size)
{
	uint8 *PR;
	TOS_MEMA_Ptr *P;
	PR=Mema_Malloc(Size);
	P=(TOS_MEMA_Ptr *)((uint8 *)Addr-sizeof(TOS_MEMA_Ptr));
	BufferCoppy(Addr,PR,P->Size-sizeof(TOS_MEMA_Ptr));
	Mema_Free(Addr);
	return PR;
}

