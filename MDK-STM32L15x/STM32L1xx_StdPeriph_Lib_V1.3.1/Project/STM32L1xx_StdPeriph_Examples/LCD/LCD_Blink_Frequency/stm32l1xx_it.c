/**
  ******************************************************************************
  * @file    LCD/LCD_Blink_Frequency/stm32l1xx_it.c 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32l1xx_it.h"

#ifdef USE_STM32L152D_EVAL 
  #include "stm32l152d_eval.h"
  #include "stm32l152d_eval_glass_lcd.h"
#elif defined USE_STM32L152_EVAL 
  #include "stm32l152_eval.h"
  #include "stm32l152_eval_glass_lcd.h"
#endif 

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup LCD_Blink_Frequency
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t counter = 1; 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32L1xx Peripherals Interrupt Handlers                        */
/******************************************************************************/
 
/**
  * @brief  This function handles EXTI Line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
  {
    /* Clear The LCD GLASS */
    LCD_GLASS_Clear();
  
    switch(counter)
    {
      case 0:
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/16 */      
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div16); 
        
        /* Display the "BLF0016" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF0016");
        
        /* Increment the Blink freqency */
        counter++;
        break;
       
      case  1: 
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/32 */ 
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div32); 
      
        /* Display the "BLF0032" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF0032");
      
        /* Increment the Blink frequency */
        counter++;
        break ;
      
      case  2:
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/64 */ 
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div64); 
        
        /* Display the "BLF0064" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF0064");
        
        /* Increment the Blink frequency */
        counter++;
        break ;
      
      case  3:
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/128 */ 
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div128); 
      
        /* Display the "BLF0128" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF0128");
      
        /* Increment the blink frequency */
        counter++;
        break ;
      
      case  4:
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/256 */ 
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div256);
       
        /* Display the "BLF0256" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF0256");
      
        /* Increment the Blink frequency */
        counter++;
        break ;
      
      case 5:
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/512 */ 
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div512); 
      
        /* Display the "BLF0512" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF0512");
      
        /* Increment the Blink frequency */
        counter++;
        break;
      default:
        /* Configure all LCD segments and Coms to blink at frequency LCDCLK/0 */ 
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div1024);
       
        /* Display the "BLF1024" message on the LCD GLASS */
        LCD_GLASS_DisplayString("BLF1024");
        #ifdef USE_STM32L152D_EVAL 
          counter = 1;
        #elif defined USE_STM32L152_EVAL 
          counter = 0;
        #endif 
        break ;
    }
  }
  /* Clears the key Button EXTI line pending bits. */
  EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
}

/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
