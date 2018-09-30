#include "os.h"


CPU_STK *OSTaskStkInit(OS_TASK_PTR p_task,
					void *p_arg,
					CPU_STK *p_stk_base,
					CPU_STK_SIZE stk_size)
{
	CPU_STK *p_stk;

	p_stk = &p_stk_base[stk_size];

	/* �쳣����ʱ�Զ�����ļĴ���*/
	*--p_stk = (CPU_STK)0x01000000u; /* xPSR : bit24 ������1 */
	*--p_stk = (CPU_STK)p_task;		  /* R15 (PC) : ����ָ�� */
	*--p_stk = (CPU_STK)0x14141414u; /* R14 (LR) */
	*--p_stk = (CPU_STK)0x12121212u; /* R12 */
	*--p_stk = (CPU_STK)0x03030303u; /* R3 */
	*--p_stk = (CPU_STK)0x02020202u; /* R2 */
	*--p_stk = (CPU_STK)0x01010101u; /* R1 */
	*--p_stk = (CPU_STK)p_arg;		  /* R0 : �����β� */

	/* �쳣����ʱ���ֶ�����ļĴ��� */
	*--p_stk = (CPU_STK)0x11111111u; /* R11 */
	*--p_stk = (CPU_STK)0x10101010u; /* R10 */
	*--p_stk = (CPU_STK)0x09090909u; /* R9 */
	*--p_stk = (CPU_STK)0x08080808u; /* R8 */
	*--p_stk = (CPU_STK)0x07070707u; /* R7 */
	*--p_stk = (CPU_STK)0x06060606u; /* R6 */
	*--p_stk = (CPU_STK)0x05050505u; /* R5 */
	*--p_stk = (CPU_STK)0x04040404u; /* R4 */

	return p_stk;
}

void OS_CPU_SysTickInit(CPU_INT32U cnts)
{
	CPU_INT32U  prio;

	/* ��д SysTick �����ؼ���ֵ */
    CPU_REG_NVIC_ST_RELOAD = cnts - 1u;

	/* ���� SysTick �ж����ȼ� */
    prio = CPU_REG_NVIC_SHPRI3;
    prio &= DEF_BIT_FIELD(24, 0);
    prio |= DEF_BIT_MASK(OS_CPU_CFG_SYSTICK_PRIO, 24);

    CPU_REG_NVIC_SHPRI3    = prio;
	
	/* ʹ�� SysTick ��ʱ��Դ������������ */
    CPU_REG_NVIC_ST_CTRL  |= CPU_REG_NVIC_ST_CTRL_CLKSOURCE |
                             CPU_REG_NVIC_ST_CTRL_ENABLE;
	
	/* ʹ�� SysTick �Ķ�ʱ�ж� */
    CPU_REG_NVIC_ST_CTRL  |= CPU_REG_NVIC_ST_CTRL_TICKINT;
}

void OSTimeTick(void)
{
	unsigned int i = 0;
	CPU_SR_ALLOC();

	//�����ٽ���
	CPU_CRITICAL_ENTER();

	for(i=0; i<OS_CFG_PRIO_MAX; i++) {
		if(OSRdyList[i].headPtr->TaskDelayTicks > 0) {
			OSRdyList[i].headPtr->TaskDelayTicks--;
			if(OSRdyList[i].headPtr->TaskDelayTicks == 0) {
				OS_PrioInsert(i);
			}
		}
	}

	//�˳��ٽ���
	CPU_CRITICAL_EXIT();

	OSSched();
}

/* SysTick �жϷ����� */
void OS_CPU_SysTickHandler(void)
{
	OSTimeTick();
}

