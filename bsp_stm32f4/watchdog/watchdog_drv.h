#ifndef WATCHDOG_DRV_H
#define WATCHDOG_DRV_H

#include <stdbool.h>
#include <stdint.h>

#define WDT_TIMEOUT_MS 10000
#define CONFIG_WATCHDOG_0 0
#define CONFIG_WATCHDOG_COUNT 1

typedef struct xWatchDog_t {
    bool is_on;
    uint32_t period_ms;
} WatchDog_t;

extern WatchDog_t WatchDog;

bool watchdog_init(void);
bool watchdog_set(uint32_t period_ms, bool status);
bool proc_watchdog(void);

#endif /* WATCHDOG_DRV_H  */
