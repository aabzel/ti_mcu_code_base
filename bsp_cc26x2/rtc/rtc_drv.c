#include "rtc_drv.h"

#include <aon_rtc.h>
#include <stddef.h>
#include <hw_types.h>
#include <stdint.h>
#include <string.h>

#include "data_utils.h"
#include "time_utils.h"
#include "read_mem.h"

SwRtc_t SwRtc;

bool rtc_init(void) {
    SwRtc.raw_sec=0;
    time_data_parse(&SwRtc.date_time,__TIMESTAMP__);
    AONRTCEnable();
    return true;
}

uint32_t rtc_get_ms(void){
    Type32Union_t un32;
    un32.u32 = AONRTCCurrentCompareValueGet();
    uint32_t time_ms = un32.u16[1]*1000 + ((uint32_t) (((double )un32.u16[0])/327.68));
    return  time_ms;
}

bool rtc_set_sec(uint32_t sec){
    bool res = false;
    SwRtc.raw_sec = sec;
    // AONRTCDisable();
    //res = write_addr_32bit(AON_RTC_BASE + AON_RTC_O_SEC, sec);
    //AONRTCEnable();
    return res ;
}
