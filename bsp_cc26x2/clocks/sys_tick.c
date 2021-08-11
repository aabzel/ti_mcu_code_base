#include "sys_tick.h"

#include <stdbool.h>
#include <stdint.h>

#include "systick.h"

volatile uint32_t g_up_time_ms = 0;

#ifdef HAS_SYS_TIC_INT
void SysTickIntHandler(void){
    g_up_time_ms++;
}
#endif /*HAS_SYS_TIC_INT*/

bool SysTickInit(void) {
    SysTickDisable();
    SysTickPeriodSet(48000); // 1mS interrupt timing

#ifdef HAS_SYS_TIC_INT
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();
#endif /*HAS_SYS_TIC_INT*/

    SysTickEnable();
	return false;
}
