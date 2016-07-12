#ifndef _COMKEYBOARD_H_
#define _COMKEYBOARD_H_

#include <Include.h>

enum KeyTypeEnum
{
	Key_UpIng=1,//按键抬起
	Key_DownIng=2,//按键按下
	Key_UpTimeOut=3,//抬起一定时间触发
	Key_DownTimeOut=4,//按下一定时间触发
	Key_DownBate=5,//按下节拍
	Key_OneClick=6,
};
enum KeyBoard_Command_FrameFlag
{
	KCFF_IsKeyMsg=1,
};
enum KeyBoardType
{
	KeyBoard_Hand=1,
	KeyBoard_Touch=2,
	KeyBoard_RockerBar=3,
	KeyBoard_Null=4,
};
enum KeyValueEnum
{
	Key_Left=0x25,//左
  Key_Up=0x26,//上
  Key_Right=0x27,//右
  Key_Down=0x28,//下
	Key_Enter=0x0d,//确认
	Key_Esc=35,//退出
	Key_Ctrl=35,
	Key_A='A',
	Key_B='B',
	Key_C='C',
	Key_D='D',
	Key_E='E',
	Key_1='1',
	Key_2='2',
	Key_3='3',
	Key_4='4',
	Key_5='5',
	Key_6='6',
	Key_7='7',
	Key_8='8',
	Key_9='9',
	Key_0='0',
	Key_X='X',
	Key_Y='Y'
	
};
typedef struct
{
	uint8 KeyType;				//键盘事件类型
	uint8 KeyValue;				//键盘键值
	int8 KeyTime;				//键时间参数
}MGUI_KeyMsg;

void Task_GetKeyBoard(void *Tags);
void Tos_TaskGetKeyBoard(void);
void Task_GetKeyBoardZigbee(void *Tags);
#endif
