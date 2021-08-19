#include "health_monitor.h"

#include <stdbool.h>
#include <string.h>

HealthMon_t HealthMon;

bool health_monotor_init(void) {
    memset(&HealthMon, 0x00, sizeof(HealthMon));
    return true;
}
