#ifndef GNSS_DRV_H
#define GNSS_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "timer_utils.h"
#include "gnss_utils.h"

#define GNSS_PERIOD_US SEC_2_US(2)

typedef struct xGnss_t {
    struct tm time_date;
    GnssCoordinate_t coordinate_cur;
    GnssCoordinate_t coordinate_last;
} Gnss_t;

extern Gnss_t Gnss;

bool gnss_proc(void);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /*GNSS_DRV_H*/
