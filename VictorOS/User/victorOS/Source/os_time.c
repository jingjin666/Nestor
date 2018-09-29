#include "os.h"

extern void OSSched(void);

void OSTimeDly(OS_TICK dly)
{
	OSTCBCurPtr->TaskDelayTicks = dly;
	OSSched();
}
