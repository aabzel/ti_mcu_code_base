#ifndef GNSS_UTILS_H
#define GNSS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>


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

bool is_valid_time(struct tm *date_time);
bool is_time_date_equal(struct tm *date_time1,
                        struct tm *date_time2);
bool is_valid_time_date(struct tm *date_time);
bool is_valid_gnss_coordinates(GnssCoordinate_t dot);
double gnss_calc_distance_m(GnssCoordinate_t dot1, GnssCoordinate_t  dot2);
double gnss_encoding_2_degrees(double in_ddmm_mmmmm);
GnssCoordinate_t encode_gnss_coordinates(GnssCoordinate_t dot_ddmm);
#ifdef __cplusplus
}
#endif

#endif /* GNSS_UTILS_H */
