#ifndef HEALTH_MONITOR_H
#define HEALTH_MONITOR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xHealthMon_t {
    bool init_error;
    uint32_t error_cnt;
    uint32_t crc_error_cnt;
} HealthMon_t;

extern HealthMon_t HealthMon;

bool health_monotor_init(void);

#ifdef __cplusplus
}
#endif

#endif /* HEALTH_MONITOR_H */
