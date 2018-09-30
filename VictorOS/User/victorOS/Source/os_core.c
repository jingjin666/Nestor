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

//������ͷ������ڵ�
void OS_RdyListInsertHead(OS_TCB *p_tcb)
{
	OS_RDY_LIST *p_rdy_list;
	OS_TCB *p_tcb2;
	
	p_rdy_list = &OSRdyList[p_tcb->prio];
	
	if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {
		//��ǰ���ȼ��»�û������
		p_rdy_list->headPtr = p_tcb;
		p_rdy_list->tailPtr = p_tcb;
		p_rdy_list->NbrEntries++;;
		p_tcb->prevPtr = (OS_TCB *)0;
		p_tcb->nextPtr = (OS_TCB *)0;
	} else {
		//��ǰ���ȼ��Ѿ�������
		p_rdy_list->NbrEntries++;
		p_tcb->nextPtr = p_rdy_list->headPtr;
		p_tcb->prevPtr = (OS_TCB *)0;
		p_tcb2 = p_rdy_list->headPtr;
		p_tcb2->prevPtr = p_tcb;
		p_rdy_list->headPtr = p_tcb;
	}
}


//������β������ڵ�
void OS_RdyListInserTail(OS_TCB *p_tcb)
{
	OS_RDY_LIST *p_rdy_list;
	OS_TCB *p_tcb2;
	
	p_rdy_list = &OSRdyList[p_tcb->prio];
	
	if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {
		//��ǰ���ȼ��»�û������
		p_rdy_list->headPtr = p_tcb;
		p_rdy_list->tailPtr = p_tcb;
		p_rdy_list->NbrEntries++;;
		p_tcb->prevPtr = (OS_TCB *)0;
		p_tcb->nextPtr = (OS_TCB *)0;
	} else {
		//��ǰ���ȼ��Ѿ�������
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
		//��ǰ���ȼ���û������
		p_rdy_list->headPtr = (OS_TCB *)0;
		p_rdy_list->tailPtr= (OS_TCB *)0;
		p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
	} else {
		//��ǰ���ȼ���������
		if(p_rdy_list->NbrEntries == (OS_OBJ_QTY)1) {
			//ֻ��һ���ڵ�
			p_rdy_list->headPtr = (OS_TCB *)0;
			p_rdy_list->tailPtr= (OS_TCB *)0;
			p_rdy_list->NbrEntries--;
			OS_PrioRemove(p_tcb->prio);
		} else if(p_rdy_list->headPtr == p_tcb) {
			//ͷ���
			p_next_node->prevPtr = (OS_TCB *)0;
			p_rdy_list->headPtr = p_next_node;
			p_rdy_list->NbrEntries--;
		} else if(p_rdy_list->tailPtr == p_tcb) {
			//β�ڵ�
			p_prev_node->nextPtr = (OS_TCB *)0;
			p_rdy_list->tailPtr = p_prev_node;
			p_rdy_list->NbrEntries--;
		} else {
			//�����ڵ�
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
	// ����ϵͳ��ʼ״̬Ϊֹ̬ͣ 
	OSRunning = OS_STATE_OS_STOPPED;

	//��ʼ������ȫ��TCB��������TCB���������л�
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;

	//��ʼ�����ȼ�����
	OSPrioCur = (OS_PRIO)0;
	OSPrioHighRdy = (OS_PRIO)0;

	//��ʼ����������б�
	OS_RdyListInit();

	//��ʼ�����ȼ��б�
	OS_PrioInit();

	//��ʼ����������
	OS_IdleTaskInit(p_err);

	if(*p_err != OS_ERR_NONE) {
		return;
	}
}

void OSStart(OS_ERR *p_err)
{
	if(OSRunning == (OS_STATE)OS_STATE_OS_STOPPED) {
		//��ȡ������ȼ�
		OSPrioHighRdy = OS_PrioGetHighest();
		OSPrioCur = OSPrioHighRdy;

		//��ȡ������ȼ�����
		OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].headPtr;
		OSTCBCurPtr = OSTCBHighRdyPtr;

		//OS��ʼ����
		OSRunning = OS_STATE_OS_RUNNING;

		//���������л����˴����᷵��
		OSStartHighRdy();

		//���е�����������������Ĵ���
		*p_err = (OS_ERR)OS_ERR_FATAL_RETURN;
	} else {
		*p_err = (OS_ERR)OS_STATE_OS_RUNNING;
	}
}

void OS_IdleTaskInit(OS_ERR *p_err)
{
	//�������������
	OSIdleTaskCtr = (OS_IDLE_CTR)0;

	//������������
	OSTask_Create((OS_TCB *)&OSIdleTaskTCB,
					(OS_TASK_PTR)OS_IdleTask,
					(void *)0,
					(OS_PRIO)(OS_CFG_PRIO_MAX-1u),
					(CPU_STK *)OSCfg_IdleTaskStkBasePtr,
					(CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
					(OS_ERR *)p_err);
}
