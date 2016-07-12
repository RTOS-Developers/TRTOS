/**
  ******************************************************************************
  * @file    SPI/SPI_TwoBoards/DataExchangeDMA/main.h
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
#elif defined USE_STM32L152_EVAL 
  #include "stm32l152_eval.h"
#endif 


/* Private typedef -----------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

/* Uncomment the line below if you will use the SPI peripheral as a Master */
#define SPI_MASTER 
/* Uncomment the line below if you will use the SPI peripheral as a Slave */
/* #define SPI_SLAVE */

/* USER_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* Communication boards SPIx Interface */
#define SPIx_DR_ADDRESS                  0x4001300C
#define SPIx_TX_DMA_CHANNEL              DMA1_Channel3
#define SPIx_TX_DMA_FLAG_TC              DMA1_FLAG_TC3
#define SPIx_TX_DMA_FLAG_GL              DMA1_FLAG_GL3
#define SPIx_RX_DMA_CHANNEL              DMA1_Channel2
#define SPIx_RX_DMA_FLAG_TC              DMA1_FLAG_TC2
#define SPIx_RX_DMA_FLAG_GL              DMA1_FLAG_GL2

#define DMAx_CLK                         RCC_AHBPeriph_DMA1

#define SPIx                             SPI1
#define SPIx_CLK                         RCC_APB2Periph_SPI1
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

#define SPIx_SCK_PIN                     GPIO_Pin_13
#define SPIx_SCK_GPIO_PORT               GPIOE
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOE
#define SPIx_SCK_SOURCE                  GPIO_PinSource13
#define SPIx_SCK_AF                      GPIO_AF_SPI1

#define SPIx_MISO_PIN                    GPIO_Pin_6
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MISO_SOURCE                 GPIO_PinSource6
#define SPIx_MISO_AF                     GPIO_AF_SPI1

#define SPIx_MOSI_PIN                    GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MOSI_SOURCE                 GPIO_PinSource7
#define SPIx_MOSI_AF                     GPIO_AF_SPI1

#define SPIx_NSS_PIN                     GPIO_Pin_4
#define SPIx_NSS_GPIO_PORT               GPIOA
#define SPIx_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_NSS_SOURCE                  GPIO_PinSource4
#define SPIx_NSS_AF                      GPIO_AF_SPI1

/* TIM Trigger Configuration */
#define TIMx                             TIM2
#define TIMx_CLK                         RCC_APB1Periph_TIM2
#define TIMx_DMA_CHANNEL                 TIM_DMA_CC2

#define TIMx_TRIGGER_PIN                 GPIO_Pin_1
#define TIMx_TRIGGER_GPIO_PORT           GPIOA
#define TIMx_TRIGGER_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define TIMx_TRIGGER_SOURCE              GPIO_PinSource1
#define TIMx_TRIGGER_AF                  GPIO_AF_TIM2
#define TIMx_CHANNEL_INIT                TIM_OC2Init
#define TIMx_CHANNEL_DMA_CHANNEL         DMA1_Channel7
#define TIMx_CHANNEL_DMA_FLAG_TC         DMA1_FLAG_TC7
#define TIMx_CHANNEL_DMA_FLAG_GL         DMA1_FLAG_GL7

#define TXBUFFERSIZE                     (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                     TXBUFFERSIZE

/* Joystick Commands */
#define CMD_RIGHT                        0x55
#define CMD_LEFT                         0xAA
#define CMD_UP                           0x33
#define CMD_DOWN                         0xCC
#define CMD_SEL                          0xFF 

#define CMD_ACK                          0x66 

/* Define numbers of bytes to transmit from TxBuffer */
#define CMD_RIGHT_SIZE                   0x01
#define CMD_LEFT_SIZE                    0x05
#define CMD_UP_SIZE                      0x14
#define CMD_DOWN_SIZE                    0x1E
#define CMD_SEL_SIZE                     TXBUFFERSIZE

/* TIM2 Autoreload and Capture Compare register values */
#define TIM_ARR                          ((uint16_t)0x1F3F)
#define TIM_CCR                          ((uint16_t)0x0FA0)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
