#include "watchdog_drv.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <ti/devices/cc13x2_cc26x2/inc/hw_memmap.h>
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC26XX.h>

#include "log.h"

WatchDog_t WatchDog;
static Watchdog_Handle watchdogHandle;

WatchdogCC26XX_Object watchdogCC26XXObjects[CONFIG_WATCHDOG_COUNT];

const WatchdogCC26XX_HWAttrs watchdogCC26XXHWAttrs[CONFIG_WATCHDOG_COUNT] = {
    /* CONFIG_WATCHDOG_0: period = 1000 */
    {.baseAddr = WDT_BASE, .reloadValue = 1000},
};

const Watchdog_Config Watchdog_config[CONFIG_WATCHDOG_COUNT] = {
    /* CONFIG_WATCHDOG_0 */
    {.fxnTablePtr = &WatchdogCC26XX_fxnTable,
     .object = &watchdogCC26XXObjects[CONFIG_WATCHDOG_0],
     .hwAttrs = &watchdogCC26XXHWAttrs[CONFIG_WATCHDOG_0]}};

const uint_least8_t CONFIG_WATCHDOG_0_CONST = CONFIG_WATCHDOG_0;
const uint_least8_t Watchdog_count = CONFIG_WATCHDOG_COUNT;

void watchdogCallback(uintptr_t watchdogHandle) {
    /*
     * If the Watchdog Non-Maskable Interrupt (NMI) is called,
     * loop until the device resets. Some devices will invoke
     * this callback upon watchdog expiration while others will
     * reset. See the device specific watchdog driver documentation
     * for your device.
     */
    WatchDog.time_out = true;
    // while(1) {
    //}
}

bool watchdog_set(uint32_t period_ms, bool status) {
    bool res = false;
    uint32_t reloadValue = 0;
    if(watchdogHandle) {
        res = true;
        /*
         * The watchdog reload value is initialized during the
         * Watchdog_open() call. The reload value can also be
         * set dynamically during runtime.
         *
         * Converts TIMEOUT_MS to watchdog clock ticks.
         * This API is not applicable for all devices.
         * See the device specific watchdog driver documentation
         * for your device.
         */
        reloadValue = Watchdog_convertMsToTicks(watchdogHandle, period_ms);

        /*
         * A value of zero (0) indicates the converted value exceeds 32 bits
         * OR that the API is not applicable for this specific device.
         */
        if(reloadValue != 0) {
            Watchdog_setReload(watchdogHandle, reloadValue);
        }
    }
    WatchDog.period_ms = period_ms;
    WatchDog.is_on = status;
    WatchDog.time_out = false;
    return res;
}

bool watchdog_init(void) {
    bool res = true;
    Watchdog_Params params;
    Watchdog_init();

    Watchdog_Params_init(&params);
    params.callbackFxn = (Watchdog_Callback)watchdogCallback;
    params.debugStallMode = Watchdog_DEBUG_STALL_ON;
    params.resetMode = Watchdog_RESET_ON;

    watchdogHandle = Watchdog_open(CONFIG_WATCHDOG_0, &params);
    if(watchdogHandle == NULL) {
        res = false;
    } else {
        res = watchdog_set(WDT_TIMEOUT_MS, true);
    }
    WatchDog.time_out = false;

    return res;
}

bool proc_watchdog(void) {
    bool res = false;
    if(WatchDog.time_out) {
        LOG_ERROR(WDT, "TimeOut");
    }
    if(WatchDog.is_on) {
        Watchdog_clear(watchdogHandle);
        res = true;
    }
    return res;
}
