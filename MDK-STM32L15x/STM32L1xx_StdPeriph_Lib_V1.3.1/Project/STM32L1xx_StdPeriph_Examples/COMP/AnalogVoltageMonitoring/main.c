/**
  ******************************************************************************
  * @file    COMP/AnalogVoltageMonitoring/main.c 
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
#include "main.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup AnalogVoltageMonitoring
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ADCVal = 0;
uint8_t LCD_String[8]= " WXYZ V ";
static uint8_t VoltageDisplay[]     = "  .   V ADCVoltage  ";
__IO uint32_t State = STATE_OVER_THRESHOLD;

/* Init Structure definition */
NVIC_InitTypeDef        NVIC_InitStructure;
COMP_InitTypeDef        COMP_InitStructure;
ADC_InitTypeDef         ADC_InitStructure;
EXTI_InitTypeDef        EXTI_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void ADC_Config(void);
void COMP_Config(void);
void GPIO_Config(void);
void LCD_Glass_Config(void);
void DisplayVoltage(uint32_t ADCVoltage);
void RestoreConfiguration(void);
void SetSysClock(void);

/* Private functions ---------------------------------------------------------*/

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

  uint32_t index = 0;
  
  /* Configure all GPIO pins in Analog mode for lowsest consumption */
  GPIO_Config();

  /* ADC configuration: Channel 18 or 31 (PB12 or PF10) is used, End Of Conversion (EOC) interrupt is enabled */
  ADC_Config();

#ifdef USE_STM32L152_EVAL
  /* LCD GLASS Configuration: LSI as LCD clock source */
  LCD_Glass_Config();
  /* Initialize the TFT-LCD */
  STM32L152_LCD_Init();
#elif defined USE_STM32L152D_EVAL 
  /* Initialize the TFT-LCD */
  STM32L152D_LCD_Init();
#endif 
  
  /* Clear the TFT-LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  
  while(1)
  {
    if (State == STATE_OVER_THRESHOLD) /* Input voltage is over the threshold */
    {
      /* Indicator LED: MCU in RUN mode */
      STM_EVAL_LEDOff(LED1);

      /* Disable COMP IRQ */
      NVIC_InitStructure.NVIC_IRQChannel = COMP_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure);
      /* Enable ADC1 IRQ */
      NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      /* COMP clock disable */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP, DISABLE);

      /* Restore MCU configuration */
      RestoreConfiguration();

      /* Enable ADC1 */
      ADC_Cmd(ADC1, ENABLE);
      /* Start ADC1 Software Conversion */
      ADC_SoftwareStartConv(ADC1);
      /* Wait for ADC to be ready */
      while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS));  

      while(State == STATE_OVER_THRESHOLD)
      {
        
        /* Display measured value on Glass LCD */
        DisplayVoltage(ADCVal);
        
        /* Display measured value on LCD */
        for (index = 0; index < 20; index++)
        {
          LCD_DisplayChar(LCD_LINE_3, (319 - (16 * index)), VoltageDisplay[index]);
        }
        /* Check if the measured value is below the threshold VREFINT: 1.22 V */
        if (ADCVal <= 0x000005EA)
        {
          State = STATE_UNDER_THRESHOLD;
        }
      }
    }
    else /* Input voltage is under the threshold */
    {
      /* LED1 ON: MCU in STOP mode */
      STM_EVAL_LEDInit(LED1);
      STM_EVAL_LEDOn(LED1);

      /* Disable ADC1 IRQ */
      NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
      NVIC_Init(&NVIC_InitStructure);
      /* Disable ADC1 */
      ADC_Cmd(ADC1, DISABLE);

      /* Configure COMP2 with interrupt enabled */
      COMP_Config();

      /* Check COMP2 output level before entering STOP mode */
      if (COMP_GetOutputLevel(COMP_Selection_COMP2) == COMP_OutputLevel_Low)
      {
        /* Disable LSI oscillator before entering STOP mode */
        RCC_LSICmd(DISABLE);

        /* Enter STOP mode with regulator in low power */
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
      }
    }
  }
}

/**
  * @brief  ADC configuration.
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  /* ADC1 Peripheral clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Channel 18 or 31 are already configured in Analog mode using GPIO registers */

  /* ADC1 Configuration */
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

#ifdef USE_STM32L152D_EVAL
   /* ADC1 regular channel 31 configuration for STM32L152D*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_31, 1, ADC_SampleTime_384Cycles);
#elif USE_STM32L152_EVAL  
  /* ADC1 regular channel 18 configuration for STM32L152*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_384Cycles);
#endif  

  /* Enable EOC interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}

/**
  * @brief  COMP2 configuration.
  * @param  None
  * @retval None
  */
void COMP_Config(void)
{
  /* COMP Peripheral clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP, ENABLE);

  /* SYSCFG Peripheral clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Close the I/O analog switch number n */
  SYSCFG_RIIOSwitchConfig(RI_IOSwitch_GR6_2, ENABLE);
  
  /* COMP2 Init: COMP2 is enabled as soon as inverting input is selected */
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_VREFINT;
  COMP_InitStructure.COMP_OutputSelect = COMP_OutputSelect_None;
  COMP_InitStructure.COMP_Speed = COMP_Speed_Slow;
  COMP_Init(&COMP_InitStructure);

  /* Configure EXTI Line 22 in interrupt mode */
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  /* Clear EXTI22 line */
  EXTI_ClearITPendingBit(EXTI_Line22);

  /* Configure COMP IRQ */
  NVIC_InitStructure.NVIC_IRQChannel = COMP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the LCD Glass. LSI is used as LCD clock source
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

  /* LSI Enable */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* LCD Clock Source Selection: LSI */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#ifdef USE_STM32L152_EVAL
  /* LCD GLASS Initialization */
  LCD_GLASS_Init();
#endif  
}


/**
  * @brief  Set the system clock at 32MHz.
  * @param  None
  * @retval None
  */
void SetSysClock(void)
{
  ErrorStatus HSEStartUpStatus;

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x0C)
    {}
  }
  else
  {
    /* If HSE fails to start-up, the application will have wrong clock 
       configuration. User can add here some code to deal with this error */
  } 
}

/**
  * @brief  Restore peripheral config before entering STOP mode.
  * @param  None
  * @retval None
  */
void RestoreConfiguration(void)
{
  /* Restore system clock to 32MHz */
  SetSysClock();

  /* Enable HSI clock for ADC */ 
  RCC_HSICmd(ENABLE);

  /* LSI Enable */
  RCC_LSICmd(ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (Schmidt trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);

#ifdef USE_STM32L152D_EVAL
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOG, ENABLE);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);
#endif
 
}

/**
  * @brief  Display the measured voltage on LCD glass.
  * @param  None
  * @retval None
  */
void DisplayVoltage(uint32_t ADCVoltage)
{
  uint32_t Digit1, Digit2, Digit3, Digit4 = 0;

  ADCVoltage = ADCVoltage * 3300;
  ADCVoltage = ADCVoltage / 4095; 
  Digit1 = ADCVoltage / 1000;
  LCD_String[1] = Digit1 + 0x30;
  VoltageDisplay[1] = Digit1 + 0x30;
  Digit2 = (ADCVoltage - (Digit1 * 1000)) / 100;
  LCD_String[2] = Digit2 + 0x30;
  VoltageDisplay[3] = Digit2 + 0x30;
  Digit3 = ((ADCVoltage - ((Digit1 * 1000) + (Digit2 * 100))) / 10);
  LCD_String[3] = Digit3 + 0x30;
  VoltageDisplay[4] = Digit3 + 0x30;
  Digit4 = (ADCVoltage - ((Digit1 * 1000) + (Digit2 * 100) + (Digit3 * 10)));
  LCD_String[4] = Digit4 + 0x30;
  VoltageDisplay[5] = Digit4 + 0x30;

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
#ifdef USE_STM32L152_EVAL
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCD_String[1], POINT_ON, APOSTROPHE_OFF, 1);
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCD_String[2], POINT_OFF, APOSTROPHE_OFF, 2);
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCD_String[3], POINT_OFF, APOSTROPHE_OFF, 3);
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCD_String[4], POINT_OFF, APOSTROPHE_OFF, 4);
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCD_String[5], POINT_OFF, APOSTROPHE_OFF, 5);
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCD_String[6], POINT_OFF, APOSTROPHE_OFF, 6);
#endif
  
  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
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
