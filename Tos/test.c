
#include "test.h"

U8 gaucTask1Stack[TASKSTACK];       /* 任务1的堆栈 */
U8 gaucTask2Stack[TASKSTACK];       /* 任务2的堆栈 */
U8 gaucTask3Stack[TASKSTACK];       /* 任务3的堆栈 */

W_TCB* gpstrTask1Tcb;               /* 任务1的TCB指针 */
W_TCB* gpstrTask2Tcb;               /* 任务2的TCB指针 */
W_TCB* gpstrTask3Tcb;               /* 任务3的TCB指针 */



/***********************************************************************************
函数功能: 测试任务切换函数1, 向串口打印字符串.
入口参数: none.
返 回 值: none.
***********************************************************************************/
void TEST_TestTask1(void)
{
    while(1)
    {
        DEV_PutString((U8*)"\r\nTask1 is running!");

        DEV_DelayMs(1000);              /* 延迟1s */

        WLX_TaskSwitch(gpstrTask3Tcb);  /* 任务切换 */
    }
}
void bb(u8 a,u8 b)
{
	U8 * str="nihao whfh";
	WLX_TaskSwitch(gpstrTask1Tcb);
	DEV_PutString(str);
	WLX_TaskSwitch(gpstrTask3Tcb);
	DEV_PutString(str);
}
void aa()
{
	
	
	bb(1,1);
	
}

/***********************************************************************************
函数功能: 测试任务切换函数2, 向串口打印字符串.
入口参数: none.
返 回 值: none.
***********************************************************************************/
void TEST_TestTask2(void)
{
    while(1)
    {
        

        DEV_DelayMs(2000);              /* 延迟2s */
aa();
  //      WLX_TaskSwitch(gpstrTask1Tcb);  /* 任务切换 */
    }
}

/***********************************************************************************
函数功能: 测试任务切换函数3, 向串口打印字符串.
入口参数: none.
返 回 值: none.
***********************************************************************************/
void TEST_TestTask3(void)
{
    while(1)
    {
        DEV_PutString((U8*)"\r\nTask3 is running!");

        DEV_DelayMs(3000);              /* 延迟3s */

        WLX_TaskSwitch(gpstrTask2Tcb);  /* 任务切换 */
    }
}

