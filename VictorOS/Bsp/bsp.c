#include "bsp.h"
#include "os_cpu.h"
#include "stm32f4xx.h"
#include "usart/bsp_debug_usart.h"

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

CPU_INT32U const  OSCfg_TickRate_Hz = 1000u;

void BSP_Tick_Init ()
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

	//运行一次需要1/180000000 秒
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
	// 1ms 产生一次中断
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}

void BSP_Init(void)
{
	BSP_Tick_Init();

	Debug_USART_Config();
}
