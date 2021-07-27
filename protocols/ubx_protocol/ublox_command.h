#ifndef U_BLOX_COMMAND_H
#define U_BLOX_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_manager.h"

bool ubx_diag_command(int32_t argc, char* argv[]);
bool ubx_send_command(int32_t argc, char* argv[]);

#define UBLOX_COMMANDS                                                                                                 \
    SHELL_CMD("ubx_diag", "ubd", ubx_diag_command, "U-Blox EVAM8M010 diag"),                                           \
        SHELL_CMD("ubx_send", "ubs", ubx_send_command, "Send U-Blox packet"),

#ifdef __cplusplus
}
#endif

#endif /* U_BLOX_COMMAND_H */
