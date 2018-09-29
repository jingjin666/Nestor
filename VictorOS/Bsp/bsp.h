#ifndef BSP_H
#define BSP_H
#include "cpu.h"

CPU_INT32U  BSP_CPU_ClkFreq(void);
void BSP_Tick_Init (void);
void BSP_Init(void);
#endif /* BSP_H */
