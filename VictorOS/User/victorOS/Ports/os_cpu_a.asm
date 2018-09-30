;*******************************************************************
; 全局变量&函数
;*******************************************************************
	IMPORT OSTCBCurPtr ; 外部文件引人的参考
	IMPORT OSTCBHighRdyPtr
	IMPORT OSPrioCur
	IMPORT OSPrioHighRdy

	EXPORT OSStartHighRdy ; 该文件定义的函数
	EXPORT OS_CPU_PendSVHandler
	EXPORT CPU_IntDis
	EXPORT CPU_IntEn
	EXPORT CPU_SR_Save
        EXPORT CPU_SR_Restore
	EXPORT CPU_CntLeadZeros
	EXPORT CPU_CntTrailZeros

;*******************************************************************
;常量
;有关内核外设寄存器定义可参考官方文档：STM32F10xxx Cortex-M3 programming manual
;系统控制块外设 SCB 地址范围：0xE000ED00-0xE000ED3F
;*******************************************************************
NVIC_INT_CTRL 	EQU 0xE000ED04 ; 中断控制及状态寄存器 SCB_ICSR。
NVIC_SYSPRI14 	EQU 0xE000ED22 ; 系统优先级寄存器 SCB_SHPR3：
							   ; bit16~23
NVIC_PENDSV_PRI EQU 0xFF 	   ; PendSV 优先级的值(最低)。
NVIC_PENDSVSET 	EQU 0x10000000 ; 触发 PendSV 异常的值 Bit28：PENDSVSET

;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************
    
	PRESERVE8
    	THUMB

    	AREA CODE, CODE, READONLY

;*******************************************************************
;                                    DISABLE and ENABLE INTERRUPTS
;
; Description: Disable/Enable interrupts.
;
; Prototypes : void  CPU_IntDis(void);
;              void  CPU_IntEn (void);
;*******************************************************************
CPU_IntDis
        CPSID   I
        BX      LR


CPU_IntEn
        CPSIE   I
        BX      LR

;********************************************************************************************************
;                                      CRITICAL SECTION FUNCTIONS
;
; Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
;               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
;               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
;               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
;
; Prototypes  : CPU_SR  CPU_SR_Save   (void);
;               void    CPU_SR_Restore(CPU_SR  cpu_sr);
;
; Note(s)     : (1) These functions are used in general like this :
;
;                       void  Task (void  *p_arg)
;                       {
;                           CPU_SR_ALLOC();                     /* Allocate storage for CPU status register */
;                               :
;                               :
;                           CPU_CRITICAL_ENTER();               /* cpu_sr = CPU_SR_Save();                  */
;                               :
;                               :
;                           CPU_CRITICAL_EXIT();                /* CPU_SR_Restore(cpu_sr);                  */
;                               :
;                       }
;********************************************************************************************************

CPU_SR_Save
        MRS     R0, PRIMASK                     ; Set prio int mask to mask all (except faults)
        CPSID   I
        BX      LR


CPU_SR_Restore                                  ; See Note #2.
        MSR     PRIMASK, R0
        BX      LR
        
;********************************************************************************************************
;                                          计算前导0函数
;
; 描述      ：
;
; 函数声明  ： CPU_DATA  CPU_CntLeadZeros(CPU_DATA  val);
;             
;********************************************************************************************************
CPU_CntLeadZeros
        CLZ     R0, R0                          ; Count leading zeros
        BX      LR

;********************************************************************************************************
;                                          计算后导0函数
;
; 描述      ：
;
; 函数声明  ： CPU_DATA  CPU_CntTrailZeros(CPU_DATA  val);
;             
;********************************************************************************************************

CPU_CntTrailZeros
        RBIT    R0, R0                          ; Reverse bits
        CLZ     R0, R0                          ; Count trailing zeros
        BX      LR
        
;*******************************************************************
;				开始一次上下文切换
;
;
;*******************************************************************
OSStartHighRdy
	LDR R0, = NVIC_SYSPRI14 ; 设置 PendSV 异常优先级为最低
	LDR R1, = NVIC_PENDSV_PRI
	STRB R1, [R0]

	MOVS R0, #0 ;设置 psp 的值为 0，开始第一次上下文切换
	MSR PSP, R0

	LDR R0, = NVIC_INT_CTRL ; 触发 PendSV 异常

	LDR R1, = NVIC_PENDSVSET
	STR R1, [R0]

	CPSIE I ; 使能总中断，NMI 和 HardFault 除外

OSStartHang
	B OSStartHang ; 程序应永远不会运行到这里

;***********************************************************************
; OS_CPU_PendSVHandler 异常
;***********************************************************************
OS_CPU_PendSVHandler
	; 关中断，NMI 和 HardFault 除外，防止上下文切换被中断
	CPSID I

	; 将 psp 的值加载到 R0
	MRS R0, PSP

	; 判断 R0，如果值为 0 则跳转到 OS_CPU_PendSVHandler_nosave
	; 进行第一次任务切换的时候，R0 肯定为 0
	CBZ R0, OS_CPU_PendSVHandler_nosave

	;-----------------------一、保存上文-----------------------------
	; 任务的切换，即把下一个要运行的任务的堆栈内容加载到 CPU 寄存器中
	;-------------------------------------------------------------- 
	; 在进入 PendSV 异常的时候，当前 CPU 的 xPSR，PC（任务入口地址），
	; R14，R12，R3，R2，R1，R0 会自动存储到当前任务堆栈，
	; 同时递减 PSP 的值，随便通过 代码：MRS R0, PSP 把 PSP 的值传给 R0

	; 手动存储 CPU 寄存器 R4-R11 的值到当前任务的堆栈
	STMDB R0!, {R4-R11}

	; 加载 OSTCBCurPtr 指针的地址到 R1，这里 LDR 属于伪指令
	LDR R1, = OSTCBCurPtr
	; 加载 OSTCBCurPtr 指针到 R1，这里 LDR 属于 ARM 指令
	LDR R1, [R1]
	;  存储 R0 的值到 OSTCBCurPtr->OSTCBStkPtr，这个时候 R0 存的是任务空闲栈的栈顶
	STR R0, [R1]

;-----------------------二、切换下文-----------------------------
; 实现 OSTCBCurPtr = OSTCBHighRdyPtr
; 把下一个要运行的任务的堆栈内容加载到 CPU 寄存器中
;--------------------------------------------------------------
OS_CPU_PendSVHandler_nosave
	; OSPrioCur = OSPrioHighRdy
	LDR R0, = OSPrioCur
	LDR R1, = OSPrioHighRdy
	LDRB R2, [R1]
	STRB R2, [R0]

	; 加载 OSTCBCurPtr 指针的地址到 R0，这里 LDR 属于伪指令
	LDR R0, = OSTCBCurPtr
	; 加载 OSTCBHighRdyPtr 指针的地址到 R1，这里 LDR 属于伪指令
	LDR R1, = OSTCBHighRdyPtr
	; 加载 OSTCBHighRdyPtr 指针到 R2，这里 LDR 属于 ARM 指令 
	LDR R2, [R1]
	; 存储 OSTCBHighRdyPtr 到 OSTCBCurPtr
	STR R2, [R0]

	; 加载 OSTCBHighRdyPtr 到 R0 
	LDR R0, [R2]
	; 加载需要手动保存的信息到 CPU 寄存器 R4-R11
	LDMIA R0!, {R4-R11}

	; 更新 PSP 的值，这个时候 PSP 指向下一个要执行的任务的堆栈的栈底
	;（这个栈底已经加上刚刚手动加载到 CPU 寄存器 R4-R11 的偏移） 
	MSR PSP, R0

	; 确保异常返回使用的堆栈指针是 PSP，即 LR 寄存器的位 2 要为 1
	ORR LR, LR, #0x04

	; 开中断
	CPSIE I

	; 异常返回，这个时候任务堆栈中的剩下内容将会自动加载到 xPSR，
	; PC（任务入口地址），R14，R12，R3，R2，R1，R0（任务的形参）
	; 同时 PSP 的值也将更新，即指向任务堆栈的栈顶。
	; 在 STM32 中，堆栈是由高地址向低地址生长的。
	BX LR 
	END
