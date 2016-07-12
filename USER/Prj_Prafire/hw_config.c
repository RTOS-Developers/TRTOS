/******************** (C) COPYRIGHT 2011 迷你嵌入式开发工作室 ********************
 * 文件名  ：hw_configs.c
 * 描述    ：USB配置函数库
 *          
 * 实验平台：野火STM32开发板
 * 硬件连接：-------------------------------
 *          | PE3-USB-MODE	 (PE3为普通I/O) |
 *          | PA11-USBDM(D-)								|
 *          | PA12-USBDP(D+)								|
 *           -------------------------------
 *
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/
#include "stm32f10x.h"
#include <..\USB\UPan\usb_library\inc\hw_config.h>
#include <..\USB\UPan\usb_library\inc\usb_lib.h>

/*
 * 函数名：Set_USBClock
 * 描述  ：配置USB时钟(48M)
 * 输入  ：无
 * 输出  ：无
 */
void Set_USBClock(void)
{
   /* USBCLK = PLLCLK */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*
 * 函数名：Enter_LowPowerMode
 * 描述  ：Power-off system clocks and power while entering suspend mode(暂时为空)
 * 输入  ：无
 * 输出  ：无
 */
void Enter_LowPowerMode(void)
{
}

/*
 * 函数名：Leave_LowPowerMode
 * 描述  ：Restores system clocks and power while exiting suspend mode(暂时为空)
 * 输入  ：无
 * 输出  ：无
 */
void Leave_LowPowerMode(void)
{
}

/*
 * 函数名：USB_Interrupts_Config
 * 描述  ：配置USB中断
 * 输入  ：无
 * 输出  ：无
 */
void USB_Interrupts_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：USB_Cable_Config
 * 描述  ：Software Connection/Disconnection of USB Cable
 * 输入  ：-NewState: new state
 * 输出  ：无
 */
void USB_Cable_Config (FunctionalState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  /* PE3 输出 0 时 D+ 接上拉电阻工作于全速模式 */ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	   /* 开漏输出 */
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	 
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);		             /* USB全速模式 */
  }
  else
  {
    GPIO_SetBits(GPIOE, GPIO_Pin_3);			             /* 普通模式 */
  }
}


/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/
