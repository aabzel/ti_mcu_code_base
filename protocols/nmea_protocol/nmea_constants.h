#ifndef NMEA_CONSTANTS_H
#define NMEA_CONSTANTS_H

#include <stdbool.h>
#include <stdint.h>


#include "timer_utils.h"

#ifdef HAS_TASKS
#define NMEA_PERIOD_US SEC_2_US(1.5)
#define NMEA_LACK_FRAME_WARNING_TIME_OUT_MS SEC_2_US(6)
#define NMEA_LACK_FRAME_ERROR_TIME_OUT_MS SEC_2_US(12)
#endif

#define NUM_OF_PARSED_SAT 10
#define NMEA_MSG_SIZE 300U
#define NMEA_OVERHEAD 4U
#define LED_GREEN_PERIOD_RTK_FIXED_MS 100U

typedef enum eGgaQuality_t {
    QUA_NO_FIX = 0,
    QUA_AUTONOMOUS_GNSS_FIX = 1,
    QUA_DIFFERENTIAL_GNSS_FIX = 2,
    QUA_RTK_FIXED = 4,
    QUA_RTK_FLOAT = 5,
    QUA_ESTIMATED_DEAD_RECKONING_FIX = 6,
} GgaQuality_t;

typedef enum eGsaNavMode_t {
    NAV_MODE_NO_FIX = 1,
    NAV_MODE_2D_FIX = 2,
    NAV_MODE_3D_FIX = 3,
} GsaNavMode_t;

/*GLL VTG RMC GNS*/
typedef enum ePosMode_t {
    POM_NO_FIX = 'N',
    POM_ESTIMATED_DEAD_RECKONING_FIX = 'E',
    POM_AUTONOMOUS_GNSS_FIX = 'A',
    POM_DIFFERENTIAL_GNSS_FIX = 'D',
    POM_RTK_FLOAT = 'F',
    POM_RTK_FIXED = 'R',
} PosMode_t;

typedef enum ePositionMode_t {
    PM_UNDEF,
    PM_RTK_FIXED,
    PM_RTK_FLOAT,
    PM_DEAD_RECKONING_FIX,
    PM_DEAD_RECKONING_FIX_BUT_USER_LIMITS_EXCEEDED,
    PM_GNSS_FIX_BUT_USER_LIMITS_EXCEEDED,
    PM_NO_POSITION_FIX,
    PM_2D_GNSS_FIX,
    PM_3D_GNSS_FIX,
    PM_COMBINED_GNSS_DEAD_RECKONING_FIX,
} PositionMode_t;



#endif /* NMEA_CONSTANTS_H */
