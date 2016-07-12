#include <Include.h>

//实用就好，不管非不非标
#if	  defined(BOARD_LCD4_3)//水位计4.3寸触摸屏
  #include <..\USER\Prj_4.3LCD\4.3LCD_Main.c>
#elif  defined(BOARD_CORE1_0)//核心板
  #include <..\USER\Prj_CanBus\CanBus_main.c>
#elif defined(SENSOR_TYPE_ANGLE)//角度传感器
	#include <..\USER\Prj_Sensor\Sensor_main.c>
#elif defined(SENSOR_TYPE_COMPASS)//电子罗盘
	#include <..\USER\Prj_Sensor\Sensor_main.c>
#elif defined(SENSOR_TYPE_0_20MV_TO_CAN)//0~20mv转CAN总线
	#include <..\USER\Prj_Sensor\Sensor_main.c>
#elif defined(SENSOR_TYPE_CXHEFT)//创星重量can传感器
	#include <..\USER\Prj_CXHeftSensor\CXHeftSensor_main.c>
#elif defined(SENSOR_TYPE_SCHEFT)//盛开特重量can传感器
	#include <..\USER\Prj_SCATHeftSensor\SCATHeft_main.c>
#elif defined(SENSOR_TYPE_CXRANGE)//创星幅度can总线传感器
	#include <..\USER\Prj_CXRangeSensor\CXRangeSe nsor_main.c>
#elif defined(BOARD_TANK_Hand)//内蒙古料管车遥控部分
	#include <..\USER\Prj_TankHand\TankHand_main.c>
#elif defined(BOARD_NJSIGDOT)//南京中升数据采集
	#include <..\USER\Prj_NJSigDot\NjSigDot_main.c>
#elif defined(BOARD_TS_Collect)//唐山罐车数据采集 
	#include <..\USER\Prj_TSCollect\TSCollect_main.c>
#elif defined(BOARD_TANK_Ctr)//内蒙古料管车控制系统主控制部分
	#include <..\USER\Prj_TankCtr\TankCtr_main.c>
#elif defined(BOARD_TANK_Ctr1)//内蒙古料管车控制系统罐控制部分
	#include <..\USER\Prj_TankCtr\TankCtr1_main.c>
#elif defined(BOARD_CENCTR)//主板（配核心板)
	#include <..\USER\Prj_CenCtr\CenCtr_main.c>
#elif defined(BOARD_REALLY)//瑞丽串口屏
	#include <..\USER\Prj_Really\Really_main.c>
#elif defined(BOARD_PRAFIRE)//吴显的野火开发板
  #include <..\USER\Prj_Prafire\Prafire_main.c>
	//#include <..\USER\Prj_Prafire\Geek-CC.c>
#elif defined(BOARD_PDATRAN)//PDA转接板
	#include <..\USER\Prj_PdaTran\Pdatran_main.c>
#elif defined(BOARD_TWOCAN)//双CAN板子
	#include <..\USER\Prj_TwoCan\TwoCan_main.c> 
#elif defined(BOARD_LCD7SJJ)//杭州升降机
	#include <..\USER\Prj_7LCDSJJ\7LCDSJJ_main.c> 
#elif defined(TOS_BOOTLOADER)//引导程序
	#include <..\USER\Prj_Boot\BootLoader_main.c>
#elif defined(BOARD_WJ4_3)//挖机4.3寸
	#include <..\USER\Prj_WJ4_3\WJ4_3_main.c>
#elif defined(BOARD_CAMERACTR)//余工摄像头控制
	#include <..\USER\Prj_CAMERACTR\CAMERACTR_main.c>
#elif defined(BOARD_CAMERAMOTR)//摄像头跟踪
	#include <..\USER\Prj_CAMERAMotr\CAMERAMotr_main.c>
#elif defined(BOARD_RS485ANGLE)//余工角度485
	#include <..\USER\Prj_RS485ANGLE\RS485ANGLE_main.c>
#elif defined(BOARD_BLACKBOX)//黑匣子
	#include <..\USER\Prj_BlackBox\BlackBox_main.c>
#elif defined(BOARD_CAMERAMRC)//摄像头跟踪遥控
	#include <..\USER\Prj_CameraMRC\CameraMRC_main.c>
#elif defined(BOARD_REMOTECONTROL)//无线插座遥控端
#include <..\USER\Prj_RemoteControl\RemoteControl_main.c>
#elif defined(BOARD_TELECONTROL)//无线插座控制端
#include <..\USER\Prj_TeleControl\TeleControl_main.c>
#elif defined(BOARD_TANK_CtrNew)//主板（配核心板)
	#include <..\USER\Prj_TankCtrNew\TankCtr_main.c>
#elif defined(BOARD_SKYBUS)//主板（配核心板)
#include <..\USER\Prj_TankCtrNew\SkyBus_main.c>
#elif defined(BOARD_TP500)//拓普瑞远程温湿度
#include <..\USER\Prj_TP500\TP500_main.c>
#elif defined(BOARD_TP500_New)//拓普瑞远程温湿度
#include <..\USER\Prj_TP500New\TP500_main.c>
#elif defined(BOARD_TP502T)
#include <..\USER\Prj_TP502T\TP500_main.c>
#elif defined(IAP_Addr0x5000)
#include <..\USER\Prj_IAP_Addr0x5000\IAP_main.c>
#elif defined(BOARD_GEEK_Bast)
#include <..\USER\Prj_Geek_Bast\Geek_main.c>
#else  
	#error "Missing define: Check Your Project Main.c!"
#endif

int tos_main()
{
	main();//跳转到man文件
	return Tos_TaskTotal;	//跳转到配置
}
