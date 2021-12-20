#include "rtc_commands.h"

#include <aon_rtc.h>
#include <inttypes.h>
#include <time.h>

#include "calendar.h"
#include "convert.h"
#include "data_utils.h"
//#include "gnss_diag.h"
#include "time_diag.h"
#include "log.h"
#include "rtc_drv.h"
#include "table_utils.h"
#include "time_utils.h"

bool rtc_disable_command(int32_t argc, char* argv[]) {
    AONRTCDisable(); /*Error*/
    return true;
}

bool rtc_diag_command(int32_t argc, char* argv[]) {
    Type64Union_t un64;
    LOG_INFO(LG_RTC, "RTC %sabled", (true == AONRTCActive()) ? "en" : "dis");
    un64.u64 = AONRTCCurrent64BitValueGet();
    LOG_INFO(LG_RTC, "Current64BitValue: %llu 0x%llx tick", un64.u64, un64.u64);
    static uint32_t rtc_time_prev_ms = 0;
    uint32_t rtc_time_diff_ms = 0;
    uint32_t sec = AONRTCSecGet();
    LOG_INFO(LG_RTC, "Sec:  %u s", sec);

    uint64_t fraction = AONRTCFractionGet();
    LOG_INFO(LG_RTC, "Fraction %llu 0x%llx", fraction, fraction);
    uint64_t msec = 1000 * fraction / (0x100000000 * 5);
    LOG_INFO(LG_RTC, "Ms %llu ms", msec);
    LOG_INFO(LG_RTC, "UpTime: %u.%03llu s", sec, msec);

    Type32Union_t un32;
    un32.u32 = AONRTCCurrentCompareValueGet();
    LOG_INFO(LG_RTC, "%u sLowWord", un32.u16[1]);
    LOG_INFO(LG_RTC, "%u subSecHiWord", un32.u16[0]);
    uint32_t ut_time_ms = rtc_get_ms();
    rtc_time_diff_ms = ut_time_ms - rtc_time_prev_ms;
    uint32_t sec_part = ut_time_ms / 1000;
    LOG_INFO(LG_RTC, "%u ms %u.%03u+%u", ut_time_ms, sec_part, ut_time_ms - sec_part * 1000, rtc_time_diff_ms);
    rtc_time_prev_ms = ut_time_ms;
    LOG_INFO(LG_RTC, "RawSec:  %u s", SwRtc.raw_sec);

    struct tm date_time = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    calendar_gettime(&date_time);
    print_time_date(&date_time);
    return true;
}

bool rtc_set_sec_command(int32_t argc, char* argv[]) {
    uint32_t sec = 0;
    bool res = false;
    if(1 == argc) {
        res = try_str2uint32(argv[0], &sec);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract sec %s", argv[0]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rss sec");
    }
    if(res) {
        res = rtc_set_sec(sec); /*Error*/
    }
    return res;
}

bool rtc_reset_command(int32_t argc, char* argv[]) {
    // AONRTCReset(); /*Error*/
    return true;
}

bool rtc_set_time_command(int32_t argc, char* argv[]) {
    bool res = true;
    struct tm date_time = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    if(1 <= argc) {
        res = try_str2int32(argv[0], &date_time.tm_hour) && res;
    }
    if(2 <= argc) {
        res = try_str2int32(argv[1], &date_time.tm_min) && res;
    }
    if(3 <= argc) {
        res = try_str2int32(argv[2], &date_time.tm_sec) && res;
    }
    if(4 <= argc) {
        res = try_str2int32(argv[3], &date_time.tm_mday) && res;
    }
    if(5 <= argc) {
        res = try_str2int32(argv[4], &date_time.tm_mon) && res;
    }
    if(6 <= argc) {
        res = try_str2int32(argv[5], &date_time.tm_year) && res;
    }

    if(res) {
        res = is_valid_time_date(&date_time);
    }

    if(res) {
        res = calendar_settime(&date_time);
        if(res) {
            LOG_INFO(SYS, "Ok!");
        } else {
            LOG_ERROR(SYS, "SetTimeErr");
        }
    }
    return res;
}
