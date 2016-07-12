#ifndef _CONFIGS_H_
#define _CONFIGS_H_
/**********************************嵌入式实时操作系统配置文件*******************

                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                  													|
|                                                 	        |
|              Real Time Operating System                   | 
|                      2014.11.8                            |
| 			  		   http://www.trtos.com                       |
|                				Config.h				                  	|
|                  				    		  	                      |             
|				  													                		 		|
|                              Oooo                         |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)     

*********************************************************************************/
#if	  defined(BOARD_LCD4_3)
#include <..\USER\Prj_4.3LCD\Ini.h>
#elif defined(BOARD_CORE1_0)
#include <..\USER\Prj_CanBus\Ini.h>
#elif defined(BOARD_TANK_Hand)
#include <..\USER\Prj_TankHand\Ini.h>
#elif defined(BOARD_TANK_Ctr1)
#include <..\USER\Prj_TankCtrNew\Ini.h>
#elif defined(BOARD_TANK_Ctr)
#include <..\USER\Prj_TankCtr\Ini.h>
#elif defined(BOARD_NJSIGDOT)
#include <..\USER\Prj_TankCtrNew\Ini.h>
#elif defined(BOARD_SENSOR) 
#include <..\USER\Prj_Sensor\Ini.h>
#elif defined(BOARD_CENCTR)
#include <..\USER\Prj_CenCtr\Ini.h>
#elif defined(BOARD_REALLY)
#include <..\USER\Prj_Really\Ini.h>
#elif defined(BOARD_PRAFIRE)
#include <..\USER\Prj_Prafire\Ini.h>
#elif defined(BOARD_PDATRAN)
#include <..\USER\Prj_PdaTran\Ini.h>
#elif defined(BOARD_TS_Collect)
#include <..\USER\Prj_TSCollect\Ini.h>
#elif defined(BOARD_TWOCAN)
#include <..\USER\Prj_TwoCan\Ini.h>
#elif defined(BOARD_LCD7SJJ)
#include <..\USER\Prj_7LCDSJJ\Ini.h>
#elif defined(TOS_BOOTLOADER)
#include <..\USER\Prj_Boot\Ini.h>
#elif defined(BOARD_WJ4_3)
#include <..\USER\Prj_WJ4_3\Ini.h>
#elif defined(BOARD_CAMERACTR)
#include <..\USER\Prj_CAMERACTR\Ini.h>
#elif defined(BOARD_CAMERAMOTR)
#include <..\USER\Prj_CAMERAMOTR\Ini.h>
#elif defined(BOARD_RS485ANGLE)
#include <..\USER\Prj_RS485ANGLE\Ini.h>
#elif defined(BOARD_BLACKBOX)
#include <..\USER\Prj_BlackBox\Ini.h>
#elif defined(BOARD_CAMERAMRC)
#include <..\USER\Prj_CameraMRC\Ini.h>
#elif defined(BOARD_REMOTECONTROL)
#include <..\USER\Prj_RemoteControl\Ini.h>
#elif defined(BOARD_TELECONTROL)
#include <..\USER\Prj_TeleControl\Ini.h>
#elif defined(BOARD_TANK_CtrNew)
#include <..\USER\Prj_TankCtrNew\Ini.h>
#elif defined(BOARD_SKYBUS)
#include <..\USER\Prj_TankCtrNew\Ini.h>
#elif defined(BOARD_TP500_New)
#include <..\USER\Prj_TP500New\Ini.h>
#elif defined(BOARD_TP502T)
#include <..\USER\Prj_TP502T\Ini.h>
#elif defined(IAP_Addr0x5000)
#include <..\USER\Prj_IAP_Addr0x5000\Ini.h>
#elif defined(BOARD_GEEK_Bast)
#include <..\USER\Prj_Geek_Bast\Ini.h>
#elif defined(BOARD_TP500L)
#include <..\USERL\PrjL_TP500\Ini.h>

#else  
	#error "您首先必须配置好您的项目配置文件Ini.h放置于User目录下"
#endif

#endif
