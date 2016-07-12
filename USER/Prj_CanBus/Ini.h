#define STM32F10X_LD
#define  USART_TX_BUFFER_SIZE 1000
#define  USART_RX_BUFFER_SIZE 300
#define  OSC_FREQ      ((uint32_t)6000000)
#define  Tos_TaskTotal            7      //任务池大小
#define  Tos_TaskStackDefSize     4000   //默认单个任务堆栈大小    
#define  Tos_RootTaskStackSize    1000
#define  Tos_DeviceTotal         10     //设备池大小
#define  Tos_TaskWatchDog        2000   //任务卡死复位时间
#define  Tos_Cpu_Freq						72000000	 //系统工作频率
#define  Board_Name            "BOARD_CORE1_0" 
#define  CPU_Model             "STM32F103ZET6" 
#define  FLASH_PARAM_PAGE	     510
#define  CAN_DeviceTotal       100
#define IWDG_ENABLE 