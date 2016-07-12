#define STM32F10X_LD
#define  USART_TX_BUFFER_SIZE 50
#define  USART_RX_BUFFER_SIZE 50
#define  OSC_FREQ      ((uint32_t)8000000)
#define  Tos_TaskTotal            5      //任务池大小
#define  Tos_TaskStackDefSize     2000   //默认单个任务堆栈大小 
#define  Tos_RootTaskStackSize    400
#define  Tos_DeviceTotal         10     //设备池大小
#define  Tos_TaskWatchDog        2000   //任务卡死复位时间
#define  Tos_Cpu_Freq						72000000	 //系统工作频率
#define  Board_Name             "BOARD_SENSOR"
#define  CPU_Model              "STM32F103C8T6" 
#define  FLASH_PARAM_PAGE	     62
#define IWDG_ENABLE 
#define COMPILE_MODE_DEBUG
//#define CANWDG_ENABLE

//#define  SENSOR_TYPE_ANGLE     //定义为传感器 角度传感器
//#define  SENSOR_TYPE_COMPASS   //定义为传感器  罗盘传感器
//#define  SENSOR_TYPE_0_20MV_TO_CAN     //定义为传感器 0-20mv转can
#define  SENSOR_TYPE_SCHEFT     //定义为传感器 0-20mv转can
//#define  SENSOR_TYPE_CXHEFT    //创星余工的称重板子
//#define SENSOR_TYPE_CXRANGE    //创星幅度传感器编码器


