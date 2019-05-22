#include "os.h"

/* 任务优先级列表 */
CPU_DATA OSPrioTbl[OS_PRIO_TBL_SIZE];

void OS_PrioInit(void)
{
	CPU_DATA i;
	
	for(i=0; i<OS_PRIO_TBL_SIZE; i++) {
		OSPrioTbl[i] = (CPU_DATA)0;
	}
}

OS_ERR OS_PrioInsert(OS_PRIO prio)
{
	CPU_DATA bit;
	CPU_DATA bit_nbr;
	OS_PRIO ix;

	if(prio >= OS_CFG_PRIO_MAX) {
		return OS_ERR_A;
	}
	
	ix = prio / DEF_INT_CPU_NBR_BITS;
	bit_nbr = prio % DEF_INT_CPU_NBR_BITS;
	bit = 1 << ((DEF_INT_CPU_NBR_BITS-1) - bit_nbr);

	OSPrioTbl[ix] |= bit; 

	//printf("%s::OSPrioTbl[%d] is 0x%08x\r\n", __FUNCTION__, ix, OSPrioTbl[ix]);
	return OS_ERR_NONE;
}

OS_ERR OS_PrioRemove(OS_PRIO prio)
{	

	CPU_DATA bit;
	CPU_DATA bit_nbr;
	OS_PRIO ix;

	if(prio >= OS_CFG_PRIO_MAX) {
		return OS_ERR_A;
	}

	ix = prio / DEF_INT_CPU_NBR_BITS;
	bit_nbr = prio % DEF_INT_CPU_NBR_BITS;
	bit = 1 << ((DEF_INT_CPU_NBR_BITS-1) - bit_nbr);
		
	OSPrioTbl[ix] &= ~bit;

	//printf("%s::OSPrioTbl[%d] is 0x%08x\r\n", __FUNCTION__, ix, OSPrioTbl[ix]);
	return OS_ERR_NONE;
}

OS_PRIO OS_PrioGetHighest(void)
{
	OS_PRIO prio = (OS_PRIO)0;

	for(int j=0; j<OS_PRIO_TBL_SIZE; j++) {
		if(OSPrioTbl[j] == 0) {
			prio += DEF_INT_CPU_NBR_BITS;
		} else {
#ifdef ASM_CLZ		
			prio += (OS_PRIO)CPU_CntLeadZeros(OSPrioTbl[j]);
			return prio;
#else
			for(int i=DEF_INT_CPU_NBR_BITS-1; i>=0; i--) {
				if(OSPrioTbl[j] & (1<<i)) {
					prio += (OS_PRIO)((DEF_INT_CPU_NBR_BITS-1)-i);
					return prio;
				}
			}
#endif			
		}
	}

	return prio;
}
