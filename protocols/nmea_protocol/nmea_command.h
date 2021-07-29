#ifndef NMEA_COMMAND_H
#define NMEA_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool nmea_diag_command(int32_t argc, char* argv[]);

#define NMEA_COMMANDS                                                                                                 \
    SHELL_CMD("nmea_diag", "nmd", nmea_diag_command, "NMEA diag"),

#ifdef __cplusplus
}
#endif

#endif /* NMEA_COMMAND_H */
