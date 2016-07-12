/**
  ******************************************************************************
  * @file    COMP/PWMSignalControl/main.c 
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

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup PWMSignalControl
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Init Structure definition */
COMP_InitTypeDef        COMP_InitStructure;
GPIO_InitTypeDef        GPIO_InitStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
uint16_t Period = 65535;

/* Private functions ---------------------------------------------------------*/
void COMP_Config(void);
void TIM_Config(void);

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

  /* TIM2 channels Configuration in PWM mode */
  TIM_Config();

  /* COMP2 Configuration */
  COMP_Config();

  while(1)
  {
  }
}

/**
  * @brief  Configures TIM2: channel1 and channel2 in PWM mode
  * @param  None
  * @retval None
  */
void TIM_Config(void)
{
  /* GPIOA Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* TIM2 pins configuration */
  /* Configure TIM2 channel 1 and channel 2: PA0 and PA1 in Alternate function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Enable Alternate function on PA0 and PA1 to be controlled by TIM2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
  
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  /* PWM1 Mode configuration: Channel1 &  Channel2 in PWM1 mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;          
  TIM_OCInitStructure.TIM_Pulse = (Period/2) + 1; /* duty = 50% */
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  
  /* Select OCREFCLR as source for clearing OC1REF or OC2REF */
  TIM_SelectOCREFClear(TIM2, TIM_OCReferenceClear_OCREFCLR);
  /* OC1REF is is cleared as soon as a high level is detected on OCREFCLR input */
  TIM_ClearOC2Ref(TIM2, TIM_OCClear_Enable);
  /* OC2REF is is cleared as soon as a high level is detected on OCREFCLR input */
  TIM_ClearOC1Ref(TIM2, TIM_OCClear_Enable);

  /* Enable TIM2 counter */
  TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  Configures COMP2: PB5 as COMP2 non inverting input
  *                           VREFINT as COMP2 inverting input
  *                           and COMP2 output to TIM2 OCREFCLR.
  * @param  None
  * @retval None
  */
void COMP_Config(void)
{
  /* GPIOB Peripheral clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PB5 in analog mode: Connect PB5 to COMP2 non inverting input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* COMP clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP, ENABLE);
  /* COMP init: Select VREFINT as inverting input (COMP2 is enabled as soon as 
     inverting input is selected) */
  COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_VREFINT;
  /* COMP init: Redirect COMP2 output to TIM2 OCREFCLR input */
  COMP_InitStructure.COMP_OutputSelect = COMP_OutputSelect_TIM2OCREFCLR;
  /* COMP init: Select Fast speed for fast response */
  COMP_InitStructure.COMP_Speed = COMP_Speed_Fast;
  COMP_Init(&COMP_InitStructure);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *          where the assert_param error has occurred.
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
