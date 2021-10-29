#ifndef ZED_F9P_DRV_H
#define ZED_F9P_DRV_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

#define ZED_F9P_PERIOD_US S_2_US(3)


typedef struct xZedF9P_t{
    struct tm time_date;
    GnssCoordinate_t coordinate_cur;
    GnssCoordinate_t coordinate_last;

    //time since update
    bool is_init;
}ZedF9P_t;

extern ZedF9P_t  ZedF9P;

bool zed_f9p_init(void);
bool zed_f9p_proc(void);

#endif /* ZED_F9P_DRV_H  */
