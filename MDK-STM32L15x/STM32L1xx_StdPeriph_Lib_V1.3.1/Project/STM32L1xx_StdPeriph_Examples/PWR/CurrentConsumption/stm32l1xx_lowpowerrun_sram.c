/**
  ******************************************************************************
  * @file    PWR/CurrentConsumption/stm32l1xx_lowpowerrun_sram.c 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   This file provides firmware functions to manage the STM32L1xx 
  *          Low Power Run Mode from Internal SRAM.
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

/** @addtogroup CurrentConsumption
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the system to enter Low Power Run mode for
  *         current consumption measurement purpose. The code is running from 
  *         Internal SRAM.
  *         The maximum clock when the system is in Low Power Run mode is ~128KHz.
  *         This mode can only be entered when Voltage Range 2 is selected.    
  * @param  None
  * @retval None
  */
#if defined ( __CC_ARM   )
/* With ARM Compiler, RAM functions are defined using the toolchain options. 
   For more details please refer to your toolchain documentation. */
  void

#elif defined ( __ICCARM__ )
 __ramfunc void

#elif defined   (  __GNUC__  )
void __attribute__((section(".data")))
#elif defined   (  __TASKING__  )
#pragma section_code_init on
#endif

LowPowerRunModeSRAM_Measure(void)
{

  /* This function should be called when the program is running from internal SRAM. */
  FLASH_RUNPowerDownCmd(ENABLE);

  /* Wait Until Key button pressed */
  while(((*(__IO uint32_t *)0x40020010) & 0x0001) == 0)
  {
  }

  /* Wait Until Key button pressed */
  while(((*(__IO uint32_t *)0x40020010) & 0x0001) != 0)
  {
  }

  /* This function should be called when the program is running from internal SRAM. */
  FLASH_RUNPowerDownCmd(DISABLE);  
}

#if defined (  __TASKING__  )
#pragma section_code_init restore
#endif


/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
