#ifndef _APP_CAN_0_20mV_H_
#define _APP_CAN_0_20mV_H_

#include <Include.h>

#ifdef   SENSOR_TYPE_0_20MV_TO_CAN

#define WEIGHT_CROSS_SIZE  2
typedef struct
{
	int32 ADC;
	float  Weight;
}WEIGHT_CROSS_TYPE;

void CAN_0_20_Command(void *Tags);

#endif
#endif


