//#define STM32F10X_HD
//#define  USART_TX_BUFFER_SIZE 1000
//#define  USART_RX_BUFFER_SIZE 300
//#define  OSC_FREQ      ((uint32_t)8000000)
//#define  Tos_TaskTotal            7      //任务池大小
//#define  Tos_TaskStackDefSize     4000   //默认单个任务堆栈大小    
//#define  Tos_RootTaskStackSize    1000
//#define  Tos_DeviceTotal         10     //设备池大小
//#define  Tos_TaskWatchDog        2000   //任务卡死复位时间
//#define  Tos_Cpu_Freq						72000000	 //系统工作频率
//#define  Board_Name            "BOARD_TANK_Hand" 
//#define  CPU_Model             "STM32F103VET6" 
//#define  FLASH_PARAM_PAGE	     510
//#define  CAN_DeviceTotal       100
////#define IWDG_ENABLE 
//#define APPLIC_ENABLE 
//#define LCD_SelectSSD1325
//#define GUI_ENABLE
////#define COMPILE_MODE_DEBUG
//#define USART1_BaudRate 9600


#define STM32F10X_HD
#define  USART_TX_BUFFER_SIZE 300
#define  USART_RX_BUFFER_SIZE 300
#define  OSC_FREQ      ((uint32_t)8000000)
#define TOS_MEMA_EN  //使能内存动态分配
#define  Tos_TaskTotal            10      //任务池大小
#ifdef TOS_MEMA_EN
#define  Tos_TaskStackDefSize     1   //默认单个任务堆栈大小
#else 
#define  Tos_TaskStackDefSize     2000   //默认单个任务堆栈大小
#endif
#define  Tos_RootTaskStackSize    1000
#define  Tos_DeviceTotal         10     //设备池大小
#define  Tos_TaskWatchDog        2000   //任务卡死复位时间
#define  Tos_Cpu_Freq						72000000	 //系统工作频率
#define  Board_Name             "测试" 
#define  CPU_Model              "STM32F103VET6" 
#define  FLASH_PARAM_PAGE	      510
#define  USART_CH_SIZE 5
#define  CAN_DeviceTotal       10
#define IWDG_ENABLE 
#define LCD_SelectSSD1325
#define GUI_ENABLE
#define USART1_BaudRate 9600
