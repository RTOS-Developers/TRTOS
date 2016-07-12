/***************************************************************************************************
	* Copyright (c) 2016, Biohermes
	* All rights reserved.
	* 文件名称： QuectelM26.h
	* 摘 要：    GSM模块
	* 当前版本： V1.0, 易建, 2016-01-05
	* 更改记录： 无
***************************************************************************************************/

#ifndef _QUECTELM26_H_
#define _QUECTELM26_H_

#include <Include.h>

#define     RX_BUFF_LEN             20                          //接收缓存数组长度
#define         M26_OK                  0                               //OK
#define         STRING_ERR              1                               //非预期返回字符串
#define         CONNECT_ERR             2                               //与模块连接错误
#define         QUERY_BAUD_ERR          3                               //波特率错误
#define         CLOSE_SOCKET_ERR        4
#define         CLOSE_CONTEXT_ERR       5
#define         SQ_ERR                  6                               //信号查询错误
#define         REG_ERR                 7                               //网络注册失败
#define         GREG_ERR                8                               //GPRS附着失败
#define         SET_CONTEXT_ERR         9
#define         ACT_CONTEXT_ERR         10
#define         QUERY_ACT_ERR           11
#define         SET_SOCKET_ERR          12
#define         ENTER_DATA_MODE_ERR     13
#define         QUERY_SOCKET_ERR        14
#define         RESET_ERR               15
#define         DEFAULT_ERR             16
#define         QUERY_PIN_ERR           17                              //无SIM卡，或SIM卡有PIN码
#define         BACK_TO_TRAN_ACCESS_MODE_ERR    18
#define         OVER_TIME               19

#define         M26_POWER_ON            GPRS_ON
#define         M26_POWER_OFF           GPRS_OFF

#define         M26_RESET_ON            PWKEY_OFF
#define         M26_RESET_OFF           PWKEY_ON


extern u8 QuectelM26_PowerUp(void);
extern u8 QuectelM26_PowerDown(void);
extern u8 QuectelM26_Init(void);                                //gsm模块初始化
extern u8 QuectelM26_ConnectNet(void);                          //gsm模块连接网络
extern u8 QuectelM26_CloseNet(void);                            //gsm关闭网络连接
extern u8 QuectelM26_CheckNet(void);                            //查询gsm网络是否已经连通
extern u8 QuectelM26_ResetModule(void);                         //模块复位
extern void QuectelM26_Demo(void);                              //M26测试Demo
uint8 QuectelM26_GetCSQ(void);
extern u16 g_u16OverTimeCnt;                                      //秒倒计时计数器，要求每秒自减1
extern u8 g_u8RxIndexW;                                         //串口接收计数器写计数器， 上限为RxDataBuff数组长度 RX_BUFF_LEN
extern u8 RxDataBuff[RX_BUFF_LEN];                              //串口接收BUFF


#endif
