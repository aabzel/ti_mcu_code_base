#include "rtc_commands.h"

#include <aon_rtc.h>
#include <inttypes.h>

#include "convert.h"
#include "data_utils.h"
#include "log.h"
#include "rtc_drv.h"
#include "table_utils.h"

bool rtc_disable_command(int32_t argc, char* argv[]){
    AONRTCDisable();
    return true;
}

bool rtc_diag_command(int32_t argc, char* argv[]) {
    Type64Union_t un64;
    LOG_INFO(LG_RTC, "RTC %sabled" , (true==AONRTCActive())?"en":"dis");
    un64.u64= AONRTCCurrent64BitValueGet();
    LOG_INFO(LG_RTC, "Current64BitValue: %llu 0x%llx tick" , un64.u64 , un64.u64);
    static uint32_t rtc_time_prev_ms=0;
    uint32_t rtc_time_diff_ms=0;
    uint32_t sec = AONRTCSecGet();
    LOG_INFO(LG_RTC, "Sec:  %u s" , sec);

    uint64_t fraction = AONRTCFractionGet();
    LOG_INFO(LG_RTC, "Fraction %llu 0x%llx", fraction,fraction);
    uint64_t msec = 1000*fraction /(0x100000000*5);
    LOG_INFO(LG_RTC, "Ms %llu ms" , msec);
    LOG_INFO(LG_RTC, "UpTime: %u.%03llu s" , sec, msec);

    Type32Union_t un32;
    un32.u32 = AONRTCCurrentCompareValueGet();
    LOG_INFO(LG_RTC, "%u sLowWord" , un32.u16[1]);
    LOG_INFO(LG_RTC, "%u subSecHiWord" , un32.u16[0]);
    uint32_t ut_time_ms = rtc_get_ms();
    rtc_time_diff_ms = ut_time_ms - rtc_time_prev_ms;
    uint32_t sec_part = ut_time_ms/1000;
    LOG_INFO(LG_RTC, "%u ms %u.%03u+%u" , ut_time_ms, sec_part, ut_time_ms-sec_part*1000, rtc_time_diff_ms );
    rtc_time_prev_ms=ut_time_ms;
    return true;
}

bool rtc_set_sec_command(int32_t argc, char* argv[]){
    uint32_t sec=0;
    bool res = false;
    if (1==argc) {
        res = try_str2uint32(argv[0], &sec);
        if(false == res) {
            LOG_ERROR(SYS, "Unable to extract sec %s", argv[0]);
        }
    } else {
        LOG_ERROR(SYS, "Usage: rss sec");
    }
    if(res){
        res= rtc_set_sec( sec);
    }
    return res;
}

bool rtc_reset_command(int32_t argc, char* argv[]) {
    AONRTCReset();
    return true;
}
