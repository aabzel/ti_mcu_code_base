#include "zed_f9p_drv.h"

#include <stdbool.h>
#include <stdint.h>

#include "clocks.h"
#ifdef HAS_FLASH_FS
#include "flash_fs.h"
#endif
#include "gnss_diag.h"
#include "log.h"

#ifdef HAS_NMEA
#include "nmea_protocol.h"
#endif

#ifdef HAS_PARAM
#include "param_ids.h"
#endif

#ifdef HAS_UBX_PTOTO
#include "ublox_driver.h"
#endif

#include "sys_config.h"

extern ZedF9P_t ZedF9P = {0};

bool zed_f9p_init(void) {
    bool res = true;

    res = set_log_level(ZED_F9P, LOG_LEVEL_INFO);
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
    /*is new GNSS samples*/
    bool res = false;
    static bool first_gnss = true;
    static bool first_time = true;
    res = is_valid_time_date(&NavInfo.date_time);
    if(res) {
        if(first_time) {
            LOG_INFO(ZED_F9P, "Init time");
            print_time_date(&NavInfo.date_time);
            first_time = false;
        }
        ZedF9P.time_date = NavInfo.date_time;
    } else {
        LOG_ERROR(ZED_F9P, "InvalidUbxTimeDate");
    }


    res = is_valid_gnss_coordinates(ZedF9P.coordinate_cur);
    if(res) {
        ZedF9P.coordinate_last = ZedF9P.coordinate_cur;
    } else {
        LOG_ERROR(ZED_F9P, "Invalid GNSS cur coordinate");
    }

    res = is_valid_gnss_coordinates(NmeaData.coordinate_dd);
    if(res) {
        ZedF9P.coordinate_cur = NmeaData.coordinate_dd;
    } else {
        LOG_ERROR(ZED_F9P, "Invalid GNSS Nmea coordinate");
        res = is_valid_gnss_coordinates(NavInfo.coordinate);
        if(res) {
            if(first_gnss) {
                LOG_INFO(ZED_F9P, "Spot valid GNSS data");
                print_coordinate(NavInfo.coordinate);
                first_gnss = false;
            }
            ZedF9P.coordinate_cur = NavInfo.coordinate;
        } else {
            LOG_ERROR(ZED_F9P, "Invalid Ubx GNSS coordinate");
        }

    }

    return res;
}

bool zed_f9p_base(GnssCoordinate_t coordinate_base){
    bool res = false;
    /*
      perform settings from here
      https://www.youtube.com/watch?v=FpkUXmM7mrc

      reset to dflt cfg
     */
    return res;
}

bool zed_f9p_rover(void){
    bool res = false;
    /*
      perform settings from here
      https://www.youtube.com/watch?v=FpkUXmM7mrc
     */
    return res;
}
