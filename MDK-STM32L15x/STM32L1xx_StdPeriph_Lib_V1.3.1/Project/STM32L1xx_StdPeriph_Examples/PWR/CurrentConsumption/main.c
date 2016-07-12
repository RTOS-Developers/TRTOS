/**
  ******************************************************************************
  * @file    PWR/CurrentConsumption/main.c 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   Main program body
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
#include "stm32l1xx.h"
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
__IO uint32_t Counter = 0, BOROptionBytes = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32l1xx_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32l1xx.c file
     */  

  /* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset RTC Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);
#ifdef BOR_MODIFY
  /* Get BOR Option Bytes */
  BOROptionBytes = FLASH_OB_GetBOR();

  if((BOROptionBytes & 0x0F) != BOR_LEVEL) 
  {
    /* Unlocks the option bytes block access */
    FLASH_OB_Unlock();

    /* Clears the FLASH pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);

    /* Select the desired V(BOR) Level ---------------------------------------*/
    FLASH_OB_BORConfig(BOR_LEVEL); 

    /* Launch the option byte loading */
    FLASH_OB_Launch();  
  }
#endif  

#if defined (LP_RUN_SRAM_MODE)
  /* Low Power Run Mode Entry:
      - System Running at MSI (~32KHz)
      - Flash 0 wait state
      - Voltage Range 2
      - Code running from Internal SRAM
      - All peripherals OFF
      - FLASH switched OFF
      - VDD from 1.65V to 3.6V
      - Current Consumption ~10.5uA
      - Wakeup using Key Button PA.00
   */
  LowPowerRunMode_Measure();
#elif defined (LP_RUN_FLASH_MODE)
  /* Low Power Run Mode Entry:
      - System Running at MSI (~32KHz)
      - Flash 0 wait state
      - Voltage Range 2
      - Code running from Internal FLASH
      - All peripherals OFF
      - VDD from 1.65V to 3.6V
      - Current Consumption ~25uA
      - Wakeup using Key Button PA.00
   */
  LowPowerRunMode_Measure();  
#elif defined (SLEEP_MODE)
  /* Sleep Mode Entry 
      - System Running at HSI (16MHz)
      - Flash 1 wait state
      - Voltage Range 2
      - Code running from Internal FLASH
      - Current Consumption ~1mA
      - Wakeup using EXTI Line (Key Button PA.00)
   */
  SleepMode_Measure();
#elif defined (LP_SLEEP_MODE)
  /* Low Power Sleep Mode Entry 
      - System Running at MSI (~32KHz)
      - Flash 0 wait state
      - Voltage Range 2
      - Code running from Internal FLASH
      - All peripherals OFF
      - VDD from 1.65V to 3.6V
      - Current Consumption ~4.07uA
      - Wakeup using EXTI Line (Key Button PA.00)
   */
  LowPowerSleepMode_Measure();
#elif defined (STOP_MODE)
  /* STOP Mode Entry 
      - Regulator in LP mode
      - LSI, HSI and HSE OFF
      - No IWDG
      - Current Consumption ~0.5uA
      - Wakeup using EXTI Line (Key Button PA.00)
   */
  StopMode_Measure();
#elif defined (STOP_RTC_LSE_MODE)
  /* STOP Mode with RTC on LSE Entry 
      - RTC Clocked by LSE external Clock (32.768KHz)
      - Regulator in LP mode
      - LSI, HSI and HSE OFF
      - No IWDG
      - Current Consumption ~1.6uA
      - Automatic Wakeup using RTC on LSE (4s)
   */
  StopRTCLSEMode_Measure();
#elif defined (STOP_RTC_LSI_MODE)
  /* STOP Mode with RTC on LSI Entry 
      - RTC Clocked by LSI
      - Regulator in LP mode
      - HSI and HSE OFF
      - No IWDG   
      - Current Consumption ~1.3uA
      - Automatic Wakeup using RTC on LSI (after ~4s)
   */
  StopRTCLSIMode_Measure();
#elif defined (STANDBY_MODE)
  /* STANDBY Mode Entry 
      - IWDG and LSI OFF
      - Current Consumption ~0.3uA
      - Wakeup using WakeUp Pin 1 (PA.00)
   */
  StandbyMode_Measure();
#elif defined (STANDBY_RTC_LSE_MODE)
  /* STANDBY Mode with RTC on LSE Entry 
      - RTC Clocked by LSE external Clock (32.768KHz)
      - IWDG and LSI OFF
      - Current Consumption ~1.3uA
      - Automatic Wakeup using RTC on LSE (after 4s)
   */
  StandbyRTCLSEMode_Measure();
#elif defined (STANDBY_RTC_LSI_MODE)
  /* STANDBY Mode with RTC on LSI Entry 
      - RTC Clocked by LSI
      - IWDG OFF
      - Current Consumption ~1.1uA
      - Automatic Wakeup using RTC on LSI (after ~4s)
  */
  StandbyRTCLSIMode_Measure();
#else
  /* Initialize LED1 on STM32L152-EVAL board */
  STM_EVAL_LEDInit(LED1);
  
  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED1);

    /* Inserted Delay */
    for(Counter = 0; Counter < 0x5FF; Counter++);
  }
#endif
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
