#include "nmea_diag.h"

#ifndef HAS_NMEA_DIAG
    #error Do not compile unused code
#endif

const char* nmea_pos_mode2std(PositionMode_t pos_mode){
    const char* name = "undef";
    switch (pos_mode) {
    case PM_RTK_FIXED:
        name="RtkFixed";break;
    case PM_RTK_FLOAT:
        name="RtkFloat";break;
    case PM_DEAD_RECKONING_FIX:
        name="DeadReckFix"; break;
    case PM_DEAD_RECKONING_FIX_BUT_USER_LIMITS_EXCEEDED:
        name="DeadReckFixUseLimExceeded";break;
    case PM_GNSS_FIX_BUT_USER_LIMITS_EXCEEDED:
        name="GnssFixButUserLimExceeded"; break;
    case PM_NO_POSITION_FIX:
        name="NoPosFix";break;
    case PM_2D_GNSS_FIX:
        name="2dGnssFix";break;
    case PM_3D_GNSS_FIX:
        name="3dGnssFix";break;
    case PM_COMBINED_GNSS_DEAD_RECKONING_FIX:
        name="CombGnssDeadReckFix"; break;
    default:
        name = "Err";
        break;

    }
    return name;
}
