#ifndef _APP_ANGLE_H_
#define _APP_ANGLE_H_
#include <Sensor.h>
#ifdef   SENSOR_TYPE_ANGLE

#define ANGLE_CROSS_SIZE  2

typedef struct
{
	int32 ADC;
	float  Angle;
}ANGLE_CROSS_TYPE;

void Angle_Command(void *Tags);

#endif
#endif
