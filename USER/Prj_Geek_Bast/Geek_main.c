#include <Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Tos_Mema.h>
#include <FuncTask_Include.h>
#include <..\USER\Prj_Geek_Bast\IO_Driver.c>
#include <..\USB\STM32_USB-FS-Device_Driver\USB_FS_Device_Driver.c>
#include <..\USER\Prj_Geek_Bast\usb_desc.c>
#include <..\USER\Prj_Geek_Bast\hw_config.c>
#include <..\USER\Prj_Geek_Bast\usb_endp.c>
#include <..\USER\Prj_Geek_Bast\usb_istr.c>
#include <..\USER\Prj_Geek_Bast\usb_prop.c>
#include <..\USER\Prj_Geek_Bast\usb_pwr.c>
#include <..\Driver\SSD1306_Driver.c>
#include <..\GUI\GUI_Include.c>
#include <TGui_3DCube.c>
#include <..\Driver\Virtual_Usart.c>
#include <..\USER\Prj_Geek_Bast\Explainer.c>
#include <..\MGUI\Menu_Base.c>
#include <..\USER\Prj_Geek_Bast\Page.c>
#include <..\USER\Prj_Geek_Bast\PageApp.c>
typedef struct Text
{
	struct Text *NextAddr;
	char  Line;
}Text;
void Task0(void *Tags)
{
	Remap_SWJ_JTAGDisable();
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
	Tos_TaskDelay(100);
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	while(1)
	{	
	Tos_TaskDelay(1000);
	LED5_ON;
	Tos_TaskDelay(100);
	LED5_OFF;
	}
}
Text *TT;

void Task1(void *Tags)
{
	Text *T;
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
	while(1)
	{
		Tos_TaskDelay(1000);
		T=TT;
		while(T!=0)
		{
		VisualCOM_WriteLine(&T->Line);
		Bast_Export(&T->Line);
		T=T->NextAddr;
		}
	}
}

void Text_Join(Text **tt,uint8 *Str)
{
	Text *t=*tt,**oldT=tt;
	uint8 Length;
	Length=GetTextLength(Str);
	if((*oldT)==0)
	{
		(*oldT)=Mema_Malloc(Length+5);
		(*oldT)->NextAddr=0;
		StringCoppy(Str,&(*oldT)->Line);
		return;
	}
	while(t->NextAddr!=0)
	{
		t=t->NextAddr;
	}
	t->NextAddr=Mema_Malloc(Length+5);
	StringCoppy(Str,&t->NextAddr->Line);
	t->NextAddr->NextAddr=0;
}



void Task2(void *Tags)
{
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
	Text_Join(&TT,"task1\n");
	Text_Join(&TT,"port.cfg(pc0,out)\n");
	Text_Join(&TT,"loop\n");
	Text_Join(&TT,"{\n");
	Text_Join(&TT," delay.ms.1000\n");
	Text_Join(&TT," port.set(pc0,0)\n");
	Text_Join(&TT," delay.ms.1000\n");
	Text_Join(&TT," port.set(pc0,1)\n");
	Text_Join(&TT,"}\n");
	while(1)
	{
		
		Tos_TaskDelay(100);
	}
}
Text *TTT;
void Task3(void *Tags)
{
	Text *T;
	Text_Join(&TTT,"task1\n");
	Text_Join(&TTT,"port.cfg(pc1,out)\n");
	Text_Join(&TTT,"port.cfg(pc2,out)\n");
	Text_Join(&TTT,"port.cfg(pc3,out)\n");
	Text_Join(&TTT,"loop\n");
	Text_Join(&TTT,"{\n");
	Text_Join(&TTT," port.set(pc1,0)\n");
	Text_Join(&TTT," delay.ms.10\n");
	Text_Join(&TTT," port.set(pc1,1)\n");
	Text_Join(&TTT," port.set(pc2,1)\n");
	Text_Join(&TTT," delay.ms.1000\n");
	Text_Join(&TTT," port.set(pc2,0)\n");
	Text_Join(&TTT," port.set(pc3,0)\n");
	Text_Join(&TTT,"}\n");
	Tos_TaskDelay(10);
	while(1)
	{
		T=TTT;
		while(T!=0)
		{
		VisualCOM_WriteLine(&T->Line);
		Bast_Export(&T->Line);
	  T=T->NextAddr;
		}
	}
}
void Task4(void *Tags)
{

	Tos_TaskDelay(10);
//	LCD_Clear_Screen(0x00);
//	TGUI_DefaultStyle();
//	TGUI_Set_Font(TGUI_FontType_12X12);
//	TGUI_Set_Color(1,1,0,0);
	while(1)
	{
//		TGUI_Draw_FormatString(0,0,100,50,"测试字体",Align_CenterTop,0);
//		TGui_Draw_Line(0,1,30,50,1);
//		TGUI_DrawRound(40,40,20,1);
	
			Tos_TaskDelay(100);
	}
}
void Task5(void *Tags)
{
	Coordinate_3D  *Tem=(Coordinate_3D  *)Tags;
	while(1)
	{
		TGui_3DCobe(Tem->X,Tem->Y,Tem->Z,Tem->D);
	}
}
const Coordinate_3D C3D_My={LCD_XSIZE-25,25,60,40};
const Coordinate_3D C3D_My1={25,25,50,40};
const TaskInitList TaskList[]={
{Task0,Null,"Task0",2000},
{Task1,Null,"Task0",2000},
{Task2,Null,"Task0",2000},
{Task3,Null,"Task0",2000},
{Task4,Null,"Task_Arrary",2000},
//{Task5,(void *)&C3D_My,"CUBE",2000},
//{Task5,(void *)&C3D_My1,"CUBE",2000},
{Task_Arrary,Null,"Task_Arrary",2000},
{MUI_Task,(void *)&MenuHome[0],"MUI_Task",2000},
{Task_GetKeyVirtual,Null,"Key",2000},
{Task_RBuffer,Null,"Key",2000},
{Null},
};
const GPIO_InitStruct IO_SSD1306_List[]=
{
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_15},//D0时钟
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_14},//D1数据
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOC,GPIO_Pin_13},//RES复位
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_9},//DC数据命令选择
	{GPIO_Speed_2MHz,GPIO_Mode_Out_PP,GPIOB,GPIO_Pin_8},//CS片选
	{0},
};

int main(void)
{
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x5000); 
	Mema_Init(&MemBuf[0]);
	DeviceList_Init();
	DeviceMount_SS1306(IO_SSD1306_List);
	Tos_ListCreate(TaskList);
	while(1);
}



