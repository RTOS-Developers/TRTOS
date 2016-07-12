/**
  ******************************************************************************
  * @file    ADC/ADC1_IDDmeas/main.c 
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
  #include "stm32l152d_eval_glass_lcd.h"
#elif defined USE_STM32L152_EVAL 
  #include "stm32l152_eval_glass_lcd.h"
#endif 

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC1_IDDmeas
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_STM32L152D_EVAL 
  #define  EVAL_RESISTOR_R62      110   /* For EVAL REVA ALPHA1 540 */ 
  #define  IDD_MEASUREMENT_PIN       GPIO_Pin_11
  #define  IDD_MEASUREMENT_GPIO_CLK  RCC_AHBPeriph_GPIOF
  #define  IDD_MEASUREMENT_GPIO      GPIOF
  #define  IDD_MEASUREMENT_ADC_CHANNEL  ADC_Channel_1b
  #define  APOSTROPHE_OFF DOUBLEPOINT_OFF

#elif defined USE_STM32L152_EVAL 
  #define  EVAL_RESISTOR_R36      130    /* value of resistor R36 is 1.3 Ohm 
                                           EVAL_RESISTOR_RATIO * 1.3 = 130  */ 
  #define  IDD_MEASUREMENT_PIN       GPIO_Pin_5
  #define  IDD_MEASUREMENT_GPIO_CLK  RCC_AHBPeriph_GPIOA
  #define  IDD_MEASUREMENT_GPIO      GPIOA
  #define  IDD_MEASUREMENT_ADC_CHANNEL  ADC_Channel_5
  #define APOSTROPHE_OFF APOSTROPHE_OFF
#endif 

#define EVAL_RESISTOR_RATIO    100    /* R36 is multiplied by 100 */
#define EVAL_MAX9938_GAIN      50     /* Ampli-op gain = 50 */
#define ADC_CONVERT_RATIO      806    /* (3300mV / 4095)* 1000 */




/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
__IO uint16_t ADCdata = 0;
__IO uint32_t VDDmV = 0, IDDrunuA = 0;
uint8_t LCDString[8];

/* Private function prototypes -----------------------------------------------*/
void ADC_Config(void);
void LCD_Glass_Config(void);
void DisplayIDDrunmA(uint32_t IDDmeas);

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

  /* LCD GLASS Configuration */
  LCD_Glass_Config();

#ifdef USE_STM32L152D_EVAL 
  /* Set VALUEUNIT_MILLIAMPERE */
  LCD_GLASS_ValueUnitConfig(VALUEUNIT_MILLIAMPERE);
#endif

  /* ADC1 channel 5 or 1 configuration */
  ADC_Config();

  while(1)
  {
    /* Read ADC conversion result */
    ADCdata = ADC_GetConversionValue(ADC1);

    /* Calculate voltage value*/
    VDDmV = (uint32_t)((uint32_t)ADCdata * (uint32_t)ADC_CONVERT_RATIO);
    
#ifdef USE_STM32L152_EVAL 
    /* Calculate the current consumption */
    IDDrunuA = (uint32_t) ((uint32_t)(((VDDmV * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                           / (uint32_t)(EVAL_RESISTOR_R36));
#elif defined USE_STM32L152D_EVAL
    /* Calculate the current consumption */
    IDDrunuA = (uint32_t) ((uint32_t)(((VDDmV * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                           / (uint32_t)(EVAL_RESISTOR_R62));

#endif    

    /* Display the IDD measured Value On the LCD Glass (mA) */
    DisplayIDDrunmA(IDDrunuA);
  }
}

/**
  * @brief  Configures the ADC1 channel5.
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  /* Enable The HSI (16Mhz) */
  RCC_HSICmd(ENABLE);

  /* Enable the GPIOF or GPIOA Clock */
  RCC_AHBPeriphClockCmd(IDD_MEASUREMENT_GPIO_CLK, ENABLE);
  /* Configure PF.11 (ADC Channel11) or PA.05 (ADC Channe5) in analog mode */
  GPIO_InitStructure.GPIO_Pin =  IDD_MEASUREMENT_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(IDD_MEASUREMENT_GPIO, &GPIO_InitStructure);

  /* Check that HSI oscillator is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

  /* ADC1 Configuration ------------------------------------------------------*/
  
  /* Enable ADC1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
#ifdef USE_STM32L152D_EVAL
  /* Select ADC Bank channel */
  ADC_BankSelection(ADC1, ADC_Bank_B);
#endif
  
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel5 or channel1 configuration */
  ADC_RegularChannelConfig(ADC1, IDD_MEASUREMENT_ADC_CHANNEL, 1, ADC_SampleTime_192Cycles);

  /* Define delay between ADC1 conversions */
  ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);
  
  /* Enable ADC1 Power Down during Delay */
  ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Wait until ADC1 ON status */
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
  }

  /* Start ADC1 Software Conversion */
  ADC_SoftwareStartConv(ADC1);

  /* Wait until ADC Channel 5 or 1 end of conversion */
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
  {
  }
}

/**
  * @brief  Configure the LCD Glass.
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
  * @brief  Display the IDD measured Value On the LCD Glass.
  * @param  IDD measure
  * @retval None
  */
void DisplayIDDrunmA(uint32_t IDDmeas)
{ 
  /* x  current value*/
  LCDString[0] =((uint8_t)(IDDrunuA / 100000)) + 0x30;
  
  /* Hundreds current value */
  LCDString[1] =((uint8_t)((IDDrunuA % 100000) / 10000)) + 0x30;
  
  /* Tens current value */
  LCDString[2] =((uint8_t)((IDDrunuA % 10000) / 1000)) + 0x30;
  
  /* 1mA current value */
  LCDString[3] =((uint8_t)((IDDrunuA % 1000) / 100)) + 0x30;
  
  /* 0.1 mA current value */
  LCDString[4] =((uint8_t)((IDDrunuA % 100 ) / 10)) + 0x30;
  
  /* 0.01 mA current value */
  LCDString[5] = ((uint8_t)(IDDrunuA % 10)) + 0x30;

  
#ifdef USE_STM32L152_EVAL 
    /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[0], POINT_OFF, APOSTROPHE_OFF, 0);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[1], POINT_OFF, APOSTROPHE_OFF, 1);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[2], POINT_ON, APOSTROPHE_OFF, 2);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[3], POINT_OFF, APOSTROPHE_OFF, 3);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[4], POINT_OFF, APOSTROPHE_OFF, 4);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[5], POINT_OFF, APOSTROPHE_OFF, 5);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar("M", POINT_OFF, APOSTROPHE_OFF, 6);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar("A", POINT_OFF, APOSTROPHE_OFF, 7);
  
#elif defined USE_STM32L152D_EVAL
    /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[0], POINT_OFF, APOSTROPHE_OFF, 0);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[1], POINT_OFF, APOSTROPHE_OFF, 1);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[2], POINT_OFF, APOSTROPHE_OFF, 2);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[3], POINT_ON, APOSTROPHE_OFF, 3);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[4], POINT_OFF, APOSTROPHE_OFF, 4);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[5], POINT_OFF, APOSTROPHE_OFF, 5);
  
    /* Display one character on LCD */
  LCD_GLASS_WriteChar("", POINT_OFF, APOSTROPHE_OFF, 6);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar("", POINT_OFF, APOSTROPHE_OFF, 7);

#endif 
  
  /*!< Request LCD RAM update */
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
