#ifndef GNSS_DIAG_H
#define GNSS_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_utils.h"

bool print_wgs84_coordinate(Wgs84Coordinate_t Wgs84Coordinate, bool one_line) ;
bool print_coordinate(GnssCoordinate_t coordinate, bool one_line);
bool print_velocity(GnssVelocity_t velocity);
char* coordinate2str(void* value);

#ifdef __cplusplus
}
#endif

#endif /* GNSS_DIAG_H */
