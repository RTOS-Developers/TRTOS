#ifndef _COBE_H_
#define _COBE_H_
#include <Include.h>


typedef struct
{
	uint16 X;
	uint16 Y;
	uint16 Z;
	uint16 D;
}Coordinate_3D;
extern const Coordinate_3D C3D_Default;
void TGui_3DCobeTest(void);
void Disply3D(float xpos,float ypos,float zpos);

#endif
