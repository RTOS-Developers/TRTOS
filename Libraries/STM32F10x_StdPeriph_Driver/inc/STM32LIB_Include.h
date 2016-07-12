#ifndef _STM32LIB_INCLUDE_H_
#define _STM32LIB_INCLUDE_H_

#include <Include.h>
#if defined(STM32L1XX_MD) ||defined(STM32L1XX_MD)
//#include <..\Libraries\STM32F10x_StdPeriph_Driver\inc\stm32f10x_include.h>
#elif defined(STM32F10X_MD)||defined(STM32F10X_MD)
#include <..\Libraries\STM32F10x_StdPeriph_Driver\inc\stm32f10x_include.h>
#endif

#endif
