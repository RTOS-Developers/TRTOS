#include <Define.h>
#include <Operation.h>
#include <math.h>
#include <stdlib.h>
/****************************************************************************
Func:校准算法
Date:2013/8/14
Vision:V1.0
Note:无符号整型
****************************************************************************/
uint16 Opera_WhithADC(uint16 m,uint16 moshu1,uint16 shuzhi1,uint16 moshu2,uint16 shuzhi2,uint16 xianzhi)
{
	 if(m<moshu1)
	 {
		 m=moshu1-m;
		 m=(uint32)m*(shuzhi2-shuzhi1)/(moshu2-moshu1);
		 m=shuzhi1-m;
		 if(m>shuzhi1)m=0;
		 return(m);
	 }
	 if(m>moshu2)
	 {
		 m=m-moshu2;
		 m=(uint32)m*(shuzhi2-shuzhi1)/(moshu2-moshu1);
		 m+=shuzhi2;
		 if(m>xianzhi)m=xianzhi;
		 return(m);
	 }
	 if((m>=moshu1)&&(m<=moshu2))
	 {
		 m=m-moshu1;
		 m=(uint32)m*(shuzhi2-shuzhi1)/(moshu2-moshu1);
		 m=m+shuzhi1;
		 if(m>xianzhi)m=xianzhi;
		 return(m);
	 } 
	 return 0;
}
/****************************************************************************
Func:校准算法
Date:2013/8/14
Vision:V1.0
Note:浮点
****************************************************************************/
float Opera_WhithADCFloat(float m,float moshu1,float shuzhi1,float moshu2,float shuzhi2,float xianzhi)
{
	 if(m<moshu1)
	 {
		 m=moshu1-m;
		 m=(double)m*(shuzhi2-shuzhi1)/(moshu2-moshu1);
		 m=shuzhi1-m;
		 if(m>shuzhi1)m=0;
		 return(m);
	 }
	 if(m>moshu2)
	 {
		 m=m-moshu2;
		 m=(double)m*(shuzhi2-shuzhi1)/(moshu2-moshu1);
		 m+=shuzhi2;
		 if(m>xianzhi)m=xianzhi;
		 return(m);
	 }
	 if((m>=moshu1)&&(m<=moshu2))
	 {
		 m=m-moshu1;
		 m=(double)m*(shuzhi2-shuzhi1)/(moshu2-moshu1);
		 m=m+shuzhi1;
		 if(m>xianzhi)m=xianzhi;
		 return(m);
	 } 
	 return 0;
}
/****************************************************************************
Date:2014-11-12
Func:生命游戏计算核心程序
Note:
****************************************************************************/
uint8 LP_LifePlan(uint8 *Buf,uint8 *OldBuf,uint8 Row,uint8 Rank)
{
	uint8 Life=0,T1,T2;
	char tyx1,tyx2,temp_xy=0,x,y;
	 for(tyx1=0;tyx1<Rank;tyx1++)
	   {
	   for(tyx2=0;tyx2<Row;tyx2++)
	   {
			 for(x=0;x<3;x++)for(y=0;y<3;y++)
			 {
				 T1=tyx1+x;if(T1<1)T1=8;if(T1>8)T1=1;
				 T2=tyx2+y;if(T2<1)T2=8;if(T2>8)T2=1;
				 if((x==1)&(y==1));
					else if(OldBuf[T1-1]&BIT(T2-1))temp_xy++;
			 }
	   if(temp_xy==3){Buf[tyx1]|=BIT(tyx2);Life++;}
	   if(temp_xy<2){Buf[tyx1]&=~BIT(tyx2);}
	   if(temp_xy>3){Buf[tyx1]&=~BIT(tyx2);}
	   temp_xy=0;
	   }
	   }
		 return Life;
}
/****************************************************************************
Date:2015-11-05
Func:傅里叶算法
Note:
****************************************************************************/
#define M_PI 3.1415926
#define PI M_PI /* pi to machine precision, defined in math.h */
#define TWOPI (2.0*PI)
void Fulye(double data[], int nn, int isign)
{
	int n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	double tempr, tempi;
	n = nn << 1;
	j = 1;
	for (i = 1; i < n; i += 2) 
	{
	if (j > i) 
		{
		tempr = data[j]; data[j] = data[i]; data[i] = tempr;
		tempr = data[j+1]; data[j+1] = data[i+1]; data[i+1] = tempr;
		}
	m = n >> 1;
	while (m >= 2 && j > m)
		{
		j -= m;
		m >>= 1;
		}
	j += m;
	}
	mmax = 2;
	while (n > mmax) 
		{
		istep = 2*mmax;
		theta = TWOPI/(isign*mmax);
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for (m = 1; m < mmax; m += 2) 
			{
			for (i = m; i <= n; i += istep)
				{
					j =i + mmax;
					tempr = wr*data[j] - wi*data[j+1];
					tempi = wr*data[j+1] + wi*data[j];
					data[j] = data[i] - tempr;
					data[j+1] = data[i+1] - tempi;
					data[i] += tempr;
					data[i+1] += tempi;
				}
				wr = (wtemp = wr)*wpr - wi*wpi + wr;
				wi = wi*wpr + wtemp*wpi + wi;
			}
	mmax = istep;
		}
}

