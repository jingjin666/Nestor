#include "os.h"

extern void OSSched(void);


void OSTimeDlyMS(CPU_INT32U ms)
{
	OSTimeDly(ms);
}

void OSTimeDly(OS_TICK dly)
{
	CPU_SR_ALLOC();

	//�����ٽ���
	CPU_CRITICAL_ENTER();
	
	OSTCBCurPtr->TaskDelayTicks = dly;

	//������Ӿ��������Ƴ�
	OS_PrioRemove(OSTCBCurPtr->prio);
	
	//�˳��ٽ���
	CPU_CRITICAL_EXIT();

	//��������
	OSSched();
}
