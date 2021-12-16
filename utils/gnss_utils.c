#include "gnss_utils.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "io_utils.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define D2R (M_PI / 180.0)

double gnss_calc_distance_m(GnssCoordinate_t dot1, GnssCoordinate_t  dot2){
    double distance = 0.0f;
    double dlong = (dot2.longitude - dot1.longitude) * D2R;
    double dlat = (dot2.latitude - dot1.latitude) * D2R;
    double a = pow(sin(dlat/2.0), 2) + cos(dot1.latitude*D2R) * cos(dot2.latitude*D2R) * pow(sin(dlong/2.0), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    distance = 6367 * c * 1000.0;
    return distance;
}

bool is_valid_gnss_coordinates(GnssCoordinate_t dot){
    bool res=false;
    if((-90.0<dot.latitude) && (dot.latitude<90.0)){
        if((-180.0<dot.longitude) && (dot.longitude<180.0)){
            res = true;
        }
        GnssCoordinate_t valid_dot={55.750964,37.617135};
        double distance = gnss_calc_distance_m(dot, valid_dot);
        if(distance < 5057696.0){
            res = true;
        }else{
            res = false;
        }
    }
    return res ;
}


double gnss_encoding_2_degrees( double in_ddmm_mmmmm){
    double ddmm_mmmmm = in_ddmm_mmmmm;
    double mm_mmmmm = 0.0;
    double dd_dddd = 0.0;
    double degrees = ((int32_t)ddmm_mmmmm) / 100 ;
    mm_mmmmm = ddmm_mmmmm - degrees * 100.0 ;
    dd_dddd = degrees + (mm_mmmmm / 60.0);
    return dd_dddd;
}

GnssCoordinate_t encode_gnss_coordinates(GnssCoordinate_t dot_ddmm){
    GnssCoordinate_t dot_dd = {0};
    dot_dd.latitude = gnss_encoding_2_degrees(dot_ddmm.latitude);
    dot_dd.longitude = gnss_encoding_2_degrees(dot_ddmm.longitude);
    return dot_dd;
}

GnssCoordinate_t gnss_encode_deg2mm(GnssCoordinate_t dot_dd){
    GnssCoordinate_t dot_mm = {0.0,0.0};
    GnssCoordinate_t equator = {0.0, dot_dd.longitude};
    GnssCoordinate_t Greenwich = {dot_dd.latitude ,0.0};
    dot_mm.longitude = gnss_calc_distance_m(dot_dd, Greenwich);
    dot_mm.latitude  = gnss_calc_distance_m(dot_dd, equator  );
    return dot_mm;
}




