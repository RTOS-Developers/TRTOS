//#define STM32F10X_MD
#define  USART_TX_BUFFER_SIZE 300
#define  USART_RX_BUFFER_SIZE 1200
#define  OSC_FREQ      ((uint32_t)8000000)
#define  MEMA_BUFFER_SIZE 20000
#define  Tos_TaskTotal            20      //����ش�С
#define  Tos_RootTaskStackSize    2000
#define  Tos_DeviceTotal         10     //�豸�ش�С
#define  Tos_TaskWatchDog        20000   //��������λʱ��
#define  Tos_Cpu_Freq						72000000	 //ϵͳ����Ƶ��
#define  Board_Name             "TP500" 
#define  CPU_Model              "STM32L151RBT6" 
#define  FLASH_PARAM_PAGE	      254
#define  USART_CH_SIZE 2
#define  COMPILE_MODE_DEBUG
//#define IWDG_ENABLE 
