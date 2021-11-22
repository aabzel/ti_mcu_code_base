#ifndef GNSS_DIAG_H
#define GNSS_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

bool print_coordinate(GnssCoordinate_t coordinate, bool one_line);
bool print_time_date(struct tm* time_date);
bool print_velocity(GnssVelocity_t velocity);
char* coordinate2str(GnssCoordinate_t* value);

#ifdef __cplusplus
}
#endif

#endif /* GNSS_DIAG_H */
