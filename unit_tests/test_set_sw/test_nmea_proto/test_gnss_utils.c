#include "test_gnss_utils.h"

#ifdef X86_64
#include <stdio.h>
#endif

#include "gnss_utils.h"
#include "unit_test_check.h"

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

