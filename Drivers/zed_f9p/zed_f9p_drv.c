#include "zed_f9p_drv.h"

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#ifdef HAS_PARAM
#include "param_ids.h"
#endif
#include "log.h"
#include "nmea_protocol.h"
#include "sys_config.h"

extern ZedF9P_t ZedF9P = {0};

bool zed_f9p_init(void) {
    bool res = true;
    ZedF9P.time_zone = 0;
#if defined(HAS_FLASH_FS) && defined(HAS_PARAM)
    uint16_t file_len = 0;
    res = mm_get(PAR_ID_TIME_ZONE, (uint8_t*)&ZedF9P.time_zone, 1, &file_len);
    if(res && (1 == file_len)) {
        ZedF9P.is_init = true;
        LOG_INFO(ZED_F9P, "Read TimeZone UTC%u", ZedF9P.time_zone);
    } else {
        LOG_ERROR(ZED_F9P, "Read TimeZone Error");
        ZedF9P.time_zone = DFLT_TIME_ZONE;

        res = mm_set(PAR_ID_TIME_ZONE, (uint8_t*)&ZedF9P.time_zone, 1);
        if(res) {
            LOG_WARNING(ZED_F9P, "Set dflt TimeZone UTC+3");
        } else {
            LOG_ERROR(ZED_F9P, "Set dflt TimeZone Error");
        }
#endif /*HAS_FLASH_FS && HAS_PARAM*/
    }
    return res;
}

bool zed_f9p_proc(void) {
    /*is new GNSS sampless*/
    ZedF9P.time_date = NmeaData.rmc.time_date;
    ZedF9P.coordinate_cur = NmeaData.rmc.coordinate;
    ZedF9P.coordinate_last = ZedF9P.coordinate_cur;
    return true;
}
