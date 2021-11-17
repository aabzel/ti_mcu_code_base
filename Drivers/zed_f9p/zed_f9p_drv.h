#ifndef ZED_F9P_DRV_H
#define ZED_F9P_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

#define ZED_F9P_PERIOD_US S_2_US(1.0)
#define DFLT_GNSS_PER_MS 1000U

typedef enum eRTKmode_t {
    RTK_ROVER,
    RTK_BASE,
} RTKmode_t;

typedef enum eTimeSystem_t {
    TIME_UTC = 0,
    TIME_GPS = 1,
    TIME_GLONASS = 2,
    TIME_BEIDOU = 3,
    TIME_GALILEO = 4,
} TimeSystem_t;

typedef struct xZedF9P_t {
    struct tm time_date;
    GnssCoordinate_t coordinate_base; /*Location of RTK base station*/
    GnssCoordinate_t coordinate_cur;
    GnssCoordinate_t coordinate_last;
    double alt_base;
    uint16_t rate_ms;
    int8_t time_zone;
    RTKmode_t rtk_mode;
    // TODO:  time since update
    bool is_init;
} ZedF9P_t;

extern ZedF9P_t ZedF9P;

bool zed_f9p_init(void);
bool zed_f9p_load_params(void);
bool zed_f9p_proc(void);
bool zed_f9p_deploy_rover(void);
bool zed_f9p_deploy_base(GnssCoordinate_t coordinate_base, double altitude_sea_lev_m);

#endif /* ZED_F9P_DRV_H  */
