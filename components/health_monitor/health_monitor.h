#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_CLOCK
#include "clocks.h"
#endif

#define HEAL_MON_PERIOD_US (SEC_2_US(4))
#define KL30_UNDERVOL_ERRPR_THRESHOLD_V 4.0f
#define KL30_UNDERVOL_WARNING_THRESHOLD_V 8.0f

typedef struct xHealthMon_t {
    bool init_error;
    bool power;
    uint32_t error_cnt;
    uint32_t crc_error_cnt;
} HealthMon_t;

extern HealthMon_t HealthMon;

bool health_monotor_init(void);
bool health_monotor_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* HEALTH_MONITOR_H */
