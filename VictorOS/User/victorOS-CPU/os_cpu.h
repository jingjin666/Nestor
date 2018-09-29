#ifndef OS_CPU_H
#define OS_CPU_H

/*
*******************************************************************
* 宏定义
*******************************************************************
*/

#ifndef NVIC_INT_CTRL
/* 中断控制及状态寄存器 SCB_ICSR */
#define NVIC_INT_CTRL *((CPU_REG32 *)0xE000ED04)
#endif

#ifndef NVIC_PENDSVSET
/* 触发 PendSV 异常的值 Bit28：PENDSVSET */
#define NVIC_PENDSVSET 0x10000000
#endif

/* 触发 PendSV 异常 */
#define OS_TASK_SW() NVIC_INT_CTRL = NVIC_PENDSVSET
/* 触发 PendSV 异常 */
#define OSIntCtxSw() NVIC_INT_CTRL = NVIC_PENDSVSET

/* 系统定时器优先级*/
#define  OS_CPU_CFG_SYSTICK_PRIO           0u

/*
*******************************************************************
* 函数声明
*******************************************************************
*/
void OSStartHighRdy(void); /* 在 os_cpu_a.s 中实现 */
void OS_CPU_PendSVHandler(void); /* 在 os_cpu_a.s 中实现 */
void OS_CPU_SysTickInit(CPU_INT32U ms); /* 在 os_cpu_c.s 中实现 */
void OS_CPU_SysTickHandler(void); /* 在 os_cpu_c.s 中实现 */
#endif /* OS_CPU_H */
