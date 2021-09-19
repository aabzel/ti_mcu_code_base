#include "clock_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "base_cmd.h"
#include "clocks.h"
#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "str_utils.h"

bool systick_set_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint32_t period24bit = 0;
        if(true == res) {
            res = try_str2uint32(argv[0], &period24bit);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract period %s", argv[0]);
            }
        }
        if(true == res) {
        }
    } else {
        LOG_ERROR(SYS, "Usage: sts period24bit");
    }
    return res;
}

bool clock_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        io_printf("up_time_ms %u ms %u s %u m" CRLF, g_up_time_ms, MS_2_S(g_up_time_ms), MS_2_MIN(g_up_time_ms));
    } else {
        LOG_ERROR(SYS, "Usage: cld");
    }
    return res;
}

bool sw_pause_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(1 == argc) {
        res = true;
        uint32_t time_ms = 0;

        if(true == res) {
            res = try_str2uint32(argv[0], &time_ms);
            if(false == res) {
                LOG_ERROR(SYS, "Unable to extract time_ms %s", argv[0]);
            }
        }

        if(true == res) {
            uint64_t cnt = 0;
            LOG_INFO(SYS, "Start");
            cnt = sw_pause_ms(time_ms);
            LOG_INFO(SYS, "End cnt: %" PRIu64 "", cnt);
        }
    } else {
        LOG_ERROR(SYS, "Usage: swp time_ms");
    }
    return res;
}
