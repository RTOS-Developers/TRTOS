/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
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
  #define GPIO_PIN_X GPIO_Pin_3
  #define GPIO_PIN_Y GPIO_Pin_7
  #define BSRR_VAL 0x88
#elif defined USE_STM32L152_EVAL 
  #include "stm32l152_eval.h"
  #define GPIO_PIN_X GPIO_Pin_0
  #define GPIO_PIN_Y GPIO_Pin_1
  #define BSRR_VAL 0x03
#endif 

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef        GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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

  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  /* Configure PD0 and PD1 or PD3 and PD7 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_X | GPIO_PIN_Y;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PD0 and  PD1 or PD3 and PD7 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */
  while (1)
  {
    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;

    /* Set PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRL = BSRR_VAL;
    /* Reset PD0 and PD2 or PD3 and PD7 */
    GPIOD->BSRRH = BSRR_VAL;
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
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
