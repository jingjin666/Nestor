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

/*���������*/
OS_EXT OS_RDY_LIST OSRdyList[OS_CFG_PRIO_MAX];

/*��ǰ����ָ��*/
OS_EXT OS_TCB *OSTCBCurPtr;
/*�������ȵ�����ָ��*/
OS_EXT OS_TCB *OSTCBHighRdyPtr;

/*����״̬*/
OS_EXT OS_STATE OSRunning;

/* ��������TCB */
OS_EXT OS_TCB OSIdleTaskTCB;

/* ������������� */
OS_EXT OS_IDLE_CTR OSIdleTaskCtr;

/* ���������ջ��ʼ��ַ*/
extern CPU_STK *const OSCfg_IdleTaskStkBasePtr;

/* ���������ջ��С */
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
