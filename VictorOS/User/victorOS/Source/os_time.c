#include "os.h"

extern void OSSched(void);


void OSTimeDlyMS(CPU_INT32U ms)
{
	OSTimeDly(ms);
}

void OSTimeDly(OS_TICK dly)
{
	CPU_SR_ALLOC();

	//进入临界区
	CPU_CRITICAL_ENTER();
	
	OSTCBCurPtr->TaskDelayTicks = dly;

	//将任务从就绪表里移除
	OS_PrioRemove(OSTCBCurPtr->prio);
	
	//退出临界区
	CPU_CRITICAL_EXIT();

	//调度任务
	OSSched();
}
