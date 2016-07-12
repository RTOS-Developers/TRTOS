/**
  ******************************************************************************
  * @file    TIM/InputCapture/main.c 
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
#include "stm32l1xx_lcd.h"

#ifdef USE_STM32L152D_EVAL
#include "stm32l152d_eval_glass_lcd.h"
#else
#include "stm32l152_eval_glass_lcd.h"
#endif


/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_Input_Capture
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
TIM_ICInitTypeDef  TIM_ICInitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t LCDStringResult[8];
extern __IO uint32_t TIM4Freq;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void TIM_Config(void);
void LCD_GLASS_Config(void);
uint8_t* DisplayFreqOnLCD(uint32_t TIMx_Freq);

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
  
  /* TIM4 Configuration */
  TIM_Config();

  /* LCD GLASS Configuration */
  LCD_GLASS_Config();
  
  while (1)
  {
    /* Display the Frequency on The LCD Glass */
    LCD_GLASS_DisplayString(DisplayFreqOnLCD(TIM4Freq));
    TIM4Freq = 0;
    Delay(2000);
  }
}
/**
  * @brief  Configures the Timer.
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  /* GPIOB clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* TIM4 channel 2 pin (PB.07) configuration */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

  /* TIM4 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM4 CH2 pin (PB.07)  
     The Rising edge is used as active edge,
     The TIM4 CCR2 is used to compute the frequency value 
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
}

/**
  * @brief  Configures the LCD Glass.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Config(void)
{
  /* LCD configs */
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

  LCD_GLASS_Init();
}

/**
  * @brief  Display the measured frequency on LCD glass.
  * @param  Frequency 
  * @retval array of char to displayed on the LCD Glass.
  */
  
uint8_t* DisplayFreqOnLCD(uint32_t TIMx_Freq) 
{

  uint8_t LCD_Digit[8];

#ifdef USE_STM32L152_EVAL
 
  LCD_Digit[0] = (TIMx_Freq / 10000000);
  LCDStringResult[0] =  LCD_Digit[0]+ 0x30;
  
  LCD_Digit[1] = ((TIMx_Freq -(10000000 * LCD_Digit[0]))/1000000) ;
  LCDStringResult[1] = LCD_Digit[1]+ 0x30;
  
  LCD_Digit[2] = (TIMx_Freq -((10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1])))/100000;
  LCDStringResult[2] =  LCD_Digit[2] + 0x30;
  
  LCD_Digit[3] =(TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - (100000 * LCD_Digit[2]))/10000;
  LCDStringResult[3] = LCD_Digit[3]+ 0x30;
  
  LCD_Digit[4] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3]))/1000;
  LCDStringResult[4] = LCD_Digit[4]+ 0x30;
  
  LCD_Digit[5] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3])-(1000 * LCD_Digit[4]))/100;
  LCDStringResult[5] = LCD_Digit[5]+ 0x30;
  
  LCD_Digit[6] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3])-(1000 * LCD_Digit[4])-(100 * LCD_Digit[5]))/10;
  LCDStringResult[6] = LCD_Digit[6]+ 0x30;
  
  LCD_Digit[7] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3])-(1000 * LCD_Digit[4])-(100 * LCD_Digit[5])-(10 * LCD_Digit[6]));
  LCDStringResult[7] = LCD_Digit[7]+ 0x30;

#elif defined USE_STM32L152D_EVAL
 
  LCD_Digit[0] = (TIMx_Freq / 10000000);
  
  LCD_Digit[1] = ((TIMx_Freq -(10000000 * LCD_Digit[0]))/1000000) ;
  LCDStringResult[0] = LCD_Digit[1]+ 0x30;
  
  LCD_Digit[2] = (TIMx_Freq -((10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1])))/100000;
  LCDStringResult[1] =  LCD_Digit[2] + 0x30;
  
  LCD_Digit[3] =(TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - (100000 * LCD_Digit[2]))/10000;
  LCDStringResult[2] = LCD_Digit[3]+ 0x30;
  
  LCD_Digit[4] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3]))/1000;
  LCDStringResult[3] = LCD_Digit[4]+ 0x30;
  
  LCD_Digit[5] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3])-(1000 * LCD_Digit[4]))/100;
  LCDStringResult[4] = LCD_Digit[5]+ 0x30;
  
  LCD_Digit[6] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3])-(1000 * LCD_Digit[4])-(100 * LCD_Digit[5]))/10;
  LCDStringResult[5] = LCD_Digit[6]+ 0x30;
  
  LCD_Digit[7] = (TIMx_Freq -(10000000 * LCD_Digit[0]) - (1000000 * LCD_Digit[1]) - 
                   (100000 * LCD_Digit[2])-(10000 * LCD_Digit[3])-(1000 * LCD_Digit[4])-(100 * LCD_Digit[5])-(10 * LCD_Digit[6]));
  LCDStringResult[6] = LCD_Digit[7]+ 0x30;

#endif
  
  return LCDStringResult;
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (0xFF * nCount); index != 0; index--)
  {
  }    
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

  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
