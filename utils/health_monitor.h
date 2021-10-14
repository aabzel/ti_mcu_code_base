#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"

#define HEAL_MON_PERIOD_US (S_2_US(2))
#define KL30_UNDERVOL_THRESHOLD_V 10.0f

typedef struct xHealthMon_t {
    bool init_error;
    uint32_t error_cnt;
    uint32_t crc_error_cnt;
} HealthMon_t;

extern HealthMon_t HealthMon;

bool health_monotor_init(void);
bool try_init(bool staus, char* message);
bool health_monotor_proc(void);

#ifdef __cplusplus
}
#endif

#endif /* HEALTH_MONITOR_H */
