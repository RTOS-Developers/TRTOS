#include <Include.h>
typedef struct
{
	const uint8 *Name;
	const uint8 *Init;
	const uint8 *Start;
}Geek_RunStruct;
const uint8 Geek_String[]=
"task1\n"
"{\n"
	"pcfg(C,3,O)\n"
	"{\n"
	 "pout(C,3,L)\n"
	 "delay.ms(1000)\n"
	 "pout(C,3,H)\n"
	 "delay.ms(1000)\n"
	"}\n"
"}"
"task2"
"{"
	"pcfg(E,5,I)"
	"pcfg(C,4,O)"
	"{"
	 "pin(E,5)pout(C,4,L)?pout(C,4,H)"
	"}"
"}"
;
void Geek_GetInfor(const uint8 *String,Geek_RunStruct *GR)
{
	GR->Name=String;
	while(*String++!='{');
	GR->Init=String;
	while(*String++!='{');
	GR->Start=String;
}
void Geek_Export(const uint8 *String)
{
	const uint8 *P=String;
	Geek_RunStruct GR;
	Geek_GetInfor(P,&GR);
	USART_WriteLine(USART2,(uint8 *)GR.Name);
	USART_WriteLine(USART2,(uint8 *)GR.Init);
//	DeBug("GR.Start:%s",GR.Start,Infor_Infor);
//	while(1)
//	{
//		for(i=0;i<sizeof(Geek_String);i++)
//		{
//			if(String[i]=='{')deep++;
//			if(String[i]=='}')deep--;
//			
//		}
//	}
	//Tos_TaskCreateFindID(TIL->vfFuncPointer,(uintbus)TIL->Tags,TIL->TaskName,TIL->StackSize);
}
