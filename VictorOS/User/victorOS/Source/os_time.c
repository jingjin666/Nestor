#include "os.h"

extern void OSSched(void);

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
