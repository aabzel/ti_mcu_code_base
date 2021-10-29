#include "gnss_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "io_utils.h"
#define D2R (M_PI / 180.0)

double gnss_calc_distance_m(GnssCoordinate_t dot1, GnssCoordinate_t  dot2){
    double	 distance=0.0f;
    double dlong = (dot2.longitude - dot1.longitude) * D2R;
    double dlat = (dot2.latitude - dot1.latitude) * D2R;
    double a = pow(sin(dlat/2.0), 2) + cos(dot1.latitude*D2R) * cos(dot2.latitude*D2R) * pow(sin(dlong/2.0), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    distance = 6367 * c * 1000.0;
    return distance;
}
 



