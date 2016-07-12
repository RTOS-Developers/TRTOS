/**
  ******************************************************************************
  * @file    FLASH/Write_Protection/main.c 
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

#if defined USE_STM32L152D_EVAL
  #include "stm32l152d_eval.h"
#elif defined USE_STM32L152_EVAL
  #include "stm32l152_eval.h"
#endif

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup Write_Protection
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define FLASH_PAGE_SIZE    ((uint16_t)0x100)
#define START_ADDR         ((uint32_t)0x08005000)
#define END_ADDR           ((uint32_t)0x08011F00)

#define FLASH_PAGES_TO_BE_PROTECTED (OB_WRP_Pages80to95 | OB_WRP_Pages96to111 | OB_WRP_Pages272to287)

/* Uncomment this line to program the Flash pages */
#define FLASH_PAGE_PROGRAM
/* Uncomment this line to Enable Write Protection */
//#define WRITE_PROTECTION_ENABLE
/* Uncomment this line to Disable Write Protection */
//#define WRITE_PROTECTION_DISABLE

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t EraseCounter = 0x0, Address = 0x0, Data = 0x17532896;
__IO uint32_t WRPR_Value = 0x0, ProtectedPages = 0x0;
__IO uint8_t NbrOfPage = 0x0;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
volatile TestStatus MemoryProgramStatus = PASSED; 

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
  
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);  

  /* Unlock the Flash Program Erase controller */  
  FLASH_Unlock();
  
  /* Unlock the Option Bytes */  
  FLASH_OB_Unlock();

  /* Clear all pending flags */      
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

  /* Get pages write protection status */
  WRPR_Value = FLASH_OB_GetWRP();

  /* Get current write protected pages and the new pages to be protected/unprotected */
  ProtectedPages =  (WRPR_Value & FLASH_PAGES_TO_BE_PROTECTED);
  
#ifdef WRITE_PROTECTION_DISABLE
  
  if(ProtectedPages != 0x0)
  {
    /* Restore write protected pages */
    FLASHStatus = FLASH_OB_WRPConfig(FLASH_PAGES_TO_BE_PROTECTED, DISABLE);

    /* Generate System Reset to load the new option byte values */
    FLASH_OB_Launch();  
  }

#elif defined WRITE_PROTECTION_ENABLE
  
  /* Check if desired pages are not yet write protected */
  if(ProtectedPages != FLASH_PAGES_TO_BE_PROTECTED)
  {
    /* Enable the pages write protection */
    FLASHStatus = FLASH_OB_WRPConfig(FLASH_PAGES_TO_BE_PROTECTED, ENABLE);
  
    /* Generate System Reset to load the new option byte values */
    FLASH_OB_Launch();
  }
  
#endif

#ifdef FLASH_PAGE_PROGRAM  
  /* Define the number of page to be erased */
  NbrOfPage = (END_ADDR - START_ADDR) / FLASH_PAGE_SIZE;

  /* The selected pages are not write protected */
  if (ProtectedPages == 0x0)
  {
    /* Clear all pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                    | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

    /* Erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
      FLASHStatus = FLASH_ErasePage(START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }
  
    /* FLASH Half Word program of data 0x17532896 at addresses defined by  
       START_ADDR and END_ADDR */
    Address = START_ADDR;

    while((Address < END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_FastProgramWord(Address, Data);
      Address = Address + 4;
    }

    /* Check the correctness of written data */
    Address = START_ADDR;

    while((Address < END_ADDR) && (MemoryProgramStatus != FAILED))
    {
      if((*(__IO uint32_t*) Address) != Data)
      {
        MemoryProgramStatus = FAILED;
      }
      Address += 4;
    }
  }
  else
  { 
    /* Error to program the FLASH: The desired pages are write protected */ 
    MemoryProgramStatus = FAILED;
  }
 
#endif

  if (MemoryProgramStatus != FAILED)
  {	
    /* OK */
    /* Turn on LD1 */
    STM_EVAL_LEDOn(LED1);
  }
  else
  { 
    /* KO */
    /* Turn on LD2 */
    STM_EVAL_LEDOn(LED2);     
  }	
  
  while (1)
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
