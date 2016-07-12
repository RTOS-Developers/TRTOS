/**
  ******************************************************************************
  * @file    SPI/SPI_MSD/main.c 
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
#include "stm32l152_eval.h"
#include "stm32l152_eval_spi_sd.h"

/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_MSD
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


/* Private define ------------------------------------------------------------*/
#define  BufferSize     512

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t Buffer_Block_Tx[BufferSize], Buffer_Block_Rx[BufferSize];
TestStatus TransferStatus1 = FAILED;
uint16_t Status = 0;

/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint8_t Offset);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

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
       
  /* Initialize Leds mounted on STM32L152-EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  
  /* Initializes the SD/SPI communication */
  Status = SD_Init();	
  
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BufferSize, 0x0);
  
  /* Write block of 512 bytes on address 0 */
  Status = SD_WriteBlock(Buffer_Block_Tx, 0, BufferSize);
  
  /* Read block of 512 bytes from address 0 */
  Status = SD_ReadBlock(Buffer_Block_Rx, 0, BufferSize);
  
  /* Check the corectness of written dada */
  TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BufferSize);
  
  if (TransferStatus1 == PASSED)
  {
    /* OK: Turn on LD1 */
    STM_EVAL_LEDOn(LED1);
  }
  else
  {
    /* Error: Turn on LD2 */
    STM_EVAL_LEDOn(LED2);
  }
  while (1)
  {
  }
}
/**
  * @brief  Fill the gloal buffer.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None.
  */
static void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint8_t Offset)
{
  uint16_t IndexTmp;
  
  /* Put in global buffer same values */
  for( IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] =IndexTmp + Offset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
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
