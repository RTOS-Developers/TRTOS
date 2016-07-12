/**
  ******************************************************************************
  * @file    COMP/PulseWidthMeasurement/main.c 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   Main program body.
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
#include "stm32l152d_eval_glass_lcd.h"
#include "stm32l152d_eval_lcd.h"
#else
#include "stm32l152_eval_glass_lcd.h"
#include "stm32l152_eval_lcd.h"
#endif


/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup PulseWidthMeasurement
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t Capture = 0;
__IO uint32_t MeasuredPulse = 0;
__IO uint32_t DisplayActive = 0;

/* Init Structure definition */
COMP_InitTypeDef COMP_InitStructure;
DAC_InitTypeDef  DAC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

/* Private functions ---------------------------------------------------------*/
void DAC_Config(void);
void COMP_Config(void);
void TIM_Config(void);
void LCD_Glass_Config(void);
void DisplayOnLCD(uint16_t data);

/**
  * @brief   Main program.
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
  
#ifdef USE_STM32L152_EVAL  
  /* LCD GLASS Configuration */
  LCD_Glass_Config();
  /* Initialize the TFT-LCD */
  STM32L152_LCD_Init();
#elif defined USE_STM32L152D_EVAL
  /* Initialize the TFT-LCD */
  STM32L152D_LCD_Init();
#else  

#endif 

  /* Clear the TFT-LCD */
   LCD_Clear(LCD_COLOR_WHITE);

  /* DAC Channel2 configuration */
  DAC_Config();

  /* COMP2 Configuration */
  COMP_Config();

  /* TIM2 Configuration in input capture mode */
  TIM_Config();
  
  while(1)
  {
    if (DisplayActive != 0)
    {
      /* Compute the pulse width in us */
      MeasuredPulse = (uint32_t)(((uint64_t) Capture * 1000000) / ((uint32_t)SystemCoreClock));
      
      /* Display measured pulse width on Glass LCD and color LCD */
      DisplayOnLCD(MeasuredPulse);
      DisplayActive = 0;
    }
  }
}

/**
  * @brief  Configures the LCD Glass.
  * @param  None
  * @retval None
  */
void LCD_Glass_Config(void)
{  
  /* Enable PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /* LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  
  /* LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  /* Init LCD Glass */
  LCD_GLASS_Init();
}

/**
  * @brief  Configures the DAC channel 2 with output buffer enabled.
  * @param  None
  * @retval None
  */
void DAC_Config(void)
{
  /* DAC clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* Fill DAC InitStructure */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits2_0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

  /* DAC Channel2 Init */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);
  /* Enable DAC Channel2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);
  /* Set DAC Channel2 DHR register: DAC_OUT2 = (3.3 * 868) / 4095 ~ 0.7 V */
  DAC_SetChannel2Data(DAC_Align_12b_R, 868);
}

/**
  * @brief  Configures COMP2: DAC channel 2 to COMP2 inverting input
  *                           and COMP2 output to TIM2 IC4.
  * @param  None
  * @retval None
  */
void COMP_Config(void)
{
  /**************************** COMP2 Config **********************************/
  /* GPIOB Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configuring PB5 in analog mode closes the switch GR6-2 so PB5 is connected
     to COMP2 non inverting input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* COMP clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP, ENABLE);
  
  /* COMP2 Init: COMP2 is enabled as soon as inverting input is selected */
  /* use DAC2 output as a reference voltage: DAC2 output is connected to COMP2
     inverting input */
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_DAC2;
  /* Redirect COMP2 output to TIM2 Input capture 4 */
  COMP_InitStructure.COMP_OutputSelect = COMP_OutputSelect_TIM2IC4;
  COMP_InitStructure.COMP_Speed = COMP_Speed_Fast;
  COMP_Init(&COMP_InitStructure);
}

/**
  * @brief  Configures TIM2 channel 4 in input capture mode
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  /**************************** TIM2 Config ***********************************/
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
  /* TIM2 Time base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  
  /* TIM2 Channel4 Input capture Mode configuration */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  /* TIM2 counter is captured at each transition detection: rising or falling edges (both edges) */
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
  /* TIM2 IRQChannel enable */  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable capture interrupt */
  TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
  
  /* Enable the TIM2 counter */
  TIM_Cmd(TIM2, ENABLE);
  
  /* Reset the flags */
  TIM2->SR = 0;
}

/**
  * @brief  Display the measured pulse on the LCD Glass.
  * @param  data: measured pulse width.
  * @retval None
  */
void DisplayOnLCD(uint16_t data)
{
  static uint8_t LCDSTRING[]     = "      US            ";
  uint32_t index = 0;

#ifdef USE_STM32L152_EVAL
  uint8_t lcdstring[8]="      US";

  /* Fill the LCDString fields with the measured value */
  lcdstring[1] = (uint8_t)((uint8_t)(data / 1000) + 0x30);
  lcdstring[2] = (uint8_t)((uint8_t)((data % 1000) / 100) + 0x30);
  lcdstring[3] = (uint8_t)((uint8_t)((data % 100) / 10) + 0x30);
  lcdstring[4] = (uint8_t)((uint8_t)(data % 10) + 0x30);

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  
 
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[0], POINT_OFF, APOSTROPHE_OFF, 0);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[1], POINT_OFF, APOSTROPHE_OFF, 1);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[2], POINT_OFF, APOSTROPHE_OFF, 2);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[3], POINT_OFF, APOSTROPHE_OFF, 3);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[4], POINT_OFF, APOSTROPHE_OFF , 4);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[5], POINT_OFF, APOSTROPHE_OFF, 5);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[6], POINT_OFF, APOSTROPHE_OFF, 6);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&lcdstring[7], POINT_OFF, APOSTROPHE_OFF, 7);

  /* Request LCD RAM update */
  LCD_UpdateDisplayRequest();
#endif

  LCDSTRING[1] = (uint8_t)((uint8_t)(data / 1000) + 0x30);
  LCDSTRING[2] = (uint8_t)((uint8_t)((data % 1000) / 100) + 0x30);
  LCDSTRING[3] = (uint8_t)((uint8_t)((data % 100) / 10) + 0x30);
  LCDSTRING[4] = (uint8_t)((uint8_t)(data % 10) + 0x30);

  /* Display measured value on LCD */
  for (index = 0; index < 20; index++)
  {
    LCD_DisplayChar(LCD_LINE_3, (319 - (16 * index)), LCDSTRING[index]);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
