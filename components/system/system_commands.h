#ifndef SYSTEM_COMMANDS_H
#define SYSTEM_COMMANDS_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ostream.h"

bool sys_rate_command(int32_t argc, char* argv[]);
bool sys_bypass_nmea_rs232_command(int32_t argc, char* argv[]);

#define SYSTEM_COMMANDS                                                                  \
    SHELL_CMD("nmea_rs", "nmrs", sys_bypass_nmea_rs232_command, "Bypass NMEA to RS232"), \
    SHELL_CMD("rates", "rate", sys_rate_command, "Rate"),

#ifdef __cplusplus
}
#endif

 
#endif /* SYSTEM_COMMANDS_H  */
