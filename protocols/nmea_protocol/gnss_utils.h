#ifndef GNSS_UTILS_H
#define GNSS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


typedef struct xGnssCoordinate_t{
    double latitude;    /* Latitude (degrees and minutes) */
    double longitude;    /* Longitude (degrees and minutes) */
//} __attribute__((packed)) GnssCoordinate_t;
} GnssCoordinate_t;

double gnss_calc_distance_m(GnssCoordinate_t dot1, GnssCoordinate_t  dot2);

#ifdef __cplusplus
}
#endif

#endif /* GNSS_UTILS_H */
