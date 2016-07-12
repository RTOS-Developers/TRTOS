
#include <Include.h>

extern const uint32 CpuIdCode[];
extern const uint8 HardWareVersion;
extern const uint8 SoftWareVersion; 
extern const uint8 SerialNumber[];
extern const uint8  FactoryName[];
extern const uint32 ProductTime;
extern uint8  SysDogState;
extern uint8  SysDogTimer0;
extern uint8  SysDogTimer1;
extern uint8  SysDogTimer2;
extern uint8  SysDogTimer3;
extern uint8  SysDogTimer4;
extern uint8  SysDogTimer5;

extern uint8 System_CheckCode(void);
extern void System_SoftDogUpdate(void);
extern uint8 System_GetRunState(void);


