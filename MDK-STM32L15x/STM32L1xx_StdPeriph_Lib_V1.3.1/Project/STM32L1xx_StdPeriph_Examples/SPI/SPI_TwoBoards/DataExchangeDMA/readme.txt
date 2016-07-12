/**
  @page SPI_DataExchangeDMA SPI Communication Boards Data Exchange using DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    SPI/SPI_TwoBoards/DataExchangeDMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-April-2015
  * @brief   Description of the SPI Communication Boards Data Exchange using 
  *          DMA example.
  ******************************************************************************
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
   @endverbatim

@par Example Description 

This example provides a small application in which joystick buttons are used 
to trigger SPI communications using DMA and though using SPI firmware library.

- Hardware Description

To use this example, you need to load it on two STM32 boards (let's call them 
Board A and Board B) then connect these two boards through SPI lines and GND.
In the STM32 master board (Board A), connect the timer output capture compare 
channel into NSS line.  
In the firmware example uncomment the dedicated line in the main.h file to use
the SPI peripheral as STM32 Master device or as STM32 Slave.

@verbatim
*------------------------------------------------------------------------------*
|                BOARD A                                BOARD B                |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________ ____|_______SCK_______|____ __________     |         |
|        |    |SPI Master|____|_______MOSI______|____|SPI Slave |    |         |
|        |    |  Device  |____|_______MISO______|____|  Device  |    |         |
|        |    |__________|____|_______NSS_______|____|__________|    |         |
|        |                    |   |             |                    |         |
|        |  O LD1  TIM_CHx O__|___|             |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim

@note
- The connection between the pins should use a short wires and a common Ground. 

- Software Description

In master board, SPI peripheral is configured as Master full duplex with DMA
and NSS hardware mode.
The TIM2 is configured to generate 4KHz PWM signal with 50% duty cycle on TIM2_CH2 
pin (PA.01), this signal is used as DMA trigger and as NSS signal input to latch
the SPI data transfers. Whereas in Slave board, SPI peripheral is configured as 
Slave Full duplex with DMA and NSS hardware mode.

At each joystick buttons press:
- The Master sends the specific command to the Slave using the TIM2_CH2 DMA 
  request (DMA1_Channel7)(the command contains the transaction code (CMD_RIGHT, 
  CMD_LEFT, CMD_UP, CMD_DOWN or CMD_SEL) and receives the ACK command from the 
  Slave using SPI_Rx DMA request(DMA1_Channel2) 

- The Slave receives the command using SPI_Rx DMA request(DMA1_Channel2) and sends 
  the ACK command using the SPI_Tx DMA request (DMA1_Channel3).

- The Master sends the defined NumberOfByte from TxBuffer using the TIM2_CH2 DMA 
  request (DMA1_Channel7) to the Slave and receives NumberOfByte from the Slave 
  using SPI_Rx DMA request(DMA1_Channel2) into Rxbuffer.

- The Slave sends the defined NumberOfByte from Txbuffer using the SPI_Tx DMA 
  request (DMA1_Channel3) to the Master and receives NumberOfByte from the Master 
  using SPI_Rx DMA request(DMA1_Channel2) into Rxbuffer.

- The Master compares the NumberOfByte received data with the defined ones into 
  TxBuffer and check the ACK command, received data correctness is signaled by LD 
  lightening and though as follow:
   - Joystick JOY_RIGHT and data correctly received ==> LD2 and LD3 ON and LD4 OFF 
   - Joystick JOY_LEFT and data correctly received  ==> LD4 ON, LD2 and LD3 are OFF
   - Joystick JOY_UP and data correctly received    ==> LD2 ON, LD3 and LD4 are OFF
   - Joystick JOY_DOWN and data correctly received  ==> LD3 ON, LD2 and LD4 are OFF
   - Joystick JOY_SEL and data correctly received   ==> LD2, LD3 and LD4 leds ON

- The Slave compares the NumberOfByte received data with the defined ones into 
  TxBuffer and check the received command,Received data correctness is signaled 
  by LED lightening and though as follow:
   - Received command CMD_RIGHT and data correctly received ==> LD2 and LD3 ON and LD4 OFF 
   - Received command CMD_LEFT and data correctly received  ==> LD2 ON, LD3 and LD4 are OFF
   - Received command CMD_UP and data correctly received    ==> LD2 ON, LD3 and LD4 are OFF
   - Received command CMD_DOWN and data correctly received  ==> LD3 ON, LD2 and LD4 are OFF
   - Received command CMD_SEL and data correctly received   ==> LD2, LD3 and LD4 leds ON


In both boards, the SPI is configured to use the slave hardware management to 
manage the NSS pin.
You can refer to the Communication scope snapshot.    
@image html SPI_DataExchangeDMA.bmp "SPI DataExchange using DMA"
 
The SysTick is configured to generate interrupt each 10ms. A dedicated counter 
inside the SysTick ISR is used to toggle the LD1 each 100ms indicating that the 
firmware is running.
A defined communication timeout is insuring that the application will not remain 
stuck if the SPI communication is corrupted.
You can adjust this timeout through the USER_TIMEOUT  define inside main.h file
depending on CPU frequency and application conditions (interrupts routines, 
number of data to transfer, baudrate, CPU frequency...).

These operations can be repeated infinitely.

@par Directory contents 

  - SPI/SPI_TwoBoards/DataExchangeDMA/stm32l1xx_conf.h    Library Configuration file
  - SPI/SPI_TwoBoards/DataExchangeDMA/stm32l1xx_it.c      Interrupt handlers
  - SPI/SPI_TwoBoards/DataExchangeDMA/stm32l1xx_it.h      Interrupt handlers header file
  - SPI/SPI_TwoBoards/DataExchangeDMA/main.c              Main program
  - SPI/SPI_TwoBoards/DataExchangeDMA/main.h              Main program header file
  - SPI/SPI_TwoBoards/DataExchangeDMA/system_stm32l1xx.c  STM32L1xx system source file
  
@note The "system_stm32l1xx.c" is generated by an automatic clock configuration 
      system and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32L1xx_Clock_Configuration_V1.1.0.xls" 
      provided with the AN3309 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a>
         
@par Hardware and Software environment

  - This example runs on STM32L1xx Ultra Low Power High-, Medium-Density and Medium-Density Plus Devices.
  
  - This example has been tested with STMicroelectronics STM32L152D-EVAL (STM32L1xx 
    Ultra Low Power High-Density) and STM32L152-EVAL (STM32L1xx Ultra Low 
    Power Medium-Density) evaluation board and can be easily tailored to any 
    other supported device and development board.

  - STM32L152-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.00, PD.01, PD.04 
      and PD.05 pins for each eval boards.
    - Use The Joystick push buttons 
       - DOWN[PE.10]
       - UP[PE.09]
       - LEFT[PE.12]
       - RIGHT[PE.11] 
       - SEL[PE.08]
    - Connect SPI1 SCK pin (PE.13) to SPI1 SCK pin (PE.13)
    - Connect SPI1 MISO pin (PA.06) to SPI1 MISO pin (PA.06)
    - Connect SPI1 MOSI pin (PA.07) to SPI1 MOSI pin (PA.07)
    - Connect SPI1 NSS pin (PA.04) to SPI1 NSS pin (PA.04)
    - Connect TIM2_CH2 pin (PA.01) to SPI1 NSS pin (PA.04) (On master board only to trigger the DMA transfers)
    - Plug the jumper JP16 to avoid the audio output on SPI1 NSS pin (PA.04). 

  - STM32L152D-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively PD.07, PG.14, PG.15 
      and PD.05 pins for each eval boards.
    - Use The Joystick push buttons 
       - DOWN[PG.08]
       - UP[PG.11]
       - LEFT[PG.06]
       - RIGHT[PG.07] 
       - SEL[PG.13]
    - Connect SPI1 SCK pin (PE.13) to SPI1 SCK pin (PE.13)
    - Connect SPI1 MISO pin (PA.06) to SPI1 MISO pin (PA.06)
    - Connect SPI1 MOSI pin (PA.07) to SPI1 MOSI pin (PA.07)
    - Connect SPI1 NSS pin (PA.04) to SPI1 NSS pin (PA.04)
    - Connect TIM2_CH2 pin (PA.01) to SPI1 NSS pin (PA.04) (On master board only to trigger the DMA transfers)


@par How to use it ? 

In order to make the program work, you must do the following:
 - Copy all source files from this example folder to the template folder under
   Project\STM32L1xx_StdPeriph_Templates
 - Open your preferred toolchain
 - If the used device is STM32L1XX_MD choose "STM32L1XX_MD(STM32L1xxxBxx)" workspace
    - Add the following files to the project source list
       - Utilities\STM32_EVAL\STM32L152_EVAL\stm32l152_eval.c
 - If the used device is STM32L1XX_HD choose "STM32L1XX_HD(STM32L1xxxDxx)" workspace
    - Add the following files to the project source list
       - Utilities\STM32_EVAL\STM32L152D_EVAL\stm32l152d_eval.c 
 - Rebuild all files and load your image into target memory
 - Run the example

@note
- Ultra Low Power Medium-density devices: - STM32L151x6xx, STM32L151x8xx, STM32L151xBxx, STM32L152x6xx,
                                            STM32L152x8xx, STM32L152xBxx, STM32L151x6xxA, STM32L151x8xxA,
                                            STM32L151xBxxA, STM32L152x6xxA, STM32L152x8xxA and STM32L152xBxxA
                                          - STM32L100x6xx, STM32L100x8xx and STM32L100xBxx
- Ultra Low Power Medium-density Plus devices: - STM32L151xCxx, STM32L152xCxx and STM32L162xCxx 
                                               - STM32L100xCxx
- Ultra Low Power High-density devices: STM32L151xDxx, STM32L152xDxx and STM32L162xDxx
- Ultra Low Power XL-density devices: STM32L151xExx, STM32L152xExx and STM32L162xExx
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */


