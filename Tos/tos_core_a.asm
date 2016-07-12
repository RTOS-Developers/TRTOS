
    INCLUDE tos_core_a.h


    ;堆栈8字节对齐
    PRESERVE8

    AREA    TOS_CORE_A, CODE, READONLY
    THUMB


    ;函数功能: 实现任务切换功能, 将寄存器备份到当前任务堆栈中, 从将要运行任务堆栈中
    ;          取出寄存器保存值并跳转到将要运行任务执行.
    ;入口参数: none.
    ;返 回 值: none.
TOS_ContextSwitch

    ;保存当前任务的堆栈信息
    SUB    R13, #0x8;堆栈指针向
    PUSH   {R0 - R12}
    MRS    R0, XPSR;加载特殊功能寄存器
    PUSH   {R0};压入特殊功能寄存器的值
    MOV    R0, R13;
    ADD    R0, #0x40
    STMDB  R0, {R0, R14};等同于PUSH

    ;保存当前任务的指针值
    LDR    R0, =gpuiCurTaskSpAddr
    LDR    R1, [R0]
    STR    R13, [R1]

    ;获取将要运行任务的指针
    LDR    R0, =guiNextTaskSp
    LDR    R13, [R0]

    ;获取将要运行任务的堆栈信息并运行新任务
    POP    {R0}
    MSR    XPSR, R0
    POP    {R0 - R12}
    ADD    R13, #4
	
    POP    {PC}

    ;函数功能: 实现从无操作系统状态切换到操作系统状态, 从Root任务堆栈中取出寄存器保
    ;          存值并跳转到Root任务执行.
    ;入口参数: none.
    ;返 回 值: none.
TOS_SwitchToTask

    ;获取将要运行任务的指针
    LDR    R0, =guiNextTaskSp;
    LDR    R13, [R0];跳转

    ;获取将要运行任务的堆栈信息并运行新任务
    POP    {R0}
    MSR    XPSR, R0;存储通用寄存器的值特殊寄存器中MODE_USR
    POP    {R0 - R12};恢复寄存器的值从堆栈
    ADD    R13, #4;跳过pstrTcb
    POP    {PC}  ;直接运行vfFuncPointer

    ALIGN
    END

