#ifndef OS_H
#define OS_H
#include "os_cfg.h"
#include "os_type.h"
#include "os_cpu.h"
#include "lib_def.h"

#ifdef OS_GLOBALS
#define OS_EXT
#else
#define OS_EXT extern
#endif

#define OS_STATE_OS_STOPPED (OS_STATE)(0u)
#define OS_STATE_OS_RUNNING (OS_STATE)(1u)

typedef struct OS_TCB_t {
	CPU_STK *stkPtr;
	CPU_STK_SIZE stkSize;
	OS_TICK TaskDelayTicks;
} OS_TCB;

typedef void (*OS_TASK_PTR)(void *p_arg);

typedef enum OS_ERR_m {
	OS_ERR_NONE = 0u,

    OS_ERR_A                         = 10000u,
    OS_ERR_ACCEPT_ISR                = 10001u,

    OS_ERR_B                         = 11000u,

    OS_ERR_C                         = 12000u,
    OS_ERR_CREATE_ISR                = 12001u,

    OS_ERR_D                         = 13000u,
    OS_ERR_DEL_ISR                   = 13001u,

    OS_ERR_E                         = 14000u,

    OS_ERR_F                         = 15000u,
    OS_ERR_FATAL_RETURN              = 15001u,
} OS_ERR;

typedef struct OS_RDY_LIST_t {
	OS_TCB *headPtr;
	OS_TCB *tailPtr;
} OS_RDY_LIST;

/*任务就绪表*/
OS_EXT OS_RDY_LIST OSRdyList[OS_CFG_PRIO_MAX];

/*当前任务指针*/
OS_EXT OS_TCB *OSTCBCurPtr;
/*即将调度的任务指针*/
OS_EXT OS_TCB *OSTCBHighRdyPtr;

/*任务状态*/
OS_EXT OS_STATE OSRunning;

/* 空闲任务TCB */
OS_EXT OS_TCB OSIdleTaskTCB;

/* 空闲任务计数器 */
OS_EXT OS_IDLE_CTR OSIdleTaskCtr;

/* 空闲任务堆栈起始地址*/
extern CPU_STK *const OSCfg_IdleTaskStkBasePtr;

/* 空闲任务堆栈大小 */
extern CPU_STK_SIZE const OSCfg_IdleTaskStkSize;

void OSTimeTick(void);	
void OSTimeDly(OS_TICK dly);

void OS_RdyListInit(void);

void OSInit(OS_ERR *p_err);
void OSStart(OS_ERR *p_err);
void OSSched (void);

CPU_STK *OSTaskStkInit(OS_TASK_PTR p_task,
					void *p_arg,
					CPU_STK *p_stk_base,
					CPU_STK_SIZE stk_size);

void OSTask_Create(OS_TCB *p_tcb,
					OS_TASK_PTR p_task,
					void *p_arg,
					CPU_STK *p_stk_base,
					CPU_STK_SIZE stk_size,
					OS_ERR *p_err);

void OS_IdleTaskInit(OS_ERR *p_err);
#endif /* OS_H */
