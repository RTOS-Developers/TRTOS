#ifndef _APP_SENSOR_H_
#define _APP_SENSOR_H_
#include <Include.h>
#include <CS1180_Driver.h>
#include <AD7705_Driver.h>

#define SENSOR_CROSS_SIZE 2


extern char *String_Hello;
extern char *String_Ok_ADC;
extern char *String_Ok;


typedef struct
{
	int32 ADC;
	float  Sensor;
}SENSOR_CROSS_TYPE;

void Sensor_Command(void *Tags);

#endif
