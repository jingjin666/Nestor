#ifndef OS_CPU_H
#define OS_CPU_H

/*
*******************************************************************
* �궨��
*******************************************************************
*/

#ifndef NVIC_INT_CTRL
/* �жϿ��Ƽ�״̬�Ĵ��� SCB_ICSR */
#define NVIC_INT_CTRL *((CPU_REG32 *)0xE000ED04)
#endif

#ifndef NVIC_PENDSVSET
/* ���� PendSV �쳣��ֵ Bit28��PENDSVSET */
#define NVIC_PENDSVSET 0x10000000
#endif

/* ���� PendSV �쳣 */
#define OS_TASK_SW() NVIC_INT_CTRL = NVIC_PENDSVSET
/* ���� PendSV �쳣 */
#define OSIntCtxSw() NVIC_INT_CTRL = NVIC_PENDSVSET

/* ϵͳ��ʱ�����ȼ�*/
#define  OS_CPU_CFG_SYSTICK_PRIO           0u

/*
*******************************************************************
* ��������
*******************************************************************
*/
void OSStartHighRdy(void); /* �� os_cpu_a.s ��ʵ�� */
void OS_CPU_PendSVHandler(void); /* �� os_cpu_a.s ��ʵ�� */
void OS_CPU_SysTickInit(CPU_INT32U ms); /* �� os_cpu_c.s ��ʵ�� */
void OS_CPU_SysTickHandler(void); /* �� os_cpu_c.s ��ʵ�� */
#endif /* OS_CPU_H */
