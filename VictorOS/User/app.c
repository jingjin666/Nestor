#include <stdio.h>

#include "cpu.h"
#include "os.h"
#include "bsp.h"
#include "stm32f4xx.h"

#define TASK1_STK_SIZE	128u
#define TASK2_STK_SIZE	128u
#define TASK3_STK_SIZE	128u

#define TASK1_PRIO	6u
#define TASK2_PRIO	12u
#define TASK3_PRIO	4u


static CPU_STK task1Stk[TASK1_STK_SIZE];
static CPU_STK task2Stk[TASK2_STK_SIZE];
static CPU_STK task3Stk[TASK2_STK_SIZE];


static OS_TCB task1TCB, task2TCB, task3TCB;

static void task1(void);
static void task2(void);
static void task3(void);


void welcomeVictorOS()
{
	printf("-------------------------------------------------------------------\r\n");
		printf("\r\n");
		//printf("*******************************************************************\r\n");
		printf(" __      __ _____  _____  _______  ____   _____    ____    _____ \r\n");
		printf(" \\ \\    / /|_   _|/ ____||__   __|/ __ \\ |  __ \\  / __ \\  / ____|\r\n");
		printf("  \\ \\  / /   | | | |        | |  | |  | || |__) || |  | || (___  \r\n");
		printf("   \\ \\/ /    | | | |        | |  | |  | ||  _  / | |  | | \\___ \\ \r\n");
		printf("    \\  /    _| |_| |____    | |  | |__| || | \\ \\ | |__| | ____) |\r\n");
		printf("     \\/    |_____|\\_____|   |_|   \\____/ |_|  \\_\\ \\____/ |_____/ \r\n");
		printf("*******************************************************************\r\n");
		printf("WELCOME TO VICTOROS                              SHARED BY EVERYONE\r\n");
	printf("-------------------------------------------------------------------\r\n");
}

static void delay_us(u32 nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
		//180MHz
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

static void delay_ms(u32 nTimer)
{
	u32 i=nTimer*1000;
	delay_us(i);
}

int main()
{
	OS_ERR p_err;

	CPU_IntDis();

	BSP_Init();
	
	welcomeVictorOS();

	OSInit(&p_err);
	
	OSTask_Create(&task1TCB,
			(OS_TASK_PTR)task1,
			0u,
			TASK1_PRIO,
			task1Stk,
			TASK1_STK_SIZE,
			&p_err);

	OSTask_Create(&task2TCB,
			(OS_TASK_PTR)task2,
			0u,
			TASK2_PRIO,
			task2Stk,
			TASK2_STK_SIZE,
			&p_err);

	OSTask_Create(&task3TCB,
		(OS_TASK_PTR)task3,
		0u,
		TASK3_PRIO,
		task3Stk,
		TASK3_STK_SIZE,
		&p_err);
	
	OSStart(&p_err);
}

void OSSched()
{
	//printf("systick\r\n");
#if 0
	//最简单的任务调度，两个任务交替执行
	if(OSTCBCurPtr == OSRdyList[0].headPtr) {
		OSTCBHighRdyPtr = OSRdyList[1].headPtr;
	} else {
		OSTCBHighRdyPtr = OSRdyList[0].headPtr;
	}
#endif

#if 0	
	if(OSTCBCurPtr == &OSIdleTaskTCB) {
		//如果当前任务是空闲任务，尝试执行任务就绪表里的任务
		//如果其中有任务延时到期，则执行其任务
		//否则继续执行空闲任务
		if(OSRdyList[0].headPtr->TaskDelayTicks == 0) {
			OSTCBHighRdyPtr = OSRdyList[0].headPtr;
		} else if(OSRdyList[1].headPtr->TaskDelayTicks == 0) {
			OSTCBHighRdyPtr = OSRdyList[1].headPtr;
		} else {
			return ;
		}
	} else {
		//如果当前任务不是空闲任务
		//首先判断任务就绪表里是否还有其他任务的延时是否到期
		//若到期则执行其他任务
		//若未到期则继续检查当前任务是否到期，若未到期则继续执行当前任务
		//否则执行空闲任务
		if(OSTCBCurPtr == OSRdyList[0].headPtr) {
			if(OSRdyList[1].headPtr->TaskDelayTicks == 0) {
				OSTCBHighRdyPtr = OSRdyList[1].headPtr;
			} else if(OSTCBCurPtr->TaskDelayTicks != 0) {
				OSTCBHighRdyPtr = &OSIdleTaskTCB;
			} else {
				return;
			}
		} else if(OSTCBCurPtr == OSRdyList[1].headPtr) {
			if(OSRdyList[0].headPtr->TaskDelayTicks == 0) {
				OSTCBHighRdyPtr = OSRdyList[0].headPtr;
			} else if(OSTCBCurPtr->TaskDelayTicks != 0) {
				OSTCBHighRdyPtr = &OSIdleTaskTCB;
			} else {
				return;
			}
		} else {
			
		}
	}
#endif

	CPU_SR_ALLOC();

	//进入临界区
	CPU_CRITICAL_ENTER();

	//查找最高优先级任务
	OSPrioHighRdy = OS_PrioGetHighest();
	OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].headPtr;

	//如果当前任务是最高优先级，则不进行任务切换
	if(OSTCBCurPtr == OSTCBHighRdyPtr) {
		//退出临界区
		CPU_CRITICAL_EXIT();
		return;
	}

	//退出临界区
	CPU_CRITICAL_EXIT();

	//任务切换
	OS_TASK_SW();
}

static void task1()
{
	for(;;) {
		printf("task1\r\n");
		OSTimeDlyMS(100);
	}
}

static void task2()
{
	for(;;) {
		printf("task2\r\n");
		OSTimeDlyMS(200);
	}
}

static void task3()
{
	for(;;) {
		printf("task3\r\n");
		OSTimeDlyMS(500);
	}
}

