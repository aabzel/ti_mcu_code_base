#include "core_driver.h"

#include <string.h>

#include "bit_utils.h"
#include "data_utils.h"
#include "sys_config.h"
#include "sys.h"
#include "read_mem.h"
#ifdef HAS_WDT
#include "watchdog_drv.h"
#endif


uint32_t cpu_get_id(void){
    uint32_t value=0;
    return value;
}


bool reboot(void) {
    bool res = false;

#ifdef HAS_WDT
    res = watchdog_set(10, 0);
#endif
    return res;
}


bool is_ram_addr(uint32_t address) {
    bool res = false;
    if((RAM_START < address) && (address <= (RAM_START + RAM_SIZE))) {
        res = true;
    }
    return res;
}
