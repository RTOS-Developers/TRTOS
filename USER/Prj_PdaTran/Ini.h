#define STM32F10X_LD
#define  USART_TX_BUFFER_SIZE 100
#define  USART_RX_BUFFER_SIZE 100
#define  OSC_FREQ      ((uint32_t)6000000)
#define  Tos_TaskTotal            5     //任务池大小
#define  Tos_TaskStackDefSize     1200   //默认单个任务堆栈大小    
#define  Tos_RootTaskStackSize    1000
#define  Tos_DeviceTotal         5     //设备池大小
#define  Tos_TaskWatchDog        8000   //任务卡死复位时间
#define  Tos_Cpu_Freq						72000000	 //系统工作频率
#define  Board_Name            "BOARD_REALLY" 
#define  CPU_Model             "STM32F103VET6" 
#define  FLASH_PARAM_PAGE	     62
#define  USART1_BaudRate       9600
#define  USART2_BaudRate       9600
#define  PDA_Buf_Size          100
#define IWDG_ENABLE 

