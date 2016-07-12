#include <Include.h>

#ifdef BOARD_TWOCAN
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <TGUI_Include.h>
#include <MGUI_Include.h>
#include <FuncTask_Include.h>
#include <Usb_Include.h>
void Task0(void *Tags)
{
	Tos_TaskDelay(2);
	DeBug_OutSet(DeBug_USART|DeBug_USART2);//调试信息通过串口打印
	DeBug_SetType(Infor_All);//只打印Infor类型的消息
	Printf_SysInfor();
	while(1)
	{

		LED2_ON;
		LED1_ON;
		LED3_ON;
		Tos_TaskDelay(100);
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		Tos_TaskDelay(900);
		//TGUI_Set_Color(Color_Green,Color_Green,TGUI_Button_ForeColor,Null);
		DeBug("TosTask0",Infor_Infor);
		DeBug("Tos_CPUInfor.Userate_CPU=%d",Tos_CPUInfor.Userate_CPU,Infor_Infor);
	}
}
void Task(void *Tags)
{
	while(1)
	{
		Tos_TaskDelay(900);
		//TGUI_Set_Color(Color_Green,Color_Green,TGUI_Button_ForeColor,Null);
		DeBug("TosTask0",Infor_Infor);
	}
}
void Task1(void *Tags)
{
	TGUI_ProcessBar ProcessBar1;
	TGUI_ProcessBar ProcessBar2;
	TGUI_ProcessBar ProcessBar3;
	TGUI_DefaultStyle();
	TGUI_Set_Font(TGUI_FontType_16X16);
 	TGUI_ProcessBar_Create(0x1001,0,0,480,15,100,Null,Null,&ProcessBar1);
  TGUI_ProcessBar_Create(0x1002,0,30,480,18,1925,Null,Null,&ProcessBar2);
 	TGUI_ProcessBar_Create(0x1002,0,60,480,18,1925,Null,Null,&ProcessBar3);
	TGUI_Set_Color(Color_Green,Color_Black,Color_Blue,Null);
	TGUI_Clear_Show();
	while(1)
	{
 		TGUI_ProcessBar_SetProcess(&ProcessBar1,Tos_CPUInfor.Userate_CPU);
 		TGUI_ProcessBar_ReDraw(&ProcessBar1,TGUI_State_ReLoad);
 		TGUI_ProcessBar_SetProcess(&ProcessBar2,TOUCH_ADC_YValue);
 		TGUI_ProcessBar_ReDraw(&ProcessBar2,TGUI_State_ReLoad);
 		TGUI_ProcessBar_SetProcess(&ProcessBar3,TOUCH_ADC_XValue);
		TGUI_ProcessBar_ReDraw(&ProcessBar3,TGUI_State_ReLoad);
		TGUI_Set_Color(Color_Red,Color_Black,Color_Black,Null);
		DeBug("TosTask1",Infor_Infor);
		Tos_TaskDelay(1000);
	}
}
void TaskKey(void *Tags)
{
	uint16 i;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetKeyBoard();
	while(1)
	{		
		Tos_TaskDelay(10);
		 CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
		  DeBug("KeyValue=%d,KeyType=%d",KeyMsg->KeyValue,KeyMsg->KeyType,Infor_Infor);
		}
	}
}

int main(void)
{
	
	RCC_Configuration();
	DeBug_Init();
	DeviceList_Init();
	DeviceMount_Usart1();
	DeviceMount_Usart2();
//	DeviceMount_Lcd();
	DeviceMount_Led();
//	DeviceMount_Spk();
	DeviceMount_Flash();
	//DeviceMount_Touch();
	Device_Init();
	Tos_Init();
 	Tos_TaskCreate(0,Task0,Null,"Task0",3000);
	//Tos_TaskCreate(1,Usart_Terminal,3000);
	Tos_TaskCreate(1,USB_HID_KeyBoard_Task,Null,"TestTask",3000);
//	Tos_TaskCreate(2,TosTask2,3000);
	Tos_TaskCreate(3,Task,Null,"dfsa",1000);
//	Tos_TaskCreate(2,Task_GetTouch,1000);
//	Tos_TaskCreate(3,Task_GetScreen,1000);
//	Tos_TaskCreate(2,Task_GetKeyBoard,Null,"Task_GetKeyBoard",3000);
	//Tos_TaskCreate(5,TGUI_Window_MsgTask,3000);
	Tos_Start();
	while(1);
}

#endif

