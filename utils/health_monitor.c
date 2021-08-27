#include "health_monitor.h"

#include <stdbool.h>
#include <string.h>

#include "log.h"

HealthMon_t HealthMon;

bool health_monotor_init(void) {
    memset(&HealthMon, 0x00, sizeof(HealthMon));
    return true;
}

bool try_init(bool status, char* message) {
    if(false == status) {
        LOG_ERROR(SYS, "init %s error", message);
    } else {
        LOG_INFO(SYS, "init %s OK", message);
    }
    return status;
}
