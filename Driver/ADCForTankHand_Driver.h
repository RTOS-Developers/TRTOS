#ifndef _ADCFORTANKHAND_DRIVER_H_
#define _ADCFORTANKHAND_DRIVER_H_
#include <Include.h>

#define TANKHAND_ADC_CHN 3
#define TANKHAND_ADC_BUFSize  10 
extern volatile uint16  TANKHAND_ADC_Value[TANKHAND_ADC_BUFSize][TANKHAND_ADC_CHN]; 
void ADCForTankHand_Init(void);
uint16 ADCForTankHand_ReadFitel(uint8 CH);
void Task_ADCFitel(void *Tags);

#endif

