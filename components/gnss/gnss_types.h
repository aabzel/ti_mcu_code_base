#ifndef GNSS_TYPES_H
#define GNSS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct xGnssCoordinate_t{
    double latitude;    /* Latitude (degrees and minutes) широта*/
    double longitude;    /* Longitude (degrees and minutes) долгота*/
/*} __attribute__((packed)) GnssCoordinate_t; Cause Fault ISR*/
} GnssCoordinate_t;


typedef struct xWgs84Coordinate_t{
    int32_t x_cm;
    int32_t y_cm;
    int32_t z_cm;
    int32_t acc_cm;
} Wgs84Coordinate_t;

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

typedef struct xLinkInfoPayload_t {
    GnssCoordinate_t coordinate_local;
    GnssCoordinate_t coordinate_remote;
} LinkInfoPayload_t;

#ifdef __cplusplus
}
#endif

#endif /* GNSS_TYPES_H */
