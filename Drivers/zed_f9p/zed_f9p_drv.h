#ifndef ZED_F9P_DRV_H
#define ZED_F9P_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

#define ZED_F9P_PERIOD_US S_2_US(1.0)

typedef enum eRTKmode_t {
    RTK_ROVER,
    RTK_BASE,
} RTKmode_t;

typedef struct xZedF9P_t {
    struct tm time_date;
    GnssCoordinate_t coordinate_base; /*Location of RTK base station*/
    GnssCoordinate_t coordinate_cur;
    GnssCoordinate_t coordinate_last;
    double alt_base;
    int8_t time_zone;
    RTKmode_t rtk_mode;
    // TODO:  time since update
    bool is_init;
} ZedF9P_t;

extern ZedF9P_t ZedF9P;

bool zed_f9p_init(void);
bool zed_f9p_proc(void);
bool zed_f9p_rover(void);
bool zed_f9p_base(GnssCoordinate_t coordinate_base, double altitude_sea_lev_m) ;

#endif /* ZED_F9P_DRV_H  */
