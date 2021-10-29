#ifndef GNSS_DIAG_H
#define GNSS_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

bool print_time_date(struct tm *time_date);
bool print_coordinate(GnssCoordinate_t coordinate);
 
#ifdef __cplusplus
}
#endif

#endif /* GNSS_DIAG_H */
