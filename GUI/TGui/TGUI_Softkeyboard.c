#include <Include.h>
#include <Tgui_Include.h>
#include <Driver_Include.h>
#include <Tools_Include.h>
#include <TGui_Softkeyboard.h>

uint8  TGUI_SoftKeyboardLoad;

TGUI_Window Window_KeyBoard;
TGUI_Button Button_KeyBoard[13];
const uint8 KeyBoard_Text[27]=
{								  
	'0',0,	'1',0,	'2',0,	'3',0,
	'4',0,	'5',0,	'6',0,	'7',0,
	'8',0,	'9',0,	'.',0,	'-',0,
	0xA1,0xFB,0
};

/*---------------------------------------------------------------------------------------------
 Func: ¥¶¿Ìœ‘ æ∆¡ƒªº¸≈Ãœ˚œ¢
 Time: 2011-6-13
 Ver.: V1.0
 Note:
---------------------------------------------------------------------------------------------*/
void TGUI_SoftKeyboard_ButtonClick(uint16 ID)
{
	uint8 KeyValue;
	TGUI_KeyMsg Kmsg;	
	Kmsg.KeyTime=0;
	switch(ID){
		case 0x0101:	KeyValue=TGUI_Key_Num0;	break;
		case 0x0102:	KeyValue=TGUI_Key_Num1;	break;
		case 0x0103:	KeyValue=TGUI_Key_Num2;	break;
		case 0x0104:	KeyValue=TGUI_Key_Num3;	break;
		case 0x0105:	KeyValue=TGUI_Key_Num4;	break;
		case 0x0106:	KeyValue=TGUI_Key_Num5;	break;
		case 0x0107:	KeyValue=TGUI_Key_Num6;	break;
		case 0x0108:	KeyValue=TGUI_Key_Num7;	break;
		case 0x0109:	KeyValue=TGUI_Key_Num8;	break;
		case 0x010A:	KeyValue=TGUI_Key_Num9;	break;
		case 0x010B:	KeyValue=TGUI_Key_Dot;	break;
		case 0x010C:	KeyValue=TGUI_Key_Sub;	break;
		case 0x010D:	KeyValue=TGUI_Key_Esc;	break;
	}
	Kmsg.KeyValue=KeyValue;
	Kmsg.KeyType=TGUI_Event_KeyClickDown;
	Tos_TaskPutQmsg(Tos_TaskCurrent,TGUI_MsgType_KeyBoard,sizeof(TGUI_KeyMsg),&Kmsg);
}

/*---------------------------------------------------------------------------------------------
 Func: ≥ı ºªØ∆¡ƒª»Ìº¸≈Ã
 Time: 2011-6-13
 Ver.: V1.0
 Note: 
---------------------------------------------------------------------------------------------*/
//void TGUI_SoftKeyboard_Create(void)
//{							
//	uint16 i,j;		   
//	TGUI_Window_Create(0x0100,0,LCD_YMAX-60,LCD_XMAX,60,"",TGUI_Style_NoTitle,&Window_KeyBoard);
//	j=0x0101;
//	for(i=0;i<12;i++){
//		TGUI_Button_Create(j,5+i*48,3,45,45,"",TGUI_Style_Default,&Button_KeyBoard[i]);
//		Button_KeyBoard[i].Text[0]=KeyBoard_Text[i*2];
//		Button_KeyBoard[i].Text[1]=0;
//		Button_KeyBoard[i].OnClick=TGUI_SoftKeyboard_ButtonClick;
//		TGUI_Window_AddControl(&Window_KeyBoard,&Button_KeyBoard[i]);
//		j++;
//	}
//	TGUI_Button_Create(j,5+i*48,3,45,45,"",TGUI_Style_Default,&Button_KeyBoard[i]);
//	TTS_StringCopy(Button_KeyBoard[i].Text,"<-");
//	//Button_KeyBoard[i].Text[0]=KeyBoard_Text[i*2];
//	//Button_KeyBoard[i].Text[1]=KeyBoard_Text[i*2+1];
//	//Button_KeyBoard[i].Text[2]=0;
//	Button_KeyBoard[i].OnClick=TGUI_SoftKeyboard_ButtonClick;
//	TGUI_Window_AddControl(&Window_KeyBoard,&Button_KeyBoard[i]);
//}


void TGUI_SoftKeyboard_Create(void)
{							
	uint16 i,j;		   
//	TGUI_Window_Create(0x0100,0,LCD_YMAX-60,LCD_XMAX,60,"",TGUI_Style_NoTitle|,&Window_KeyBoard);

	TGUI_Window_Create(0x0100,0,LCD_YMAX-40,LCD_XMAX,40,"",TGUI_Style_NoTitle|TGUI_Style_NoBcolor|TGUI_Style_NoFrame,&Window_KeyBoard);

	j=0x0101;
	for(i=0;i<12;i++){
		TGUI_Button_Create(j,8+i*36,4,31,31,"",TGUI_Style_Default,&Button_KeyBoard[i]);
		Button_KeyBoard[i].Text[0]=KeyBoard_Text[i*2];
		Button_KeyBoard[i].Text[1]=0;
		Button_KeyBoard[i].OnClick=TGUI_SoftKeyboard_ButtonClick;
		TGUI_Window_AddControl(&Window_KeyBoard,&Button_KeyBoard[i]);
		j++;
	}
	TGUI_Button_Create(j,8+i*36,4,31,31,"",TGUI_Style_Default,&Button_KeyBoard[i]);
	StringCoppy("<-",Button_KeyBoard[i].Text);
	//Button_KeyBoard[i].Text[0]=KeyBoard_Text[i*2];
	//Button_KeyBoard[i].Text[1]=KeyBoard_Text[i*2+1];
	//Button_KeyBoard[i].Text[2]=0;
	Button_KeyBoard[i].OnClick=TGUI_SoftKeyboard_ButtonClick;
	TGUI_Window_AddControl(&Window_KeyBoard,&Button_KeyBoard[i]);
}


/*---------------------------------------------------------------------------------------------
 Func: œ‘ æ∆¡ƒª»Ìº¸≈Ã
 Time: 2011-6-13
 Ver.: V1.0
 Note:
---------------------------------------------------------------------------------------------*/
void TGUI_SoftKeyboard_Show(void)
{
	if(TGUI_SoftKeyboardLoad==0){
		TGUI_Window_ReDraw(&Window_KeyBoard,0);
		TGUI_SoftKeyboardLoad=1;
	}
}

/*---------------------------------------------------------------------------------------------
 Func: “˛≤ÿ∆¡ƒª»Ìº¸≈Ã
 Time: 2011-6-13
 Ver.: V1.0
 Note:
---------------------------------------------------------------------------------------------*/
void TGUI_SoftKeyboard_Hidden(void)
{
	 if(TGUI_SoftKeyboardLoad){
	 	TGUI_SoftKeyboardLoad=0;
	 }
}











