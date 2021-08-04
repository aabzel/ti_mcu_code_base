#include "core_driver.h"

#include <string.h>
#include <sys_ctrl.h>

#include "bit_utils.h"
#include "read_mem.h"
#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif

uint32_t cpu_get_id(void){
    uint32_t value;
    value = read_addr_32bit(CPU_SCS_CPUID);

    return value;
}

bool reboot(void) {
    bool res = false;
    SysCtrlSystemReset();
#ifdef HAS_WDT
    res = watchdog_set(10, 0);
#endif
    return res;
}
