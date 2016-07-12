#ifndef _MLX91204_DRIVER_H_
#define _MLX91204_DRIVER_H_
#include <Include.h>

typedef struct
{
	uint16 MaxValue;
	uint16 MinValue;
	uint16 CenValue;
	uint16 MaxValue1;
	uint16 MinValue1;
	uint16 CenValue1;
}MLX_ValueStuct;

extern MLX_ValueStuct MLX_Value;
float MLX_ConverterAngle(void);
void MLX_ClearMaxMin(void);
void MLX_GetMaxMin(uint16 Data,uint16 Data1);

#endif

