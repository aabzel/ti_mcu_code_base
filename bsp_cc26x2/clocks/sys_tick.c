#include "sys_tick.h"

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#include "systick.h"

volatile uint32_t g_up_time_ms = 0;

#ifdef HAS_SYS_TIC_INT
void SysTickIntHandler(void){
    /*overflow after 49 days*/
    g_up_time_ms++;
}
#endif /*HAS_SYS_TIC_INT*/

bool SysTickInit(void) {
    g_up_time_ms = 0;
    SysTickDisable();
    SysTickPeriodSet(SYS_FREQ/1000); // 1mS interrupt timing

#ifdef HAS_SYS_TIC_INT
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();
#endif /*HAS_SYS_TIC_INT*/

    SysTickEnable();
	return false;
}
