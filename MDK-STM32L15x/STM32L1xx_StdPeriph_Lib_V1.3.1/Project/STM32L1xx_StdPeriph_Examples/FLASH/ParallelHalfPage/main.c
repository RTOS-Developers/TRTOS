/**
  ******************************************************************************
  * @file    FLASH/ParallelHalfPage/main.c 
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
#include "stm32l152d_eval.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_ParallelHalfPage
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define FLASH_PAGE_SIZE              0x100
#define FLASH_BANK1_START_ADDR       0x08004000 /* FLASH BANK1 starts at 0x08000000 and ends at 0x08030000 */
#define FLASH_BANK2_START_ADDR       0x08043000 /* FLASH BANK2 starts at 0x08030000 and ends at 0x08060000 */
#define FLASH_HALF_PAGE_NUMBER       0x100 /* Number of half page to be written (128bytes) */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO TestStatus MemoryProgramStatus = PASSED;
uint32_t Address1 = 0, j = 0, Address2 = 0;
uint32_t Data1[32] = {0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 
                      0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 
                      0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 
                      0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 0x00336699, 
                     };

uint32_t Data2[32] = {0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA,
                      0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA,
                      0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA,
                      0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA, 0x114477AA,
                     };                     
                       
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
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

  /* Unlock the FLASH Program memory */ 
  FLASH_Unlock();

  /* Clear all pending flags */      
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);

  Address1 = FLASH_BANK1_START_ADDR;
  Address2 = FLASH_BANK2_START_ADDR; 
  
  /* Erase the FLASH Program memory pages */
  for(j = 0; j < (FLASH_HALF_PAGE_NUMBER / 2); j++)
  {
    FLASHStatus = FLASH_EraseParallelPage(Address1 + (FLASH_PAGE_SIZE * j), Address2 + (FLASH_PAGE_SIZE * j));

    if (FLASHStatus != FLASH_COMPLETE)
    {
      MemoryProgramStatus = FAILED;
    }
    else
    {
      FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                      | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);
    }     
  }
   
  Address1 = FLASH_BANK1_START_ADDR;
  Address2 = FLASH_BANK2_START_ADDR; 
  j = 0;
  
  /* Write the FLASH Program memory using HalfPage operation */
  while(j++ < FLASH_HALF_PAGE_NUMBER)
  {
    FLASHStatus = FLASH_ProgramParallelHalfPage(Address1, Data1, Address2, Data2);

    if(FLASHStatus == FLASH_COMPLETE)
    {
      Address1 += 128;
      Address2 += 128;
    }
    else
    {
      FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                      | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);
    } 
  }
  
  Address1 = FLASH_BANK1_START_ADDR;
  Address2 = FLASH_BANK2_START_ADDR;  
    
  /* Check the written data */
  while(Address1 < (FLASH_BANK1_START_ADDR + (FLASH_HALF_PAGE_NUMBER * 0x80)))
  {
    if((*(__IO uint32_t*)Address1 != Data1[0]) && (*(__IO uint32_t*)Address2 != Data2[0]))
    {
      MemoryProgramStatus = FAILED;
    }
    Address1 += 4;
    Address2 += 4;
  } 
  
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
