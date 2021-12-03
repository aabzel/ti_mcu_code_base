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
        io_printf("period: %u ms" CRLF, Led[LED_INDEX_GREEN].period_ms);
        io_printf("duty  : %u %%" CRLF, Led[LED_INDEX_GREEN].duty);
        io_printf("phase : %u ms" CRLF, Led[LED_INDEX_GREEN].phase_ms);
    } else {
        LOG_ERROR(SYS, "Usage: lg ");
    }
    return res;
}

bool led_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(4 == argc) {
        res = true;
        uint8_t led_num = 0;
        if(true == res) {
            res = try_str2uint8(argv[0], &led_num);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract led_num %s", argv[0]);
            }
        }


        if(true == res) {
            res = try_str2uint32(argv[0], &Led[led_num].period_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract period %s", argv[0]);
            }
        }

        if(true == res) {
            res = try_str2uint8(argv[1], &Led[led_num].duty);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract duty %s", argv[1]);
            }
        }

        if(true == res) {
            res = try_str2uint32(argv[2], &Led[led_num].phase_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract phase %s", argv[2]);
            }
        }

    } else {
        LOG_ERROR(SYS, "Usage: ls num period_ms duty phase_ms");
        LOG_INFO(SYS, "period");
        LOG_INFO(SYS, "duty 0....100 ");
        LOG_INFO(SYS, "phase");
    }
    return res;
}
