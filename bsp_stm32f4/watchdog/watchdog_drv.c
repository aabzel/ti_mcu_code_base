#include "watchdog_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

bool watchdog_set(uint32_t period_ms, bool status) {
    bool res = false;
    return res;
}

bool watchdog_init(void) {
    bool res = true;

    return res;
}

bool proc_watchdog(void) {
    bool res = false;
    if(WatchDog.is_on) {
        Watchdog_clear(watchdogHandle);
        res = true;
    }
    return res;
}
