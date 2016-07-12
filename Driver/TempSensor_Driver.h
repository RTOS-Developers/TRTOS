#ifndef _TEMPSENSOR_DRIVER_H_
#define _TEMPSENSOR_DRIVER_H_
#include <include.h>

extern __IO uint16 ADCTemp_ConvertedValue;
void Temp_ADC1_Init(uintbus Addr);

#endif
