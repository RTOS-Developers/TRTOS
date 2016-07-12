#ifndef _SCA100T_H_
#define _SCA100T_H_
#include  <Include.h>

#define MEAS 0x00 //测量模式
#define RWTR 0x08 //读写温度数据寄存器
#define STX  0x0E //x通道自检
#define STY  0x0F //Y通道自检
#define RDAX 0x10 //读Ｘ通道加速度值
#define RDAY 0x11 //读Ｙ通道加速度值
#define RDSR 0x0a //读取状态寄存器

#define SCACLK_L    GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define SCACLK_H    GPIO_SetBits(GPIOB,GPIO_Pin_7) 

#define SCAOUT_L    GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define SCAOUT_H    GPIO_SetBits(GPIOB,GPIO_Pin_6) 
#define SCAOUT_D    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)

#define SCAIN_L     GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define SCAIN_H     GPIO_SetBits(GPIOB,GPIO_Pin_5) 

#define SCACS_L     GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define SCACS_H     GPIO_SetBits(GPIOB,GPIO_Pin_4) 


extern uint16 AngleXY[2];
void GPIO_SCA100T_Init(void);
uint16 SPI_GetX(void);
uint16 SPI_GetY( void );
void SCA_Init( void );
uint16 SPI_GetReg(uint8 Cmd,uint8 Length);
void Task_GetAngle(void *Tags);
#endif
