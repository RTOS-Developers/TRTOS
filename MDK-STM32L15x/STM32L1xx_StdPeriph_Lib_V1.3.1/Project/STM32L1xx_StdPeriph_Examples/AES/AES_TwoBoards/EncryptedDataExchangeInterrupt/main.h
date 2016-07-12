/**
  ******************************************************************************
  * @file    AES/AES_TwoBoards/EncryptedDataExchangeInterrupt/main.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#ifdef USE_STM32L152D_EVAL
#include "stm32l152d_eval.h"
#else
#include "stm32l152_eval.h"
#endif
  
/* Exported typedef -----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;  
/* Exported define ------------------------------------------------------------*/
/* Uncomment the line below if you will use the I2C peripheral as a Master*/
 #define I2C_MASTER 
/* Uncomment the line below if you will use the I2C peripheral as a Slave*/
/* #define I2C_SLAVE */  
  
/* To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral
   input clock) must be a multiple of 10 MHz */
/* Uncomment the line below if you will use the I2C peripheral in Fast Mode */
/* #define FAST_I2C_MODE */
  
/* Uncomment the line below if you will use the I2C peripheral in 10-bit addressing
   mode */
/* #define I2C_10BITS_ADDRESS */
  
/* Define I2C Speed --------------------------------------------------------*/
#ifdef FAST_I2C_MODE
 #define I2C_SPEED 340000
 #define I2C_DUTYCYCLE I2C_DutyCycle_16_9  
#else /* STANDARD_I2C_MODE*/
 #define I2C_SPEED 100000
 #define I2C_DUTYCYCLE  I2C_DutyCycle_2
#endif /* FAST_I2C_MODE*/
  
/* Define Slave Address  ---------------------------------------------------*/
#ifdef I2C_10BITS_ADDRESS
 #define SLAVE_ADDRESS (uint16_t)0x0330
#else /* I2C_7BITS_ADDRESS */
 #define SLAVE_ADDRESS 0x30
#endif /* I2C_10BITS_ADDRESS */
 
/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the I2C communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, speed, CPU
   frequency...). */ 
#define USER_TIMEOUT                  ((uint32_t)0x64) /* Waiting 1s */  
  
/* Master Mode Selected */
#define MASTER_MODE_TRANSMITTER       0x01
#define MASTER_MODE_RECEIVER          0x00 

/* I2Cx Communication boards Interface */
#define I2Cx                          I2C1
#define I2Cx_CLK                      RCC_APB1Periph_I2C1
#define I2Cx_EV_IRQn                  I2C1_EV_IRQn
#define I2Cx_ER_IRQn                  I2C1_ER_IRQn
#define I2Cx_EV_IRQHANDLER            I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHANDLER            I2C1_ER_IRQHandler


#define I2Cx_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define I2Cx_SDA_GPIO_PORT            GPIOB
#define I2Cx_SDA_AF                   GPIO_AF_I2C1
  
#define I2Cx_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define I2Cx_SCL_GPIO_PORT            GPIOB
#define I2Cx_SCL_AF                   GPIO_AF_I2C1

#ifdef USE_STM32L152D_EVAL
#define I2Cx_SDA_PIN                  GPIO_Pin_9
#define I2Cx_SDA_SOURCE               GPIO_PinSource9
#define I2Cx_SCL_PIN                  GPIO_Pin_8
#define I2Cx_SCL_SOURCE               GPIO_PinSource8
#else
#define I2Cx_SDA_PIN                  GPIO_Pin_7
#define I2Cx_SDA_SOURCE               GPIO_PinSource7
#define I2Cx_SCL_PIN                  GPIO_Pin_6
#define I2Cx_SCL_SOURCE               GPIO_PinSource6
#endif

#define TXBUFFERSIZE   (countof(TxBuffer))
#define RXBUFFERSIZE   TXBUFFERSIZE

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
  
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
