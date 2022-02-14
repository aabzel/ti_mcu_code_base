#ifndef GNSS_DRV_H
#define GNSS_DRV_H

/*GNSS receiver invariant component*/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "timer_utils.h"
#include "gnss_utils.h"

#ifndef HAS_GNSS
#error "It is needed to add HAS_GNSS option"
#endif

#define GNSS_PERIOD_US SEC_2_US(2)

typedef struct xGnss_t {
    struct tm time_date;
    GnssCoordinate_t coordinate_cur;
    GnssCoordinate_t coordinate_last;
    bool first_time;
    bool first_gnss;
} Gnss_t;

extern Gnss_t Gnss;

bool gnss_init(void);
bool gnss_proc(void);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /*GNSS_DRV_H*/
