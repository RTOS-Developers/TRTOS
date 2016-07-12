#define STM32F10X_CL
#define  USART_TX_BUFFER_SIZE 100
#define  USART_RX_BUFFER_SIZE 100
#define  OSC_FREQ      ((uint32_t)8000000)
#define  Tos_TaskTotal            7      //任务池大小
#define  Tos_TaskStackDefSize     4000   //默认单个任务堆栈大小
#define  Tos_RootTaskStackSize    1000
#define  Tos_DeviceTotal         5     //设备池大小
#define  Tos_TaskWatchDog        8000   //任务卡死复位时间
#define  Tos_Cpu_Freq						72000000	 //系统工作频率
#define  Board_Name            "两路CAN总线" 
#define  CPU_Model             "STM32F103VET6" 
#define  FLASH_PARAM_PAGE	     254
#define IWDG_ENABLE 
