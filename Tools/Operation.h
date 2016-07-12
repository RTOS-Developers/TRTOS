#ifndef _OPERATION_H_
#define _OPERATION_H_

#include <Define.h>

uint16 Opera_WhithADC(uint16 m,uint16 moshu1,uint16 shuzhi1,uint16 moshu2,uint16 shuzhi2,uint16 xianzhi);
float Opera_WhithADCFloat(float m,float moshu1,float shuzhi1,float moshu2,float shuzhi2,float xianzhi);
uint8 LP_LifePlan(uint8 *Buf,uint8 *OldBuf,uint8 Row,uint8 Rank);
void Fulye(double data[], int nn, int isign);
#endif
