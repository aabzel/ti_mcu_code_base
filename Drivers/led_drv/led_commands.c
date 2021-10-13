#include "led_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "io_utils.h"
#include "led_drv.h"
#include "log.h"

bool led_get_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        io_printf("period: %u ms" CRLF, greenLed.period_ms);
        io_printf("duty  : %u %%" CRLF, greenLed.duty);
        io_printf("phase : %u ms" CRLF, greenLed.phase_ms);
    } else {
        LOG_ERROR(SYS, "Usage: lg ");
    }
    return res;
}

bool led_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(3 == argc) {
        res = true;

        if(true == res) {
            res = try_str2uint32(argv[0], &greenLed.period_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract period %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &greenLed.duty);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract duty %s", argv[1]);
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[2], &greenLed.phase_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract phase %s", argv[2]);
            }
        }

    } else {
        LOG_ERROR(SYS, "Usage: ls period_ms duty phase_ms");
        LOG_INFO(SYS, "period");
        LOG_INFO(SYS, "duty 0....100 ");
        LOG_INFO(SYS, "phase");
    }
    return res;
}
