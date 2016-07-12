#ifndef _WS2812_DRIVER_H_
#define _WS2812_DRIVER_H_
#include <Include.h>


typedef struct
{
	uint8 R;
	uint8 G;
	uint8 B;
}RGBColorStruct;

void WS2812_WriteDot(GPIO_TypeDef *GPIOx,uint16_t Port,uint32 *Color);
void WS2812_SendList(uint32 *D);

#endif
