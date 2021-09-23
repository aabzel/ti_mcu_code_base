#include "clock_commands.h"

#include <Temperature.h>
#include <ccfgread.h>
#include <inttypes.h>
#include <osc.h>
#include <stdio.h>
#include <systick.h>

#include "base_cmd.h"
#include "clocks.h"
#include "convert.h"
#include "ctype.h"
#include "data_utils.h"
#include "diag_report.h"
#include "io_utils.h"
#include "log.h"
#include "ostream.h"
#include "str_utils.h"
#include "sys_tick.h"

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
            SysTickPeriodSet(period24bit);
        }
    } else {
        LOG_ERROR(SYS, "Usage: sts period24bit");
    }
    return res;
}

static char* xoscfreq2str(uint32_t code) {
    char* name = "undef";
    switch(code) {
    case CCFGREAD_XOSC_FREQ_24M:
        name = "24M";
        break;
    case CCFGREAD_XOSC_FREQ_48M:
        name = "48M";
        break;
    case CCFGREAD_XOSC_FREQ_HPOSC:
        name = "HPOSC";
        break;
    case CCFGREAD_XOSC_FREQ_TCXO:
        name = "TCXO";
        break;
    default:
        name = "unspec";
        break;
    }
    return name;
}

bool clock_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = true;
        // PRCM_BASE +PRCM_O_OSCIMSC
        int16_t temp_c = Temperature_getTemperature();
        uint32_t millivolt = OSCHF_DebugGetCrystalAmplitude();
        uint32_t code = CCFGRead_XOSC_FREQ();
        uint32_t cnt = clock_incr_per_ms(100);
        io_printf("XOSC_FREQ %s" CRLF, xoscfreq2str(code));
        io_printf("increments per sec: %u" CRLF, cnt * 10);
        io_printf("T: %u C" CRLF, temp_c);
        io_printf("CrystalAmpl: %u mV" CRLF, millivolt);
        io_printf("up_time_ms %u ms %u s %u m" CRLF, g_up_time_ms, MS_2_S(g_up_time_ms), MS_2_MIN(g_up_time_ms));
        io_printf("SysTickPeriod %u" CRLF, SysTickPeriodGet());
        io_printf("SysTickValue %u" CRLF, SysTickValueGet());

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
