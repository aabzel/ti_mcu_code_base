#include "health_monitor.h"

#include <stdbool.h>
#include <string.h>

#ifdef HAS_ADC
#include "adc_drv.h"
#endif
#include "log.h"
#include "sys_config.h"

HealthMon_t HealthMon;

bool health_monotor_init(void) {
    bool res = true;
    memset(&HealthMon, 0x00, sizeof(HealthMon));

    return res;
}

bool health_monotor_proc(void) {
    bool res = false;
#ifdef HAS_ADC
    float vKl30 = 0.0;
    vKl30 = adc_get_value_by_dio(DIO_KL30_ADC, true);
    if(vKl30 < KL30_UNDERVOL_THRESHOLD_V) {
        LOG_ERROR(HMOM, "vKl30 %f too low", vKl30);
        res = false;
    } else {
        res = true;
    }
#endif

    return res;
}
