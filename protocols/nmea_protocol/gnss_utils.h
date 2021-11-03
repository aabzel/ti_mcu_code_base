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
/*} __attribute__((packed)) GnssCoordinate_t; Cause Fault ISR*/
} GnssCoordinate_t;

typedef struct xGnssVelocity_t{
    double velocity_north;
    double velocity_east;
    double velocity_down;
    double speed;
    double speed_ground;
    double heading;
    double accuracy_speed;
    double accuracy_course;
} GnssVelocity_t;

double gnss_calc_distance_m(GnssCoordinate_t dot1, GnssCoordinate_t  dot2);

#ifdef __cplusplus
}
#endif

#endif /* GNSS_UTILS_H */
