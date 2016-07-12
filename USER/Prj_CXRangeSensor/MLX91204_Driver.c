#include <Include.h>
#include <..\USER\Prj_CXRangeSensor\MLX91204_Driver.h>
#include <..\USER\Prj_CXRangeSensor\ADC_Driver.h>

MLX_ValueStuct MLX_Value;
float MLX_ConverterAngle()
{
	float Bx,Rx;
		float Temp;
	Bx=(float)ADC_Buffer[0];
	Bx-=(float)MLX_Value.CenValue;
	Rx=(float)ADC_Buffer[2];
	Rx-=(float)MLX_Value.CenValue1;
	Temp=atan((float)Bx/(float)Rx);
	//Temp*=350;
	return Temp;
}
void MLX_ClearMaxMin()
{
	MLX_Value.MaxValue=0;
	MLX_Value.MinValue=32767;
	MLX_Value.MaxValue1=0;
	MLX_Value.MinValue1=32767;
}
void MLX_GetMaxMin(uint16 Data,uint16 Data1)
{
	if(Data>MLX_Value.MaxValue)MLX_Value.MaxValue=Data;
	if(Data<MLX_Value.MinValue)MLX_Value.MinValue=Data;
	MLX_Value.CenValue=(MLX_Value.MaxValue+MLX_Value.MinValue)/2;
	if(Data1>MLX_Value.MaxValue1)MLX_Value.MaxValue1=Data1;
	if(Data1<MLX_Value.MinValue1)MLX_Value.MinValue1=Data1;
	MLX_Value.CenValue1=(MLX_Value.MaxValue1+MLX_Value.MinValue1)/2;
}

