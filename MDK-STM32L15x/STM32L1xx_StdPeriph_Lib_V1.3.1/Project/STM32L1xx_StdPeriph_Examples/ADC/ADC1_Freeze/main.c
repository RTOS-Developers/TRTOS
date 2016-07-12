/**
  ******************************************************************************
  * @file    ADC/ADC1_Freeze/main.c 
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
#include "main.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC1_Freeze
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define ADC_RATIO                   ((uint16_t) 806) /*ADC_RATIO = ( 3.3 * 1000 * 1000)/4095 */
#define ASCII_NUM_0                 0x30

#ifdef USE_STM32L152D_EVAL 
  #define  EVAL_RESISTOR_R62      110   /* For EVAL REVA ALPHA1 540 */  
  #define  RV3_MEASUREMENT_ADC_CHANNEL    ADC_Channel_31
  #define  IDD_MEASUREMENT_ADC_CHANNEL    ADC_Channel_1b
  #define  IDD_MEASUREMENT_GPIO      GPIOF
  #define  IDD_MEASUREMENT_GPIO_CLK  RCC_AHBPeriph_GPIOF
  #define  GPIO_PIN_X GPIO_Pin_10
  #define  GPIO_PIN_Y GPIO_Pin_11

#elif defined USE_STM32L152_EVAL 
  #define  EVAL_RESISTOR_R36      130    /* value of resistor R36 is 1.3 Ohm 
                                           EVAL_RESISTOR_RATIO * 1.3 = 130  */    
  #define  RV3_MEASUREMENT_ADC_CHANNEL    ADC_Channel_18
  #define  IDD_MEASUREMENT_ADC_CHANNEL    ADC_Channel_5
  #define  IDD_MEASUREMENT_GPIO      GPIOA
  #define  IDD_MEASUREMENT_GPIO_CLK  RCC_AHBPeriph_GPIOA
  #define  GPIO_PIN_X GPIO_Pin_12
  #define  GPIO_PIN_Y GPIO_Pin_5
#endif 

#define EVAL_RESISTOR_RATIO    100    /* R36 is multiplied by 100 */
#define EVAL_MAX9938_GAIN      50     /* Ampli-op gain = 50 */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef           ADC_InitStructure;
GPIO_InitTypeDef          GPIO_InitStructure;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
NVIC_InitTypeDef          NVIC_InitStructure;
__IO   uint8_t digit1 = 0, digit2 = 0;
/* Private function prototypes -----------------------------------------------*/
void ADC_Config(void);
void LCD_Glass_Config(void);
void TIM2_Config(void);

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

  /* ADC1 configuration: channel (18 or 31)  and channel (5 or 1b)   */
  ADC_Config();

  /* Configure the STM32L1XX-EVAL KEY Push button */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI); 
  
#ifdef USE_STM32L152D_EVAL 
  /* Set VALUEUNIT_MILLIAMPERE */
  LCD_GLASS_ValueUnitConfig(VALUEUNIT_MILLIAMPERE);
#endif
  
  /* TIM2 Configuration */
  TIM2_Config();
  /* Request LCD RAM update */
  LCD_UpdateDisplayRequest();

  while (1)
  {   
  }
}

/**
  * @brief  Configures the ADC1: channel (18 or 31)  and channel (5 or 1b).
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{
  /* Enable The HSI (16Mhz) */
  RCC_HSICmd(ENABLE);
  
  /* Check that HSI oscillator is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
   
  /* Configure RV3 input voltage */

  /* Enable the GPIOF Clock */
  RCC_AHBPeriphClockCmd(IDD_MEASUREMENT_GPIO_CLK, ENABLE);
  
  /* Configure PF.10 (ADC Channel31) or PA.12 (ADC channel 18) in analog mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_X;
  GPIO_Init(IDD_MEASUREMENT_GPIO, &GPIO_InitStructure);
  
  /* Configure the IDD input */
  
  /* Configure PF.11 (ADC channel 1b) or PA.05 (ADC channel 5) in analog mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Y;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(IDD_MEASUREMENT_GPIO, &GPIO_InitStructure);

  /* ADC1 Configuration ------------------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
  
#ifdef USE_STM32L152D_EVAL
  /* Select ADC Bank channel */
  ADC_BankSelection(ADC1, ADC_Bank_B);
#endif
  
  /* ADC1 Configuration of channel18/31 and channel5/1b : continuous mode, external trigger (TIM2) */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 2;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 Regular Channel Config */
  ADC_RegularChannelConfig(ADC1, IDD_MEASUREMENT_ADC_CHANNEL, 1, ADC_SampleTime_192Cycles);
  ADC_RegularChannelConfig(ADC1, RV3_MEASUREMENT_ADC_CHANNEL, 2, ADC_SampleTime_192Cycles);

  /* Enables the ADC1 Power Down during Delay */ 
  ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
  
  /* Delay until the converted data has been read */
  ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait until the ADC1 is ready */
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
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
  * @brief  Configures TIM2.
  * @param  None
  * @retval None
  */
void TIM2_Config(void)
{
  /* Enable TIM2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFF;         
  TIM_TimeBaseStructure.TIM_Prescaler = 0x1;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* Enable TIM2 Update Interrupt*/ 
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable TIM2 */       
  TIM_Cmd(TIM2,ENABLE);
}

/**
  * @brief  Display the ADC converted Values on the LCD Glass.
  * @param  data1: VDD voltage.
  * @param  data2: RV3 voltage.
  * @retval None
  */
void DisplayOnLCD(uint16_t data1, uint16_t data2) 
{
  uint16_t rv3voltage=0;
  uint32_t iddrun = 0;

  
#ifdef USE_STM32L152D_EVAL 
  uint8_t LCDString[7]="00V 00";
#elif defined USE_STM32L152_EVAL 
  uint8_t LCDString[9]="00V 00MA";
#endif 

  /* Calculate the current consumption */
#ifdef USE_STM32L152D_EVAL 
  iddrun = (uint32_t) ((uint32_t)(((data1 * 1000 * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                           / (uint32_t)(EVAL_RESISTOR_R62));
  
#elif defined USE_STM32L152_EVAL 
  iddrun = (uint32_t) ((uint32_t)(((data1 * 1000 * EVAL_RESISTOR_RATIO) / EVAL_MAX9938_GAIN))\
                           / (uint32_t)(EVAL_RESISTOR_R36));  
#endif 

  /* digit1 value*/
  digit1 = (uint8_t)(iddrun / 1000);
  /* digit2 value */
  digit2 = (uint8_t)((iddrun % 1000) / 100);
  
  /* Fill the LCDString fields with the current Voltage */
#ifdef USE_STM32L152D_EVAL 
  LCDString[5] = (uint8_t)((uint8_t)(digit1) + ASCII_NUM_0);
  LCDString[6] = (uint8_t)((uint8_t)(digit2) + ASCII_NUM_0);
#elif defined USE_STM32L152_EVAL  
  LCDString[4] = (uint8_t)((uint8_t)(digit1) + ASCII_NUM_0);
  LCDString[5] = (uint8_t)((uint8_t)(digit2) + ASCII_NUM_0);
#endif 

  /* Calculate RV3 voltage value*/
  rv3voltage = (uint16_t)((uint32_t)((uint32_t)data2 * (uint32_t)ADC_RATIO) / (uint32_t)1000);
  
  /* digit1 value*/
  digit1 = (uint8_t)(rv3voltage / 1000);
  /* digit2 value */
  digit2 = (uint8_t)((rv3voltage % 1000) / 100);

  /* Fill the LCDString fields with the current Voltage */
  LCDString[0] = (uint8_t)((uint8_t)(digit1) + ASCII_NUM_0);
  LCDString[1] = (uint8_t)((uint8_t)(digit2) + ASCII_NUM_0);
 
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  
#ifdef USE_STM32L152_EVAL  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[0], POINT_ON, APOSTROPHE_OFF, 0);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[1], POINT_OFF, APOSTROPHE_OFF, 1);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[2], POINT_OFF, APOSTROPHE_OFF, 2);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[3], POINT_OFF, APOSTROPHE_OFF, 3);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[4], POINT_ON, APOSTROPHE_OFF , 4);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[5], POINT_OFF, APOSTROPHE_OFF, 5);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[6], POINT_OFF, APOSTROPHE_OFF, 6);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[7], POINT_OFF, APOSTROPHE_OFF, 7);
  
#elif defined USE_STM32L152D_EVAL 
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[0], POINT_OFF, DOUBLEPOINT_OFF, 0);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[1], POINT_ON, DOUBLEPOINT_OFF, 1);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[2], POINT_OFF, DOUBLEPOINT_OFF, 2);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[3], POINT_OFF, DOUBLEPOINT_OFF, 3);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[4], POINT_OFF, DOUBLEPOINT_OFF , 4);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[5], POINT_OFF, DOUBLEPOINT_OFF, 5);
  
  /* Display one character on LCD */
  LCD_GLASS_WriteChar(&LCDString[6], POINT_ON, DOUBLEPOINT_OFF, 6);

#endif 
  
  /* Request LCD RAM update */
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
