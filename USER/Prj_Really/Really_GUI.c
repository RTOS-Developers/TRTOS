#include <Include.h>
#include <Really_Driver.h>
#include <Really_GUI.h>
#include <text.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <Really_Driver.h>
#include <Really_GUI.h>
#include <FuncTask_Include.h>
#include <HandKey_Driver.h>
#include <App_Really.h>
#include <Usart1_Driver.h>
uint8 Really_DevIndex=0;
void Really_ShowStatus(BOOL qh,BOOL zy,BOOL jh)
{
	uint8 Status[]="重心偏前左侧";
	if(qh)BufferCoppy("后",&Status[6],2);
	if(zy)BufferCoppy("右",&Status[8],2);
	if(jh)StringCoppy("重心均衡    ",&Status[0]);
	print_string_xy(ReallyID,2,217,(uint8 *)"状态:");
	print_string_xy(ReallyID,40,217,(uint8 *)Status);
}
void Really_DrawOneNum(uint8 Num,BOOL En)
{
	
}
void Really_DrawNum(uint8 Num)
{
	uint8 i;
	uint8 Buf[2];
	uint8 x=40;
	uint8 y=25;
	for(i=0;i<PDA_Dev_Size+1;i++)
	{
		if(Num==i)draw_circle(0xff,i*30+x,y+12,10,0xff,0x00,0x00);
		else draw_circle(0xff,i*30+x,y+12,10,0x07,0x07,0x00);
		Buf[0]=i+'1';
		Buf[1]=0;
		print_string_xy(ReallyID,i*30+x-3,y+4,(uint8 *)&Buf[0]);
	}
}
void Really_CreateBase(uint8 *Title)
{
	clear_display(0xff);
	foreground_color(0xff,0x0,0x0,0xff);
	background_color(0xff,Color_Back,Color_Back,Color_Back);
	background_nor_transparen(0xff);
	windows(0xff,0,0,320,240,Title);
	draw_line(0xff,0,44,320,44,0x00,0xff,0x00);
	simplified_chinese(0xff);
	font_size_16(0xff);
}
void Really_TextButton(uint16 x,uint16 y,char *String,BOOL ClickTrue)
{
	uint8 Length;
  Length=GetTextLength(String); 
	Length*=4+5;
	if(ClickTrue)button_click(ReallyID,x,y,Length,25);
	else button(ReallyID,x,y,Length,25);
	print_string_xy(ReallyID,x+3,y+4,(uint8 *)String);
}
void Really_ShowFloat(uint16 x,uint16 y,float Value,uint8 DotSize)
{
	uint8 Buf[20];
	GetStringFromNumber(&Buf[0],Value,'D',DotSize);
	print_string_xy(ReallyID,x,y,&Buf[0]);
}
void Really_ShowOption(uint8 Index,char *Name,char *Unit,f32 Value,uint8 DotSize)
{
	uint8 Buf[20];
	uint16 Really_Space=30;
	uint8 TopSpare=56;
	GetStringFromNumber(&Buf[0],Value,'D',DotSize);
	textbox(0xff,190,TopSpare-5+Index*Really_Space,70,25);
	print_string_xy(0xff,90,TopSpare+Index*Really_Space,(uint8 *)Name);//项目名
	print_string_xy(0xff,265,TopSpare+Index*Really_Space,(uint8 *)Unit);//单位
	background_color(0xff,0xff,0xff,0xff);
	foreground_color(0xff,0x00,0x00,0x00);
	print_string_xy(0xff,194,TopSpare+Index*Really_Space,&Buf[0]);//数值
	background_color(0xff,Color_Back,Color_Back,Color_Back);
	foreground_color(0xff,0x0,0x0,0xff);
}
void _Really_ShowOption(uint8 Index,char *Name,char *Unit,f32 Value,f32 Max,uint8 DotSize,uint8 Mode)
{
	uint8 Buf[20];
	uint16 Really_Space=30;
	uint8 TopSpare=66;
	if(Mode&RRM_Data)
	{
	GetStringFromNumber(&Buf[0],Value,'D',DotSize);
	textbox(0xff,35,TopSpare-5+Index*Really_Space,50,25);
	if(Value>Max){
		background_color(0xff,0xff,0x00,0x00);
		foreground_color(0xff,0xff,0xff,0xff);
	}else{
	background_color(0xff,0xff,0xff,0xff);
	foreground_color(0xff,0x00,0x00,0x00);
	}
	print_string_xy(0xff,37,TopSpare+Index*Really_Space,&Buf[0]);//数值
	background_color(0xff,Color_Back,Color_Back,Color_Back);
	foreground_color(0xff,0x0,0x0,0xff);
	}
	if(Mode&RRM_Base)
	{
	print_string_xy(0xff,2,TopSpare+Index*Really_Space,(uint8 *)Name);//项目名
	print_string_xy(0xff,88,TopSpare+Index*Really_Space,(uint8 *)Unit);//单位
	}
}
void __Really_ShowOption(uint8 Index,char *Name,char *Unit,f32 Value,uint8 DotSize)
{
	uint8 Buf[20];
	uint16 Really_Space=30;
	uint8 TopSpare=66;
	GetStringFromNumber(&Buf[0],Value,'D',DotSize);
	textbox(0xff,220,TopSpare-5+Index*Really_Space,65,25);
	print_string_xy(0xff,138,TopSpare+Index*Really_Space,(uint8 *)Name);//项目名
	print_string_xy(0xff,287,TopSpare+Index*Really_Space,(uint8 *)Unit);//单位
	background_color(0xff,0xff,0xff,0xff);
	foreground_color(0xff,0x00,0x00,0x00);
	print_string_xy(0xff,223,TopSpare+Index*Really_Space,&Buf[0]);//数值
	background_color(0xff,Color_Back,Color_Back,Color_Back);
	foreground_color(0xff,0x0,0x0,0xff);
}
void Really_ButtonList(uint8 Select,uint8 Max)
{
	uint16 Really_Space=30;
	uint8 BSelect;
	uint8 YSelect;
	uint8 i;
	char Buf[10];
	YSelect=Select%6;
	BSelect=Select/6;
	StringCoppy("设备(01)",&Buf[0]);
	draw_rectangle_fill(0xff,5,30,75,200,Color_Back,Color_Back,Color_Back);
	for(i=0;i<6;i++)
	{
		Buf[5]=(BSelect*6+i)/10+'0';
		Buf[6]=(BSelect*6+i)%10+'0';
		if((BSelect*6+i-1)>=Max){
			return;
		}
		Really_TextButton(5,Really_Space*i+40,&Buf[0],(BOOL)(YSelect==i));
	}
}
void Really_ClearTab(uint8 Page)
{
	uint8 Buf[10];
	
	StringCoppy("第一页",&Buf[0]);
	Buf[2]=Page/10+'0';
	Buf[3]=Page%10+'0';
	frame(0xff,80,35,235,200,&Buf[0]);
	draw_rectangle_fill(0xff,90,50,210,180,Color_Back,Color_Back,Color_Back);
}


float zz,zl1,zl2,zl3,zl4,zypz,qhpz,qhpy,zypy,cqhpz,zez,ez1,ez2,ez3,ez4;
void Really_DevSelect(uint8 Dev,uint8 Mode)
{
	uint8 i=0;
	uint8 *Data;
	uint8 Buf[20];
	StringCoppy("设备(  )",&Buf[0]);
	Buf[5]=(Dev+1)/10+'0';
	Buf[6]=(Dev+1)%10+'0';
	Data=&PDA_Buf[Dev][0];
	//frame(0xff,5,35,310,200,&Buf[0]);
	
	if(Mode&RRM_Clear)draw_rectangle_fill(0xff,0,40,320,240,Color_Back,Color_Back,Color_Back);
	draw_line(0xff,0,50,320,50,0x00,0xff,0x00);
	Really_DrawNum(Dev);
	zez=GetWordFromBytes(&PDA_Buf[Dev][i*2+12],1);zez/=100;i++;
	ez1=GetWordFromBytes(&PDA_Buf[Dev][i*2+12],1);ez1/=100;i++;
	ez2=GetWordFromBytes(&PDA_Buf[Dev][i*2+12],1);ez2/=100;i++;
	ez3=GetWordFromBytes(&PDA_Buf[Dev][i*2+12],1);ez3/=100;i++;
	ez4=GetWordFromBytes(&PDA_Buf[Dev][i*2+12],1);ez4/=100;i++;
	i=0;
	zz=GetWordFromBytes(&PDA_Buf[Dev][i*2],1);zz/=100;
	_Really_ShowOption(i++,"总重","(t)",zz,zez,2,Mode);
	zl1=GetWordFromBytes(&Data[i*2+2],1);zl1/=100;
	_Really_ShowOption(i++,"重一","(t)",zl1,ez1,2,Mode);
	zl2=GetWordFromBytes(&Data[i*2+2],1);zl2/=100;
	_Really_ShowOption(i++,"重二","(t)",zl2,ez2,2,Mode);
	zl3=GetWordFromBytes(&Data[i*2+2],1);zl3/=100;
	_Really_ShowOption(i++,"重三","(t)",zl3,ez3,2,Mode);
	zl4=GetWordFromBytes(&Data[i*2+2],1);zl4/=100;
	_Really_ShowOption(i++,"重四","(t)",zl4,ez4,2,Mode);

	i=0;
	qhpz=(zl1+zl3)-(zl2+zl4);
	__Really_ShowOption(i++,"箱前后偏重","(t)",qhpz,2);
	zypz=(zl1+zl2)-(zl3+zl4);
	__Really_ShowOption(i++,"箱左右偏重","(t)",zypz,2);
	qhpy=6000*qhpz/zz;
	__Really_ShowOption(i++,"箱前后偏移","(mm)",qhpy,0);
  zypy=1170*zypz/zz;
	__Really_ShowOption(i++,"箱左右偏移","(mm)",zypy,0);
  cqhpz=qhpz*40/31;
	__Really_ShowOption(i++,"车前后偏重","(t)",cqhpz,2);
	
	Really_ShowStatus((BOOL)(qhpz<0),(BOOL)(zypz<0),(BOOL)((qhpz==0)&(zypz==0)));
	
}


void Really_Init()
{
	USART1_Struct_Init(9600);
	Tos_TaskDelay(1000);
	//Really_CreateBase("查看");
	
	baud(0xff,115200);
	baud(0xff,115200);
	USART1_Struct_Init(115200);
	Tos_TaskDelay(100);
}

void Really_InforPage()
{
	windows(0xff,10,10,300,220,"制造商与产品信息:");
	frame(0xff,15,45,290,80,"制造商信息");
	print_string_xy(0xff,20,60,"制造商:宜昌三大安科电气科技有限公司");
	print_string_xy(0xff,20,80,"联系电话:0717-6470699");
	print_string_xy(0xff,20,100,"公司网站:http://www.ljxz.net/");
	frame(0xff,15,140,290,80,"产品信息");
	print_string_xy(0xff,20,155,"系统:TRTOS");
	print_string_xy(0xff,20,175,"平台:ARM");
	print_string_xy(0xff,20,195,"网站:http://www.trtos.com/");
}
void Really_ShowInfor()
{
	
	uint8 i=0;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];	
	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetHandKey();
  Tos_TaskGetPDA();
	//Really_CreateBase();
	//display_ID();
	Really_InforPage();
	while(1)
	{
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
			if(KeyMsg->KeyValue!=NULL)
			{
				SleepCount=0;
			}
			if(KeyMsg->KeyValue==Key_Down)
			{
				Tos_TaskDeleteQmsg();
				return;
			}
		}
	}
	
}


void Really_Task(void *Tags)
{
	
	uint8 i=0;
	Tos_ArrayMessage *CurMsg,Msg[10];
  MGUI_KeyMsg *KeyMsg,KeyMsgBuf[10];	
	Really_Init();
R:	for(i=0;i<10;i++)Msg[i].Qmsg_Addr=&KeyMsgBuf[i];
	Tos_TaskCreateQmsg(&Msg[0],10);
	Tos_TaskGetHandKey();
  Tos_TaskGetPDA();
	Really_CreateBase("查看");
	Really_DevSelect(Really_DevIndex,RRM_Base|RRM_Data|RRM_Clear);
	while(1)
	{
		CurMsg=Tos_TaskGetQMsg();
		if(CurMsg)
		{
			KeyMsg=CurMsg->Qmsg_Addr;
			if(KeyMsg->KeyValue==Key_Up)
			{
				Really_ShowInfor();
				goto R;
			}
			if(KeyMsg->KeyValue==Key_Right)if(Really_DevIndex<PDA_Dev_Size)Really_DevIndex++;
			if(KeyMsg->KeyValue==Key_Left)if(Really_DevIndex)Really_DevIndex--;
			if(KeyMsg->KeyValue!=NULL)
			{
				Really_DevSelect(Really_DevIndex,RRM_Base|RRM_Data);
				SleepCount=0;
				
			}else Really_DevSelect(Really_DevIndex,RRM_Base|RRM_Data);
		}
	}
}

