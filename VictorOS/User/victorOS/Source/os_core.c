#include "os.h"
#include "os_cpu.h"
#include "os_cfg_app.h"

void OS_RdyListInit(void)
{
	OS_PRIO i;
	OS_RDY_LIST *p_rdy_list;

	for(i = 0; i < OS_CFG_PRIO_MAX; i++) {
		p_rdy_list = &OSRdyList[i];
		p_rdy_list->headPtr = (OS_TCB *)0;
		p_rdy_list->tailPtr = (OS_TCB *)0;
		p_rdy_list->NbrEntries= (OS_OBJ_QTY)0;
	}
}

//在链表头部插入节点
void OS_RdyListInsertHead(OS_TCB *p_tcb)
{
	OS_RDY_LIST *p_rdy_list;
	OS_TCB *p_tcb2;
	
	p_rdy_list = &OSRdyList[p_tcb->prio];
	
	if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {
		//当前优先级下还没有任务
		p_rdy_list->headPtr = p_tcb;
		p_rdy_list->tailPtr = p_tcb;
		p_rdy_list->NbrEntries++;;
		p_tcb->prevPtr = (OS_TCB *)0;
		p_tcb->nextPtr = (OS_TCB *)0;
	} else {
		//当前优先级已经有任务
		p_rdy_list->NbrEntries++;
		p_tcb->nextPtr = p_rdy_list->headPtr;
		p_tcb->prevPtr = (OS_TCB *)0;
		p_tcb2 = p_rdy_list->headPtr;
		p_tcb2->prevPtr = p_tcb;
		p_rdy_list->headPtr = p_tcb;
	}
}


//在链表尾部插入节点
void OS_RdyListInserTail(OS_TCB *p_tcb)
{
	OS_RDY_LIST *p_rdy_list;
	OS_TCB *p_tcb2;
	
	p_rdy_list = &OSRdyList[p_tcb->prio];
	
	if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {
		//当前优先级下还没有任务
		p_rdy_list->headPtr = p_tcb;
		p_rdy_list->tailPtr = p_tcb;
		p_rdy_list->NbrEntries++;;
		p_tcb->prevPtr = (OS_TCB *)0;
		p_tcb->nextPtr = (OS_TCB *)0;
	} else {
		//当前优先级已经有任务
		p_rdy_list->NbrEntries++;
		p_tcb->nextPtr = (OS_TCB *)0;
		p_tcb->prevPtr = p_rdy_list->tailPtr;
		p_tcb2 = p_rdy_list->tailPtr;
		p_tcb2->nextPtr = p_tcb;
		p_rdy_list->tailPtr = p_tcb;
	}
}

void OS_RdyListInsert(OS_TCB *p_tcb)
{
	OS_PrioInsert(p_tcb->prio);

	if(p_tcb->prio == OSPrioCur) {
		OS_RdyListInserTail(p_tcb);
	} else {
		OS_RdyListInsertHead(p_tcb);
	}
}

void OS_RdyListRemove(OS_TCB *p_tcb)
{
	OS_RDY_LIST *p_rdy_list;

	OS_TCB *p_next_node, *p_prev_node;

	p_rdy_list = &OSRdyList[p_tcb->prio];

	p_next_node = p_tcb->nextPtr;
	p_prev_node = p_tcb->prevPtr;

	if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {
		//当前优先级下没有任务
		p_rdy_list->headPtr = (OS_TCB *)0;
		p_rdy_list->tailPtr= (OS_TCB *)0;
		p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
	} else {
		//当前优先级下有任务
		if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)1) {
			//只有一个节点
			p_rdy_list->headPtr = (OS_TCB *)0;
			p_rdy_list->tailPtr= (OS_TCB *)0;
			p_rdy_list->NbrEntries--;
			OS_PrioRemove(p_tcb->prio);
		} else if(p_rdy_list->headPtr == p_tcb) {
			//头结点
			p_next_node->prevPtr = (OS_TCB *)0;
			p_rdy_list->headPtr = p_next_node;
			p_rdy_list->NbrEntries--;
		} else if(p_rdy_list->tailPtr == p_tcb) {
			//尾节点
			p_prev_node->nextPtr = (OS_TCB *)0;
			p_rdy_list->tailPtr = p_prev_node;
			p_rdy_list->NbrEntries--;
		} else {
			//其他节点
			p_prev_node->nextPtr = p_tcb->nextPtr;
			p_next_node->prevPtr = p_tcb->prevPtr;
			p_rdy_list->NbrEntries--;
		}
	}

	p_tcb->nextPtr = (OS_TCB *)0;
	p_tcb->prevPtr = (OS_TCB *)0;
}

void OSInit(OS_ERR *p_err)
{
	// 配置系统初始状态为停止态 
	OSRunning = OS_STATE_OS_STOPPED;

	//初始化两个全局TCB，这两个TCB用于任务切换
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;

	//初始化优先级变量
	OSPrioCur = (OS_PRIO)0;
	OSPrioHighRdy = (OS_PRIO)0;

	//初始化任务就绪列表
	OS_RdyListInit();

	//初始化优先级列表
	OS_PrioInit();

	//初始化空闲任务
	OS_IdleTaskInit(p_err);

	if(*p_err != OS_ERR_NONE) {
		return;
	}
}

void OSStart(OS_ERR *p_err)
{
	if(OSRunning == (OS_STATE)OS_STATE_OS_STOPPED) {
		//获取最高优先级
		OSPrioHighRdy = OS_PrioGetHighest();
		OSPrioCur = OSPrioHighRdy;

		//获取最高优先级任务
		OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].headPtr;
		OSTCBCurPtr = OSTCBHighRdyPtr;

		//OS开始运行
		OSRunning = OS_STATE_OS_RUNNING;

		//启动任务切换，此处不会返回
		OSStartHighRdy();

		//运行到这里代表发生了致命的错误
		*p_err = (OS_ERR)OS_ERR_FATAL_RETURN;
	} else {
		*p_err = (OS_ERR)OS_STATE_OS_RUNNING;
	}
}

void OS_IdleTaskInit(OS_ERR *p_err)
{
	//清零任务计数器
	OSIdleTaskCtr = (OS_IDLE_CTR)0;

	//创建空闲任务
	OSTask_Create((OS_TCB *)&OSIdleTaskTCB,
					(OS_TASK_PTR)OS_IdleTask,
					(void *)0,
					(OS_PRIO)(OS_CFG_PRIO_MAX-1u),
					(CPU_STK *)OSCfg_IdleTaskStkBasePtr,
					(CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
					(OS_ERR *)p_err);
}
