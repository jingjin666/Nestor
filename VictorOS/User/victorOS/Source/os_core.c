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
	}
}

void OSInit(OS_ERR *p_err)
{
	// ����ϵͳ��ʼ״̬Ϊֹ̬ͣ 
	OSRunning = OS_STATE_OS_STOPPED;

	//��ʼ������ȫ��TCB��������TCB���������л�
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;

	//��ʼ����������б�
	OS_RdyListInit();

	//��ʼ����������
	OS_IdleTaskInit(p_err);

	if(*p_err != OS_ERR_NONE) {
		return;
	}
}

void OSStart(OS_ERR *p_err)
{
	if(OSRunning == (OS_STATE)OS_STATE_OS_STOPPED) {
		OSTCBHighRdyPtr = OSRdyList[0].headPtr;
		OSStartHighRdy();
		*p_err = (OS_ERR)OS_ERR_FATAL_RETURN;
	} else {
		*p_err = (OS_ERR)OS_STATE_OS_RUNNING;
	}
}

void OS_IdleTaskInit(OS_ERR *p_err)
{
	//�������������
	OSIdleTaskCtr = 0;

	//������������
	OSTask_Create((OS_TCB *)&OSIdleTaskTCB,
					(OS_TASK_PTR)OS_IdleTask,
					(void *)0,
					(CPU_STK *)OSCfg_IdleTaskStkBasePtr,
					(CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
					(OS_ERR *)p_err);
}
