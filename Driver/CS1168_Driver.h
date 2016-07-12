#ifndef _CS1168_DRIVER_H_
#define _CS1168_DRIVER_H_
#include <include.h>



extern int16 CS1168_ADC;

void CS1168_GPIO_Init(void);
uint8 IniCS1168(void);
uint8 CS1168_ReceiveByte(void);
int16 CS1168_ReadADC(void);

#endif
