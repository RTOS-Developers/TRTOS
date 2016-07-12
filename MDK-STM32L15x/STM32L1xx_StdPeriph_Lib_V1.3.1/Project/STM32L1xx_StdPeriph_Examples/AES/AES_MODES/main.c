/**
  ******************************************************************************
  * @file    AES/AES_MODES/main.c 
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
#include "stm32l152d_eval.h"
#include "stm32l152d_eval_lcd.h"


/** @addtogroup STM32L1xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup AES_MODES
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AES_TEXT_SIZE    32

/* Define AES MODE  ----------------------------------------------------------*/
/* Uncomment the line below if you will use the ECB Mode */
/* #define AES_ECB */
/* Uncomment the line below if you will use the CBC Mode */
/* #define AES_CBC */ 
/* Uncomment the line below if you will use the CTR Mode */
/* #define AES_CTR */

#if !defined (AES_ECB) && !defined (AES_CBC) && !defined (AES_CTR)
 #error "Please select first the AES MODE"
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
AES_InitTypeDef AES_InitStructure;
AES_KeyInitTypeDef  AES_KeyInitStructure;
AES_IVInitTypeDef AES_IVInitStructure;

uint8_t PlainText[AES_TEXT_SIZE] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                                    0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
                                    0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
                                    0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

uint8_t ComputedPlainText[AES_TEXT_SIZE];

uint8_t ExpectedCipherText[AES_TEXT_SIZE] = {0x3a,0xd7,0x7b,0xb4,0x0d,0x7a,0x36,0x60,
                                             0xa8,0x9e,0xca,0xf3,0x24,0x66,0xef,0x97,
                                             0x3a,0xd7,0x7b,0xb4,0x0d,0x7a,0x36,0x60,
                                             0xa8,0x9e,0xca,0xf3,0x24,0x66,0xef,0x97};

uint8_t ExpectedCipherText_CBC[AES_TEXT_SIZE] = {0x76,0x49,0xab,0xac,0x81,0x19,0xb2,0x46,
                                                 0xce,0xe9,0x8e,0x9b,0x12,0xe9,0x19,0x7d,
                                                 0x4c,0xbb,0xc8,0x58,0x75,0x6b,0x35,0x81,
                                                 0x25,0x52,0x9e,0x96,0x98,0xa3,0x8f,0x44};

uint8_t ExpectedCipherText_CTR[AES_TEXT_SIZE] = {0x3b,0x3f,0xd9,0x2e,0xb7,0x2d,0xad,0x20,
                                                 0x33,0x34,0x49,0xf8,0xe8,0x3c,0xfb,0x4a,
                                                 0xc4,0xe0,0x30,0xac,0xa9,0xa3,0x0c,0x3c,
                                                 0x33,0x0c,0x35,0xf5,0x08,0x64,0xb4,0x75};

uint8_t CipherText[AES_TEXT_SIZE];

uint8_t EncryptionKey[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
                             0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

uint8_t InitVector[16] = {0x00, 0x01, 0x02, 0x03, 0x04,0x05, 0x06, 0x07, 
                          0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

/* Private function prototypes -----------------------------------------------*/
ErrorStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

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
       
  /* Initialize the LCD */ 
  STM32L152D_LCD_Init();
 
  /* Initialize LEDs available on STM32L15X-EVAL board ************************/
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Enable AES AHB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_AES, ENABLE);

  /*============================================================================
                              Encryption mode
   ===========================================================================*/

#if defined AES_ECB
  AES_ECB_Encrypt(EncryptionKey, PlainText, AES_TEXT_SIZE, CipherText);
#elif defined AES_CBC
  AES_CBC_Encrypt(EncryptionKey, InitVector, PlainText, AES_TEXT_SIZE, CipherText);
#else /* CTR Mode */
  AES_CTR_Encrypt(EncryptionKey, InitVector, PlainText, AES_TEXT_SIZE, CipherText);
#endif

  /* Clear the LCD */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Set the Back Color */
  LCD_SetBackColor(LCD_COLOR_BLUE);

  /* Read the CipherText and check content correctness */
#if defined AES_ECB
  if(Buffercmp(ExpectedCipherText, CipherText, AES_TEXT_SIZE) != ERROR)
#elif defined AES_CBC
  if(Buffercmp(ExpectedCipherText_CBC, CipherText, AES_TEXT_SIZE) != ERROR) 
#else /* CTR */
  if(Buffercmp(ExpectedCipherText_CTR, CipherText, AES_TEXT_SIZE) != ERROR)
#endif
  {
    /* OK */
    /* Turn on LED1 */
    STM_EVAL_LEDOn(LED1);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);
    LCD_DisplayStringLine(LCD_LINE_2, "      Success       ");
  }
  else
  { 
    /* KO */
    /* Turn on LED2 */
    STM_EVAL_LEDOn(LED2);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine(LCD_LINE_2, "       Failed       ");
  }

  LCD_DisplayStringLine(LCD_LINE_1, "     Encryption     ");
#if defined AES_ECB
    LCD_DisplayStringLine(LCD_LINE_0, "      ECB Mode      ");
#elif defined AES_CBC
    LCD_DisplayStringLine(LCD_LINE_0, "      CBC Mode      ");
#else /* CTR */
    LCD_DisplayStringLine(LCD_LINE_0, "      CTR Mode      ");
#endif

  /*============================================================================
                              Decryption mode
   ===========================================================================*/

#if defined AES_ECB
  AES_ECB_Decrypt(EncryptionKey, CipherText, AES_TEXT_SIZE, ComputedPlainText);
#elif defined AES_CBC
  AES_CBC_Decrypt(EncryptionKey, InitVector, CipherText, AES_TEXT_SIZE, ComputedPlainText);
#else /* CTR Mode */
  AES_CTR_Decrypt(EncryptionKey, InitVector, CipherText, AES_TEXT_SIZE, ComputedPlainText);
#endif
  
  /* Read the ComputedPlainText and check content correctness */
  if(Buffercmp(PlainText, ComputedPlainText, AES_TEXT_SIZE) != ERROR)
  {
    /* OK */
    /* Turn on LED3 */
    STM_EVAL_LEDOn(LED3);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_GREEN);    
    LCD_DisplayStringLine(LCD_LINE_6, "      Success       ");
  }
  else
  { 
    /* KO */
    /* Turn on LED4 */
    STM_EVAL_LEDOn(LED4);
    /* Set the Text Color */
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine(LCD_LINE_6, "       Failed       ");
  }

  LCD_DisplayStringLine(LCD_LINE_5, "     Decryption     ");
#if defined AES_ECB
    LCD_DisplayStringLine(LCD_LINE_4, "      ECB Mode      "); 
#elif defined AES_CBC
    LCD_DisplayStringLine(LCD_LINE_4, "      CBC Mode      "); 
#else /* CTR */
    LCD_DisplayStringLine(LCD_LINE_4, "      CTR Mode      ");
#endif

  while (1)
  {
  }
}

/**
  * @brief  Compares two 8-bit buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval - SUCCESS: pBuffer1 identical to pBuffer2
  *         - ERROR: pBuffer1 differs from pBuffer2
  */
ErrorStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return ERROR;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return SUCCESS;
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
