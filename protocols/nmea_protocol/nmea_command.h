#ifndef NMEA_COMMAND_H
#define NMEA_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool nmea_messages_command(int32_t argc, char* argv[]);
bool nmea_data_command(int32_t argc, char* argv[]);
bool nmea_stat_command(int32_t argc, char* argv[]);

#define NMEA_COMMANDS                                                                                                 \
    SHELL_CMD("nmea_stat", "nms", nmea_stat_command, "NMEA message stat"),                                            \
    SHELL_CMD("nmea_dat", "nmd", nmea_data_command, "NMEA data"), \
    SHELL_CMD("nmea_mess", "nmm", nmea_messages_command, "NMEA data"),

#ifdef __cplusplus
}
#endif

#endif /* NMEA_COMMAND_H */
