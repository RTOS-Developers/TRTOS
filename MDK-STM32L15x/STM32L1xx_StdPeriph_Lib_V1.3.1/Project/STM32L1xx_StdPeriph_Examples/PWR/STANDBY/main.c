/**
  ******************************************************************************
  * @file    PWR/STANDBY/main.c 
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

#ifdef USE_STM32L152D_EVAL
#include "stm32l152d_eval.h"
#include "stm32l152d_eval_glass_lcd.h"
#else
#include "stm32l152_eval.h"
#include "stm32l152_eval_glass_lcd.h"
#endif

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup PWR_STANDBY
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_InitTypeDef   RTC_InitStructure;
RTC_TimeTypeDef   RTC_TimeStructure;

/* Private function prototypes -----------------------------------------------*/
void RCC_Config(void);
void LCD_ShowTimeCalendar(void);
void SysTickConfig(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
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

  /* Initialize LEDs and Key Button mounted on STM32L152X-EVAL board */       
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);
  
  /* System clocks configuration */
  RCC_Config();

  /* LCD GLASS Initialization */
  LCD_GLASS_Init();
  
  /* Clear the LCD GLASS */
  LCD_GLASS_Clear();
 
  /* Enable WKUP pin 1 */
  PWR_WakeUpPinCmd(PWR_WakeUpPin_1, ENABLE);  

  /* Configure the SysTick to generate an interrupt each 250 ms */
  SysTickConfig();
  
  while (1)
  {
  }
}

/**
  * @brief  Configures the different system and peripherals clocks.
  * @param  None
  * @retval None
  */
void RCC_Config(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC Domain */
  PWR_RTCAccessCmd(ENABLE);

  /* Check if the StandBy flag is set */
  if (PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
  {
    /* Clear StandBy flag */
    PWR_ClearFlag(PWR_FLAG_SB);

    /* Turn on LED2 */
    STM_EVAL_LEDOn(LED2);
    
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
    /* No need to configure the RTC as the RTC config(clock source, enable,
       prescaler,...) are kept after wake-up from STANDBY */
  }
  else
  {
    /* RTC Configuration ******************************************************/
    /* Reset RTC Domain */
    RCC_RTCResetCmd(ENABLE);
    RCC_RTCResetCmd(DISABLE);

    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Set the RTC time base to 1s */
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0x00FF;
    RTC_Init(&RTC_InitStructure);


    /* Set the time to 01h 00mn 00s AM */
    RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours   = 0x01;
    RTC_TimeStructure.RTC_Minutes = 0x00;
    RTC_TimeStructure.RTC_Seconds = 0x00;  
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);
  }

  /* Clear RTC Alarm A flag */ 
  RTC_ClearFlag(RTC_FLAG_ALRAF);
}

/**
  * @brief  Configures the SysTick to generate an interrupt each 250 ms.
  * @param  None
  * @retval None
  */
void SysTickConfig(void)
{
  /* SysTick interrupt each 250 ms with SysTick Clock equal to 4MHz */
  if (SysTick_Config((SystemCoreClock/8) / 4))
  { 
    /* Capture error */ 
    while (1);
  }

  /* Select AHB clock(HCLK) divided by 8 as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

  /* Set SysTick Preemption Priority to 1 */
  NVIC_SetPriority(SysTick_IRQn, 0x04);
}

/**
  * @brief  Prints the Time Calendar in the GLASS LCD.
  * @param  None
  * @retval None
  */

void LCD_ShowTimeCalendar(void)
{
  uint8_t tmp = 0;
  
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

#ifdef USE_STM32L152_EVAL    
  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Hours & 0xF0) >> 0x04) + 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, APOSTROPHE_OFF, 0);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Hours & 0x0F))+ 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, APOSTROPHE_OFF, 1);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Minutes & 0xF0) >> 0x04) + 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, APOSTROPHE_OFF, 3);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Minutes & 0x0F))+ (uint8_t)0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, APOSTROPHE_OFF, 4);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Seconds & 0xF0) >> 0x04)+ 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, APOSTROPHE_OFF, 6);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Seconds & 0x0F)) + 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, APOSTROPHE_OFF, 7); 

#elif USE_STM32L152D_EVAL  
  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Hours & 0xF0) >> 0x04) + 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, DOUBLEPOINT_OFF, 0);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Hours & 0x0F))+ 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, DOUBLEPOINT_OFF, 1);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Minutes & 0xF0) >> 0x04) + 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, DOUBLEPOINT_OFF, 2);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Minutes & 0x0F))+ (uint8_t)0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, DOUBLEPOINT_OFF, 3);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Seconds & 0xF0) >> 0x04)+ 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, DOUBLEPOINT_OFF, 4);

  tmp = (char)(((uint8_t)(RTC_TimeStructure.RTC_Seconds & 0x0F)) + 0x30);
  LCD_GLASS_WriteChar(&tmp, POINT_OFF, DOUBLEPOINT_OFF, 5);
#endif
  
  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();
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
