#ifndef GNSS_UTILS_H
#define GNSS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gnss_types.h"

#ifdef HAS_SX1262
#include "sx1262_drv.h"
#endif

#define METER_TO_MM(METER) ((METER) * 1000)
#define MM_TO_METER(MM) ( (MM)/1000.0 )

bool gnss_update_link_info(GnssCoordinate_t coordinate_local,
                           GnssCoordinate_t coordinate_remote);
GnssCoordinate_t gnss_encode_deg2mm(GnssCoordinate_t dot_dd);
bool is_valid_gnss_coordinates(GnssCoordinate_t dot);
double gnss_calc_distance_m(GnssCoordinate_t dot1, GnssCoordinate_t  dot2);
double gnss_encoding_2_degrees(double in_ddmm_mmmmm);
GnssCoordinate_t encode_gnss_coordinates(GnssCoordinate_t dot_ddmm);
double gnss_calc_azimuth_deg(GnssCoordinate_t rover,
                         GnssCoordinate_t beacon);
#ifdef __cplusplus
}
#endif

#endif /* GNSS_UTILS_H */
