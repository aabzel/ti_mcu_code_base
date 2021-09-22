#include "watchdog_commands.h"


#include "convert.h"
#include "io_utils.h"
#include "log.h"
#include "watchdog_drv.h"

bool wdt_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        io_printf("state : %u" CRLF, WatchDog.is_on);
        io_printf("period: %u" CRLF, WatchDog.period_ms);
    } else {
        LOG_ERROR(UART, "Usage: wdd");
    }
    return res;
}

bool wdt_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(2 == argc) {
        res = true;
        uint32_t period_ms = 0;
        uint8_t status = 0;

        if(true == res) {
            res = try_str2uint32(argv[0], &period_ms);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract period_ms %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &status);
            if(false == res) {
                LOG_ERROR(UART, "Unable to extract status %s", argv[1]);
            }
        }

        if(true == res) {
            res = watchdog_set(period_ms, status);
            if(false == res) {
                LOG_ERROR(UART, "Unable to set watchdog");
            } else {
                io_printf(CRLF);
            }
        }

    } else {
        LOG_ERROR(UART, "Usage: wds period_ms status");
        LOG_INFO(UART, "period_ms");
        LOG_INFO(UART, "status");
    }
    return res;
}
