#include "test_gnss_utils.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "gnss_utils.h"
#include "float_utils.h"
#include "unit_test_check.h"

bool test_gnss_azimuth(void) {
#ifdef X86_64
   printf("\n%s()",__FUNCTION__);
#endif
    double azimuth = 0.0;
	GnssCoordinate_t rover={0.0, 0.0};
	GnssCoordinate_t beacon_north={40.0, 0.0};
    GnssCoordinate_t beacon_south={-10.0, 0.0};
    GnssCoordinate_t beacon_west={0.0, -10.0};
    GnssCoordinate_t beacon_east={0.0, 10.0};
    azimuth = gnss_calc_azimuth_deg(rover, beacon_west);
    EXPECT_TRUE(is_double_equal_absolute(270.0, azimuth, 0.5) );

	azimuth = gnss_calc_azimuth_deg(rover, beacon_north);
	EXPECT_TRUE(is_double_equal_absolute(0.0, azimuth, 0.5) );

    azimuth = gnss_calc_azimuth_deg(rover, beacon_south);
    EXPECT_TRUE(is_double_equal_absolute(180.0, azimuth, 0.5) );


    azimuth = gnss_calc_azimuth_deg(rover, beacon_east);
    EXPECT_TRUE(is_double_equal_absolute(90.0, azimuth, 0.5) );
    return true;
}

bool test_gnss_utils(void) {
#ifdef X86_64
   printf("\n%s()",__FUNCTION__);
#endif
  GnssCoordinate_t dot1={55.75512120792506, 37.61651447625873};/*tower s*/
  GnssCoordinate_t dot2={55.74891993335733, 37.61263145873993}; /*tower s*/
  double distance_m = gnss_calc_distance_m(dot1, dot2);
#ifdef X86_64
   printf("\ndistance:%f m",distance_m);
#endif
  EXPECT_NEAR(731, distance_m, 10);
  return true;
}

