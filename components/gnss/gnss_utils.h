#ifndef GNSS_UTILS_H
#define GNSS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define METER_TO_MM(METER) ((METER) * 1000)
#define MM_TO_METER(MM) ( (MM)/1000.0 )

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