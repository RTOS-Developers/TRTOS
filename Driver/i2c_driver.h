
#ifndef _TTS_IIC_H_
#define _TTS_IIC_H_

#include <include.h>

#define I2C_HOST_SDA_SET	0x01
#define I2C_HOST_SDA_CLR	0x02
#define I2C_HOST_SCL_SET	0x03
#define I2C_HOST_SCL_CLR	0x04
#define I2C_HOST_SDA_DAT	0x05

typedef uint8 (*I2C_HOST_IO_CONTROL)(uint8 ControlMode);


/*------------------------------------------------------------------------------
 Func: 启动I2C总线
------------------------------------------------------------------------------*/
void I2C_Start(I2C_HOST_IO_CONTROL I2C_IOCON);
/*------------------------------------------------------------------------------
 Func: 停止I2C总线控制
------------------------------------------------------------------------------*/
void I2C_Stop(I2C_HOST_IO_CONTROL I2C_IOCON);
/*------------------------------------------------------------------------------
 Func: 释放I2C总线控制
------------------------------------------------------------------------------*/
void I2C_Drop(I2C_HOST_IO_CONTROL I2C_IOCON);
/*------------------------------------------------------------------------------
 Func: 发送ACK信号
------------------------------------------------------------------------------*/
void I2C_SendAck(I2C_HOST_IO_CONTROL I2C_IOCON,uint8 Ack);
/*------------------------------------------------------------------------------
 Func: 向I2C总线写数据 
 Time: 2010-7-20
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 I2C_SendBytes(I2C_HOST_IO_CONTROL I2C_IOCON,uint8 *TxBuffer,uint8 TxLenth);
/*------------------------------------------------------------------------------
 Func: 从I2C总线读数据 
 Time: 2010-7-20
 Ver.: V1.0
 Note:
------------------------------------------------------------------------------*/
uint8 I2C_RecvBytes(I2C_HOST_IO_CONTROL I2C_IOCON,uint8 *RxBuffer,uint8 RxLenth);
/*------------------------------------------------------------------------------
 Func: 写I2C总线
 Time: 2010-7-20
 Ver.: V1.0
 Note: return 0/无响应   1/发送正常 
------------------------------------------------------------------------------*/
uint8 I2C_WriteDatas(I2C_HOST_IO_CONTROL I2C_IOCON,uint8 Addr,uint8 *Cmd,uint8 CmdLen,uint8 *TxBuffer,uint8 TxLenth);
/*------------------------------------------------------------------------------
 Func: 读I2C总线
 Time: 2010-7-20
 Ver.: V1.0
 Note: return 0/无响应  1/正常
------------------------------------------------------------------------------*/
uint8 I2C_ReadDatas(I2C_HOST_IO_CONTROL I2C_IOCON,uint8 Addr,uint8 *Cmd,uint8 CmdLen,uint8 *RxBuffer,uint8 RxLenth);

#endif
