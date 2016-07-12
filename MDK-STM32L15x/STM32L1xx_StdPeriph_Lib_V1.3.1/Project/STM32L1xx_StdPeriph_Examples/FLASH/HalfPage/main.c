/**
  ******************************************************************************
  * @file    FLASH/HalfPage/main.c 
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

#if defined USE_STM32L152D_EVAL
  #include "stm32l152d_eval.h"
#elif defined USE_STM32L152_EVAL
  #include "stm32l152_eval.h"
#endif

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_HalfPage
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#if defined (STM32L1XX_HD)
#define CODE_IN_SRAM
/* #define CODE_IN_FLASH_BANK1 */
/* #define CODE_IN_FLASH_BANK2 */
#endif /* STM32L1XX_HD */

#define FLASH_PAGE_SIZE        0x100

#if defined (STM32L1XX_MD)
  #define FLASH_START_ADDR                0x08004000
  #define FLASH_END_ADDR                  0x08006000
#elif defined (STM32L1XX_HD)
  #if defined (CODE_IN_SRAM)
    #define FLASH_START_ADDR              0x08014000
    #define FLASH_END_ADDR                0x08036000
  #elif defined (CODE_IN_FLASH_BANK1)
    #define FLASH_START_ADDR              0x08034000
    #define FLASH_END_ADDR                0x08036000
  #elif defined (CODE_IN_FLASH_BANK2)
    #define FLASH_START_ADDR              0x08004000
    #define FLASH_END_ADDR                0x08006000
  #endif /* CODE_IN_SRAM */
#endif /* STM32L1XX_MD */

#if !defined (FLASH_START_ADDR) && !defined (FLASH_END_ADDR)
  #error "Please select first the FLASH Start and End addresses in main.c file..."
#endif 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO TestStatus MemoryProgramStatus = PASSED;
uint32_t NbrOfPage = 0, j = 0, Address = 0;
uint32_t Data[32] = {0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,
                     0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,
                     0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,
                     0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,0x11223344,
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

  Address = FLASH_START_ADDR;
  NbrOfPage = ((FLASH_END_ADDR - Address) + 1 ) >> 8; 
  
  /* Erase the FLASH Program memory pages */
  for(j = 0; j < NbrOfPage; j++)
  {
    FLASHStatus = FLASH_ErasePage(Address + (FLASH_PAGE_SIZE * j));

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
   
  Address = FLASH_START_ADDR;
  
  /* Write the FLASH Program memory using HalfPage operation */
  while(Address <= FLASH_END_ADDR)
  {
    FLASHStatus = FLASH_ProgramHalfPage(Address, Data);

    if(FLASHStatus == FLASH_COMPLETE)
    {
      Address = Address + 128;
    }
    else
    {
      FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                      | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR | FLASH_FLAG_OPTVERRUSR);
    } 
  }
  
  Address = FLASH_START_ADDR;
  
  /* Check the written data */
  while(Address < FLASH_END_ADDR)
  {
    if(*(__IO uint32_t*)Address != Data[0])
    {
      MemoryProgramStatus = FAILED;
    }
    Address = Address + 4;
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
