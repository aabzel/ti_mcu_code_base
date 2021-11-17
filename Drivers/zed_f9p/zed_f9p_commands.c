#include "zed_f9p_commands.h"

#include <inttypes.h>
#include <stdio.h>

#include "convert.h"
#include "flash_fs.h"
#include "gnss_diag.h"
#include "io_utils.h"
#include "log.h"
#include "param_ids.h"
#include "zed_f9p_drv.h"

bool zed_f9p_diag_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        io_printf("UTC %d" CRLF, ZedF9P.time_zone);
        res = print_time_date(&ZedF9P.time_date);
        io_printf("Current" CRLF);
        print_coordinate(ZedF9P.coordinate_cur, true);
        io_printf("Last" CRLF);
        print_coordinate(ZedF9P.coordinate_last, true);
        res = true;
    } else {
        LOG_ERROR(ZED_F9P, "Usage: zfd");
    }
    return res;
}

bool zed_f9p_base_command(int32_t argc, char* argv[]) {
    bool res = false;
    GnssCoordinate_t coordinate_base;
    double altitude_sea_lev_m = 0.0;
    if(0 == argc) {
        res = zed_f9p_load_params();
    }
    if(1 <= argc) {
        res = try_str2double(argv[0], &coordinate_base.latitude);
    }
    if(2 <= argc) {
        res = try_str2double(argv[1], &coordinate_base.longitude);
    }
    if(3 <= argc) {
        res = try_str2double(argv[2], &altitude_sea_lev_m);
    }
    if((2 <= argc) && res) {
        ZedF9P.coordinate_base = coordinate_base;
#ifdef HAS_PARAM
        res = mm_set(PAR_ID_BASE_LOCATION, (uint8_t*)&ZedF9P.coordinate_base, sizeof(GnssCoordinate_t));
        if(false == res) {
            LOG_ERROR(ZED_F9P, "ParamSetError");
        }
#endif /*HAS_PARAM*/
    }

    if(res) {
        res = zed_f9p_deploy_base(ZedF9P.coordinate_base, altitude_sea_lev_m);
        if(res) {
            LOG_INFO(ZED_F9P, "RTKBaseStarted");
        }
    }

    if(false == res) {
        LOG_ERROR(ZED_F9P, "Error");
        LOG_ERROR(ZED_F9P, "Usage: zfb lat lon alt");
    }
    return res;
}

bool zed_f9p_rover_command(int32_t argc, char* argv[]) {
    bool res = false;
    if(0 == argc) {
        res = zed_f9p_deploy_rover();
        if(res) {
            LOG_INFO(ZED_F9P, "OK");
        }
    }
    if(false == res) {
        LOG_ERROR(ZED_F9P, "Usage: zfr");
    }
    return res;
}
