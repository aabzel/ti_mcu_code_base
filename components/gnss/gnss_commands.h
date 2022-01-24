#ifndef GNSS_COMMANDS_H
#define GNSS_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAS_UBLOX
#include "ublox_command.h"
#else
#define UBLOX_COMMANDS
#endif

#ifdef HAS_NMEA
#include "nmea_command.h"
#else
#define NMEA_COMMANDS
#endif

#ifdef HAS_RTCM3
#include "rtcm3_commands.h"
#else
#define RTCM3_COMMANDS
#endif

#ifdef HAS_ZED_F9P
#include "zed_f9p_commands.h"
#else
#define ZED_F9P_COMMANDS
#endif

#define GNSS_COMMANDS                                                                                                   \
    LORA_COMMANDS                                                                                                       \
    NMEA_COMMANDS                                                                                                       \
    RTCM3_COMMANDS                                                                                                      \
    UBLOX_COMMANDS                                                                                                      \
    ZED_F9P_COMMANDS

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /*GNSS_COMMANDS_H*/
