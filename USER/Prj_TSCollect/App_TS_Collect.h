#ifndef _APP_TS_COLLECT_H_
#define _APP_TS_COLLECT_H_
#include <Include.h>
#define SENSOR_CH_SIZE    2
#define SENSOR_CTR_COUNT  14
#define SENSOR_CROSS_SIZE 2

typedef struct
{
	uint16 ADC;
	float  Sensor;
}SENSOR_CROSS_TYPE;

typedef struct 
{
	uint32 AllStatus;//0
	uint16 ADC_Value[SENSOR_CH_SIZE];//1
	float  SensorValue[SENSOR_CH_SIZE];//2
	uint32 CTR_DATABIT;//4
	uint32 SIG_DATABIT;//8
	uint16 AD6x[4];//12
	uint8 ADCChnenel;
	SENSOR_CROSS_TYPE CROSS_DATA[SENSOR_CROSS_SIZE][SENSOR_CH_SIZE];
}DevRun_ValueStruct;

extern DevRun_ValueStruct DevRun_Value;
void TaskReadCanToCom(void *Tags);

#endif

