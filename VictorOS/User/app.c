#include <stdio.h>

#include "cpu.h"
#include "os.h"
#include "bsp.h"
#include "stm32f4xx.h"

#define TASK1_STK_SIZE	128u
#define TASK2_STK_SIZE	128u

static CPU_STK task1Stk[TASK1_STK_SIZE];
static CPU_STK task2Stk[TASK2_STK_SIZE];

static OS_TCB task1TCB, task2TCB;

static void task1(void);
static void task2(void);

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

/* 软件延时，不必纠结具体的时间 */
void delay( uint32_t count )
{
	for (; count!=0; count--);
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
			task1Stk,
			TASK1_STK_SIZE,
			&p_err);

	OSTask_Create(&task2TCB,
			(OS_TASK_PTR)task2,
			0u,
			task2Stk,
			TASK2_STK_SIZE,
			&p_err);

	OSRdyList[0].headPtr = &task1TCB;
	OSRdyList[1].headPtr = &task2TCB;
	
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
	//任务切换
	OS_TASK_SW();
}

static void task1()
{
	for(;;) {
		printf("task1\r\n");

		//delay(20000000);

		//OSSched();

		OSTimeDly(100);
	}
}

static void task2()
{
	for(;;) {
		printf("task2\r\n");
		
		//delay(20000000);
		
		//OSSched();
		
		OSTimeDly(200);
	}
}
