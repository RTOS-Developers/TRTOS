#ifndef _PID_H_
#define _PID_H_
#include <include.h>

/*-------------------------------------------------------------------------
 Func: 控制类PID算法库
-------------------------------------------------------------------------*/
typedef struct 
{
    f16 Kp;                                     //比例项常系数
    f16 Ki;                                     //积分项常系数
    f16 Kd;                                     //微分项常系数    
    int16 Error;                                //当前输出误差
    int16 LError;                               //上次输出误差
    int16 LLError;                              //上上次输出误差
    int16 Output;                               //当前输出值
    int16 Outset;                               //输出设定值
    int16 MaxOutput;                            //输出值上限  
    int16 MinOutput;                            //输出值下限
}PID;
void PID_Create(PID *PID,f16 Kp,f16 Ki,f16 Kd,int16 MaxOut,int16 MinOut);
#endif
