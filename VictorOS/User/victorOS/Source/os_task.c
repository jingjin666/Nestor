#include "os.h"

void OSTaskInitTCB(OS_TCB *p_tcb)
{
	p_tcb->stkPtr = (CPU_STK *)0;
	p_tcb->stkSize = (CPU_STK_SIZE)0u;
	p_tcb->TaskDelayTicks = (OS_TICK)0u;
	p_tcb->prio = (OS_PRIO)OS_PRIO_INIT;
	p_tcb->nextPtr = (OS_TCB *)0;
	p_tcb->prevPtr = (OS_TCB *)0;
}

void OSTask_Create(OS_TCB *p_tcb,
					OS_TASK_PTR p_task,
					void *p_arg,
					OS_PRIO prio,
					CPU_STK *p_stk_base,
					CPU_STK_SIZE stk_size,
					OS_ERR *p_err)
{
	CPU_STK *p_sp;

	CPU_SR_ALLOC();
	
	OSTaskInitTCB(p_tcb);
	
	p_sp = OSTaskStkInit(p_task, p_arg, p_stk_base, stk_size);

	p_tcb->stkPtr = p_sp;
	p_tcb->stkSize = stk_size;

	p_tcb->prio = prio;

	//进入临界区
	CPU_CRITICAL_ENTER();
	
	OS_RdyListInsert(p_tcb);

	//退出临界区
	CPU_CRITICAL_EXIT();
	
	*p_err = OS_ERR_NONE;
}
