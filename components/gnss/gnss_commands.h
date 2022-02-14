#ifndef GNSS_COMMANDS_H
#define GNSS_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef HAS_UBLOX
#include "ublox_commands.h"
#else
#define UBLOX_COMMANDS
#endif

#ifdef HAS_NMEA
#include "nmea_commands.h"
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

bool gnss_data_command(int32_t argc, char* argv[]);

#define GNSS_COMMANDS                                                                                                   \
    LORA_COMMANDS                                                                                                       \
    NMEA_COMMANDS                                                                                                       \
    RTCM3_COMMANDS                                                                                                      \
    UBLOX_COMMANDS                                                                                                      \
    ZED_F9P_COMMANDS                                                                                                    \
    SHELL_CMD("gnss_data", "gnss", gnss_data_command, "GNSS data"),


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /*GNSS_COMMANDS_H*/
