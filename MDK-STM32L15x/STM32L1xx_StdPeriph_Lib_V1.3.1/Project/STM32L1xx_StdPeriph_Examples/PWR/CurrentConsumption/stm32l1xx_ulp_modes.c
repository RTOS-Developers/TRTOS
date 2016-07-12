/**
  ******************************************************************************
  * @file    PWR/CurrentConsumption/stm32l1xx_ulp_modes.c 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the STM32L1xx Ultra Low Power Modes:           
  *           - Low Power Run Mode from Internal SRAM
  *           - Low Power Run Mode from Internal FLASH
  *           - Sleep Mode
  *           - Low Power Sleep Mode
  *           - STOP mode with or without RTC
  *           - STANDBY mode with or without RTC
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_ulp_modes.h"
#ifdef USE_STM32L152D_EVAL
#include "stm32l152d_eval.h"
#else
#include "stm32l152_eval.h"
#endif

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup CurrentConsumption
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t index = 0;
GPIO_InitTypeDef GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the system to enter Low Power Run mode for
  *         current consumption measurement purpose.
  *         The maximum clock when the system is in Low Power Run mode is ~128KHz.
  *         This mode can only be entered when Voltage Range 2 is selected. 
  *         Low Power Run Mode from SRAM:
  *         =============================
  *           - System Running at MSI (~32KHz)
  *           - Flash 0 wait state  
  *           - Voltage Range 2
  *           - Code running from Internal SRAM
  *           - All peripherals OFF
  *           - FLASH switched OFF
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~10.5uA
  *           - Wakeup using Key Button PA.00  
  *         Low Power Run Mode from FLASH:
  *         ==============================
  *           - System Running at MSI (~32KHz)
  *           - Flash 0 wait state  
  *           - Voltage Range 2
  *           - Code running from Internal FLASH
  *           - All peripherals OFF
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~25uA 
  *           - Wakeup using Key Button PA.00   
  * @param  None
  * @retval None
  */
void LowPowerRunMode_Measure(void)
{
  /* Configure the System Clock to MSI Range 0 (65KHz). ----------------------*/

  /* RCC system reset */
  RCC_DeInit();
  
  /* Flash 0 wait state */
  FLASH_SetLatency(FLASH_Latency_0);
    
  /* Disable Prefetch Buffer */
  FLASH_PrefetchBufferCmd(DISABLE);

  /* Disable 64-bit access */
  FLASH_ReadAccess64Cmd(DISABLE);

  /* Enable the PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Select the Voltage Range 2 (1.5V) */
  PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);
  
  /* Wait Until the Voltage Regulator is ready */
  while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET)
  {
  }
        
  /* HCLK = SYSCLK/2 = ~32KHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div2);

  /* PCLK2 = HCLK */
  RCC_PCLK2Config(RCC_HCLK_Div1);

  /* PCLK1 = HCLK */
  RCC_PCLK1Config(RCC_HCLK_Div1);

  /* Set MSI clock range to 65.536KHz */
  RCC_MSIRangeConfig(RCC_MSIRange_0);
  
  /* Select MSI as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);

  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x00)
  {} 
  
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);  
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, DISABLE);

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
  /* Enter RUN LP Mode */
  PWR_EnterLowPowerRunMode(ENABLE);

  /* Wait until the system enters RUN LP and the Regulator is in LP mode */
  while(PWR_GetFlagStatus(PWR_FLAG_REGLP) == RESET)
  {
  }

/* Jump to Internal SRAM and Switch the internal FLASH OFF */
#if defined (LP_RUN_SRAM_MODE)
  LowPowerRunModeSRAM_Measure();
  
#elif defined (LP_RUN_FLASH_MODE)
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
#endif
 

  /* Exit the RUN LP Mode */	
  PWR_EnterLowPowerRunMode(DISABLE);
  
  /* Wait until the system exits RUN LP and the Regulator is in main mode */
  while(PWR_GetFlagStatus(PWR_FLAG_REGLP) != RESET)
  {
  }   

  
  /* Infinite loop */  
  while (1)
  {    
  }
}

/**
  * @brief  This function configures the system to enter Sleep mode for
  *         current consumption measurement purpose.
  *         Sleep Mode
  *         ==========  
  *            - System Running at HSI (16MHz)
  *            - Flash 1 wait state  
  *            - Voltage Range 2
  *            - Code running from Internal FLASH
  *            - Current Consumption ~1mA 
  *            - Wakeup using EXTI Line (Key Button PA.00)   
  * @param  None
  * @retval None
  */
void SleepMode_Measure(void)
{
  /* Configure System Clock to HSI (16MHz) */
  __IO uint32_t StartUpCounter = 0, HSIStatus = 0;
    
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSI */
  RCC_HSICmd(ENABLE);

  /* Wait till HSI is ready and if Time out is reached exit */
  do
  {
    HSIStatus = RCC_GetFlagStatus(RCC_FLAG_HSIRDY);
    StartUpCounter++;  
  } while((HSIStatus == 0) && (StartUpCounter != HSI_STARTUP_TIMEOUT));


  if (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
  {
    HSIStatus = (uint32_t)0x01;
  }
  else
  {
    HSIStatus = (uint32_t)0x00;
  } 

  if (HSIStatus == 0x01)
  {
    /* Enable 64-bit access */
    FLASH_ReadAccess64Cmd(ENABLE);
   
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(ENABLE);
  
    /* Flash 1 wait state */
    FLASH_SetLatency(FLASH_Latency_1);    

    /* Enable the PWR APB1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
    /* Select the Voltage Range 2 (1.5V) */
    PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);
  
    /* Wait Until the Voltage Regulator is ready */
    while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET)
    {
    } 

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div1);

    /* Select HSI as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

    /* Wait till HSI is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x04)
    {}
  }
  else
  { 
    /* If HSI fails to start-up, the application will have wrong clock configuration.
    User can add here some code to deal with this error */

    /* Go to infinite loop */
    while (1)
    {}
  }

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);  
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, DISABLE);

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_EXTI);

  /* Request to enter SLEEP mode with regulator ON */
  PWR_EnterSleepMode(PWR_Regulator_ON, PWR_SLEEPEntry_WFI);

  /* Initialize LED1 on STM32L152-EVAL board */
  STM_EVAL_LEDInit(LED1);

  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED1);

    /* Inserted Delay */
    for(index = 0; index < 0x5FFFF; index++);
  }
}

/**
  * @brief  This function configures the system to enter Low Power Sleep mode for
  *         current consumption measurement purpose.
  *         The maximum clock when the system is in Low Power Run mode is ~128KHz.
  *         This mode can only be entered when Voltage Range 2 is selected. 
  *         Low Power Sleep Mode
  *         ====================   
  *           - System Running at MSI (~32KHz)
  *           - Flash 0 wait state  
  *           - Voltage Range 2
  *           - Code running from Internal FLASH
  *           - All peripherals OFF
  *           - VDD from 1.65V to 3.6V
  *           - Current Consumption ~4.07uA
  *           - Wakeup using EXTI Line (Key Button PA.00)      
  * @param  None
  * @retval None
  */
void LowPowerSleepMode_Measure(void)
{
  /* Configure the System Clock to MSI Range 0 (65KHz). ----------------------*/

  /* RCC system reset */
  RCC_DeInit();
  
  /* Flash 0 wait state */
  FLASH_SetLatency(FLASH_Latency_0);
    
  /* Disable Prefetch Buffer */
  FLASH_PrefetchBufferCmd(DISABLE);    

  /* Disable 64-bit access */
  FLASH_ReadAccess64Cmd(DISABLE);

  /* Enable the PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Select the Voltage Range 2 (1.5V) */
  PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);
  
  /* Wait Until the Voltage Regulator is ready */
  while(PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET)
  {
  }
        
  /* HCLK = SYSCLK/2 = ~32KHz */
  RCC_HCLKConfig(RCC_SYSCLK_Div2);

  /* PCLK2 = HCLK */
  RCC_PCLK2Config(RCC_HCLK_Div1);

  /* PCLK1 = HCLK */
  RCC_PCLK1Config(RCC_HCLK_Div1);

  /* Set MSI clock range to 65.536KHz */
  RCC_MSIRangeConfig(RCC_MSIRange_0);
  
  /* Select MSI as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);

  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x00)
  {} 
  
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */  
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);  
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, DISABLE);

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_EXTI);
  
  /* Enable The ultra Low Power Mode */
  PWR_UltraLowPowerCmd(ENABLE);
  
  /* Enable the power down mode during Sleep mode */
  FLASH_SLEEPPowerDownCmd(ENABLE);
  
  /* Request to enter SLEEP mode with regulator in low power mode */
  PWR_EnterSleepMode(PWR_Regulator_LowPower, PWR_SLEEPEntry_WFI);

  /* Initialize LED1 on STM32L152-EVAL board */
  STM_EVAL_LEDInit(LED1);
  

  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED1);

    /* Inserted Delay */
    for(index = 0; index < 0x5FF; index++);
  }
}

/**
  * @brief  This function configures the system to enter Stop mode for
  *         current consumption measurement purpose.
  *         STOP Mode
  *         =========
  *           - Regulator in LP mode
  *           - LSI, HSI and HSE OFF
  *           - No IWDG
  *           - Current Consumption ~0.5uA
  *           - Wakeup using EXTI Line (Key Button PA.00)  
  * @param  None
  * @retval None
  */
void StopMode_Measure(void)
{
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);  
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, DISABLE);
  
  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
    
  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);

  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

  /* Initialize LED1 on STM32L152-EVAL board */
  STM_EVAL_LEDInit(LED1);

  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED1);

    /* Inserted Delay */
    for(index = 0; index < 0x5FF; index++);
  }
}

/**
  * @brief  This function configures the system to enter Stop mode with RTC 
  *         clocked by LSE  for current consumption measurement purpose.
  *         STOP Mode with RTC clocked by LSE
  *         =================================   
  *           - RTC Clocked by LSE external Clock (32.768KHz)
  *           - Regulator in LP mode
  *           - LSI, HSI and HSE OFF
  *           - No IWDG
  *           - Current Consumption ~1.6uA
  *           - Automatic Wakeup using RTC clocked by LSE (4s)  
  * @param  None
  * @retval None
  */
void StopRTCLSEMode_Measure(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  
  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();  

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
  GPIO_Init(GPIOF, &GPIO_InitStructure);  
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, DISABLE);
  
  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
 
  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: 4s      
     RTC Clock Source LSE 32.768KHz  
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(0x1FFF);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
    
  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);
    
  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED1);

    /* Inserted Delay */
    for(index = 0; index < 0x5FF; index++);
  }
}

/**
  * @brief  This function configures the system to enter Stop mode with RTC 
  *         clocked by LSI for current consumption measurement purpose.
  *         STOP Mode with RTC clocked by LSI
  *         =================================   
  *           - RTC Clocked by LSI
  *           - Regulator in LP mode
  *           - HSI and HSE OFF
  *           - No IWDG
  *           - Current Consumption ~1.3uA
  *           - Automatic Wakeup using RTC clocked by LSI (after ~4s)  
  * @param  None
  * @retval None
  */
void StopRTCLSIMode_Measure(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  EXTI_InitTypeDef  EXTI_InitStructure;
  
  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /*!< LSI Enable */
  RCC_LSICmd(ENABLE);

  /*!< Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /*!< RTC Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
  GPIO_Init(GPIOF, &GPIO_InitStructure);  
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, DISABLE);

  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
    
  /* EXTI configuration *******************************************************/
  EXTI_ClearITPendingBit(EXTI_Line20);
  EXTI_InitStructure.EXTI_Line = EXTI_Line20;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: ~4s 
     RTC Clock Source LSI ~37KHz  
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(0x2421);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
      
  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);
    
  /* Enter Stop Mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED1);

    /* Inserted Delay */
    for(index = 0; index < 0x5FF; index++);
  }
}

/**
  * @brief  This function configures the system to enter Standby mode for
  *         current consumption measurement purpose.
  *         STANDBY Mode
  *         ============
  *           - IWDG and LSI OFF
  *           - Current Consumption ~0.3uA
  *           - Wakeup using WakeUp Pin 1 (PA.00)    
  * @param  None
  * @retval None
  */
void StandbyMode_Measure(void)
{
  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }

  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);

  /* Clear PWR WakeUp flag */
  PWR_ClearFlag(PWR_FLAG_WU);

  /* Enable WKUP pin 1 */
  PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);

  /* Request to enter STANDBY mode */
  PWR_EnterSTANDBYMode();
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures the system to enter Standby mode with RTC 
  *         clocked by LSE for current consumption measurement purpose.
  *         STANDBY Mode with RTC clocked by LSE
  *         ====================================
  *           - RTC Clocked by LSE external Clock (32.768KHz)
  *           - IWDG and LSI OFF
  *           - Current Consumption ~1.3uA 
  *           - Automatic Wakeup using RTC clocked by LSE (after 4s)   
  * @param  None
  * @retval None
  */
void StandbyRTCLSEMode_Measure(void)
{
  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }

  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();  

  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: 4s      
     RTC Clock Source LSE 32.768KHz  
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(0x1FFF);

  /* Clear PWR WakeUp flag */
  PWR_ClearFlag(PWR_FLAG_WU);
  
  /* Clear RTC WakeUp (WUTF) flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);
  
  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE); 

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE);
    
  /* Request to enter STANDBY mode */
  PWR_EnterSTANDBYMode();
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function configures the system to enter Standby mode with RTC 
  *         clocked by LSI for current consumption measurement purpose.
  *         STANDBY Mode with RTC clocked by LSI
  *         ====================================   
  *           - RTC Clocked by LSI
  *           - IWDG OFF
  *           - Current Consumption ~1.1uA
  *           - Automatic Wakeup using RTC clocked by LSI (after ~4s)    
  * @param  None
  * @retval None
  */
void StandbyRTCLSIMode_Measure(void)
{
  /*  Configure Key Button*/
  STM_EVAL_PBInit(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) == RESET)
  {
  }
  /* Wait Until Key button pressed */
  while(STM_EVAL_PBGetState(BUTTON_KEY) != RESET)
  {
  }
    
  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /*!< LSI Enable */
  RCC_LSICmd(ENABLE);

  /*!< Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /*!< RTC Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();  

  /* Enable Ultra low power mode */
  PWR_UltraLowPowerCmd(ENABLE);

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16, Wakeup Time Base: ~4s 
     RTC Clock Source LSI ~37KHz  
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
  RTC_SetWakeUpCounter(0x2421);

  /* Clear PWR WakeUp flag */
  PWR_ClearFlag(PWR_FLAG_WU);
  
  /* Clear RTC WakeUp (WUTF) flag */
  RTC_ClearFlag(RTC_FLAG_WUTF);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Enable Wakeup Counter */
  RTC_WakeUpCmd(ENABLE); 

  /* Request to enter STANDBY mode */
  PWR_EnterSTANDBYMode();
  
  /* Infinite loop */
  while (1)
  {
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
